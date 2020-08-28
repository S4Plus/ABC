# pyceac 

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [安装](#安装)
  - [Clang](#clang)
    - [LLVM requirements](#llvm-requirements)
    - [python](#python)
    - [CMake](#cmake)
    - [Clang](#clang-1)
      - [源码编译方式安装](#源码编译方式安装)
      - [使用 apt 脚本安装（不推荐）](#使用-apt-脚本安装不推荐)
  - [pyceac](#pyceac-1)
  - [cloc](#cloc)

<!-- /code_chunk_output -->

## 安装

### Clang

* [官网安装流程](http://clang.llvm.org/get_started.html)

#### LLVM requirements

* [链接](https://llvm.org/docs/GettingStarted.html#requirements)
* 包含软件、硬件与 C++ 工具链三部分
    * 硬件基本都满足要求
    * Ubuntu 满足软件要求
    * 需要安装 GCC
        * Ubuntu 系统自带
        * 此处总结了教程中给的[GCC5.1 安装脚本](./GCC-5.1.0.sh)

#### python

* Python is needed for running the test suite.

#### CMake

* Ubuntu 16.04 自带版本过低
* 从[官网]( https://cmake.org/download/)下载安装即可

#### Clang

##### 源码编译方式安装
    这是最推荐的安装方式，但消耗资源较多，建议机器内存至少为 8G ，且留有较大 swap 区

* clone
    ```bash
    git clone https://github.com/llvm/llvm-project.git
    ```
* build
    注意在官网的 `make` 后增加 `make install`
    ```bash
    cd llvm-project
    mkdir build
    cd build
    cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm
    make
    make install
    ```

##### 使用 apt 脚本安装（不推荐）

>pyceac基于源码编译的llvm/clang，当使用ubuntu的apt源时，首先要同时安装libclang-dev，其次该方式只有libclang.so.1，需要ln -s到libclang.so。然后修改cmakelists的路径（如 /usr/lib/llvm-10）即可。这种方式根据系统和源不同不可控，所以文档里明确地给了官方的源码编译方式。

* [脚本来源](https://apt.llvm.org/)
    ```bash
    bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
    ```
* 链接 libclang.so
* 修改 

### pyceac

* 运行 code 文件夹下的 `tool-build.sh` 即可

### cloc 
* 使用 apt 安装即可
    ```bash
    sudo apt install cloc
    ```

