#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "thread_pool.h"

void *do_task(void *arg);
int basename(char **name,char *path);
int pathcat(char **dest, const char *arg1, const char *arg2);
int scan_dir(thread_pool *pool, char *src, const char *dest);
