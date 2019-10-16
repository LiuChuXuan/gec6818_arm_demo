

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "thread_pool.h"

void *do_task(void *arg);

void handle(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
	pthread_mutex_unlock(mutex);
}

//所有的线程要做的事情
void *routine(void *arg)
{

	thread_pool *pool = (thread_pool *)arg;

	while (1) //执行完任务继续取任务执行
	{
		struct task *tmp = NULL;
		//0、注册线程取消处理函数--》他是一个带参宏
		pthread_cleanup_push(handle, (void *)&pool->lock);

		//1、上锁
		pthread_mutex_lock(&pool->lock);

		//2、如果任务链表为空，并且线程池不关闭就睡眠等待
		while (pool->waiting_tasks == 0 && pool->shutdown == false)
		{
			//线程被唤醒，开始上锁，如果上锁成功，就退出阻塞
			printf("pthread_cond_wait： [%lu]\n", pthread_self());
			pthread_cond_wait(&pool->cond, &pool->lock);
		}

		//3、如果任务链表为空，并且线程池关闭，就解锁退出线程
		if (pool->waiting_tasks == 0 && pool->shutdown == true)
		{
			pthread_mutex_unlock(&pool->lock);

			pthread_exit(NULL);
		}

		//4、删除节点，更新线程池正在等待执行任务个数
		tmp = pool->task_list->next;
		pool->task_list->next = tmp->next;
		tmp->next = NULL;

		pool->waiting_tasks--;

		//5、解锁
		pthread_mutex_unlock(&pool->lock);

		//6、清理线程取消处理函数
		pthread_cleanup_pop(0); //写0直接清理线程取消处理函数，非0先执行线程取消处理函数，再清理

		//7、设置线程不可取消
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		//struct task *tmp = p;

		//8、执行任务，任务节点内存释放
		(tmp->do_task)(tmp->arg); //-->调用任务函数，通过函数指针（地址）
		//任务链表的节点
		/* struct task
	{
		void *(*do_task)(void *arg);
		void *arg;
		struct task *next;

	}; */

		free(tmp);
		//9、设置线程可以取消
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}
	pthread_exit(NULL);
}

//初始化线程池			线程池地址			开始时创建多少个线程
bool init_pool(thread_pool *pool, unsigned int threads_number)
{
	if (pool == NULL || threads_number < 1)
	{
		return false;
	}

	//1、初始化互斥锁和条件变量
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);

	//2、初始化线程池关闭标志、正在等待执行任务个数
	pool->shutdown = false;
	pool->waiting_tasks = 0;

	//3、新建任务链表头节点,申请内存保存线程id号
	pool->task_list = calloc(1, sizeof(struct task));
	pool->task_list->next = NULL;

	pool->tids = calloc(1, MAX_ACTIVE_THREADS * sizeof(pthread_t));
	if (pool->tids == NULL || pool->task_list == NULL)
	{
		perror("calloc failed!");
		return false;
	}

	int i;
	int ret;
	//4、创建线程，并保存线程id号 --5 ---第3个出错
	for (i = 0; i < threads_number; i++)
	{
		ret = pthread_create(&pool->tids[i], NULL, routine, (void *)pool);
		if (ret != 0)
		{
			printf("pthread_create failed:%s\n", strerror(ret));
			break;
		}
	}
	//一个线程都未创建成功
	if (i == 0)
	{
		free(pool->task_list);
		free(pool->tids);
		return false;
	}

	//5、初始化正在活跃的线程个数
	pool->active_threads = i;

	return true;
}

// 添加任务--》把任务加入任务链表    线程池地址   要执行函数的地址  要传递的参数
bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *arg)
{
	if (pool == NULL || do_task == NULL)
	{
		printf("add task failed: args errors\n");
		return false;
	}

	//1、新建任务节点，把任务内容插入节点
	struct task *new = calloc(1, sizeof(struct task));
	if (new == NULL)
	{
		perror("calloc failed!");
		return false;
	}
	new->do_task = do_task;
	new->arg = arg;
	new->next = NULL;

	//2、上互斥锁，访问任务链表
	pthread_mutex_lock(&pool->lock);

	//判断正在等待执行的任务是否会溢出
	if (pool->waiting_tasks == MAX_WAIT_TASKS)
	{
		pthread_mutex_unlock(&pool->lock);
		free(new);
		return false;
	}

	struct task *p = pool->task_list;

	//3、找到最后一个节点，把任务节点插入链表末尾
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = new;

	//4、更新正在等待执行的任务个数
	pool->waiting_tasks++;

	//5、解锁
	pthread_mutex_unlock(&pool->lock);

	//6、唤醒一个线程执行任务
	pthread_cond_signal(&pool->cond);

	return true;
}

//增加线程池里面的线程   线程池地址            要增加的线程个数
int add_thread(thread_pool *pool, unsigned int additional_threads)
{
	//判断参数传递是否出错
	if (pool == NULL || additional_threads == 0)
	{
		printf("add_thread failed: args errors\n");
		return -1;
	}
	//判断线程池里面活跃的线程个数是否已满
	if (pool->active_threads == MAX_ACTIVE_THREADS)
	{

		printf("active_threads is full\n");
		return -1;
	}

	//active_threads == 15 ,  要再创建 10个线程， 最多只能创建5个
	//if(15+10 >= 20)			20-15					5

	//active_threads == 15 ,  要再创建 3个线程， 最多只能创建3个
	//if(15+3 < 20)						3

	//当要创建的线程个数+当前活跃的线程个数 大于或等于 最大的线程活跃个时，计算最多要创建的线程个数
	if (additional_threads + pool->active_threads >= MAX_ACTIVE_THREADS)
	{

		additional_threads = MAX_ACTIVE_THREADS - pool->active_threads;
	}

	int i;
	int ret;
	//4、创建线程，并保存线程id号
	//		3							3+2 == 5
	//			3  4

	int add_num_pthread = 0;
	//			15					15+3
	for (i = pool->active_threads; i < pool->active_threads + additional_threads; i++)
	{
		ret = pthread_create(&pool->tids[i], NULL, routine, (void *)pool);
		if (ret != 0) //只要一个创建线程失败，就不在继续创建，直接退出该函数
		{
			printf("pthread_create failed:%s\n", strerror(ret));

			break;
		}
		else if (ret == 0) //创建线程成功
		{
			printf("pthread_cancel success:[%lu]\n", pool->tids[i]);
		}
	}
	//实际增加的线程个数
	add_num_pthread = i - pool->active_threads;
	//更新当前线程活跃的个数
	pool->active_threads = i;

	return add_num_pthread;
}

//删除线程		 线程池地址 			要删除的线程个数
int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	if (pool == NULL || removing_threads == 0)
	{
		printf("remove_thread failed: args errors\n");
		return -1;
	}
	if (pool->active_threads == 1)
	{
		printf("remove_thread only 1: can not delete\n");
		return -1;
	}

	//active_threads == 3 ,  要删除 4个线程， 最多删除2个
	//if(3 <= 4)								3-1 = 2

	//active_threads == 3 ,  要删除 2个线程， 最多只能创建2个
	//if(3 > 2) 									2

	//要删除的线程个数大于当前活跃的线程个数，计算最多删除的线程的个数

	if (pool->active_threads <= removing_threads)
	{
		//2
		removing_threads = pool->active_threads - 1;
	}

	int i;
	int ret;
	//			3-1								2-2
	//			3-1								2-2
	for (i = pool->active_threads - 1; i > pool->active_threads - 1 - removing_threads; i--)
	{
		ret = pthread_cancel(pool->tids[i]);
		if (ret != 0)
		{
			printf("pthread_cancel failed:[%lu]!\n", pool->tids[i]);

			break;
		}
		else
		{
			printf("pthread_cancel success:[%lu]!\n", pool->tids[i]);
		}
	}
	//更新当前线程活跃个数
	pool->active_threads = i + 1;

	return pool->active_threads;
}

//销毁线程池
bool destroy_pool(thread_pool *pool)
{

	if (pool == NULL)
	{
		printf("destroy_thread failed: args errors\n");
		return -1;
	}
	//（1）更新线程池关闭标志
	pool->shutdown = true;

	//（2）唤醒线程
	pthread_cond_broadcast(&pool->cond);
	int i;
	int ret;
	int flag = pool->active_threads;
	for (i = flag - 1; i >= 0; i--)
	{
		ret = pthread_join(pool->tids[i], NULL);
		if (ret != 0)
		{
			printf("pthread_join failed[%lu]!\n", pool->tids[i]);
		}
		else
		{
			pool->active_threads--; //更新活跃的线程个数
			printf("pthread_join success:[%lu]!\n", pool->tids[i]);
		}
	}

	free(pool->tids);
	free(pool->task_list);
	free(pool);
}