#ifndef TABLES_H
#define TABLES_H

#include <stdbool.h>
#include <pthread.h>

struct SumTable {
    int id;
    int sum;
    bool is_ready;
};

struct ParityTable {
    int id;
    bool is_parity;
    bool is_ready;
};

struct ThreadArgs {
    struct SumTable *s;
    struct ParityTable *p;
    pthread_mutex_t *mutex; // т.к мы его уже создали!
    bool sum_paused;
    bool parity_paused;
    int sum_delay_ms;       // задержка потока суммы
    int parity_delay_ms;    // задержка потока четности
    bool stop_requested;    // завершение работы потоков
};

#endif
