#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "stringreverse.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using stringreverse::StringReply;
using stringreverse::StringRequest;
using stringreverse::StringReverse;

// Server Implementation
class ReverseServiceImplementation final : public StringReverse::Service {
  Status sendRequest(ServerContext* context, const StringRequest* request,
                     StringReply* reply) override {
    // Obtains the original string from the request
    std::string a = request->original();

    // String reversal
    int n = a.length();
    for (int i = 0; i < n / 2; i++) std::swap(a[i], a[n - i - 1]);

    reply->set_reversed(a);
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  ReverseServiceImplementation service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which
  // communication with client takes place
  builder.RegisterService(&service);

  // Assembling the server
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port: " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}