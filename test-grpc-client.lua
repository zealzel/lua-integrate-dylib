local ffi = require("ffi")

ffi.cdef([[
  // 與 C interface 對應
  const char* call_hello(const char* target, const char* name);
  void free(void* ptr);
]])

-- 載入同目錄下的 dylib
local lib = ffi.load("./libgrpc_client.dylib")

-- 與遠端 gRPC server 溝通
local target = "127.0.0.1:50051"
local name = "Kevin"
local res = lib.call_hello(target, name)
print(string.format("Server replied: %s", ffi.string(res)))

-- 釋放 malloc 的字串
lib.free(ffi.cast("void*", res))
