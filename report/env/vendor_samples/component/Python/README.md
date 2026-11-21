component Python调用流程：
1，将参考案例文件：ATK安装根目录/Help/Examples/08-二次开发案例/ATKComponentPythonTest.py，复制到：当前目录
2，在路径 安装目录/IntegratingWithATK/component执行命令，复制依赖库到当前目录
	windows:
		.\GetAllDeps.bat "Python"
	linux:
		sed -i 's/\r$//' GetAllDeps.sh && ./GetAllDeps.sh Python
3，复制安装目录下的AstroData文件夹到当前目录
4，在当前目录执行命令：
	windows:
		python ATKComponentPythonTest.py
	linux:(若有多个版本，需要具体说，3.8版本命令如下)
		python3.8 ATKComponentPythonTest.py