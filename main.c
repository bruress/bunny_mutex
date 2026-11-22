#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // для sleep()

#include "include/tables.h"
#include "include/sum_date.h"
#include "include/parity_sum_date.h"

int main() {
    // инициализация структур и полей
    struct SumTable sum_table = {0, false};
    struct ParityTable parity_table = {false, false};

    struct SumTable *s = &sum_table;     // указатель на таблицу с суммами
    struct ParityTable *p = &parity_table;  // указатель на таблицу с четностью

    while (1) {
        s->sum = sum_date();
        printf("Сумма: %d\n", s->sum);

        p->is_parity = sum_date_is_bool(s->sum);
        printf("%s\n", p->is_parity ? "Сумма четная\n" : "Сумма нечетаная\n");
    }
    return 0;
}
