#ifndef _SCAN_PICTURE_H_
#define _SCAN_PICTURE_H_

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bit_bmp.h"
#include "list.h"

int is_bmp(char *path);
int scan_picture(char *path, node_t head);

#endif