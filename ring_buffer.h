/**@brief 仿照linux kfifo写的ring buffer
 *@atuher Anker  date:2013-12-18
* ring_buffer.h
 * */

#ifndef KFIFO_HEADER_H
#define KFIFO_HEADER_H
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct ring_buffer
{
    char         *buffer;     //缓冲区
    uint32_t     size;       //大小
    uint32_t     in;         //入口位置
    uint32_t       out;        //出口位置
    pthread_mutex_t *f_lock;    //互斥锁
};
//初始化缓冲区
struct ring_buffer* ring_buffer_init(char *buffer, uint32_t size, pthread_mutex_t *f_lock);
//释放缓冲区
void ring_buffer_free(struct ring_buffer *ring_buf);
uint32_t ring_buffer_avail_len(const struct ring_buffer *ring_buf);
uint32_t ring_buffer_get(struct ring_buffer *ring_buf, char *buffer, uint32_t size);
uint32_t ring_buffer_put(struct ring_buffer *ring_buf, char *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
