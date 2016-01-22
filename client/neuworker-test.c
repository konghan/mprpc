
#include "neuworker.h"

#include "logger.h"

#include <pthread.h>
#include <semaphore.h>

/*
 * dispatch task test
 */
struct test_task{
    sem_t lock;
};

static neuworker_t  gtask_nw;
static pthread_t    gtask_th;
static struct test_task  gtask_tt;
    
static void test_dispatch_task_routine(struct neuworker *nw, void *data){
    struct test_task *tt = (struct test_task *)data;
    static int count = 0;
    
    log_info("========do task %d============\n", count++);

    sem_post(&tt->lock);
}

static void *neuworker_thread_routine(void *arg){
    neuworker_t *nw = (neuworker_t *)arg;

    log_info("thread is running...\n");

    neuworker_dispatch(nw);

    return NULL;
}


int neuworker_test_dispatch_task(){
    neutask_t       nt;
    struct timespec ts;

    neuworker_init(&gtask_nw, NULL);
    
    sem_init(&gtask_tt.lock, 0, 0);
    nt.data = &gtask_tt;
    nt.dotask = test_dispatch_task_routine;

    pthread_create(&gtask_th, NULL, neuworker_thread_routine, &gtask_nw);

    if(neuworker_dispatch_task(&gtask_nw, &nt) != 0){
        log_warn("dispatch task fail!\n");
        pthread_cancel(gtask_th);
        return -1;
    }
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 1;

    if(sem_timedwait(&gtask_tt.lock, &ts) != 0){
        log_warn("task not doing\n");
        pthread_cancel(gtask_th);
        return -1;
    }

    pthread_cancel(gtask_th);

    return 0;
}

/*
 * neuworker client & server test
 */

static neuworker_t gcs_server;
static pthread_t   gcs_thserver;
static neuworker_t gcs_client;
static pthread_t   gcs_thclient;

static int neuworker_test_cs_init_server(){
    neuworker_init(&gcs_server, NULL);
    pthread_create(&gcs_thserver, NULL, neuworker_thread_routine, &gcs_server);

    // init sever
    return 0;
}

static int neuworker_test_cs_init_client(){    
    neuworker_init(&gcs_client, NULL);
    pthread_create(&gcs_thclient, NULL, neuworker_thread_routine, &gcs_client);

    // init client

    return 0;
}

static int neuworker_test_cs_run(int conns, int msgs){
    return 0;
}

/*
int neuworker_watch_event(neuworker_t *nw, neuevent_t *ev){
    return dispatch_event_task(nw, ev, watch_event_routine);

int neuworker_close_event(neuworker_t *nw, neuevent_t *ev){
    return dispatch_event_task(nw, ev, close_event_routine);
}
*/

int main(){

    logger_init(NULL, "127.0.0.1", 4040, 0);
    
    neuworker_test_dispatch_task();

//    sleep(10);

    return 0;
}

