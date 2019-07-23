#ifndef __BIT_BMP_H_
#define __BIT_BMP_H_
struct bitmap_header//文件头 -->14个字节
{
	unsigned short	type; //文件类型，必须为BM
	unsigned int  size; // 位图文件大小
	unsigned short reserved1; //预留位
	unsigned short reserved2;
	unsigned int offbits; // bmp图像文件头数据偏移量
}__attribute__((packed));//--》忽略该结构体地址对齐

struct bitmap_info//像素头 --》40个字节
{
	unsigned int size; // 本结构大小	
	unsigned int width; //像素点宽度
	unsigned int height; //像素点高度
	unsigned short planes;//目标设备的级别，必须为1

	unsigned short bit_count; // 色深每个像素点所占的位数24bit
	unsigned int compression; //是否压缩，0表示不压缩
	unsigned int size_img; // bmp数据大小，必须是4的整数倍
	unsigned int X_pel;//位图水平分辨率
	unsigned int Y_pel;//位图垂直分辨率
	unsigned int clrused;//位图实际使用的颜色表中的颜色数
	unsigned int clrImportant;//位图显示过程中重要的颜色数
}__attribute__((packed));

#endif
