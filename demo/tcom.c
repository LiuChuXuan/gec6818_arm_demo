/** 
 *  练习:线程间通信
 *  创建两个子线程，将数据从一个子线程发到另外一个子线程
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_buf
{
    char *buf;             //内存地址
    int size;              //内存大小
    pthread_mutex_t mutex; //互斥锁变量
} t_buf, *t_buf_t;

void *func_t1(void *buffer);
void *func_t2(void *buffer);

int main(void)
{
    t_buf thread_buffer;
    memset(&thread_buffer, 0, sizeof(thread_buffer));

    //创建一块共同存放信息的内存
    thread_buffer.buf = (char *)malloc(20 * sizeof(char));
    memset(thread_buffer.buf, 0, 20 * sizeof(char));
    thread_buffer.size = 20 * sizeof(char);

    //初始化互斥锁
    pthread_mutex_init(&(thread_buffer.mutex), NULL);

    //创建两个线程id号
    pthread_t tid_1, tid_2;

    //创建线程,将线程id号传进去，将函数指针传进去，将共享的内存地址传进去
    pthread_create(&tid_1, NULL, func_t1, (void *)(&thread_buffer));
    pthread_create(&tid_2, NULL, func_t2, (void *)(&thread_buffer));

    pthread_exit(0);
}
void *func_t1(void *buffer)
{
    //上锁
    pthread_mutex_lock(&(((t_buf_t)buffer)->mutex));

    //接收键盘数据
    fgets(((t_buf_t)buffer)->buf, 20, stdin);

    //解锁
    pthread_mutex_unlock(&(((t_buf_t)buffer)->mutex));
    return NULL;
}
void *func_t2(void *buffer)
{
    //上锁
    pthread_mutex_lock(&(((t_buf_t)buffer)->mutex));

    //输出内存中的数据到屏幕上
    printf("hello ! %s\n", (char *)(((t_buf_t)buffer)->buf));

    //释放堆空间内存
    free(((t_buf_t)buffer)->buf);

    //解锁
    pthread_mutex_unlock(&(((t_buf_t)buffer)->mutex));
    return NULL;
}