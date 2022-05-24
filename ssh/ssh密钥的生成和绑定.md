#ssh密钥配置
密钥配置是为了让自己的电脑可以连接github。
###要连接github，首先要设置好用户信息，在命令行中输入如下命令

git config --global user.name “输入名称”
git config --global user.email “输入email”

###用如下的命令查看自己的配置

git config --list

如果显示出自己刚刚配置过的用户名和邮箱，说明配置成功。

###下面需要生成一组密钥，在命令行中进入目标目录然后输入如下命令

ssh-keygen -t rsa -C “xxxxx@xxxxx.com”
（注意输入自己的刚刚配置邮箱地址）

###用如下命令查看生成的公钥

cat ~/.ssh/id_rsa.pub 
（注意密钥是以ssh-rsa 开头，以账号的注册邮箱结尾的）

登录github，右键头像，点击setting，然后点击SSH and GPG keys，点击 New SSH key，将刚刚查看的密钥复制到指定位置

###完成之后，在命令行中输入如下命令查看是否成功

ssh -T git@github.com

若显示Hi......，说明配置成功