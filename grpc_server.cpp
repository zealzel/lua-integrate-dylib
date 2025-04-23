// grpc_server.cpp
#include "hello.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloReply;
using helloworld::HelloRequest;
using helloworld::HelloService;

// 1) 實作服務
class HelloServiceImpl final : public HelloService::Service {
  public:
    Status SayHello(ServerContext* /*ctx*/, const HelloRequest* req, HelloReply* rep) override {
        rep->set_message("Hello, " + req->name());
        std::cout << "Received: " << req->name() << std::endl;
        return Status::OK;
    }
};

// 2) 提供 C API 供 Lua 呼叫，啟動並阻塞於 Wait()
extern "C" {
void start_server(const char* port) {
    std::string addr = std::string("[::]:") + port;
    HelloServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "gRPC Server listening on " << addr << std::endl;
    server->Wait();
}
}
