/*---------------------------------------
*功能描述:  蜂鸣器驱动 
*创建者：   粤嵌技术部
*创建时间： 2015,01,01
---------------------------------------
*修改日志：
*修改内容：
*修改人：
*修改时间：
----------------------------------------*/
/*************************************************
*头文件
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <cfg_type.h>

#define DEVICE_NAME     "beep"                    //设备名字
#define  BUZZER_GPIO    (PAD_GPIO_C + 14)       //管脚号


/*************************************************
*控制函数
*cmd  设置蜂鸣器的状态 0 ---on   1----off
*args 设置蜂鸣器的管教 1
*************************************************/
//static long test_ioctl(struct inode *inode, struct file *file, unsigned int  cmd, unsigned long args)

static long test_ioctl(struct file *file, unsigned int  cmd, unsigned long args)
{

	if(args!=1) {
		printk("the args is out of range 1 \n");
		return -EINVAL;
	}

	switch(cmd){
		case 1:
			gpio_set_value(BUZZER_GPIO, 0);           //off设置管教的值0
			printk("cmd = 0 , args=%ld\n", args);
			break;
		case 0:
			gpio_set_value(BUZZER_GPIO, 1) ;          //on设置管教的值1
			printk("cmd = 1 , args=%ld\n", args);
			break;
		default:
			printk("the cmd is out of range(0,1) \n"); 
			return -EINVAL;
			break;
	}
	return 0;
}

/*************************************************
*文件操作集
*************************************************/
static const struct file_operations chrdev_fops = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl = test_ioctl,
};

/*************************************************
*杂项设备
*************************************************/
static struct miscdevice buz_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &chrdev_fops,
};

/********************************************************************
*驱动的初始化函数--->从内核中申请资源（内核、中断、设备号、锁....）
********************************************************************/
static int __init gec6818_buzzer_init(void) 
{
	int ret;
	ret = gpio_request( BUZZER_GPIO , "BUZZER");         //gpio管脚申请
	if(ret <0) {
		printk("EXYNOS6818_GPX3(5) can not request \n" );
		goto fail_gpio_request;
	}

	gpio_direction_output( BUZZER_GPIO, 0);       //将GPIO设置为输出"0"
	
	ret = misc_register(&buz_misc);                      //自动生成设备文件
	if(ret <0){
		printk("can not register buz_misc \n");
		goto err_misc_register;
	}
	
	return 0;                                           //驱动注册成功，返回0；驱动注册失败，返回一个负数错误码

//出错处理
err_misc_register:	
fail_gpio_request:

	gpio_free(BUZZER_GPIO);		
	return  ret;

}

/*****************************************************************
*驱动退出函数 --->将申请的资源还给内核
*****************************************************************/
static void __exit gec6818_buzzer_exit(void)            
{
	gpio_free(BUZZER_GPIO);
	misc_deregister(&buz_misc);
	printk("the driver is exiting!\n");
}

module_init(gec6818_buzzer_init);             //驱动的入口函数会调用一个用户的初始化函数
module_exit(gec6818_buzzer_exit);             //驱动的出口函数会调用一个用户的退出函数


//驱动的描述信息： #modinfo  *.ko , 驱动的描述信息并不是必需的。
MODULE_AUTHOR("ZOROE@GEC");                   //驱动的作者
MODULE_DESCRIPTION("Buzzer of driver");       //驱动的描述
MODULE_LICENSE("GPL");                        //遵循的协议


