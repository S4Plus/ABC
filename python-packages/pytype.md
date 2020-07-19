## git submodule update 太慢（不使用全局代理的解决方法）
(base) liux@liux-K45VD:~/repos/pytype$ vim .gitmodules
在gitee新建仓库，选择导入已有仓库（填入目标url，比如https://github.com/python/typeshed.git），然后点击“创建”
将其中的
[submodule "typeshed"]
        path = typeshed
        url = https://github.com/python/typeshed.git
[submodule "googletest"]
        path = googletest
        url = https://github.com/google/googletest
[submodule "cpython"]
        path = cpython
        url = https://github.com/python/cpython.git



对应的url换成gitee的'https://gitee.com/liux_n/typeshed.git',其余两个类似。
（如果git clone XXX.git 的过程比较慢的话，也可以类似操作，速度高达 Mb/s)
# 
pip install -e .过程中

## 遇到
ERROR: Command errored out with exit status 1: /home/liux/anaconda3/bin/python /home/liux/anaconda3/lib/python3.7/site-packages/pip install --ignore-installed --no-user --prefix /tmp/pip-build-env-tsgg9zm_/overlay --no-warn-script-location --no-binary :none: --only-binary :none: -i https://pypi.org/simple -- 'setuptools>=40.8.0' ninja wheel Check the logs for full command output.

若是没有安装setuptools，则先安装，若是已经安装 python -m pip install --upgrade setuptools，更新它

## 
ERROR: Command errored out with exit status 1:
   command: /home/liux/anaconda3/bin/python /home/liux/anaconda3/lib/python3.7/site-packages/pip/_vendor/pep517/_in_process.py get_requires_for_build_wheel /tmp/tmpv3pifqds
       cwd: /home/liux/repos/pytype
  Complete output (29 lines):
  Force-cleaning 'out' directory.
  Running CMake ...

  AssertionError: Running CMake failed!


solve: sudo apt-get install cmake
## 
-- Found BISON: /usr/bin/bison (found suitable version "3.0.4", minimum required is "3.0.2")
  -- Could NOT find FLEX (missing: FLEX_EXECUTABLE) (Required is at least version "2.5.35")
  CMake Error at cmake/modules/PyTypeUtils.cmake:16 (message):
    PyType requires 'flex'.  The minimum required 'flex' version is 2.5.35.
  Call Stack (most recent call first):
    CMakeLists.txt:10 (include)
 解决：pytype的运行同时需要flex 和bison （sudo apt-get install flex ）


## Collecting typed-ast
  Downloading typed_ast-1.4.1-cp37-cp37m-manylinux1_x86_64.whl (737 kB)
     |███████████████                 | 348 kB 10 kB/s eta 0:00:37ERROR: Exception:
Traceback (most recent call last):
  File "/home/liux/anaconda3/lib/python3.7/site-packages/pip/_vendor/urllib3/contrib/pyopenssl.py", line 313, in recv_into
    return self.connection.recv_into(*args, **kwargs)
  File "/home/liux/anaconda3/lib/python3.7/site-packages/OpenSSL/SSL.py", line 1840, in recv_into
    self._raise_ssl_error(self._ssl, result)
  File "/home/liux/anaconda3/lib/python3.7/site-packages/OpenSSL/SSL.py", line 1646, in _raise_ssl_error
    raise WantReadError()
OpenSSL.SSL.WantReadError

During handling of the above exception, another exception occurred:


中间安装requirements.txt内容中的某一项报错的话，单独pip install XXX

## 在安装并且更新过setuptools后，仍然报错ERROR: Command errored out with exit status 1: /home/liux/anaconda3/bin/python /home/liux/anaconda3/lib/python3.7/site-packages/pip install --ignore-installed --no-user --prefix /tmp/pip-build-env-j23jwaim/overlay --no-warn-script-location --no-binary :none: --only-binary :none: -i https://pypi.org/simple -- 'setuptools>=40.8.0' ninja wheel Check the logs for full command output.


那就再更新一次
python -m pip install --upgrade setuptools

## 最后一步：重启终端生效
## 错误程序
若是不报错，即sucess，可能是它的错误不在pytype错误类里面。






