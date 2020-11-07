# Quantum Circuits(qiskit.circuit)

Quantum Circuit API

## Quantum Circuit Construction

### [QuantumRegister](https://qiskit.org/documentation/stubs/qiskit.circuit.QuantumRegister.html#qiskit.circuit.QuantumRegister)

```python
class QuantumRegister(size, name=None)
```

实现一个量子寄存器。创建一个新的通用寄存器。

**属性**



### Qubit

实现一个量子比特。创建一个量子比特。

```python 
class Qubit(register, index);
```

**参数**

+ register(`QuantumRegister`)-一个量子寄存器
+ index(`int`)-插入量子比特的索引

**产生错误**

`CircuitError`-如果所给的量子寄存器不是一个合法的`QuantumRegister`类

**属性**

| 项             | 说明                 |
| -------------- | -------------------- |
| Qubit.index    | 返回量子比特的索引值 |
| Qubit.register | 返回量子比特的寄存器 |

## Gates and Instruction

### Gate

简介：幺正量子门。创建一个新的量子门

```python
class Gate(name, num_qubits, params, label=None)
```

**参数说明**

| 参数项     | 数据类型          | 说明                 |
| ---------- | ----------------- | -------------------- |
| name       | `str`             | 门的`QObj`名称       |
| num_qubits | `int`             | 门操作的量子比特个数 |
| params     | `List`            | 参数列表             |
| label      | `Optional`[`str`] | 门的可选标签         |

**属性**

| 属性                  | 说明                                                  |
| --------------------- | ----------------------------------------------------- |
| `Gate.decompositions` | 获得基于SessionEquivalenceLibrary的指令对当前门的分解 |
| `Gate.definition`     | 返回基于其他基本门的表示                              |
| `                     |                                                       |

+ `Gate.definition`从结果上看，可以认为是基于基本门的分解，其返回值是包含分解信息的元组组成的列表；对比之下