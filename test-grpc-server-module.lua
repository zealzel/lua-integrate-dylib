-- 确保当前目录可搜到 .so
package.cpath = "./?.so;" .. package.cpath

-- Add the absolute path to package.cpath
package.cpath = package.cpath .. ";/Users/fit0269/projects/lua-integrate-dylib/?.so"
local grpc = require("grpc_server_module")

-- 启动（非阻塞）
print("Starting server on 50051 …")
grpc.start_server("50051")

-- 这行会立即执行，因为服务器在后台运行
print("Server started in background. You can do other things now.")

-- 等待用户输入来停止服务器
print("Press Enter to stop the server...")
io.read()

-- 停止服务器
print("Stopping server...")
grpc.stop_server()
print("Server stopped.")
