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
#include <thread>

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

static int l_sleep_for_5_seconds(lua_State* L) {
    std::cout << "start sleeping" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "end sleeping" << std::endl;
    return 0;
}

// 2) 全局 server 实例
static std::unique_ptr<Server> g_server;
static std::unique_ptr<std::thread> g_server_thread;

// 3) Lua 调用的 start/stop 接口
static int l_start_server(lua_State* L) {
    const char* port = luaL_checkstring(L, 1);
    // 如果已经在跑，就先停掉
    if (g_server) {
        g_server->Shutdown();
        if (g_server_thread) {
            g_server_thread->join();
            g_server_thread.reset();
        }
        g_server.reset();
    }

    std::string addr = "[::]:" + std::string(port);
    auto* svc = new HelloServiceImpl();
    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(svc);
    g_server = builder.BuildAndStart();
    std::cout << "gRPC Server listening on " << addr << std::endl;

    // 在新线程中运行服务器
    g_server_thread = std::make_unique<std::thread>([svc]() {
        g_server->Wait();
        delete svc;
    });
    
    return 0;
}

static int l_stop_server(lua_State* L) {
    if (g_server) {
        g_server->Shutdown();
        if (g_server_thread) {
            g_server_thread->join();
            g_server_thread.reset();
        }
        g_server.reset();
        std::cout << "gRPC Server stopped." << std::endl;
    }
    return 0;
}

// 4) 注册到 Lua：require("grpc_server_module")
extern "C" int luaopen_grpc_server_module(lua_State* L) {
    static const luaL_Reg funcs[] = {
        {"start_server", l_start_server},
        {"stop_server", l_stop_server},
        {"sleep_for_5_seconds", l_sleep_for_5_seconds},
        {NULL, NULL}};
    luaL_newlib(L, funcs);
    return 1;
}
