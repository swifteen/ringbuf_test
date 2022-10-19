/**@brief ring buffer测试程序，创建两个线程，一个生产者，一个消费者。
 * 生产者每隔1秒向buffer中投入数据，消费者每隔2秒去取数据。
 *@atuher Anker  date:2013-12-18
 * */
#include "ringbuf.h"
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#define DEBUG

#if 0
#define DEBUG_LOG(format,...) do{\
    printf("{File:[%s],Func:[%s],Line:[%d] ",__FILE__,__FUNCTION__,__LINE__);\
    printf(format,##__VA_ARGS__);
}while(0)
#else
#define DEBUG_LOG(format,...)
#endif

typedef struct student_info
{
    uint64_t stu_id;
    uint32_t age;
    uint32_t score;
}student_info;
static uint64_t g_stu_id = 0;
static FILE* s_fp_consumer = NULL;
static FILE* s_fp_producer = NULL;

void print_student_info(const student_info *stu_info)
{
    assert(stu_info);
    DEBUG_LOG("id:%lu\t",stu_info->stu_id);
    DEBUG_LOG("age:%u\t",stu_info->age);
    DEBUG_LOG("score:%u\n",stu_info->score);
}

student_info * get_student_info(time_t timer,student_info *stu_info)
{
    srand(timer);
    stu_info->stu_id = g_stu_id++;
    stu_info->age = rand() % 30;
    stu_info->score = rand() % 101;
    print_student_info(stu_info);
    return stu_info;
}

void * consumer_proc(void *arg)
{
    struct ringbuf_t *ring_buf = (struct ringbuf_t *)arg;
    student_info stu_info;
    while(1)
    {
	    if(ringbuf_len(ring_buf) > 0)
	    {
				DEBUG_LOG("consumer------------------------------------------\n");
				DEBUG_LOG("get a student info from ring buffer[%d].\n",sizeof(student_info));
				unsigned int ret = ringbuf_out(ring_buf, &stu_info, 1);
				if(ret != 1)
				{
					DEBUG_LOG("------------------------------------------ring buffer empty ,length: %u\n", ringbuf_avail(ring_buf));
					usleep(1000);
				}
				else
				{
#ifdef DEBUG
					ret = fwrite(&stu_info, sizeof(student_info), 1, s_fp_consumer);
					if(ret != 1)
					{
						DEBUG_LOG("------------------------------------------s_fp_consumer write failed,ret[%d]errno[%d]\n\n",ret,errno);
					}
#endif
					DEBUG_LOG("ring buffer length: %u\n", ringbuf_avail(ring_buf));
					print_student_info(&stu_info);
					DEBUG_LOG("------------------------------------------ok\n\n");
				}
		}
		else
		{
			usleep(1000);
		}
    }
    return (void *)ring_buf;
}

void * producer_proc(void *arg)
{
    time_t cur_time;
    struct ringbuf_t *ring_buf = (struct ringbuf_t *)arg;
    while(1)
    {
    	if(ringbuf_avail(ring_buf) > 0)
    	{
				DEBUG_LOG("producer******************************************\n");
				time(&cur_time);
				srand(cur_time);
				int seed = rand() % 11111;
				student_info stu_info;
				get_student_info(cur_time + seed,&stu_info);
				DEBUG_LOG("put a student info to ring buffer[%d].\n",sizeof(student_info));
				unsigned int ret = ringbuf_in(ring_buf, &stu_info, 1);
				if(ret != 1)
				{
					DEBUG_LOG("******************************************ring buffer full ,length: %u\n", ringbuf_avail(ring_buf));
					usleep(1000);
				}
				else
				{
#ifdef DEBUG
					ret = fwrite(&stu_info, sizeof(student_info), 1, s_fp_producer);
					if(ret != 1)
					{
						DEBUG_LOG("******************************************s_fp_producer write failed,ret[%d]errno[%d]\n\n",ret,errno);
					}
#endif
					DEBUG_LOG("ring buffer length: %u\n", ringbuf_avail(ring_buf));
					DEBUG_LOG("******************************************ok\n\n");
				}
		}
		else
		{
			usleep(1000);
		}
    }
    return (void *)ring_buf;
}

int consumer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, consumer_proc, arg);
    if (err != 0)
    {
	    fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
	        errno, strerror(errno));
	    return -1;
    }
    return tid;
}
int producer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, producer_proc, arg);
    if (err != 0)
    {
	    fprintf(stderr, "Failed to create producer thread.errno:%u, reason:%s\n",
	        errno, strerror(errno));
	    return -1;
    }
    return tid;
}


int main()
{
    pthread_t consume_pid, produce_pid;
	const int MAX_STUDENT_NUM = 10;
	student_info stuArray[MAX_STUDENT_NUM];
    struct ringbuf_t ring_buf;
	ringbuf_init(&ring_buf,sizeof(student_info), stuArray, sizeof(stuArray));
#ifdef DEBUG
    if((s_fp_consumer = fopen("/mnt/udisk/consumer","w")) == NULL)
    {
        printf("\n Can't open s_fp_consumer\n");
        return -1;
    }
    if((s_fp_producer = fopen("/mnt/udisk/producer","w")) == NULL)
    {
        printf("\n Can't open s_fp_producer\n");
        return -1;
    }

#endif
	
    printf("multi thread test.......\n");
    produce_pid  = producer_thread((void*)&ring_buf);
    consume_pid  = consumer_thread((void*)&ring_buf);
    pthread_join(produce_pid, NULL);
    pthread_join(consume_pid, NULL);
	
    fclose(s_fp_consumer);
    fclose(s_fp_producer);
    return 0;
}
