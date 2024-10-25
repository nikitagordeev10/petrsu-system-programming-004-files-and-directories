/**
 * task_2.c — Собственный упрощенный аналог утилиты du: 
 * Достаточно выдавать использование дискового пространства для всех файлов, указанных в командной строке. 
 * Работать с каталогами не требуется.
 *
 * Copyright (c) <2022> <Гордеев Никита>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat buf;

    /* Проверка на наличие аргументов */
    if (argc < 2) {
        fprintf(stderr, "Введите название файла\n");
        exit(EXIT_FAILURE);
    }
    
    /* Использование дискового пространства для всех файлов */
    for (int k = 1; k < argc; k++) {
        if (lstat(argv[k], &buf) < 0) {
            printf("du: невозможно получить доступ %s: Нет такого файла или каталога\n", argv[k]);
            continue;
        }
        printf("Disk Usage:\n%ld\t%s \n", (buf.st_blocks) / 2, argv[k]);
    }

    exit(0);
}