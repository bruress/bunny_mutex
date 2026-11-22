#ifndef TABLES_H
#define TABLES_H

#include <stdbool.h>
#include <pthread.h>

struct SumTable {
    int sum;
    bool is_ready;
};

struct ParityTable {
    bool is_parity;
    bool is_ready;
};

struct ThreadArgs {
    struct SumTable *s;
    struct ParityTable *p;
    pthread_mutex_t *mutex; // т.к мы его уже создали!
};

#endif
