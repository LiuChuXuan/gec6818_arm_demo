# 控制LED灯的亮灭和蜂鸣器叫  
1.编译gec6818_beep.c和led.c文件，生成驱动程序gec6816_beep.ko 和 led.ko  
2.然后安装驱动(.ko)文件   
3.最后编译led_beep.c，生成可执行程序 

## 安装驱动：
> insmod led.ko  
> insmod gec6818_beep.ko  

如果驱动已经存在则需要先卸载原先驱动  
[root@GEC6818 /193]#insmod led.ko  ---》驱动已存在，不能安装  
insmod: can't insert 'led.ko': File exists  

## 卸载驱动：
> rmmod led  
> rmmod gec6818_beep  

## 查看驱动：
> lsmod  
	
	
[root@GEC6818 /193]#insmod led.ko  ---》驱动已存在，不能安装  
insmod: can't insert 'led.ko': File exists  
