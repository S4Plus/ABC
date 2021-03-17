### 从源代码编译

参考[Build from source](https://www.tensorflow.org/install/source)和[How to build TensorFlow 2.0 on Ubuntu 18.04 (x86_64) with Bazelisk](https://gist.github.com/philwo/f3a8144e46168f23e40f291ffe92e63c)

#### 下载 TensorFlow

```
git clone https://github.com/tensorflow/tensorflow.git
cd tensorflow
git checkout branch_name  # r1.15, r2.2, r2.3, etc.
```

#### 安装 Bazel

##### 使用 bazelisk 自动安装不同的版本

安装 bazelisk，参考 [Installing Bazel using Bazelisk](https://docs.bazel.build/versions/3.4.0/install-bazelisk.html)

- `npm install -g @bazel/bazelisk`
  - 安装 npm: `sudo apt install npm`
- `go get github.com/bazelbuild/bazelisk`
  - 安装 Go: `sudo apt install golang`
  - 需要将go加入PATH，如果没有 bazel 只有 bazelisk，需要 `ln -s bazelisk bazel`

生成 TensorFlow 需要的 bazel 版本配置文件，如果没有的话（一般是 0.26.1，可参考 configure.py 文件_TF_MIN_BAZEL_VERSION ）：`echo "0.26.1" > .bazelversion`

运行 `bazel version` 下载安装对应的 bazel

##### 直接安装对应版本的 bazel

参考 [Installing Bazel on Ubuntu](https://docs.bazel.build/versions/3.4.0/install-ubuntu.html)

#### 安装 python 和 pip 包

编译 TensorFlow r1.15 版本本人使用的 python 版本是 3.6

安装 pip 包，参考 [Issue](https://github.com/tensorflow/tensorflow/issues/41061#issuecomment-662222308)，注意安装正确的版本，不然编译会出现各种问题。

```
Python 3.6.9

TF-1.14:
pip install 'numpy<1.19.0'
pip install -U pip six wheel mock
pip install future
pip install keras_applications==1.0.4 --no-deps
pip install keras_preprocessing==1.0.2 --no-deps
pip install pandas

bazel version: bazel-0.25.2-installer-linux-x86_64.sh

TF-1.15 & TF-2.0:
pip install 'numpy<1.19.0'
pip install -U pip six wheel mock
pip install future
pip install keras_applications==1.0.8 --no-deps
pip install keras_preprocessing==1.1.2 --no-deps
pip install pandas

bazel version: bazel-0.26.1-installer-linux-x86_64.sh
```

#### 编译 TensorFlow

配置环境

- 使用虚拟环境，运行：`python configure.py`
- 使用系统环境，运行：`./configure`

编译

- `bazel build //tensorflow/tools/pip_package:build_pip_package`

#### 安装 TensorFlow

```
./bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
pip install /tmp/tensorflow_pkg/tensorflow-version-tags.whl
```
