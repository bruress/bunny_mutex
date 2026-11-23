#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iup.h>
#include <iupcontrols.h>
#include <iupim.h>

#include "gui.h"
#include "tables.h"

// выход из ПО
int btn_exit_cb(Ihandle *self) {
    return IUP_CLOSE;
}

// остановка потока суммы
int stop_sum_cb(Ihandle *self) {
    struct ThreadArgs *args = (struct ThreadArgs *)IupGetAttribute(self, "THREAD_ARGS");    // достаем из кнопки адрес структуры
    pthread_mutex_lock(args->mutex);
    args->sum_paused=true;
    pthread_mutex_unlock(args->mutex);
    return  IUP_DEFAULT;
}

// остановка потока четости
int stop_parity_cb(Ihandle *self) {
    struct ThreadArgs *args = (struct ThreadArgs *)IupGetAttribute(self, "THREAD_ARGS");    // достаем из кнопки адрес структуры
    pthread_mutex_lock(args->mutex);
    args->parity_paused=true;
    pthread_mutex_unlock(args->mutex);
    return IUP_DEFAULT;
}

// остановка всех потоков
int stop_all_cb(Ihandle *self) {
    struct ThreadArgs *args = (struct ThreadArgs *)IupGetAttribute(self, "THREAD_ARGS");
    pthread_mutex_lock(args->mutex);
    args->sum_paused=true;
    args->parity_paused=true;
    pthread_mutex_unlock(args->mutex);
    return IUP_DEFAULT;
}

// обновление числа при движении ползунка
int slider_value_cb(Ihandle *self) {
    Ihandle *number_field = IupGetAttributeHandle(self, "NUMBER_FIELD");
    int value = (int)(IupGetDouble(self, "VALUE")+0.5);                 // округление в большую сторону
    IupSetInt(self, "VALUE", value);
    IupSetInt(number_field, "VALUE", value);
    return IUP_DEFAULT;
}

// обновление ползунка при вводе числа
int number_value_cb(Ihandle *self) {
    const char *text = IupGetAttribute(self, "VALUE");
    if (text[0] == '\0') return IUP_DEFAULT;                // чтобы не перескочить в 0
    Ihandle *slider = IupGetAttributeHandle(self, "SLIDER");
    IupSetInt(slider, "VALUE", IupGetInt(self, "VALUE"));
    return IUP_DEFAULT;
}

// рандомная загрузка bunny
int bunny_rand(Ihandle *self) {
    Ihandle *label_image = IupGetAttributeHandle(self, "LABEL");
    int index = rand() % 9;
    Ihandle *image = IupGetAttributeHandleId(self, "IMAGE", index);
    IupSetAttributeHandle(label_image, "IMAGE", image);
    return IUP_DEFAULT;
}

int gui(struct ThreadArgs *args) {
// чтобы работало до комплияции, в случае windows
#ifdef __linux__
    setenv("GDK_BACKEND", "x11", 0);
#endif

    IupOpen(NULL, NULL);            // инициализация библиотеки iup
    IupControlsOpen();              // инициализация библиотеки для таблиц matrix
    srand((unsigned)time(NULL));    // инициализация рандомайзера

    // инициализация элементов
    Ihandle *button_stop_t1 = IupButton("Стоп", NULL);
    Ihandle *button_stop_t2 = IupButton("Стоп", NULL);

    IupSetAttribute(button_stop_t1, "THREAD_ARGS", (char *)args);
    IupSetCallback(button_stop_t1, "ACTION", (Icallback) stop_sum_cb);


    IupSetAttribute(button_stop_t2, "THREAD_ARGS", (char *)args);
    IupSetCallback(button_stop_t2, "ACTION", (Icallback) stop_parity_cb);

    Ihandle *button_reset = IupButton("Стоп", NULL);
    Ihandle *button_exit = IupButton("Выход", NULL);

    IupSetAttribute(button_reset, "THREAD_ARGS", (char *)args);
    IupSetCallback(button_reset, "ACTION", (Icallback) stop_all_cb);


    Ihandle *label = IupLabel("OMG!!! It's bunny!");
    Ihandle *label_image = IupLabel(NULL);
    Ihandle *label_task = IupLabel("С помощью мьютексов организовать работу параллельных вычислительных потоков.\n\
Первый поток подсчитывает сумму сегодняшней даты и времени (г+м+д+ч+мин+с) и записывает в таблицу.\n\
Второй поток проверяет, четное ли число записано в таблице, если да – устанавливает соответствующее поле таблицы в true, если нет – в false");

    Ihandle *sum_table = IupMatrix(NULL);
    Ihandle *parity_table = IupMatrix(NULL);

    Ihandle *slider_t1 = IupVal("VERTICAL");
    Ihandle *number_field_t1 = IupText(NULL);

    Ihandle *slider_t2 = IupVal("VERTICAL");
    Ihandle *number_field_t2 = IupText(NULL);

    Ihandle *images[9];
    Ihandle *timer = IupTimer();


    // настройка таймера раз в 5 с
    IupSetAttribute(timer, "TIME", "5000");
    IupSetCallback(timer, "ACTION_CB", (Icallback)bunny_rand);

    // размеры таблицы сумм
    IupSetAttribute(sum_table, "NUMLIN", "20");
    IupSetAttribute(sum_table, "NUMCOL", "2");
    IupSetAttribute(sum_table, "RASTERWIDTH1", "45");
    IupSetAttribute(sum_table, "RASTERWIDTH2", "110");
    IupSetAttribute(sum_table, "HEIGHTDEF", "5");
    IupSetAttribute(sum_table, "NUMLIN_VISIBLE", "20");
    IupSetAttribute(sum_table, "NUMCOL_VISIBLE", "2");
    IupSetAttribute(sum_table, "EXPAND", "NO");         // никакого растягивания, из-за него не видно ползунки!

    // прокрутка и запрет редактирования
    IupSetAttribute(sum_table, "SCROLLBAR", "VERTICAL");
    IupSetAttribute(sum_table, "READONLY", "YES");

    // заголовки таблицы сумм
    IupSetAttribute(sum_table, "0:1", "№");
    IupSetAttribute(sum_table, "0:2", "Сумма");

    // пример суммы
    IupSetAttribute(sum_table, "1:1", "1");
    IupSetAttribute(sum_table, "1:2", "2090");

    // размеры таблицы четности
    IupSetAttribute(parity_table, "NUMLIN", "20");
    IupSetAttribute(parity_table, "NUMCOL", "2");
    IupSetAttribute(parity_table, "RASTERWIDTH1", "45");
    IupSetAttribute(parity_table, "RASTERWIDTH2", "110");
    IupSetAttribute(parity_table, "HEIGHTDEF", "5");
    IupSetAttribute(parity_table, "NUMLIN_VISIBLE", "20");
    IupSetAttribute(parity_table, "NUMCOL_VISIBLE", "2");
    IupSetAttribute(parity_table, "EXPAND", "NO");         // никакого растягивания, из-за него не видно ползунки!

    // прокрутка и запрет редактирования
    IupSetAttribute(parity_table, "SCROLLBAR", "VERTICAL");
    IupSetAttribute(parity_table, "READONLY", "YES");

    // заголовки таблицы четности
    IupSetAttribute(parity_table, "0:1", "№");
    IupSetAttribute(parity_table, "0:2", "Чётность");

    // пример четности
    IupSetAttribute(parity_table, "1:1", "1");
    IupSetAttribute(parity_table, "1:2", "true");

    // инициализация ползунков
    IupSetAttribute(slider_t1, "MIN", "0");
    IupSetAttribute(slider_t1, "MAX", "100");
    IupSetAttribute(slider_t1, "VALUE", "50");
    IupSetAttribute(slider_t1, "RASTERSIZE", "40x250");
    IupSetAttribute(slider_t1, "EXPAND", "NO");

    IupSetAttribute(slider_t2, "MIN", "0");
    IupSetAttribute(slider_t2, "MAX", "100");
    IupSetAttribute(slider_t2, "VALUE", "50");
    IupSetAttribute(slider_t2, "RASTERSIZE", "40x250");
    IupSetAttribute(slider_t2, "EXPAND", "NO");

    // инициализация полей ввода
    IupSetAttribute(number_field_t1, "MASKINT", "0:100");      // ввод только целых чисел от нуля до ста
    IupSetAttribute(number_field_t1, "NC", "3");               // не больше трех символов
    IupSetAttribute(number_field_t1, "VALUE", "50");
    IupSetAttribute(number_field_t1, "RASTERSIZE", "60x");
    IupSetAttribute(number_field_t1, "EXPAND", "NO");
    IupSetAttribute(number_field_t1, "ALIGNMENT", "ACENTER");

    IupSetAttribute(number_field_t2, "MASKINT", "0:100");      // ввод только целых чисел от нуля до ста
    IupSetAttribute(number_field_t2, "NC", "3");               // не больше трех символов
    IupSetAttribute(number_field_t2, "VALUE", "50");
    IupSetAttribute(number_field_t2, "RASTERSIZE", "60x");
    IupSetAttribute(number_field_t2, "EXPAND", "NO");
    IupSetAttribute(number_field_t2, "ALIGNMENT", "ACENTER");

    IupSetAttribute(button_stop_t1, "RASTERSIZE", "80x34");
    IupSetAttribute(button_stop_t2, "RASTERSIZE", "80x34");
    IupSetAttribute(button_reset, "RASTERSIZE", "120x36");
    IupSetAttribute(button_exit, "RASTERSIZE", "120x36");

    IupSetAttribute(label_task, "WORDWRAP", "YES");
    IupSetAttribute(label_task, "RASTERSIZE", "1x85");
    IupSetAttribute(label_task, "EXPAND", "HORIZONTAL");
    IupSetAttribute(label_task, "ALIGNMENT", "ALEFT:ATOP");

    // связь ползнуков с полями
    IupSetAttributeHandle(slider_t1, "NUMBER_FIELD", number_field_t1);
    IupSetAttributeHandle(slider_t2, "NUMBER_FIELD", number_field_t2);

    // связь поля с ползунком
    IupSetAttributeHandle(number_field_t1, "SLIDER", slider_t1);
    IupSetAttributeHandle(number_field_t2, "SLIDER", slider_t2);

    // кастомная обработка полузнка
    IupSetCallback(slider_t1, "VALUECHANGED_CB", (Icallback) slider_value_cb);
    IupSetCallback(slider_t2, "VALUECHANGED_CB", (Icallback) slider_value_cb);

    // кастомная обработка ввода числа
    IupSetCallback(number_field_t1, "VALUECHANGED_CB", (Icallback) number_value_cb);
    IupSetCallback(number_field_t2, "VALUECHANGED_CB", (Icallback) number_value_cb);

    // загрузка картинок
    for (int i = 0; i < 9; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "img/bunny/bunny_%02d.png", i + 1);
        images[i] = IupLoadImage(filename);
        IupSetAttributeHandleId(timer, "IMAGE", i, images[i]);
    }

    // выбор случайной картинки
    int index = rand() % 9;
    IupSetAttributeHandle(label_image, "IMAGE", images[index]);
    IupSetAttributeHandle(timer, "LABEL", label_image);

    // размещение слайдеров
    Ihandle *slider_box_t1 = IupVbox(slider_t1, number_field_t1, button_stop_t1, NULL);
    IupSetAttribute(slider_box_t1, "MARGIN", "10x10");
    IupSetAttribute(slider_box_t1, "GAP", "8");
    IupSetAttribute(slider_box_t1, "ALIGNMENT", "ACENTER");
    Ihandle *slider_frame_t1 = IupFrame(slider_box_t1);
    IupSetAttribute(slider_frame_t1, "TITLE", "Поток №1");

    Ihandle *slider_box_t2 = IupVbox(slider_t2, number_field_t2, button_stop_t2, NULL);
    IupSetAttribute(slider_box_t2, "MARGIN", "10x10");
    IupSetAttribute(slider_box_t2, "GAP", "8");
    IupSetAttribute(slider_box_t2, "ALIGNMENT", "ACENTER");
    Ihandle *slider_frame_t2 = IupFrame(slider_box_t2);
    IupSetAttribute(slider_frame_t2, "TITLE", "Поток №2");

    Ihandle *controls = IupHbox(slider_frame_t1, slider_frame_t2, NULL);
    IupSetAttribute(controls, "MARGIN", "0x0");
    IupSetAttribute(controls, "GAP", "8");
    IupSetAttribute(controls, "EXPAND", "NO");

    Ihandle *sum_frame = IupFrame(sum_table);
    IupSetAttribute(sum_frame, "TITLE", "Таблица потока №1");

    Ihandle *parity_frame = IupFrame(parity_table);
    IupSetAttribute(parity_frame, "TITLE", "Таблица потока №2");

    Ihandle *bunny_box = IupVbox(IupFill(), label, label_image, IupFill(), NULL);
    IupSetAttribute(bunny_box, "MARGIN", "0x0");
    IupSetAttribute(bunny_box, "ALIGNMENT", "ACENTER");
    IupSetAttribute(bunny_box, "GAP", "8");
    IupSetAttribute(bunny_box, "EXPAND", "VERTICAL");

    Ihandle *tables = IupHbox(controls, IupFill(), sum_frame, parity_frame, IupFill(), bunny_box, NULL);
    IupSetAttribute(tables, "GAP", "12");
    IupSetAttribute(tables, "ALIGNMENT", "ATOP");
    IupSetAttribute(tables, "MARGIN", "0x0");
    IupSetAttribute(tables, "EXPAND", "HORIZONTAL");

    Ihandle *task_box = IupVbox(label_task, NULL);
    IupSetAttribute(task_box, "MARGIN", "12x12");
    Ihandle *task_frame = IupFrame(task_box);
    IupSetAttribute(task_frame, "TITLE", "Задание");

    Ihandle *actions = IupVbox(button_reset, button_exit, NULL);
    IupSetAttribute(actions, "MARGIN", "0x0");
    IupSetAttribute(actions, "GAP", "10");
    IupSetAttribute(actions, "EXPAND", "NO");

    Ihandle *footer = IupHbox(task_frame, actions, NULL);
    IupSetAttribute(footer, "MARGIN", "0x0");
    IupSetAttribute(footer, "GAP", "16");
    IupSetAttribute(footer, "ALIGNMENT", "ATOP");
    IupSetAttribute(footer, "EXPAND", "HORIZONTAL");

    Ihandle *vbox = IupVbox(
        tables,
        footer,
        NULL
    );
    IupSetAttribute(vbox, "MARGIN", "16x16");
    IupSetAttribute(vbox, "GAP", "16");

    Ihandle *dlg = IupDialog(vbox);                                 // создание окна с элементами из контейнера
    IupSetAttribute(dlg, "TITLE", "Синхронизация процессов");       // заголовок окна
    IupSetAttribute(dlg, "RESIZE", "NO");
    IupSetAttribute(dlg, "RASTERSIZE", "1000x700");
    IupSetCallback(button_exit, "ACTION", (Icallback) btn_exit_cb); // назначение обработчика нажатия кнопки

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(timer, "RUN", "YES");
    IupMainLoop();

    IupDestroy(timer);
    IupClose();
    return EXIT_SUCCESS;
}
