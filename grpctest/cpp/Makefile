#COMPLIER

CXX = g++
NVXX = nvcc
PROTOC = /home/hibbert/library/usr/local/bin/protoc

#FLAGS
CXXFLAGS = -std=c++11 -I../..
CPPFLAGS = -I/home/hibbert/library/usr/local/include -pthread
NVXXFLAGS = 
LINKFLAGS = -L/usr/local/cuda/lib64 -L../.. -lcuda -lcudart -lglog -lgflags -lboost_program_options
LDFLAGS += -L/home/hibbert/library/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

#OTHER
TARGETS = web_server.out

PROTOS_PATH = ../proto
SRC_CPP = ./main.cpp
SRC_CC = ./ccrf_decode.cc
SRC_PROTO = grpc.pb.h grpc.grpc.pb.h

OBJS_CPP := $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJS_CU := $(patsubst %.cu, %.o, $(SRC_CU))
OBJS_CC := $(patsubst %.cc, %.o, $(SRC_CC))
OBJS_PROTO := $(patsubst %.pb.h, %.pb.o, $(SRC_PROTO))

vpath %.proto $(PROTOS_PATH)

#MAKE
all:$(TARGETS)

web_server.out: $(OBJS_PROTO) $(OBJS_CC) $(OBJS_CPP) $(OBJS_CU)
	$(CXX) $^ $(LDFLAGS)  ../../proto/*.o ../../crf/extractors/*.o -o $@ $(LINKFLAGS)

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

ccrf_decode.o: ccrf_decode.cc
	g++ -c ccrf_decode.cc -o ccrf_decode.o -std=c++11 -I../.. -I/home/hibbert/library/usr/local/include -pthread

%.o: %.cu
	$(NVXX) -c $< -o $@ $(NVXXFLAGS)

clean:
	rm -rf *.o $(TARGETS)
