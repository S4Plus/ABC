1.使用dotnet生成dll文件
新建一个文件夹，在该文件夹目录下打开 powershell(shift + 鼠标右键)
C:\dotnetruntime\runtime\.dotnet\dotnet.exe new console 
![5c25fbd9-08fe-47df-8e23-049d54ef3f8f.png](使用dotnet运行编译程序流程_files/5c25fbd9-08fe-47df-8e23-049d54ef3f8f.png)


C:\dotnetruntime\runtime\.dotnet\dotnet.exe build
![cddd5fde-16fb-458e-8302-b4e3c92ccd31.png](使用dotnet运行编译程序流程_files/cddd5fde-16fb-458e-8302-b4e3c92ccd31.png)


ls
2对dll文件进行编译
$env:COMPlus_JitDump="Main"  （双引号内填函数名，*就是全部函数）$env:COMPlus_JitDump="*"
 C:\dotnetruntime\runtime\artifacts\bin\coreclr\Windows_NT.x64.Debug\CoreRun.exe .\bin\Debug\net5.0\dotnetTest.dll
![6bcce49f-48a8-44d6-ad92-17d39a811fad.png](使用dotnet运行编译程序流程_files/6bcce49f-48a8-44d6-ad92-17d39a811fad.png)


3设置环境变量
变量名 CORE_LIBRARIES
变量值 C:\dotnetruntime\runtime\.dotnet\shared\Microsoft.NETCore.App\5.0.0-rc.2.20475.5

部分编译过程的output解读：
生成的dll代码：
![c73a7584-90df-4aab-bdac-0dc59f9fcabd.png](使用dotnet运行编译程序流程_files/c73a7584-90df-4aab-bdac-0dc59f9fcabd.png)
分配寄存器：
![413a34b6-29f2-49e2-ade9-be89c125e3de.png](使用dotnet运行编译程序流程_files/413a34b6-29f2-49e2-ade9-be89c125e3de.png)
编译生成成的汇编指令：


![be888a71-d262-427e-9f1a-929c4c9e10ba.png](使用dotnet运行编译程序流程_files/be888a71-d262-427e-9f1a-929c4c9e10ba.png)


编译dll文件的源程序
https://github.com/dotnet/runtime/blob/master/src/coreclr/src/jit/compiler.cpp
https://github.com/dotnet/runtime/blob/ee2355c801d892f2894b0f7b14a20e6cc50e0e54/docs/design/coreclr/jit/viewing-jit-dumps.md

