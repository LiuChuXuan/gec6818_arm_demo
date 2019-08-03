ALL_C = \
./main.c \
\
./2048/source/*.c \
\
./led_beep/led_beep.c \
\
./list/list.c \
\
./picture_viewer/source/*.c \
\
./screen_io/source/*.c 

2048_h = ./2048/include 
picture_h = ./picture_viewer/include 
list_h = ./list
screen_h = ./screen_io/include

CC = arm-linux-gcc
#CC = gcc

a.out :  $(ALL_C) $(picture_h) $(2048_h) $(list_h) $(screen_h)
	$(CC) $(ALL_C)  -I $(picture_h) -I $(2048_h) -I $(list_h) -I $(screen_h) -o a.out