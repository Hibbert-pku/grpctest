// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: helloworld.proto
#ifndef GRPC_helloworld_2eproto__INCLUDED
#define GRPC_helloworld_2eproto__INCLUDED

#include "helloworld.pb.h"

#include <grpc++/impl/internal_stub.h>
#include <grpc++/impl/rpc_method.h>
#include <grpc++/impl/proto_utils.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/async_unary_call.h>
#include <grpc++/status.h>
#include <grpc++/stream.h>
#include <grpc++/stub_options.h>

namespace grpc {
class CompletionQueue;
class ChannelInterface;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace helloworld {

class Greeter GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::helloworld::HelloReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::helloworld::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::helloworld::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::helloworld::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface, public ::grpc::InternalStub {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status SayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::helloworld::HelloReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::helloworld::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::helloworld::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }

   private:
    ::grpc::ClientAsyncResponseReader< ::helloworld::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_SayHello_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::SynchronousService {
   public:
    Service() : service_(nullptr) {}
    virtual ~Service();
    virtual ::grpc::Status SayHello(::grpc::ServerContext* context, const ::helloworld::HelloRequest* request, ::helloworld::HelloReply* response);
    ::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;
   private:
    ::grpc::RpcService* service_;
  };
  class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {
   public:
    explicit AsyncService();
    ~AsyncService() {};
    void RequestSayHello(::grpc::ServerContext* context, ::helloworld::HelloRequest* request, ::grpc::ServerAsyncResponseWriter< ::helloworld::HelloReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
  };
};

}  // namespace helloworld


#endif  // GRPC_helloworld_2eproto__INCLUDED