/*---------------------------------------
*��������:  ���������� 
*�����ߣ�   ��Ƕ������
*����ʱ�䣺 2015,01,01
---------------------------------------
*�޸���־��
*�޸����ݣ�
*�޸��ˣ�
*�޸�ʱ�䣺
----------------------------------------*/
/*************************************************
*ͷ�ļ�
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

#define DEVICE_NAME "beep"			  //�豸����
#define BUZZER_GPIO (PAD_GPIO_C + 14) //�ܽź�

/*************************************************
*���ƺ���
*cmd  ���÷�������״̬ 0 ---on   1----off
*args ���÷������Ĺܽ� 1
*************************************************/
//static long test_ioctl(struct inode *inode, struct file *file, unsigned int  cmd, unsigned long args)

static long test_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{

	if (args != 1)
	{
		printk("the args is out of range 1 \n");
		return -EINVAL;
	}

	switch (cmd)
	{
	case 1:
		gpio_set_value(BUZZER_GPIO, 0); //off���ù̵ܽ�ֵ0
		printk("cmd = 0 , args=%ld\n", args);
		break;
	case 0:
		gpio_set_value(BUZZER_GPIO, 1); //on���ù̵ܽ�ֵ1
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
*�ļ�������
*************************************************/
static const struct file_operations chrdev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = test_ioctl,
};

/*************************************************
*�����豸
*************************************************/
static struct miscdevice buz_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &chrdev_fops,
};

/********************************************************************
*�����ĳ�ʼ������--->���ں���������Դ���ںˡ��жϡ��豸�š���....��
********************************************************************/
static int __init gec6818_buzzer_init(void)
{
	int ret;
	ret = gpio_request(BUZZER_GPIO, "BUZZER"); //gpio�ܽ�����
	if (ret < 0)
	{
		printk("EXYNOS6818_GPX3(5) can not request \n");
		goto fail_gpio_request;
	}

	gpio_direction_output(BUZZER_GPIO, 0); //��GPIO����Ϊ���"0"

	ret = misc_register(&buz_misc); //�Զ������豸�ļ�
	if (ret < 0)
	{
		printk("can not register buz_misc \n");
		goto err_misc_register;
	}

	return 0; //����ע��ɹ�������0������ע��ʧ�ܣ�����һ������������

//��������
err_misc_register:
fail_gpio_request:

	gpio_free(BUZZER_GPIO);
	return ret;
}

/*****************************************************************
*�����˳����� --->���������Դ�����ں�
*****************************************************************/
static void __exit gec6818_buzzer_exit(void)
{
	gpio_free(BUZZER_GPIO);
	misc_deregister(&buz_misc);
	printk("the driver is exiting!\n");
}

module_init(gec6818_buzzer_init); //��������ں��������һ���û��ĳ�ʼ������
module_exit(gec6818_buzzer_exit); //�����ĳ��ں��������һ���û����˳�����

//������������Ϣ�� #modinfo  *.ko , ������������Ϣ�����Ǳ���ġ�
MODULE_AUTHOR("ZOROE@GEC");				//����������
MODULE_DESCRIPTION("Buzzer of driver"); //����������
MODULE_LICENSE("GPL");					//��ѭ��Э��
