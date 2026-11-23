#include "tables.h"
#include "parity_sum_date.h"
#include "parity_worker.h"
#include "worker_wait.h"

void *parity_worker(void *arg) {
    struct ThreadArgs *args = arg; // получаем указатель на структуру
    while(1) {
        pthread_mutex_lock(args->mutex);
        if (args->stop_requested) {
            pthread_mutex_unlock(args->mutex);
            return NULL;
        }
        if (args->s->is_ready == true && args->parity_paused == false) {
            args->p->is_parity = sum_date_is_bool(args->s->sum);
            args->p->id=args->s->id;
            args->s->is_ready = false;
            args->p->is_ready = true;
        }
        int delay_ms = args->parity_delay_ms; // текущая задержка потока
        pthread_mutex_unlock(args->mutex);

        worker_wait(args, delay_ms);
    }
    return NULL;
}
