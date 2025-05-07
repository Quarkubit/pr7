#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void search_in_files (const char *file_name, const char *word) {
    FILE *file = fopen(file_name, "r");
    if (!file){
        perror("Ошибка открытия файла");
        return;
    }

    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (strstr(line, word)) {
            printf("%s:%d:\"%s\"", file_name, line_count, line);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    printf("\n\tStart working!\n\n");

    // Получение домашней директории и проверка
    const char *home_dir = getenv("HOME");
    printf("%s\n", home_dir);
    if (!home_dir) {
        fprintf(stderr, "Ошибка: не удалось определить домашнюю директорию\n");
        return EXIT_FAILURE;
    }

    //Получение директории по умолчанию
    char default_dir[PATH_MAX];
    snprintf(default_dir, sizeof(default_dir), "%s/files", home_dir);
    printf("%s\n", default_dir);

    //Обработка аргументов
    const char *dir_path = (argc > 2) ? argv[1] : default_dir;
    printf("%s\n", dir_path);
    const char *word = (argc > 2) ? argv[2] : argv[1];
    printf("%s\n", word);

    return EXIT_SUCCESS;
}
