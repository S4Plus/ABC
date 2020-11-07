# QPandaAPI

​	该文件夹主要保存QPanda API的阅读笔记以及关于所实现的某些功能的笔记。API笔记基于对QPanda的`include`文件夹下的头文件以及相应的库文件的阅读。分类上基于`include`的目录树。

## 代码阅读

这一部分根据`include/`头文件目录树进行分类，放在不同的markdown文件当中：

| 文件夹名                                                     | 备注     |
| ------------------------------------------------------------ | -------- |
| [include/Core/QuantumCircuit](./Core.md)                     | 量子线路 |
| [include/Core/Utilities/Tools](./Utilities.Tools.md)         | 效能工具 |
| [include/Core/Utilities/QProgInfo](./Utilities.QProgInfo.md) | 程序信息 |
| [include/Core/QuantumMachine](./QuantumMachine.md)           | 量子机器 |

注意事项

+ 如果需要调用某一优化类对线路进行优化，大部分情况下不需要直接调用类的接口。在这个优化类对应的库文件最后往往会有通用接口一栏，其中包含了已经封装好的优化函数。