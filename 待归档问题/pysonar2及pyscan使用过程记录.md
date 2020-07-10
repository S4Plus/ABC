# pysonar2及pyscan使用过程记录(ubuntu20.04)

## 安装jdk
进入华为mirror搜索jdk, 下载openjdk11.02
```bash
进入terminal
sudo mkdir /usr/java 
cd /usr/java
把下载的.tar.gz压缩文件放在/usr/java目录下 (先修改权限, sudo chomd 777 /usr/java)
解压
tar -zxvf +压缩文件名
设置环境变量:
sudo gedit /etc/profile  #这里是对所有用户都有效的环境变量, 每次开机时都会运行一次, 如果想只在当前用户中设置环境变量, 在bashrc中修改
在最后加上
JAVA_HOME=/usr/java/jdk-11.0.2
CLASSPATH=$JAVA_HOME/lib/
PATH=$PATH:$JAVA_HOME/bin
export PATH JAVA_HOME CLASSPATH
点save, 然后重启电脑生效
java --version验证环境变量是否添加成功
下载maven
在maven官网(http://maven.apache.org/download.cgi)下载apache-maven-3.6.3-bin.tar.gz
tar xzvf apache-maven-3.6.3-bin.tar.gz #解压,生成 apache-maven-3.6.3文件夹
设置环境变量:
sudo gedit /etc/profile
在最后加上
export MAVEN_HOME=/home/用户名/Downsloads/apache-maven-3.6.3   (因为apache-maven-3.6.3在Downloads中, 也可放到其他文件夹)
export PATH=$PATH:$MAVEN_HOME/bin
点save, 然后重启电脑生效
mvn -v验证环境变量是否添加成功
打开apache-maven-3.6.3中的settings.xml文件(先修改权限)
按照 https://blog.51cto.com/14497235/2430282 添加两段代码
保存
git下pysonar2的文件夹
切入pysonar2目录, 执行mvn package 这次会报错
然后修改pysonar2目录中的pom文件,参考 https://www.cnblogs.com/xieyang-yang/p/10694140.html 的方法一, version就是2.22.1不用换
plugin就插在文件里已有的一对plugin之后即可
save后再次执行mvn package
显示build success
后面根据(https://github.com/yinwang0/pysonar2)操作即可
对python2.7标准库的扫描结果如下:(ubuntu20.04需先设置python2.7位默认python,不过不设置,跳过这里对之后用pyscan不影响)
---------------- analysis summary ----------------
- total time: 0:1:57
- modules loaded: 662
- semantic problems: 222799
- failed to parse: 0
- number of definitions: 753616
- number of cross references: 837940
- number of references: 837940
- resolved names: 185496
- unresolved names: 2886
- name resolve rate:  98%
---------------- memory stats ----------------
- total collections: 196
- total collection time: 0:0:1
- total memory: 578M

Generating HTML
Created directory: /home/nullbyte/桌面/pysonar2/./html
Adding xref links
  5% ( 44359 of 753616)   SPEED: 66505/s   AVG SPEED: 66505/s   ETA: 0:0:10   PARSE ERRS: 0       
Adding ref links
 24% (205742 of 837940)   SPEED: 410662/s   AVG SPEED: 410662/s   ETA: 0:0:1   P 57% (485354 of 837940)   SPEED: 558107/s   AVG SPEED: 484385/s   ETA: 0:0:0   P 87% (734573 of 837940)   SPEED: 437994/s   AVG SPEED: 467583/s   ETA: 0:0:0   P100% (837940 of 837940)   SPEED: 432497/s   AVG SPEED: 462950/s   ETA: 0:0:0   P 24% (159 of 660)   SPEED:   316/s   AVG SPEED:   316/s   ETA: 0:0:1   PARSE ERR 67% (444 of 660)   SPEED:   568/s   AVG SPEED:   442/s   ETA: 0:0:0   PARSE ERR100% (660 of 660)   SPEED:   768/s   AVG SPEED:   514/s   ETA: 0:0:0   PARSE ERRS: 0       
Wrote 662 files to ./html
---------------- memory stats ----------------
- total collections: 213
- total collection time: 0:0:1
- total memory: 976M

```
## 安装anaconda
在software updates换成科大源后下载anaconda(清华镜像网站)
按照 (https://www.linuxidc.com/Linux/2020-02/162304.htm) (https://zhuanlan.zhihu.com/p/32925500)安装
安装后关闭terminal或者重启电脑, 之后打开terminal自动进入base虚拟环境
```bash
conda update conda 安装最新版conda
conda create --name python2 python=2.7 创建名叫python2的虚拟环境, 地址为anaconda3/envs
conda create --name python3 python=3.8.2 numpy pandas 创建python3虚拟环境, 且该环境安装了numpy和pandas
conda activate python2 切换base虚拟环境到python2虚拟环境
conda deactivate 退出虚拟环境
conda activate 进入虚拟环境
```
## 使用pyscan
在 https://git.lug.ustc.edu.cn/yuzhang/python/-/tree/master/toolbox%2FfeatureScanner 中下载featurescanner
用其中的 default_args_class.py作为pyscan测试文件
```bash
在pycharm中打开 default_args_class.py
在pycharm的terminal中将(base)环境切到python2环境
conda activate python2
cd到pysonar2文件夹下, 用pysonar2处理
java -jar target/pysonar-2.1.2.jar /<default_args_class.py的路径> ./html 
(如java -jar target/pysonar-<version>.jar /home/xu/Downloads/default_args_class.py ./html)
此时在pysonar2的html文件夹中生成default_args_class.py.html
在pycharm中打开toolbox文件夹, project interpreter设置conda environment--existing environment--interpreter选择anaconda3/envs/python3/bin/python3
在pycharm的terminal中将(base)环境切到python3环境
conda activate python3
用pip3安装几个包astpretty, beautifulsoup4, prettytable
pip3 install astpretty
之后的两个同理
cd到featurescanner文件夹下, 执行
python3 featureScanner.py -s /<default_args_class.py的地址> -t <default_args_class.py.html的地址>
(如 python3 featureScanner.py -s /home/xu/Downloads/default_args_class.py -t /home/xu/pysonar/pysonar2/html/default_args_class.py.html)
或者
python3 featureScanner.py -s /home/xu/Downloads/default_args_class.py -t /home/xu/pysonar/pysonar2/html/default_args_class.py.html >usr_log
此时会在当前目录下生成usr_log.txt文件, 并将此次输出重定向到该文件  (那是否有办法将prettytable输出的表格重定向到xlsx文件?)
```