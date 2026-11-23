#include <unistd.h>
#include <stdio.h>

#include "tables.h"
#include "parity_sum_date.h"
#include "parity_worker.h"

void *parity_worker(void *arg) {
    struct ThreadArgs *args = arg; // получаем указатель на структуру
    while(1) {
        pthread_mutex_lock(args->mutex);
        if (args->s->is_ready == true && args->parity_paused == false) {
            args->p->is_parity = sum_date_is_bool(args->s->sum);
            args->p->id=args->s->id;
            args->s->is_ready = false;
            args->p->is_ready = true;
            printf("%s\n", args->p->is_parity ? "Сумма четная" : "Сумма нечетная");
        }
        pthread_mutex_unlock(args->mutex);
        sleep(1);
    }
    return NULL;
}
