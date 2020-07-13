# pyscan

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [pyscan](#pyscan)
  - [安装过程](#安装过程)
  - [问题及解决办法](#问题及解决办法)

<!-- /code_chunk_output -->

## 安装过程
### 一.安装pysonar2
tips: 先安装jdk11及以上的版本, 并将java添加到环境变量
下载maven
在maven官网(http://maven.apache.org/download.cgi)下载apache-maven-3.6.3-bin.tar.gz
```bash
tar xzvf apache-maven-3.6.3-bin.tar.gz #解压,生成 apache-maven-3.6.3文件夹
```
设置环境变量:
```
sudo gedit /etc/profile
在最后加上
export MAVEN_HOME=/home/用户名/Downsloads/apache-maven-3.6.3   (因为apache-maven-3.6.3在Downloads中, 也可放到其他文件夹)
export PATH=$PATH:$MAVEN_HOME/bin
点save, 然后重启电脑生效
```
打开apache-maven-3.6.3中的settings.xml文件(先修改权限)
按照 https://blog.51cto.com/14497235/2430282 添加两段代码
保存
下载pysonar2的文件夹(https://github.com/yinwang0/pysonar2)
切入pysonar2目录, 修改其中的pom文件,参考 https://www.cnblogs.com/xieyang-yang/p/10694140.html 的方法一, version就是2.22.1不用换
plugin就插在文件里已有的一对plugin之后即可
save后再次执行mvn package
显示build success

### 二. 安装pyscan
tips: 这里已用anaconda创建了python2, python3两个虚拟环境, 对应的python版本分别为python2.7, python3.8.2
在 https://git.lug.ustc.edu.cn/yuzhang/python/-/tree/master/toolbox%2FfeatureScanner 中下载整个toolbox文件夹或者featurescanner
用其中的 default_args_class.py作为pyscan测试文件
在pycharm中打开 default_args_class.py
在pycharm的terminal中将(base)环境切到python2环境 
conda activate python2
cd到pysonar2文件夹下, 用pysonar2处理
```
java -jar target/pysonar-2.1.2.jar /<default_args_class.py的路径> ./html 
(如java -jar target/pysonar-<version>.jar /home/xu/Downloads/default_args_class.py ./html)
```
此时在pysonar2的html文件夹中生成default_args_class.py.html
在pycharm中打开toolbox文件夹, project interpreter选择anaconda创建的python3
在pycharm的terminal中将(base)环境切到python3环境
```
conda activate python3
```
pip3安装几个包astpretty, beautifulsoup4, prettytable
```
pip3 install astpretty
pip3 install beautifulsoup4
pip3 install prettytable
```
cd到featurescanner文件夹下, 执行
```
python3 featureScanner.py -s /<default_args_class.py的地址> -t <default_args_class.py.html的地址>
(如 python3 featureScanner.py -s /home/xu/Downloads/default_args_class.py -t /home/xu/pysonar/pysonar2/html/default_args_class.py.html)
```
成功生成表格

### 三. 安装Python3.8.3
下载Python-3.8.3.tgz，新建一个文件夹用于解压（tar -zxvf Python-3.8.3.tgz)
cd Python-3.8.3
./configure（需要安装gcc）
make
sudo make install
#添加软链接，删除原来的链接
rm -rf /usr/bin/python3
ln -s /usr/python38/Python-3.8.3/bin/python /usr/bin/python3


<!-- /code_chunk_output -->

## 问题及解决办法
### 一.pysonar2
mvn package时遇到 Error while storing the mojo status
```
在安装maven的时候，按照网上的教程在/etc/profile里设置路径的话mvn package的时候会出现这种问题
		解决办法：设置路径不在/etc/profile里操作，环境变量写在~/.bash_profile里
		export PATH="/usr/maven/apache-maven-3.6.3/bin:$PATH"
```
mvn package时遇到 Cannot create resource output directory: / pysonar2/target/classes
```
问题：target资源被占用
解决办法：pysonar2 不要 clone 到 / 目录下，应该放到 ~ 目录下
```
mvn package时遇到测试错误
```
mvn package -Dmaven.test.failure.ignore=true跳过测试出错
```

### 二. pyscan
使用pip时遇到报错
```
python3 -m pip install 包名 -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com
此处若只有python3 -m pip install 包名 的话无法正常pip install
```
使用python3 featureScanner.py -s /xxxx -t /xxxx 的时候遇到报错：
```
遇到报错ModuleNotFoundError: No module named '_ctypes':
	  sudo apt-get install libffi-dev
		从“./configure”步骤开始重装python3.8
```
```
遇到报错from _bz2 import BZ2Compressor, BZ2Decompressor
        ModuleNotFoundError: No module named ‘_bz2’：
              sudo apt-get install bzip2-dev
              从“./configure”步骤开始重装python3.8
```
```
遇到报错Could not import the lzma modle； Your installed Python is incomplete：
	sudo apt-get install liblzma-dev
	python3 -m pip install backports.lzma -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com
	vim /usr/local/lib/python3.8/lzma.py
	文件的第27行可以看到：
		from _lzma import *
		from _lzma import _encode_filter_properties, _decode_filter_properties
	这里改成
		try:
			from _lzma import *
			from _lzma import _encode_filter_properties, _decode_filter_properties
		except:
			from backports.lzma import *
			from backports.lzma import _encode_filter_properties, _decode_filter_properties
```
