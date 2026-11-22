#include <stdbool.h>
#include <stdio.h>

#include "sum_date.h"
#include "parity_sum_date.h"


int main() {
    int sum = 0;
    bool flag = false;

    sum = sum_date();
    printf("Сумма: %d\n", sum);

    flag = sum_date_is_bool(sum);
    printf("%s\n", flag ? "Сумма четная\n" : "Сумма нечетаная\n");

    return 0;
}
