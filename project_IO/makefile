ALL_C = \
./main.c \
\
./picture_viewer/source/scan_picture.c \
./picture_viewer/source/picture_viewer.c \
\
./2048/source/game_2048.c \
./2048/source/list_2048.c \
./2048/source/imshow_2048.c \
\
./list/list.c \
\
./led_beep/led_beep.c \
\
./screen_io/source/input_sc.c \
./screen_io/source/print_sc.c \
./screen_io/source/show_bmp.c 


2048_h = ./2048/include 
picture_h = ./picture_viewer/include 
list_h = ./list
screen_h = ./screen_io/include

CC = arm-linux-gcc
#CC = gcc

a.out :  $(ALL_C) $(picture_h) $(2048_h) $(list_h) $(screen_h)
	$(CC) $(ALL_C)  -I $(picture_h) -I $(2048_h) -I $(list_h) -I $(screen_h) -o a.out