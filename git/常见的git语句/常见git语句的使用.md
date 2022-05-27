#git的使用

该文档是以github作为服务器使用git命令，首先要去gitHub上创建一个新的库，点击右上角的“＋”

在本地建立一个文件夹作为本地库

###本地库内的文件用以下命令与库相关联

git add 文件名

###将本地库与远程库相连接

 git remote add origin git@github.com:用户名/库名.git

 ###查看已经建立的连接

 git remote -v

###从github上下载内容用如下命令

git clone git@github.com:用户名/文件名.git
（后面像地址一样的东西可以从浏览器上在github的界面里获得，先点code，然后点ssh）

###更新文件要求首先进入源文件的目录下，然后用一下命令

git pull git@github.com:用户名/文件名.git
这样本地的文件就得到了更新

###上传文件同样要求先进入源文件目录，然后用命令

git push git@github.com:用户名/文件名.git
这样github上的文件就得到了更新

第一次上传文件时用到的命令有些特别

git push -u origin master

###创建分支

git branch 分支名

###切换分支

git checkout 分支名

###查看分支

git branch

###分支内容合并

git merge 分支1 
（这里将分支1的内容合并到当前分支内）

###删除分支

git branch -d 分支名

###查看远程分支，用以下命令

git branch -r

###直接克隆远程分支

git clone -b 分支名  git@github.com:用户名/文件名.git

不直接克隆远程分支的话，需要在本地建立相同名称的分支，然后更新它

下面网站中有对git指令更加详细的介绍，同时给出了一些有关状态的问题以及解决方法
https://blog.csdn.net/weixin_43934742/article/details/120332181