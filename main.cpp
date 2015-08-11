#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <memory>
#include <string>

#include"cuda.h"

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>
#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("12345 ");
    int* a;
    int* b;
        a = (int *)malloc(sizeof(int) * 10);
        b = (int *)malloc(sizeof(int) * 10);
        for(int i=0;i<10;i++) {
                a[i]=1;
                b[i]=i;
        }
        test_kernel(a, b);
        for(int i=0;i<10;i++)
                printf("%d ",a[i]);
        printf("\n");
    std::cout << request->name() << std::endl;
    reply->set_message(prefix + request->name());
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

