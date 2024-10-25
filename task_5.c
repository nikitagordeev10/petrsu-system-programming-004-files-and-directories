/**
* 5.c -- аналог функции ftw()
*
* Copyright (c) <2022> <Гордеев Никита>
*
* This code is lisenced under a MIT-style licence.
*/

#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define FTW_F 1 /* файл, не являющийся каталогом */
#define FTW_D 2 /* каталог */
#define FTW_DNR 3 /* каталог, который недоступен для чтения */
#define FTW_NS 4 /* файл, информацию о котором  невозможно получить с помощью stat */
#define MAXPATH 255

char fullpath[MAXPATH]; /* полный путь к каждому из файлов */


/* тип функции, которая будет вызываться для каждого встреченного файла */
typedef int Myfunc(const char *, const struct stat *, int);

Myfunc myfunc;

int myftw(Myfunc *);

int main(int argc, char *argv[]){

    if (argc != 2){
        perror("Использование: ftw <начальный_каталог>");
	    exit(EXIT_FAILURE);
    }

    strcpy(fullpath, argv[1]);
    return myftw(myfunc);
}

/*
 * Выполняет обход дерева каталогов, начиная с "fullpath".
 * Если "fullpath" не является каталогом, для него вызывается lstat(),
 * func() и затем выполняется возврат.
 * Для каталогов производится рекурсивный вызов функции.
 */
int myftw(Myfunc *func){

    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    char *path;

    /* ошибка вызова функции stat */
    if(lstat(fullpath, &statbuf) < 0){ /* ошибка вызова функции stat */
	perror("Ошибка в получении информации об объекте\n");
	exit(EXIT_FAILURE);
    }

    /* не каталог */
    if(!S_ISDIR(statbuf.st_mode)){
	return (func(fullpath, &statbuf, FTW_F));
    }


    /* каталог недоступен */
    if((dp = opendir(fullpath)) == NULL){
        return (func(fullpath, &statbuf, FTW_DNR));
    }

    /*
    * Это каталог. Сначала вызвать функцию func(),
    * а затем обработать все файлы в этом каталоге.
    */
    int ret = 0;
    if((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return ret;

    /* добавление символа слэша в путь */
    path = fullpath + strlen(fullpath);
    *path++ = '/';
    *path = 0;

    /* пропустить каталоги "." и ".." */
    while((dirp = readdir(dp)) != NULL){
        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;

	    /* добавить имя после слеша */
        strcpy(path, dirp->d_name);

	    /* рекурсия */
        if((ret = myftw(func)) != 0)
            break; //выход по ошибке

    }

    /* стереть часть строки от слеша и до конца */
    path[-1] = 0;


    if (closedir(dp) < 0) {
        perror("невозможно закрыть каталог\n");
        exit(EXIT_FAILURE);
    }
    return ret;
}

int myfunc(const char *pathname, const struct stat *statptr, int type){

    switch (type) {
        /* файл */
        case FTW_F:
            printf("%s\n", pathname);
            break;

        /* каталог */
    	case FTW_D:
            printf("%s\n", pathname);
            break;

        /* каталог, закрытый для чтения */
     	case FTW_DNR:	
            printf("закрыт доступ к каталогу %s", pathname);
    	    break;

        /* невозможно получить информацию об этом файле */
    	case FTW_NS:	
            printf("ошибка вызова функции stat для %s", pathname);
            break;

        /* объект, не подходящий к вышеперечисленному */
    	default:	
    	    printf("неизвестный тип %d для файла %s", type, pathname);
    }

    return 0;
}