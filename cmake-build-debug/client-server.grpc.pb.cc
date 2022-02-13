// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: client-server.proto

#include "client-server.pb.h"
#include "client-server.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace test {

static const char* Greeter_method_names[] = {
  "/test.Greeter/Message",
};

std::unique_ptr< Greeter::Stub> Greeter::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Greeter::Stub> stub(new Greeter::Stub(channel, options));
  return stub;
}

Greeter::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Message_(Greeter_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Greeter::Stub::Message(::grpc::ClientContext* context, const ::test::Request& request, ::test::Reply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::test::Request, ::test::Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Message_, context, request, response);
}

void Greeter::Stub::async::Message(::grpc::ClientContext* context, const ::test::Request* request, ::test::Reply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::test::Request, ::test::Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Message_, context, request, response, std::move(f));
}

void Greeter::Stub::async::Message(::grpc::ClientContext* context, const ::test::Request* request, ::test::Reply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Message_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::test::Reply>* Greeter::Stub::PrepareAsyncMessageRaw(::grpc::ClientContext* context, const ::test::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::test::Reply, ::test::Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Message_, context, request);
}

::grpc::ClientAsyncResponseReader< ::test::Reply>* Greeter::Stub::AsyncMessageRaw(::grpc::ClientContext* context, const ::test::Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMessageRaw(context, request, cq);
  result->StartCall();
  return result;
}

Greeter::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::test::Request, ::test::Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::test::Request* req,
             ::test::Reply* resp) {
               return service->Message(ctx, req, resp);
             }, this)));
}

Greeter::Service::~Service() {
}

::grpc::Status Greeter::Service::Message(::grpc::ServerContext* context, const ::test::Request* request, ::test::Reply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace test
