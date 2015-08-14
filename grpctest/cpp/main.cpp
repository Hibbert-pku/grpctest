#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>
#include "grpctest/cpp/grpc.grpc.pb.h"
#include "grpctest/cpp/ccrf_decode.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using nlu_grpc::NluRequest;
using nlu_grpc::NluReply;
using nlu_grpc::Greeter;

class GreeterServiceImpl final : public Greeter::Service {
  Status SendSentence(ServerContext* context, const NluRequest* request,
                  NluReply* reply) override {
    std::string sentence = ccrf_decode(request->name());
    reply->set_message(sentence);
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}

