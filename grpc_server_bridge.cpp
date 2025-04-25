// grpc_server_bridge.cpp
#include "hello.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

// gRPC 服务的 C++ 实现
class HelloServiceImpl final : public helloworld::HelloService::Service {
  public:
    grpc::Status SayHello(grpc::ServerContext* /*ctx*/, const helloworld::HelloRequest* req,
                          helloworld::HelloReply* rep) override {
        std::string msg = "Hello, " + req->name();
        rep->set_message(msg);
        std::cout << "Received: " << req->name() << std::endl;
        return grpc::Status::OK;
    }
};

// 全局保存 server，方便在 stop_server 时 shutdown
static std::unique_ptr<grpc::Server> g_server;

extern "C" {
// 启动并阻塞运行 gRPC Server
void start_server(const char* port) {
    std::string addr = "[::]:" + std::string(port);
    HelloServiceImpl* service = new HelloServiceImpl();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    g_server = builder.BuildAndStart();

    std::cout << "gRPC Server listening on " << addr << std::endl;
    g_server->Wait();
    delete service;
}

// 停止 gRPC Server（如果正在运行）
void stop_server() {
    if (g_server) {
        g_server->Shutdown();
        g_server.reset();
        std::cout << "gRPC Server stopped." << std::endl;
    }
}
}
