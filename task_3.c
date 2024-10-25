/**
 * task_3.c — собственный аналог утилиты readlink
 *
 * Copyright (c) <2022> <Гордеев Никита>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <linux/limits.h>

int main(int argc, char *argv[])
{
    
    char linkpath[PATH_MAX + 1];
    int len;

    /* Проверка на наличие аргументов */
    if (argc < 2) {
        printf("Введите путь к файлу\n");
        exit(EXIT_FAILURE);
    }

    if (-1 == (len = readlink(argv[1], linkpath, PATH_MAX))) {
        fprintf(stderr, "Ошибка при вызове readlink: %s\n", strerror(errno));
        exit(1);
    }
    linkpath[len] = '\0';

    fprintf(stdout, "Имя исходного файла: %s\n", linkpath);

    exit(0);
}