-- 确保当前目录可搜到 .so
package.cpath = "./?.so;" .. package.cpath

local grpc = require("grpc_server_module")

-- 启动（会阻塞）
print("Starting server on 50051 …")
grpc.start_server("50051")

-- 停止（如果走到这里）
print("Server stopped.")
