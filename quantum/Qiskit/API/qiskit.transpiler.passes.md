# Transpiler Passes(qiskit.transpiler.passes)

## 引用

引用自李权熹学长的笔记

### Qiskit 编译和PassManager

Qiskit中负责管理代码编译pass的类是[PassManager](https://github.com/Qiskit/qiskit-terra/blob/37036f494d875fdb029b921043a0a8df4a4c2eb8/qiskit/transpiler/passmanager.py#L27)。
但是实际负责运行各pass的是[RunningPassManager](https://github.com/Qiskit/qiskit-terra/blob/37036f494d875fdb029b921043a0a8df4a4c2eb8/qiskit/transpiler/runningpassmanager.py#L32)。
RunningPassManager会管理属性（`property_set`字段，一个字典），调度pass。qiskit中所有pass都是[BasePass](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/basepasses.py#L51)的子类，并且可分为两种：

- 分析pass，基类为[AnalysisPass](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/basepasses.py#L101)。这些pass负责分析线路的某个属性，然后写入`property_set`（指向PassManager的`property_set`字段）。线路对分析pass是只读的。
- 变换pass，基类为[TransformationPass](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/basepasses.py#L106)。这些pass负责对线路进行变换。有些pass需要某些分析pass给出的结果（往往这些依赖被写到require里面），因此需要读取属性。属性对变换pass是只读的。

### Qiskit四级编译优化

代码地址：

- [Level 0](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/preset_passmanagers/level0.py)
- [Level 1](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/preset_passmanagers/level1.py)
- [Level 2](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/preset_passmanagers/level2.py)
- [Level 3](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/preset_passmanagers/level3.py)

#### Level 0

Level 0不进行任何优化。流程如下：

- 如果用户给定了初始映射，则应用这个映射（SetLayout），否则执行平凡初始映射（TrivialLayout）。
- 把空闲比特作为辅助比特添加进去，然后应用初始映射（FullAncillaAllocation, EnlargeWithAncilla, ApplyLayout）
- 分解为基础门（Unroller）
- 检查线路是否满足映射（CheckMap）
- 如不满足，执行SWAP
	- 在Measure之前插入barrier（BarrierBeforeFinalMeasurements）
	- 执行随机映射算法（StochasticSwap）
	- 分解SWAP门（Decompose）
- 检查比特连接是否是双向的，如果不是，执行以下操作
	- 检查CX方向性（CheckCXDirection）
	- 改写CX（插入H门）满足方向性（CXDirection）
- 当qubit处于基态(zero state)时移除reset操作（RemoveResetInZeroState）

#### Level 1

Level 1执行轻量级优化算法。流程如下：

- 如果用户给定了初始映射，则应用这个映射（SetLayout），否则执行平凡初始映射（TrivialLayout）。
- **检查线路是否满足映射**（CheckMap）
- 把空闲比特作为辅助比特添加进去，然后应用初始映射（FullAncillaAllocation, EnlargeWithAncilla, ApplyLayout）- 分解为基础门（Unroller）
- 检查线路是否满足映射（CheckMap）
- 如不满足，执行SWAP
	- 在Measure之前插入barrier（BarrierBeforeFinalMeasurements）
	- **分解多比特门**（Unroll3qOrMore）
	- 执行随机映射算法（StochasticSwap）
	- 分解SWAP门（Decompose）
- 检查比特连接是否是双向的，如果不是，执行以下操作
	- 检查CX方向性（CheckCXDirection）
	- 改写CX（插入H门）满足方向性（CXDirection）
- 当qubit处于基态时移除reset操作（RemoveResetInZeroState）
- 执行优化，直到深度（Depth）达到fixpoint（FixPoint）
	- **单比特门优化**（Optimize1qGates）
	- **CX对消**（CXCancellation）

#### Level 2

Level 2执行中等量级优化算法。流程如下：

- **分解为基础门**（Unroller）
- 如果用户给定了初始映射，则应用这个映射（SetLayout）
	- 否则，如有硬件信息，**使用readout和保真度最好的CX**
	- 否则，**使用使得qubit的连接性最强的layout**（DenseLayout）
- 把空闲比特作为辅助比特添加进去，然后应用初始映射（FullAncillaAllocation, EnlargeWithAncilla, ApplyLayout）- 检查线路是否满足映射（CheckMap）
- 如不满足，执行SWAP
	- 在Measure之前插入barrier（BarrierBeforeFinalMeasurements）
	- 分解多比特门（Unroll3qOrMore）
	- 执行随机映射算法（StochasticSwap）
	- 分解SWAP门（Decompose）
- 检查比特连接是否是双向的，如果不是，执行以下操作
	- 检查CX方向性（CheckCXDirection）
	- 改写CX（插入H门）满足方向性（CXDirection）
- 当qubit处于基态时移除reset操作（RemoveResetInZeroState）
- 执行优化，直到深度（Depth）达到fixpoint（FixPoint）
	- 单比特门优化（Optimize1qGates）
	- **基于对易性的门的对消**（CommutativeCancellation）

#### Level 3

Level 3执行重量级优化算法。流程如下：

- 分解为基础门（Unroller）
- 如果用户给定了初始映射，则应用这个映射（SetLayout）
	- 否则，使用使得qubit的连接性最强的layout（DenseLayout）
	- 否则，如有硬件信息，使用readout和保真度最好的CX（**NoiseAdaptiveLayout**）
- 把空闲比特作为辅助比特添加进去，然后应用初始映射（FullAncillaAllocation, EnlargeWithAncilla, ApplyLayout）- 检查线路是否满足映射（CheckMap）
- 如不满足，执行SWAP
	- 在Measure之前插入barrier（BarrierBeforeFinalMeasurements）
	- 分解多比特门（Unroll3qOrMore）
	- 执行随机映射算法（StochasticSwap）
	- 分解SWAP门（Decompose）
- 检查比特连接是否是双向的，如果不是，执行以下操作
	- 检查CX方向性（CheckCXDirection）
	- 改写CX（插入H门）满足方向性（CXDirection）
- 执行优化，直到深度（Depth）达到fixpoint（FixPoint）
	- **当qubit处于基态时移除reset操作**（RemoveResetInZeroState）
	- **分组**（Collect2qBlocks）
	- **将每组合成一个酉节点**（ConsolidateBlocks）
	- **分解为基本门**（Unroller）
	- 单比特门优化（Optimize1qGates）
	- 基于对易性的门的对消（CommutativeCancellation）
	- **移除测量前的交换操作, 并优化测量**（OptimizeSwapBeforeMeasure）
	- **在测量前移除对角矩阵的量子门**（RemoveDiagonalGatesBeforeMeasure）
	- 对于有向边的硬件，**改写CX（插入H门）满足方向性**（CXDirection）

## Hoare逻辑

​	这部分的思想参考论文[arXiv: 1810.00375](https://arxiv.org/pdf/1810.00375)

​	这一部分并没有更新在Qiskit的官方文档上。主要由位于qiskit/transpiler/passes/optimization下的两个文件——[hoare_opt.py](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/optimization/hoare_opt.py)和[\_gate\_extension.py](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/optimization/_gate_extension.py)组成。

### 扩展门类

​	`_gate_extension.py`利用Hoare逻辑使用过程所需要的函数对Qiskit提供的门类进行拓展，即平凡条件函数和后条件函数。如果平凡条件的返回值为`true`，说明当前量子门是平凡的，当前量子比特处于经典态。无法被忽略的函数/门可能是不平凡的或者有着未知的后条件。

​	该文件中对下面的标准门进行了拓展：

```python
if HAS_Z3:
    # FLIP GATES #
    # XGate
    XGate._postconditions = lambda self, x1, y1: y1 == Not(x1)
    CXGate._postconditions = lambda self, x1, y1: y1 == Not(x1)
    CCXGate._postconditions = lambda self, x1, y1: y1 == Not(x1)

    # YGate
    YGate._postconditions = lambda self, x1, y1: y1 == Not(x1)
    CYGate._postconditions = lambda self, x1, y1: y1 == Not(x1)

    # PHASE GATES #
    # IdGate
    IGate._postconditions = lambda self, x1, y1: y1 == x1

    # ZGate
    ZGate._trivial_if = lambda self, x1: True
    ZGate._postconditions = lambda self, x1, y1: y1 == x1
    CZGate._trivial_if = lambda self, x1: True
    CZGate._postconditions = lambda self, x1, y1: y1 == x1

    # SGate
    SGate._trivial_if = lambda self, x1: True
    SGate._postconditions = lambda self, x1, y1: y1 == x1
    SdgGate._trivial_if = lambda self, x1: True
    SdgGate._postconditions = lambda self, x1, y1: y1 == x1

    # TGate
    TGate._trivial_if = lambda self, x1: True
    TGate._postconditions = lambda self, x1, y1: y1 == x1
    TdgGate._trivial_if = lambda self, x1: True
    TdgGate._postconditions = lambda self, x1, y1: y1 == x1

    # RzGate = U1Gate
    RZGate._trivial_if = lambda self, x1: True
    RZGate._postconditions = lambda self, x1, y1: y1 == x1
    CRZGate._trivial_if = lambda self, x1: True
    CRZGate._postconditions = lambda self, x1, y1: y1 == x1
    U1Gate._trivial_if = lambda self, x1: True
    U1Gate._postconditions = lambda self, x1, y1: y1 == x1
    CU1Gate._trivial_if = lambda self, x1: True
    CU1Gate._postconditions = lambda self, x1, y1: y1 == x1

    # MULTI-QUBIT GATES #
    # SwapGate
    SwapGate._trivial_if = lambda self, x1, x2: x1 == x2
    SwapGate._postconditions = lambda self, x1, x2, y1, y2: And(x1 == y2, x2 == y1)
    CSwapGate._trivial_if = lambda self, x1, x2: x1 == x2
    CSwapGate._postconditions = lambda self, x1, x2, y1, y2: And(x1 == y2, x2 == y1)
```

​	其中，`x1`和`x2`应为门执行前量子比特的状态，`y1`和`y2`应为门执行后量子比特的状态。利用后条件在两者之间建立联系。不过值得注意的是，在论文中提到的CX门的平凡条件并没有在这里体现。除此之外，实现的平凡条件也只有两类：Z门和SWAP门。其中Z门的平凡条件为始终返回真，即Z门总是平凡的。

​	

## Pass列表

### Optimizations

#### [CrosstalkAdaptiveSchedule](https://qiskit.org/documentation/stubs/qiskit.transpiler.passes.CrosstalkAdaptiveSchedule.html#qiskit.transpiler.passes.CrosstalkAdaptiveSchedule)

+ **文件位置** [crosstalk_adaptive_schedule.py](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/analysis/count_ops.py)

+ **功能** 利用适应性的指令调度减少存在的串扰

+ **输入参数** 
	+ backend_prop([BackendProperties](https://qiskit.org/documentation/stubs/qiskit.providers.models.BackendProperties.html#qiskit.providers.models.BackendProperties))：后端属性对象

	+ crosstalk_prop(dict)：串扰属性对象
		+ crosstalk_prop\[g1\]\[g2\]指当g1和g2同时执行的时候g1的条件错误概率。其中g1应为形式为(x,y)的双量子比特元组，其中x和y为物理量子比特id，而g2可以双量子比特元组(x,y)或者单量子比特(x)。我们忽略了单量子比特门之间的串扰，不被特别说明的门对被认为是没有串扰的。

		+ 例子：crosstalk_prop = {(0,1):{(2,3):0.2, (2):0.15}, (4,5):{(2,3):0.1}, (2,3):{(0,1):0.05, (4,5):0.05}}

		+ crosstalk_prop的键为由CX门的有序元组组成的元组。例如，(0,1)对应的是硬件中CX 0,1

		+ 每一个键都有对应的字典值说明与邻近的门之间的条件错误概率，例如`(0,1):{(2,3):0.2, (2):0.15}`说明当CNOT 0,1与CNOT 2,3同时运行时，具有0.2的错误率；而与在2上的单量子比特门同时运行时，具有0.15的错误率。
	+ weight_factor(float)：为$\min (\omega \sum_{\forall g\in G}(\log g.\epsilon) - (1-\omega)\sum_{\forall q \in Q}(q.t/q.T))$表达式中的$\omega$权值

		+ $\omega$可以从0变化到1，当$\omega = 0$时说明只优化退相干错误，当$\omega = 1$时说明只优化串扰噪声。对于不同的应用，需要对$\omega$进行调整以得到最好的结果

	+ measured_qubits(list)：在特定的电路上将被测量的一系列量子比特

		+ 当随后的模块中插入了测量门，这个参数才有用处。

#### [CommutativeCancellation](https://qiskit.org/documentation/stubs/qiskit.transpiler.passes.CommutativeCancellation.html#qiskit.transpiler.passes.CommutativeCancellation)

+ **代码位置** [commutative_cancellation.py](https://qiskit.org/documentation/_modules/qiskit/transpiler/passes/optimization/commutative_cancellation.html#CommutativeCancellation)

+ **功能** 通过对易关系消除冗余门(自伴算子，Hermitian算子)。

  用于消除自逆的旋转/门。消除规则利用的是线路中存在的对易关系。考虑的门包括：

  ```
  H, X, Y, Z, CX, CY, CZ
  ```

  遍历每个量子位以生成抵消字典。抵消字典规则如下：

  + 对于1-量子比特门，其键为`(gate_type, qubit_id, commutation_set_id)`，其值为共享类型，量子比特和对易门集合id的门的列表。
  + 对于2-量子比特门，其键为`(gate_type, first_qbit, sec_qbit, first_commutation_set_id)`，其值为共享门类型，量子比特和对易门集合的门列表。

+ **属性**

	| 函数名                                         | 函数说明                 |
	| ---------------------------------------------- | ------------------------ |
	| CommutativeCancellation.is_analysis_pass       | 检查当前Pass是否是分析遍 |
	| CommutativeCancellation.is_transformation_pass | 检查当前Pass是否是转换遍 |

+ 方法

	| 方法名                           | 功能描述                               |
	| -------------------------------- | -------------------------------------- |
	| CommutativeCancellation.name()   | 返回Pass的名称                         |
	| ComutativeCacncellation.run(dag) | 在`dag`上运行CommutativeCancellation遍 |

+ 具体分析

	​		在该Pass的优化过程中，需要调用到继承属性`property_set['commutation_set']`的内容。因为在运行该优化遍之前，需要调用`commutation_analysis`，或者使用`PassManager`类。这一过程与其说是搜索线路中存在已有的对易关系，倒不如说是根据存在的对易关系对线路进行化简。
	
	​		其化简思路如下：
	
	+  对于{cx, cy, cz, h, y}门，采取的优化方式是相邻的门进行消除
	+ 对于`x_rotation`类门{rx, x}和`z_rotation`类门`{z,u1,rz,t,s}`则是分别通过合并进行化简
	
	



