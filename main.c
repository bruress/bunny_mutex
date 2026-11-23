#include <stdbool.h>
#include <pthread.h> // для мьютекса

#include "tables.h"
#include "parity_worker.h"
#include "sum_worker.h"
#include "gui.h"

int main(void) {
    // инициализация структур и полей
    struct SumTable sum_table = {0, false};
    struct ParityTable parity_table = {false, false};

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // инициализация мьютекса

    // сразу задаем значения полей
    struct ThreadArgs args = {
        .s = &sum_table,
        .p = &parity_table,
        .mutex = &mutex,
        .parity_paused = false,
        .sum_paused = false
    };

    pthread_t t1, t2;

    pthread_create(&t1, NULL, sum_worker, &args);
    pthread_create(&t2, NULL, parity_worker, &args);

    gui(&args);

    pthread_join(t1,NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}
