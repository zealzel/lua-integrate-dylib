local ffi = require("ffi")
ffi.cdef([[
  // 與 C++ 的 extern "C" 對應
  void start_server(const char* port);
]])
local lib = ffi.load("./libgrpc_server.dylib")
print("啟動 gRPC Server …")
-- 這行會阻塞，直到手動 Ctrl+C 終止
lib.start_server("50051")
