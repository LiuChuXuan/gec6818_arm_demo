#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#define MAX_ACTIVE_THREADS 5
#define MAX_WAIT_TASKS 30

//任务链表的节点
struct task
{
	void *(*do_task)(void *arg);
	void *arg;
	struct task *next;
};
//线程池
typedef struct Thread_pool
{
	pthread_mutex_t lock;		 // 互斥锁，保护任务队列
	pthread_cond_t cond;		 // 条件变量，同步所有线程
	bool shutdown;				 // 线程池销毁标记
	struct task *task_list;		 // 任务链队列指针
	pthread_t *tids;			 // 线程ID存放位置
	unsigned int waiting_tasks;  // 任务链队列中等待的任务个数
	unsigned int active_threads; // 当前活跃线程个数

} thread_pool;

//初始化线程池			线程池地址			开始时创建多少个线程
bool init_pool(thread_pool *pool, unsigned int threads_number);

// 添加任务--》把任务加入任务链表    线程池地址   要执行函数的地址  要传递的参数
bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *arg);

//增加线程池里面的线程   线程池地址            要增加的线程个数
int add_thread(thread_pool *pool, unsigned int additional_threads);

//删除线程		 线程池地址 			要删除的线程个数
int remove_thread(thread_pool *pool, unsigned int removing_threads);
void *routine(void *arg);

//销毁线程池
bool destroy_pool(thread_pool *pool);

#endif
