// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: grpc.proto

#include "grpc.pb.h"
#include "grpc.grpc.pb.h"

#include <grpc++/async_unary_call.h>
#include <grpc++/channel_interface.h>
#include <grpc++/impl/client_unary_call.h>
#include <grpc++/impl/rpc_service_method.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/stream.h>
namespace nlu_grpc {

static const char* Greeter_method_names[] = {
  "/nlu_grpc.Greeter/SendSentence",
};

std::unique_ptr< Greeter::Stub> Greeter::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Greeter::Stub> stub(new Greeter::Stub(channel));
  return stub;
}

Greeter::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : ::grpc::InternalStub(channel), rpcmethod_SendSentence_(Greeter_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel->RegisterMethod(Greeter_method_names[0]))
  {}

::grpc::Status Greeter::Stub::SendSentence(::grpc::ClientContext* context, const ::nlu_grpc::NluRequest& request, ::nlu_grpc::NluReply* response) {
  return ::grpc::BlockingUnaryCall(channel(), rpcmethod_SendSentence_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::nlu_grpc::NluReply>* Greeter::Stub::AsyncSendSentenceRaw(::grpc::ClientContext* context, const ::nlu_grpc::NluRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::nlu_grpc::NluReply>(channel(), cq, rpcmethod_SendSentence_, context, request);
}

Greeter::AsyncService::AsyncService() : ::grpc::AsynchronousService(Greeter_method_names, 1) {}

Greeter::Service::~Service() {
  delete service_;
}

::grpc::Status Greeter::Service::SendSentence(::grpc::ServerContext* context, const ::nlu_grpc::NluRequest* request, ::nlu_grpc::NluReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

void Greeter::AsyncService::RequestSendSentence(::grpc::ServerContext* context, ::nlu_grpc::NluRequest* request, ::grpc::ServerAsyncResponseWriter< ::nlu_grpc::NluReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
  AsynchronousService::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
}

::grpc::RpcService* Greeter::Service::service() {
  if (service_ != nullptr) {
    return service_;
  }
  service_ = new ::grpc::RpcService();
  service_->AddMethod(new ::grpc::RpcServiceMethod(
      Greeter_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Greeter::Service, ::nlu_grpc::NluRequest, ::nlu_grpc::NluReply>(
          std::mem_fn(&Greeter::Service::SendSentence), this)));
  return service_;
}


}  // namespace nlu_grpc
