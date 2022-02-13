#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::Status;
using test::Greeter;
using test::Reply;
using test::Request;

class Client {
public:
    Client(std::shared_ptr<Channel> channel)
            : stub_(Greeter::NewStub(channel)) {}


    std::string Message_to_server(const std::string& message) {
        Request request;
        request.set_text(message);
        Reply reply;
        ClientContext context;
        Status status = stub_->Message(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "server error";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    ChannelArguments args;
    int port;
    std::cin >> port;

    Client greeter(grpc::CreateCustomChannel(
            "192.168.2.78:" + std::to_string(port), grpc::InsecureChannelCredentials(),args));

    while(true){
        std::string message;
        std::cin >> message;
        std::string reply = greeter.Message_to_server(message);
        std::cout << "Greeter received: " << reply << std::endl;
    }

    return 0;
}
