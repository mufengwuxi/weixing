## windows下编译

根据电脑安装的Vistual Studio版本，打开相应的解决方案

例如，如果安装是Vistual Studio 2022，则打开vs2022文件夹下的解决方案(.sln文件)

打开解决方案后，**需要重定向工程**，在工程配置中选择电脑所安装的Windows SDK版本

配置修改完成后即可编译

## linux下编译


在命令行中进入该目录，运行以下命令：

./configure
make

注意需要给./configure赋予可执行权限：

chmod +x ./configure


