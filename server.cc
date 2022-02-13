#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else

#include "helloworld.grpc.pb.h"

#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using test::Greeter;
using test::Reply;
using test::Request;

class GreeterServiceImpl final : public Greeter::Service {
    Status Message(ServerContext *context, const Request *request,
                   Reply *reply) override {

        std::cerr << request->text() << "\n";
        reply->set_message(request->text());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address_first("192.168.2.78:12345");
    std::string server_address_second("0.0.0.0:12345");

    GreeterServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address_first, grpc::InsecureServerCredentials());
    builder.AddListeningPort(server_address_second, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address_first << " and " << server_address_second << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
