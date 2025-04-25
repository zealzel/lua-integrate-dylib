// grpc_server_module.cpp
extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

#include "hello.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
// #include <lauxlib.h>
// #include <lua.h>
#include <memory>
#include <string>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloReply;
using helloworld::HelloRequest;
using helloworld::HelloService;

// 1) gRPC 服务实现
class HelloServiceImpl final : public HelloService::Service {
  public:
    Status SayHello(ServerContext* /*ctx*/, const HelloRequest* req, HelloReply* rep) override {
        std::string msg = "Hello, " + req->name();
        rep->set_message(msg);
        std::cout << "Received: " << req->name() << std::endl;
        return Status::OK;
    }
};

// 2) 全局 server 实例
static std::unique_ptr<Server> g_server;

// 3) Lua 调用的 start/stop 接口
static int l_start_server(lua_State* L) {
    const char* port = luaL_checkstring(L, 1);
    // 如果已经在跑，就先停掉
    if (g_server)
        g_server->Shutdown(), g_server.reset();

    std::string addr = "[::]:" + std::string(port);
    auto* svc = new HelloServiceImpl();
    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(svc);
    g_server = builder.BuildAndStart();
    std::cout << "gRPC Server listening on " << addr << std::endl;

    // 阻塞等待—如果你需要非阻塞，可以放到新线程
    g_server->Wait();
    delete svc;
    return 0;
}

static int l_stop_server(lua_State* L) {
    if (g_server) {
        g_server->Shutdown();
        g_server.reset();
        std::cout << "gRPC Server stopped." << std::endl;
    }
    return 0;
}

// 4) 注册到 Lua：require("grpc_server_module")
extern "C" int luaopen_grpc_server_module(lua_State* L) {
    static const luaL_Reg funcs[] = {
        {"start_server", l_start_server}, {"stop_server", l_stop_server}, {NULL, NULL}};
    luaL_newlib(L, funcs);
    return 1;
}
