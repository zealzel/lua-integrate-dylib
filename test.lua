-- test.lua
local ffi = require("ffi")

-- 宣告 C 函式原型
ffi.cdef([[
    int sum(int a, int b);
]])

-- 載入同目錄下的 libsum.dylib
local lib = ffi.load("./libsum.dylib")

-- 呼叫 C 函式並印出結果
local a, b = 10, 32
print(string.format("%d + %d = %d", a, b, lib.sum(a, b)))
