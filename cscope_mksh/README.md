project_files
-----------------------------------------------------------------------
名称：project_files(默认)
作用：提供需要建立符号引用的文件列表。
格式：
/home/usr/dir1/dir2/
dir3/dir4
.
!dir5
!dir6/test.c

可以是绝对路径或者相对路径或者文件名，'.'号代表当前路径。
路径名前面加'!'叹号表示排除的路径或者文件，即不需要为该目标建立符号引用。
不加叹号表示需要建立符号引用的目标。
路径表示该路径下所有子目录和文件都需要建立符号引用。


脚本名称：mkcssym.sh
(1) ./mkcssym.sh 
不带参数使用默认参数，默认的列表文件是 project_files，默认输出路径是当前路径。
(2) ./mkcssym.sh pf=PRO_FILE
pf选项表示指定列表文件，可以相对路径或绝对路径，该文件一定要存在。
(3) ./mkcssym.sh out=OUT_DIR
out选项表示输出路径，可以相对路径或绝对路径，该路径一定要存在。

输出文件为：
cscope.out 
cscope.out.in
cscope.out.po 

vim加载： cscope add cscope.out

