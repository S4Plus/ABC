# QPanda 例子

**注意事项**

+ 本文件主要介绍QPanda
+ 放在`Test`文件夹下的程序使用了Google Test项目，未安装gtest并且想要利用它进行测试的同学可以到[链接](https://blog.csdn.net/csm201314/article/details/75136605)处下载。如果不想要使用gtest的，可以将对应的`TEST`宏替换成`main`函数，并做相应的修改。

## 文档样例

### 编译量子程序

**量子程序转化QASM**

​	QPanda2提供了QASM转换工具接口`std::string convert_qprog_to_qasm(QProg &, QuantumMachine *)`。该接口使用起来非常简单，具体可参考文档的实例。

## 代码样例

### QCircuitOptimizer.test.cpp

程序的重点在于利用`QPanda::sub_cir_optimizer()`函数对于子线路进行优化。

