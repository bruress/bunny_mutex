#ifndef TABLES_H
#define TABLES_H

#include <stdbool.h>

struct SumTable {
    int sum;
    bool is_ready;
};

struct ParityTable {
    bool is_parity;
    bool is_ready;
};

#endif
