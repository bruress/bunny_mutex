#include <time.h>

#include "worker_wait.h"

// ожидание с проверкой завершения программы
void worker_wait(struct ThreadArgs *args, int delay_ms) {
    while (delay_ms > 0) {
        pthread_mutex_lock(args->mutex);
        bool stop_requested = args->stop_requested;
        pthread_mutex_unlock(args->mutex);
        if (stop_requested) return;

        int step_ms = delay_ms < 50 ? delay_ms : 50; // ожидание небольшими частями
        struct timespec delay = {
            .tv_sec = 0,
            .tv_nsec = step_ms * 1000000L
        };
        nanosleep(&delay, NULL); // пауза без захвата мьютекса
        delay_ms -= step_ms;
    }
}
