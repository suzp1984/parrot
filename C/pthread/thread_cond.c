#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>

//static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
// 这里虽然是 P/V ，但是用 cond 确实更方便。
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int i = 0;

void get ()
{
  pthread_mutex_lock (&mutex);
  while ( i == 0 ) // 队列下限
    pthread_cond_wait (&cond, &mutex); // 唤醒其它线程进行检测。

  --i;
  pthread_cond_signal (&cond);
  printf ("Current size: %d\n", i);

  pthread_mutex_unlock (&mutex);
}

void put ()
{
  pthread_mutex_lock (&mutex); 
  while ( i == 3 ) // 队列上限
    pthread_cond_wait (&cond, &mutex);

  ++i;
  pthread_cond_signal (&cond); // 唤醒其它线程
  printf ("Now size: %d\n", i);

  pthread_mutex_unlock (&mutex);
}

void *thf ( void *arg )
{
  while ( 1 )
    {
      sleep(2);
      put ();
    }
}

int main ()
{
  pthread_t tid;
  pthread_create (&tid, NULL, thf, NULL);

  sleep (3);
  while ( 1 )
    get ();
}
