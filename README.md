# Using luajit

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

# Using lua

## method1

test-gprc-sever.lua -> grpc_server.so -> libgrpc_server.dylib

### compile libgrpc_server.dylib (from grpc_server_bridge.cpp)

```bash
clang++ -std=c++17 -stdlib=libc++ -shared -fPIC \
    grpc_server_bridge.cpp \
    hello.pb.cc hello.grpc.pb.cc \
    -o libgrpc_server.dylib \
    $(pkg-config --cflags grpc++ protobuf) \
    $(pkg-config --libs grpc++ protobuf) \
    -pthread -ldl
```

### compile grpc_server.so (from grpc_server_lua.c)

```bash
cc -O2 -bundle -undefined dynamic_lookup \
-I$(brew --prefix lua)/include/lua \
-o grpc_server.so \
grpc_server_lua.c \
-L. -lgrpc_server
```

### Run the server

```bash
lua test-grpc-server.lua
```

### Run the client

```bash
python py_server_client/py_client.py
```

## method2

test-gprc-sever-module.lua -> grpc_server_module.so

### compile grpc_server_module.so (from grpc_server_module.c)

```bash
clang++ -std=c++17 -stdlib=libc++ -O2 -bundle -undefined dynamic_lookup \
  -I/opt/homebrew/include/lua5.4 \
  grpc_server_module.cpp hello.pb.cc hello.grpc.pb.cc \
  -o grpc_server_module.so \
  $(pkg-config --cflags grpc++ protobuf) \
  $(pkg-config --libs grpc++ protobuf)
```

### Run the server

```bash
lua test-grpc-server-module.lua
```

### Run the client

```bash
python py_server_client/py_client.py
```
