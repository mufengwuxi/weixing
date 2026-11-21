component Java调用流程：
1，将参考案例文件：ATK安装根目录/Help/Examples/08-二次开发案例/ATKComponentJavaTest.java，复制到：当前目录/com/atk/test
2，在路径 安装目录/IntegratingWithATK/component执行命令，复制依赖库到当前目录
	windows:
		.\GetAllDeps.bat "Java"
	linux:
		sed -i 's/\r$//' GetAllDeps.sh && ./GetAllDeps.sh Java
3，复制安装目录下的AstroData文件夹到当前目录
4，在当前目录执行命令：
	windows:
		chcp 65001
		javac -cp ATKComponentJava.jar -encoding utf-8 com/atk/test/ATKComponentJavaTest.java
		java "-Dfile.encoding=UTF-8" -cp ".;ATKComponentJava.jar" com/atk/test/ATKComponentJavaTest
	linux:
		export LD_LIBRARY_PATH=$PWD
		javac -cp ATKComponentJava.jar -encoding utf-8 com/atk/test/ATKComponentJavaTest.java
		java -cp ".:ATKComponentJava.jar" com/atk/test/ATKComponentJavaTest