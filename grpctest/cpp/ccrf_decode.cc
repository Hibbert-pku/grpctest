// Copyright 2015 MISingularity, Inc.
// All rights reserved.
// Author: jiahua.liu@misingularity.io (Liu Jiahua)

//
// The decoder for a Chain CRF (Conditional Random Field) model
// Load a trained CCRF model and produce lattice output for test data.
//

#include <string>
#include <vector>
#include <boost/algorithm/string/predicate.hpp>
#include <gflags/gflags.h>

#include "base/allocator.h"
#include "base/dataset.h"
#include "base/math_util.h"
#include "crf/attr_extractor.h"
#include "crf/attr_extractor_manager.h"
#include "crf/ccrf.h"
#include "crf/ccrf_dispatcher.h"
#include "crf/ccrf_model_loader.h"
#include "crf/xccrf_vg_calculator.h"
#include "tools/ccrf_parser.h"
#include "util/proto_utils.h"
#include "grpctest/cpp/ccrf_decode.h"

#ifndef GFLAGS_GFLAGS_H_
namespace gflags = google;
#endif  // GFLAGS_GFLAGS_H_

DEFINE_double(threshold, 0.1, "Threshold to qualify as a word candidate");
DEFINE_int32(max_word_length, 15, "Max length of a word");

DEFINE_string(model_path, "crf/data/ccrf_model_weights", "path to ccrf model file");
DEFINE_string(testingset, "crf/data/ccrf_test_example", "Url for testing, absolute if starts with '/'.");
DEFINE_string(extractor_config, "crf/data/ccrf_extractors_desc.prototxt", "config file for extractors");

DEFINE_string(grammar_path, "crf/data/ccrf_grammar", "Data path to output data file");
DEFINE_string(feature_map_path, "crf/data/ccrf_feature_map", "Data path to output data file");

using namespace nlu;

int findRuleIndex(const Grammar<2, HostAllocator>& grmr, int x, int y) {
  IntArray ridxs(grmr.GetApplicableRules(0, x));
  for (int ri = 0; ri < ridxs.size(); ri++) {
    Rule<2> rule(grmr.GetRule(ridxs[ri]));
    if (rule[1] == y) {
      return ridxs[ri];
    }
  }
  return -1;
}

class StrTagSeqCollector : public TagSeqCollector {
 public:
  StrTagSeqCollector(const KeyIdMap* dict) : lbldict_(dict) {}

  virtual ~StrTagSeqCollector() {}
  virtual void Collect(int stt, int end, int tag, float value) {
    spans_.push_back(std::pair<int, int>(stt, end));
    tags_.push_back(lbldict_->getKey(tag));
    merits_.push_back(value);
    std::stringstream ss;
        ss << stt;
        ss << '~';
        ss << end;
        ss << ' ';
        ss << lbldict_->getKey(tag);
        ss << ' ';
//        ss << value;
//        ss << '\n';
        x = x + ss.str();
  }

  std::vector<std::pair<int, int>> spans_;
  std::vector<std::string> tags_;
  std::vector<float> merits_;
  std::string x;
 private:
  const KeyIdMap* lbldict_;
};

std::string ccrf_decode(std::string input_str) {

  // First we need to get ccrf model
  typedef float T;
  CCRFModelLoader<HostAllocator> model_loader;
  model_loader.Load(FLAGS_grammar_path, FLAGS_feature_map_path);
  CCRFA2FMap<HostAllocator>* map = model_loader.ccrfmap;

  int nc = map->num_of_classes();
  std::unique_ptr<KeyIdMap> lbldict = std::move(model_loader.tagkid);
  std::unique_ptr<KeyIdMap> atrdict = std::move(model_loader.atrkid);

  // We then need to parse the extractor configuration so that we get attributes
  // that we want.
  AttrExtractorManager extractor_manager;
  extractor_manager.LoadFromProtoText(FLAGS_extractor_config);

  VectorStringTransformer<HostAllocator>* transformer =
      new VectorStringTransformer<HostAllocator>(&extractor_manager, atrdict->getMap());

  // Now we load ccrf model weights
  std::ifstream in(FLAGS_model_path);
  int size = -1;
  in >> size;
  CHECK (size == map->size());

  T* params_ = HostAllocator<float>().allocate(map->size());
  Array<T, HostAllocator> wts(map->size(), params_);
  for (auto i = 0; i < size; i++) {
    in >> wts[i];
  }
  in.close();

  // Build up tag relation for further use, see BILatticeDecoder in ccrf.h
  int iindex[nc];
  int urule_index[nc];
  for (auto k = 0u; k < lbldict->size(); k++) {
    std::string label = lbldict->getKey(k);
    if (boost::ends_with(label, "-B")) {
      int idxB = lbldict->get(label);
      int idxI = lbldict->get(label.substr(0, label.length()-2) + "-I");
      iindex[idxB] = idxI;
      iindex[idxI] = -1;
      urule_index[idxB] = findRuleIndex(map->GetGrammar(), idxB, idxI);
      urule_index[idxI] = findRuleIndex(map->GetGrammar(), idxI, idxI);
    }
  }

  // Get test data from the disk
  CHECK(!FLAGS_testingset.empty());
  std::unique_ptr<DataSetBuilder<std::string>> flbldr(new FileLineDataSetBuilder(FLAGS_testingset));
  std::unique_ptr<DataSet<std::string>> testDataset(flbldr->Build());

  // Decoding part
  RawSentenceParser parser;
  CCRFComputation<T> comp(*map);
  comp.SetWeights(&wts);
    std::vector<std::string> seq = parser.Transform(input_str);
    AttrChain<HostAllocator, int> attrChain = transformer->Transform(seq);

    CCRFDispatcher<T, ExactExpCalc, HostAllocator> ccrf_dispatcher;
    ccrf_dispatcher.Init(&attrChain, &comp);
    ccrf_dispatcher.Forward(&comp);
    ccrf_dispatcher.Backward(&comp);
    LogAdder<T, ExactExpCalc> log_summer;
    for (int i = 0; i < nc; i++) {
      log_summer.push_back(comp.fvars_.get(seq.size()-1, i));
    }
    T logz = log_summer.LogSum();

    StrTagSeqCollector collector(lbldict.get());
    BILatticeDecoder<T> lattice_decoder(comp, log(FLAGS_threshold), FLAGS_max_word_length, iindex, urule_index);
    for (int stt = 0; stt < seq.size(); stt++) {
      for (int tag = 0; tag < nc; tag++) {
        if (iindex[tag] != -1) {
          lattice_decoder.LatticeDecode(stt, tag, logz, &collector);
        }
      }
    }

  HostAllocator<float>().deallocate(params_);
  return collector.x;
}
