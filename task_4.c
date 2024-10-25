/**
 * task_4.c — собственный упрощенный аналог утилиты chmod
 * Для упрощенного аналога РЕЖИМ указывать в формате, который используется в выводе ls -l:
 * Т.е. ./mychmod 0755 файл или ./mychmod -rwxr-xr-x файл.
 *
 * Copyright (c) <2022> <Гордеев Никита>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* Проверка на наличие аргументов */
    if (argc != 3) {
	    printf("Введите путь к файлу: ./task_4 -rwxr-xr-x [файл] \n");        
        exit(EXIT_FAILURE);
    }
    if (strlen(argv[1]) != 10) {
	    printf("Установките права: -rwxr-xr-x\n");
	    exit(EXIT_FAILURE);
    }

    /* Аргументы для проверки */
    mode_t mode = 0;
    int set_constants[9] = { S_IXUSR, S_IXGRP, S_IRUSR, S_IRGRP, S_IWGRP, S_IWUSR, S_IROTH, S_IWOTH, S_IXOTH };
    char rights[3] = { 'r', 'w', 'x' };

    /* Установка режима */
    for (int i = 1; i < 10; i++) {
        if (argv[1][i] == rights[0] || argv[1][i] == rights[1] || argv[1][i] == rights[2])
            mode |= set_constants[i - 1];
    }

    /* Проверка режима */
    if (-1 == chmod(argv[2], mode)) {
        printf("Ошибка chmod\n");
        exit(EXIT_FAILURE);
    }

    exit(0);
}

// Материалы
// У. Ричард Стивенс, Стивен А. Раго Таблица 4.8. Константы режимов для функции chmod, определенные в файле <sys/stat.h> // UNIX. Профессиональное программирование. - С. 152.