# pyscan

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [pyscan](#pyscan)
  - [安装过程](#安装过程)

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
