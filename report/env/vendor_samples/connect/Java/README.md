Connect Java调用流程：
1，将参考案例文件：ATK安装根目录/Help/Examples/08-二次开发案例/ATKConnectJavaTest.java，复制到：当前目录/com/atk/test
2，在当前目录执行命令：
	windows:
		javac -cp ATKConnectJava.jar -encoding utf-8 com/atk/test/ATKConnectJavaTest.java
		java -cp ".;ATKConnectJava.jar" com/atk/test/ATKConnectJavaTes
	linux:
		javac -cp ATKConnectJava.jar -encoding utf-8 com/atk/test/ATKConnectJavaTest.java
		java -cp ".:ATKConnectJava.jar" com/atk/test/ATKConnectJavaTes