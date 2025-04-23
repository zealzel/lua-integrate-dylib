## Step 1: Install dependencies

```bash
brew install protobuf grpc
pip install grpcio
pip install grpcio-tools
```

## Step 2: Compile dylib

```bash
clang++ -std=c++17 -dynamiclib \
  hello.pb.cc hello.grpc.pb.cc grpc_server.cpp \
  -o libgrpc_server.dylib \
  $(pkg-config --cflags --libs protobuf grpc++) \
  -pthread -ldl
```

## Step 3: Run the server

```bash
luajit server.lua
```

## Step 4: Run the client

```bash
python3 py_server_client/py_client.py
```