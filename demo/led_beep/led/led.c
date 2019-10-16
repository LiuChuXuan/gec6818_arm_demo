/*---------------------------------------
*��������:  ʵ��LED������
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <cfg_type.h>

#define DEVICE_NAME "Led" //�����豸����

//����Led�Ľṹ�壬�����ܽź�����
struct led
{
	int gpio;
	char *name;
};

//����Led�Ĺܽź�����
static struct led led_gpios[] = {
	{PAD_GPIO_B + 26, "led1"},
	{PAD_GPIO_C + 11, "led2"},
	{PAD_GPIO_C + 7, "led3"},
	{PAD_GPIO_C + 12, "led4"},
};

#define LED_NUM 4	  //ARRAY_SIZE(led_gpios)
#define TEST_MAX_NR 4  //����������������
#define TEST_MAGIC 'x' //�������

#define LED1 _IO(TEST_MAGIC, 0)
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3)

/*************************************************
*led_open����
*************************************************/
static int led_open(struct inode *inode, struct file *filp)
{
	printk(DEVICE_NAME ":open\n");
	return 0;
}

/*************************************************
*LED���ƺ���
*************************************************/
static long gec5260_leds_ioctl(struct file *filp, unsigned int cmd,
							   unsigned long arg)
{
#if 1
	printk("led_num = %d \n", LED_NUM);
	if (_IOC_TYPE(cmd) != TEST_MAGIC)
		return -EINVAL;
	if (_IOC_NR(cmd) > TEST_MAX_NR)
		return -EINVAL;

	gpio_set_value(led_gpios[_IOC_NR(cmd)].gpio, arg);
	printk(DEVICE_NAME ": %d %lu\n", _IOC_NR(cmd), arg);

#endif
	//printk("xxxx %lu, %d xxxxx \n", cmd, arg);
	return 0;
}

/*************************************************
*�ļ�������
*************************************************/
static struct file_operations gec5260_led_dev_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.unlocked_ioctl = gec5260_leds_ioctl,
};

/*************************************************
*�����豸
*************************************************/
static struct miscdevice gec5260_led_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &gec5260_led_dev_fops,
};

/********************************************************************
*�����ĳ�ʼ������--->���ں���������Դ���ںˡ��жϡ��豸�š���....��
********************************************************************/
static int __init gec5260_led_dev_init(void)
{
	int ret, i;

	for (i = 0; i < LED_NUM; i++)
	{
		ret = gpio_request(led_gpios[i].gpio, led_gpios[i].name); //io����
		if (ret)												  //ʧ�����ӡ���Ǹ��ܽ�����ʧ��
		{
			printk("%s: request GPIO %d for LED failed, ret = %d\n", led_gpios[i].name, led_gpios[i].gpio, ret);
			return ret;
		}
		gpio_direction_output(led_gpios[i].gpio, 0); //����ioΪ����ܽ�
	}

	ret = misc_register(&gec5260_led_dev); //�����豸����
	printk(DEVICE_NAME "\tinitialized\n"); //��ʾ����ɹ�
	return ret;
}

/*****************************************************************
*�����˳����� --->���������Դ�����ں�
*****************************************************************/
static void __exit gec5260_led_dev_exit(void)
{
	int i;
	//�ͷŹܽ�
	for (i = 0; i < LED_NUM; i++)
	{
		gpio_free(led_gpios[i].gpio);
	}
	//�Ƴ������豸
	misc_deregister(&gec5260_led_dev);
}

module_init(gec5260_led_dev_init); //��������ں��������һ���û��ĳ�ʼ������
module_exit(gec5260_led_dev_exit); //�����ĳ��ں��������һ���û����˳�����

/***************************************************************
*������������Ϣ�� #modinfo  *.ko , ������������Ϣ�����Ǳ���ġ�
***************************************************************/
MODULE_AUTHOR("ZOROE@GEC");				 //����������
MODULE_DESCRIPTION("the LED of driver"); //����������
MODULE_LICENSE("GPL");					 //��ѭ��Э��
