/**
 * task_1.c — программа, получающая один аргумент командной строки - путь к объекту файловой системы, 
 * выводящая для указанного объекта всю информацию, которую можно получить с помощью функций семейства *stat(). 
 * Должны корректно обрабатываться все варианты типа объекта: 
 * Файл, каталог, символьное устройство, блочное устройство, конвейер, символическая ссылка или сокет
 *
 * Copyright (c) <2022> <Гордеев Никита>
 *
 * This code is licensed under a MIT-style license.
 */

// У. Ричард Стивенс, Стивен А. Раго 4.2. Функции stat, fstat и lstat // UNIX. Профессиональное программирование. . - 2018. - С. 138.
// struct stat {
//  mode_t st_mode; /* тип файла и режим (права доступа) */
//  ino_t st_ino; /* номер индексного узла */
//  dev_t st_dev; /* номер устройства (файловой системы) */
//  dev_t st_rdev; /* номер устройства для специальных файлов */
//  nlink_t st_nlink; /* количество ссылок */
//  uid_t st_uid; /* идентификатор пользователя владельца */
//  gid_t st_gid; /* идентификатор группы владельца */
//  off_t st_size; /* размер в байтах, для обычных файлов */
//  struct timespec st_atim; /* время последнего обращения к файлу */
//  struct timespec st_mtim; /* время последнего изменения файла */
//  struct timespec st_ctim; /* время последнего изменения состояния файла */
//  blksize_t st_blksize; /* оптимальный размер блока ввода/вывода */
//  blkcnt_t st_blocks; /* количество занятых дисковых блоков */
// }; 

#include <stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{

    int i;
    struct stat buf;
    char *ptr;

    /* Проверка на наличие аргументов */
    if (argc < 2) {
        fprintf(stderr, "Введите путь к файлу\n");
        exit(EXIT_FAILURE);
    }

    /* Вывод типа файла для каждого аргумента командной строки */
    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) < 0) {
            perror("ошибка вызова функции lstat");
            exit(EXIT_FAILURE);
            continue;
        }

        if (S_ISREG(buf.st_mode))
            ptr = "обычный файл";
        else if (S_ISDIR(buf.st_mode))
            ptr = "каталог";
        else if (S_ISCHR(buf.st_mode))
            ptr = "файл символьного устройства";
        else if (S_ISBLK(buf.st_mode))
            ptr = "файл блочного устройства";
        else if (S_ISFIFO(buf.st_mode))
            ptr = "fifo";
        else if (S_ISLNK(buf.st_mode))
            ptr = "символическая ссылка";
        else if (S_ISSOCK(buf.st_mode))
            ptr = "сокет";
        else
            ptr = "** неизвестный тип файла **";
        printf("%s\n", ptr);
    }

    /* Конвертация даты из Timestamp */
    char my_atime[80];
    time_t t_1 = buf.st_atime; /*st_mtime is type time_t */
    struct tm lt_1;
    localtime_r(&t_1, &lt_1); /* convert to struct tm */
    strftime(my_atime, sizeof my_atime, "%a, %d %b %Y %T", &lt_1);

    char my_mtime[80];
    time_t t_2 = buf.st_mtime; /*st_mtime is type time_t */
    struct tm lt_2;
    localtime_r(&t_2, &lt_2); /* convert to struct tm */
    strftime(my_mtime, sizeof my_mtime, "%a, %d %b %Y %T", &lt_2);

    char my_ctime[80];
    time_t t_3 = buf.st_ctime; /*st_mtime is type time_t */
    struct tm lt_3;
    localtime_r(&t_3, &lt_3); /* convert to struct tm */
    strftime(my_ctime, sizeof my_ctime, "%a, %d %b %Y %T", &lt_3);

    /* Вывод информации о файле*/
    printf("Номер индексного узла: %ld\n", buf.st_ino);
    printf("Номер устройства (файловой системы): %ld\n", buf.st_dev);
    printf("Номер устройства для специальных файлов: %ld\n", buf.st_rdev);
    printf("Количество ссылок: %ld\n", buf.st_nlink);
    printf("Идентификатор пользователя владельца: %d\n", buf.st_uid);
    printf("Идентификатор группы: %d\n", buf.st_gid);
    printf("Размер в байтах, для обычных файлов: %ld\n", buf.st_size);
    printf("Время последнего обращения к файлу: %s\n", my_atime);
    printf("Время последнего изменения файла: %s\n", my_mtime);
    printf("Время последнего изменения состояния файла: %s\n", my_ctime);

    /* Стандарт POSIX.1 не требует наличия полей st_rdev, st_blksize и st_blocks */
    printf("Оптимальный размер блока ввода/вывода : %ld\n", buf.st_blksize);
    printf("Количество занятых дисковых блоков: %ld\n", buf.st_blocks);

    exit(0);
}
