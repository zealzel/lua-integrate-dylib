// grpc_client.cpp
#include "hello.grpc.pb.h"
#include <cstdlib>
#include <cstring>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloReply;
using helloworld::HelloRequest;
using helloworld::HelloService;

// 真正呼叫 gRPC server 的類別
class HelloClient {
  public:
    HelloClient(std::shared_ptr<Channel> channel) : stub_(HelloService::NewStub(channel)) {}

    std::string SayHello(const std::string& target, const std::string& name) {
        HelloRequest req;
        req.set_name(name);
        HelloReply rep;
        ClientContext ctx;

        // 連線的 target 寫在 Channel 裡
        auto status = stub_->SayHello(&ctx, req, &rep);
        if (!status.ok()) {
            return "RPC failed: " + status.error_message();
        }
        return rep.message();
    }

  private:
    std::unique_ptr<HelloService::Stub> stub_;
};

// C interface，讓 LuaJIT FFI 呼叫
extern "C" {
// target 例如 "localhost:50051"
const char* call_hello(const char* target, const char* name) {
    HelloClient client(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
    std::string reply = client.SayHello(target, name);
    // 複製到 malloc 的空間，交給 Lua 去 free
    char* cstr = (char*)std::malloc(reply.size() + 1);
    std::memcpy(cstr, reply.c_str(), reply.size() + 1);
    return cstr;
}
}
