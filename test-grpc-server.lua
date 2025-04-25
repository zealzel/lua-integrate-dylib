local grpc = require("grpc_server")

-- 这会阻塞当前 Lua 线程，直到你在其他终端/线程调用 stop_server()
print("Starting gRPC server on port 50051...")
grpc.start_server("50051")
print("Server has stopped.")
