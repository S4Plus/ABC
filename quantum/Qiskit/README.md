# Qiskit 学习笔记

## Qiskit安装

### 辅助工具安装

​	在安装Qiskit包之前，我们需要安装`anaconda`和`jupyter`两项工具。

#### Anaconda

##### 介绍

​	Anaconda（[官方网站](https://link.zhihu.com/?target=https%3A//www.anaconda.com/download/%23macos)）就是可以便捷获取包且对包能够进行管理，同时对环境可以统一管理的发行版本。Anaconda包含了conda、Python在内的超过180个科学包及其依赖项。

##### 安装

​	可以在官网下载对应系统的包进行安装，可以参考网站[2]。

#### Jupyter安装

##### 介绍

​	Jupyter Notebook（前身是IPython Notebook）是一个基于Web的交互式计算环境，用于创建Jupyter Notebook文档。Notebook一词可以通俗地引用许多不同的实体，主要是Jupyter Web应用程序、Jupyter Python Web服务器或Jupyter文档格式（取决于上下文）。

##### 安装

在新创建的conda环境中，利用pip包管理器安装即可：

```
pip3 install jupyter notebook
```

此时你可以通过`which jupyter`指令看到jupyter在你的新环境中，例如对于我来说，我的虚拟环境名为`qiskit`，其输出为：

```
/home/parallels/Documents/Anaconda/envs/qiskit/bin/jupyter
```

也可以使用conda安装：

```
conda install jupyter
```

### Qiskit 安装[4]

#### 安装

​	创建一个只有安装Python的最小环境：

```
conda create -n name_of_my_env python=3.8
```

​	激活你的环境：

```
conda activate name_of_my_env
```

​	接下来，安装Qiskit包，其中包含了Terra, Aer, Ignis和Aqua

```sh
pip install qiskit
```

​	如果包成功安装，你可以用`pip list | grep qiskit`检查，结果如下：

```
qiskit                0.19.6
qiskit-aer            0.5.2
qiskit-aqua           0.7.3
qiskit-ibmq-provider  0.7.2
qiskit-ignis          0.3.3
qiskit-terra          0.14.2
```

​	如果你希望使用可视化功能，推荐安装visualization包作为额外需求：

```sh
pip install qiskit[visualization]
```

·	安装好了之后，你可以通过Python将其导入到环境中：

```Python
import qiskit
```

#### 访问IBM量子系统

​	完成了之前的步骤后，你就可以通过Qiskit.Aer提供的模拟器在自己的经典计算机上执行量子电路。如果你想要在真实机器上运行，你可以按照以下步骤：

1. 创建一个IBM账号；

2. 打开导航栏中的用户->**My Account**；
3. 点击**Copy token**将令牌复制到剪贴板；
4. 在本地执行以下指令以将API令牌存储在本地的配置文件`qiskitrc`当中，替代`MY_API_TOKEN`为你在剪贴板中存取的值：

```python
from qiskit import IBMQ
IBMQ.save_account(`MY_API_TOKEN`)
```

### Q&A

**问题一**

Q: 无法创建新环境。执行`conda create -n qiskit python=3.8`后终端输出

```sh
Collecting package metadata (repodata.json): done
Solving environment: done
```

然后直接退出，但是`conda env list`中没有qiskit。

A: 解决办法：利用`conda clean --all`清理安装包，之后再重新创建新环境。

**问题二**

Q: conda删除虚拟环境出错。在用conda建立虚拟环境后，未指定python版本

```sh
conda create -n env_name
conda remove -n env_name --all
```

出现：PackagesNotFoundError: The following packages are missing from the target environment:

A: 采用以下命令即可

```sh
conda env remove -n env_name
```

## Qiskit 使用

在工作目录下运行：

```
jupyter notebook
```

以我的电脑为例，之后会输出：

```
(qiskit)  chenmingyu@parallels-vm  ~/Documents/work/Qiskit  jupyter notebook
[I 18:16:24.346 NotebookApp] Serving notebooks from local directory: /home/parallels/Documents/work/Qiskit
[I 18:16:24.347 NotebookApp] The Jupyter Notebook is running at:
[I 18:16:24.347 NotebookApp] http://localhost:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf
[I 18:16:24.347 NotebookApp]  or http://127.0.0.1:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf
[I 18:16:24.347 NotebookApp] Use Control-C to stop this server and shut down all kernels (twice to skip confirmation).
```

此时代表jupyter已经运行在[网页](http://127.0.0.1:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf)上。

之后关于jupyter的使用请参照[3]，而当前文件夹下包含了以下若干个程序：

| 程序         | 完成功能                                                     |
| ------------ | ------------------------------------------------------------ |
| plot_H.ipynb | 程序来自[1]，主要完成了Qiskit的基本使用，如量子电路的构建和模拟器上运行量子电路 |
| Ep3.ipynb    | 程序来自[5]系列的Ep3，部分内容与上一个程序相同，主要对比了在模拟器上运行和在真实机器上运行之间的差别。在程序中实现了在IBMQ机器运行程序 |
| Ep4.ipynb    | 程序来自[5]系列的Ep4，程序分别用Bloch球，状态向量和测量的方式表示了量子态 |
| Ep5.ipynb    | 程序来自[5]系列的Ep5，程序利用量子线路实现了量子信息的传送   |

### 注意事项

1. 如果在jupyter使用过程中出现ImportError，请查看Jupyter当前使用的Python版本是否是包所安装的对应版本。

## 参考

[1] [Getting Started with Qiskit](https://qiskit.org/documentation/getting_started.html)

[2] [Anaconda介绍、安装及使用教程](https://zhuanlan.zhihu.com/p/32925500)

[3] [Jupyter Notebook介绍、安装及使用教程](https://zhuanlan.zhihu.com/p/33105153)

[4] [Installing Qiskit](https://qiskit.org/documentation/install.html)

[5] [Video series:Coding with QIskit](https://www.youtube.com/playlist?list=PLOFEBzvs-Vvp2xg9-POLJhQwtVktlYGbY)