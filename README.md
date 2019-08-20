<<<<<<< HEAD
# This is DolphinOS source code
	DolphinOS is a 32-bits Operating system, I plan to support 64-bits CPU. 
	I use ubuntu OS to write the C code.
	GCC version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.9).
	Ensure the version of software is the same as me.
	The OS can run on the qemu virual machine(X86 platform).
	The OS is installed in virtual floppy disk. 
	In version 0.03B, the OS do not enter the vbe2.0 display mode. 
	In version 0.04 i change the virual box to qemu, because the qemu runs faster than bochs.
	Version 0.05a------The character lib has been completed.
	Version 0.06 the stack checked function of gcc is disable.
	Version 0.07d new function: you can put the decimal number in screen.
	Version 0.08a complete the momery mangment system and kernel thread
# How do you run DolphinOS?
	First, you need install a linux operating system like ubuntu, CentOS. 
	And then you need install qemu, gcc(V5.40) & nasm in linux.
	Finally you open the terminal in DolphinOS folder and enter the command: make run (make sure you are root.) 
# New 0.07 version
	In this version, I change the paging to 0x100000(1mb low physical memory address) in order to implenment the memory mangment system. 
# New 0.08 version
	In this version, I add the 'Thread' in OS and I written the code that can be compiled in 64-bits operating systems. For makefile file, makefile32 is used to in 32-bits operating systems and makefile is used to in 64-bits operating systems. 
# Fans QQ group
	Future_Technology: 375678777. 
	There are development environment which is a virtual machine(VMware) file.
	you can get the download link in the QQ group.
# memory allocation image
![image](https://github.com/FTOlaf/DolphinOS/blob/master/memory_addr.PNG)


# 这是DolphinOS源代码
	DolphinOS是一个32位的操作系统，未来我会计划支持64位。
	我使用Ubuntu编写的源代码。
	gcc的版本是 5.4.0 20160609 （Ubuntu 5.4.0-6ubuntu1~16.04.9）。
	确保您运行时支持软件和我的一样。
	目前操作系统运行在qemu下（x86平台）。
	操作系统被安装在虚拟软盘中。
	在0.03b版本中，操作系统将不会进入vbe2.0显示模式。
	在0.04版本中，我已经将虚拟机从bochs切换到了qemu。
	在0.05a版本中，字符库已经被完成。
	在0.06版本中，gcc的堆栈保护已经被禁用。
# 如何运行DolphinOS？
	首先，你需要安装一个Linux操作系统，类似于Ubuntu, centos。然后你需要在Linux系统中安装qemu，gcc和nasm。
	最后你需要在DolphinOS文件夹下打开terminal，输入make run命令（确保你现在已经取得root权限）。
# 0.07新版本
	在这个版本中，我将页表移动到了0x100000处（低端物理地址），目的是为了实现内存管理系统。
# 0.08新版本
	在这个版本中我在系统中加入了线程，而且我编写了可以在64位系统上运行的代码。makefile32是被用在32位系统上的。makefile是被用在64位系统上的.
# QQ粉丝群 
	未来科技：375678777
	这里有我使用的开发环境，一个虚拟机文件。你可以从qq群得到下载地址。
# 内存分配图
![image](https://github.com/FTOlaf/DolphinOS/blob/master/memory_addr.PNG)
#  内存分配
	0~4kb 页目录表地址，不可以使用
	4~12kb 页表地址，不可以使用，因为定义了0~8mb内存分页映射，所以这里无法使用
	......
	64kb~222kb 内核区域，其他进程无法使用，但是现在内核没有128kb大小，所以在这一区域定义一些表量也不会出问题，但是尽量不要在这一区域定义全局变量
	576kb(0x90000)~640kb 中断描述符表，地址被定义在0x90000，总共64kb大小。
	1088kb(0x110000)栈指针esp的栈顶(现未将中断栈和任务栈分开)。
	5mb(0x500000)读取gdt地址的时候临时使用的一个地址，占用空间大概32bits，4bytes。
	5mb+8kb(0x502000)bitmap的起始地址, bitmap_kernel的起始地址。
	5mb+8kb+0.81kb(0x50233e)bitmap_user的起始地址
	5mb+8kb+1.62kb(0x50267c)kernel
=======
