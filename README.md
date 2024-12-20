# malloc_hook
## 介绍
实现一个简单的malloc hook，通过LD_PRELOAD的方式，malloc/free函数增加一些调试信息。

## 编译
```shell
mkdir build && cd build
cmake ..
make
```

## 测试
```shell
LD_PRELOAD=./lib/libmalloc_hook.so ./malloc_hook_main
```
