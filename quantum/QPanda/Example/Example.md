# QPanda 测试用例

**注意事项**

+ 本文件主要介绍QPanda

+ 放在`Test`文件夹下的程序使用了Google Test项目，未安装gtest并且想要利用它进行测试的同学可以到[链接](https://blog.csdn.net/csm201314/article/details/75136605)处下载。如果不想要使用gtest的，可以将对应的`TEST`宏替换成`main`函数，并做相应的修改。

## QCircuitOptimizer.test.cpp

程序的重点在于利用`QPanda::sub_cir_optimizer()`函数对于子线路进行优化。

