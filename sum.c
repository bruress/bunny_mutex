#include <stdio.h>
#include <time.h>
#include "stdbool.h"
#include "sum_date.h"

int sum_date () {
    int sum = 0;
    bool flag = false;

    // получаем текущее unix-время
    time_t mytime = time(NULL);

    struct tm *now;             // указатель на структуру, описанной в time.h
    now =  localtime(&mytime);  // возвращает АДРЕС разложенной структуры

    // (*now).tm_sec = now->tm.sec
    sum = now->tm_sec + now->tm_min + now->tm_hour + now->tm_mday + (now->tm_mon+1) + (now->tm_year + 1900);

    printf("Сумма: %d\n", sum);

    if (sum%2==0) {
        flag=true;
    } else {
        flag=false;
    }
    printf("%d\n", flag);

    return sum;
}
