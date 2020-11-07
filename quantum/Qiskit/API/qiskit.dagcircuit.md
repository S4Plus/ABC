# DAG Circuit(Qiskit.dagcircuit)

## DAG Circuits

### [DAGCircuit](https://qiskit.org/documentation/stubs/qiskit.dagcircuit.DAGCircuit.html#qiskit.dagcircuit.DAGCircuit)

将量子线路作为DAG图保存

线路中存在三种类型的结点：输入(input)，输出(output)和操作(operation)。结点之间通过对应于量子比特和比特的有向边相连。

可以用于创建一个新的DAG线路。

**属性**

| 项                        | 说明                                      |
| ------------------------- | ----------------------------------------- |
| `DAGCircuit.calibrations` | 返回校准字典                              |
| `DAGCircuit.clbits`       | 返回经典比特的列表                        |
| `DAGCircuit.global_phase` | 返回线路的全局相位                        |
| `DAGCircuit.node_counter` | 返回DAG图的结点数                         |
| `DAGCircuit.qubits`       | 返回量子比特的列表（作为Qubit实例的列表） |
| `DAGCircuit.wires`        | 返回有序的线列表                          |

**方法**

| 方法名                             | 说明                         |
| ---------------------------------- | ---------------------------- |
| `DAGCircuit.add_creg`              | 将所有导线添加到经典寄存器中 |
| `DAGCircuit.add_qreg`              | 将所有导线添加到量子寄存器中 |
| `DAGCircuit.ancestors`             | 返回某个DAG结点的祖先集合    |
| `DAGCircuit.apply_operation_back`  | 对线路的输出执行某个操作     |
| `DAGCircuit.apply_operation_front` | 对线路的输入执行某个操作     |
|                                    |                              |



### [DAGNode](https://qiskit.org/documentation/stubs/qiskit.dagcircuit.DAGNode.html#qiskit.dagcircuit.DAGNode)

用于表示在DAG线路中储存在一个结点中的信息。

被用于作为`*_nodes()`函数的返回值，以供应需要使用结点信息的函数。

创建一个结点。

```python
class DAGNode(type=None, op=None, name=None, qargs=None, cargs=None, condition=None, wire=None, nid=-1)
```

**属性**

| 项              | 说明                                                       |
| --------------- | ---------------------------------------------------------- |
| `DAGNode.op`    | 返回对应于当前结点操作的指令对象，否则返回`None`           |
| `DAGNode.qargs` | 返回量子比特的列表，否则返回空列表                         |
| `DAGNode.wire`  | 返回比特对象，否则返回`None`。对于非输入输出结点会调用失败 |

**注意**

+ DAGNode.op返回的数据类型为`Gate`而不是