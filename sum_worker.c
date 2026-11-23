#include <pthread.h>

#include "tables.h"
#include "sum_date.h"
#include "sum_worker.h"
#include "worker_wait.h"

void *sum_worker(void *arg) {
    struct ThreadArgs *args = arg;
    int i = 1;
    while (1) {
        pthread_mutex_lock(args->mutex);
        if (args->stop_requested) {
            pthread_mutex_unlock(args->mutex);
            return NULL;
        }
        if (args->s->is_ready == false && args->sum_paused == false) {
            args->s->sum = sum_date();
            args->s->id=i++;
            args->s->is_ready = true;
            args->p->is_ready = false;
        }
        int delay_ms = args->sum_delay_ms; // текущая задержка потока
        pthread_mutex_unlock(args->mutex);

        worker_wait(args, delay_ms);
    }
    return NULL;
}
