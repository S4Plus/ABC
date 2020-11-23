> 王顺洪 刘硕

## 对调用LLVM API的目标程序进行debug调试

以`fib_gen.cpp`生成斐波那契数为例，该程序调用LLVM API产生LLVM IR。

执行

```
g++ fib_gen.cpp -o fib_gen `llvm-config --cxxflags --ldflags --libs --system-libs`
```

将目标程序编译成可执行文件`fib_gen`。

然后对`fib_gen`进行调试

```
gdb fib_gen
```

对fib_gen中调用的API，如对Line 28的`new Module("fib", context)`，设置断点为`b Module.cpp :78`即可设置断点。

如果用这种调试，需要手动在目标程序里面调用LLVM的优化pass才可以对pass进行调试。

## 对IR代码用opt进行debug调试

以`hello.ll`代码为例，该代码由clang编译产生。

进入gdb调试opt

```	shell
gdb opt
```

添加参数，这里以调用死代码删除为例

```shell
set args hello.ll -dce -S
```

在死代码删除对应的`DCE.cpp`文件中添加断点

```shell
b DCE.cpp:<line_number>
```

如果不清楚会调用哪个函数，可以使用`rbreak`命令对所有函数加上断点

```shell
rbreak DCE.cpp:.
```

然后执行，即可对该优化pass进行调试

## 对于c代码用clang进行debug调试

参考[博客](https://blog.csdn.net/shijiu2012/article/details/89435837)

> 这种调试方法需要用到clang，调用链比较长，调试起来比较慢

