# QPandaAPI

​	该文件夹主要保存QPanda API的阅读笔记以及关于所实现的某些功能的笔记。API笔记基于对QPanda的`include`文件夹下的头文件以及相应的库文件的阅读。分类上基于`include`的目录树。

## 代码阅读

### Core

​	这一部分记录在[Core.md](./Core.md)中

### Uitilities

​	这一部分记录在[Utilities.md](./Utilities.md)中

## 量子线路替换

​	这个部分分为两个部分，第一个是读取QPandaConfig.json中所保存的替换线路数据，第二个部分是程序实现量子线路替换算法。

​	量子线路替换算法的执行流程如下：

```flow
st=>start: Start
op1=>operation: QCircuitOptimizer::sub_cir_optimizer()
op2=>operation: QCircuitOptimizer::do_optimize()
op3=>operation: ProcessOnTraversing::run_traversel()
e=>end
st->op1->op2->op3
```



​	调用量子线路替换算法的API为`QCircuitOptimizer::sub_cir_optimizer()`函数

