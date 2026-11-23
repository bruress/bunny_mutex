#include <stdbool.h>
#include <pthread.h> // для мьютекса

#include "tables.h"
#include "parity_worker.h"
#include "sum_worker.h"
#include "gui.h"

int main(void) {
    // инициализация структур и полей
    struct SumTable sum_table = {0, 0, false};
    struct ParityTable parity_table = {0, false, false};

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // инициализация мьютекса

    // сразу задаем значения полей
    struct ThreadArgs args = {
        .s = &sum_table,
        .p = &parity_table,
        .mutex = &mutex,
        .parity_paused = false,
        .sum_paused = false,
        .sum_delay_ms = 1000,
        .parity_delay_ms = 1000,
        .stop_requested = false
    };

    pthread_t t1, t2;

    pthread_create(&t1, NULL, sum_worker, &args);
    pthread_create(&t2, NULL, parity_worker, &args);

    gui(&args);

    pthread_mutex_lock(&mutex);
    args.stop_requested = true; // остановка потоков после закрытия окна
    pthread_mutex_unlock(&mutex);

    pthread_join(t1,NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}
