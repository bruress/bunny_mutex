#include <pthread.h>
#include <threads.h>
#include <unistd.h>
#include <stdio.h>

#include "tables.h"
#include "sum_date.h"
#include "sum_worker.h"

void *sum_worker(void *arg) {
    struct ThreadArgs *args = arg;
    while (1) {
        pthread_mutex_lock(args->mutex);
        if (args->s->is_ready == false && args->sum_paused == false) {
            args->s->sum = sum_date();
            args->s->is_ready = true;
            args->p->is_ready = false;
            printf("Сумма: %d\n", args->s->sum);
        }
        pthread_mutex_unlock(args->mutex);
        sleep(1);
    }
    return NULL;
}
