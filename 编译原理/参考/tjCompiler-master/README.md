### 项目启动流程如下：
一、打开命令行，在源代码目录下新建build文件夹，并进入build
~~~C++
mkdir build
cd build
~~~

二、执行如下命令，生成makefile文件

~~~c++
cmake -G "Unix Makefiles" ..
~~~

该命令会按照Cmakelists.txt的要求把所需文件联合编译打包生成makefile文件，并把程序所需的输入文件复制到相应的文件夹下（这里设置的是run）

三、执行make，编译代码，生成可执行文件main.exe

四、运行可执行文件，输入需要编译的源文件

这里提供了报告中的测试用例(test.txt)，和三个构造的错误测试用例(TestWrong.txt)，程序运行会给出相应编译结果。

参考项目：https://github.com/juyaoliu/C-minus-compiler.git
