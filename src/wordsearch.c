#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

void search_in_file(const char *filename, const char *word) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[1024];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strstr(line, word)) {
            printf("%s:%d: %s", filename, line_number, line);
        }
    }

    fclose(file);
}

void search_in_directory(const char *dir_path, const char *word) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Ошибка открытия директории");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем только . и ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        // Рекурсивный вызов для поддиректорий (включая скрытые)
        if (entry->d_type == DT_DIR) {
            search_in_directory(path, word);
        }
        // Обработка файлов (включая скрытые)
        else if (entry->d_type == DT_REG) {
            search_in_file(path, word);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    // Определение директории по умолчанию (~/files)
    const char *home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "Ошибка: не удалось определить домашнюю директорию\n");
        return EXIT_FAILURE;
    }

    char default_dir[PATH_MAX];
    snprintf(default_dir, sizeof(default_dir), "%s/files", home_dir);

    const char *dir_path = (argc > 1) ? argv[1] : default_dir;
    const char *word = (argc > 2) ? argv[2] : NULL;

    if (!word) {
        fprintf(stderr, "Использование: %s [директория] <искомое_слово>\n", argv[0]);
        return EXIT_FAILURE;
    }

    search_in_directory(dir_path, word);
    return EXIT_SUCCESS;
}
