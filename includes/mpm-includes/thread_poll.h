/*
  Author:Oscuro1111 (email:sahiljarial1999@gmail.com)
*/
#ifndef ___MPM_THREAD_POLL_LIB___
#define ___MPM_THREAD_POLL_LIB___

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "./circular_queue.h"

typedef struct __thread_poll_worker
{
	pthread_t id;
	uint32_t timeout;

	void (*work_func)(void *);
	char *name;
	void *arg;
} poll_worker;

typedef struct __thread_poll
{
	queue *workers;
	pthread_mutex_t queue_lock;
	volatile __sig_atomic_t current;
	uint32_t max_num;
} _poll;

typedef struct __worker_polls__
{
	pthread_mutex_t lock;
	_poll poll;
	sigset_t mask;
	sigset_t oldmask;
	struct sigaction oact;
} worker_poll;

typedef struct __poll_worker_arg_wrraper
{
	worker_poll *_poll;
	poll_worker *worker;
	int sig_no;
} poll_worker_arg;

typedef struct __sig_return_value
{
	poll_worker *worker;
	worker_poll *poll;
} poll_sig_retvalue;

typedef struct __thread_args__
{
	void *data;
	int rt_val;
} thr_arg;

int put_poll_worker(worker_poll *wrk_poll, poll_worker *wrk);


/*Starting worker thread.*/
int start_worker(worker_poll *wrk_poll, poll_worker *wrk);

/*Get worker thread from pool.
if all thread workers are busy then it ,
block the process and wait for thread any thread,
to finish get back and free worker*/
poll_worker *get_worker_poll(worker_poll *wrk_poll);


//initialize worker queue for use.
int worker_poll_init(worker_poll *poll,const uint32_t MAX_NUM_WRK_THREADS);

/*
 allocate worker poll.
*/
worker_poll *allocate_wrk_poll();

/*
  Free worker poll data structure.
  It will wait for runing thread before freeing and free resources and locks.
  calling and using it after that will cause SIGFAULTS.
*/
void wrk_poll_free(worker_poll *wrk_poll);
#endif