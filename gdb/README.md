#gdb的使用

###安装gdb
 
apt-get update
apt-get install gdb 

###使用gdb调试程序时需要先将程序编译，并加入 -g 子段

gcc -g 文件.c -o 文件1


###进入gdb模式

gdb

###打开文件

file 文件1

下面介绍一下gdb模式下的常用命令

###显示栈信息

bt

###切换到栈中的某一帧

frame x
（x会在bt命令中显示）

###往栈顶/栈底移动x帧

up/down x 
（当不输入x时，默认为1）

###打印x的内容

print x
（x可以是变量、对象、数组）

###打印x的地址

print &x

###打印某一地址的内容

print *地址

###设置断点

break x.c:n
(在x文件中第n行设置断点)

###设置执行断点时自动查看某些内容

command n
（n是断点序号）

###执行到循环完成

u

###单步调试

s
或者
n

###执行到函数返回

finish

###改变变量值

set var 变量名 = 值

###打印本地变量

info local

###强制返回

return
