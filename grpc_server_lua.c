// grpc_server_lua.c

#include <lauxlib.h>
#include <lua.h>

// 声明你在 libgrpc_server.dylib 中导出的函数
// （注意：这两个符号必须跟 dylib 中的一致）
extern void start_server(const char *port);
extern void stop_server(void);

// Lua -> C 调用 start_server(port)
static int l_start_server(lua_State *L) {
    const char *port = luaL_checkstring(L, 1);
    start_server(port);
    return 0; // no values returned to Lua
}

// Lua -> C 调用 stop_server()
static int l_stop_server(lua_State *L) {
    stop_server();
    return 0;
}

// luaopen_* 入口，Lua `require("grpc_server")` 时被调用
int luaopen_grpc_server(lua_State *L) {
    static const luaL_Reg funcs[] = {{"start_server", l_start_server},
                                     {"stop_server", l_stop_server},
                                     {NULL, NULL}};
    luaL_newlib(L, funcs);
    return 1;
}
