#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void search_in_files (const char *file_name, const char *word) {
    //Открытие необходимого файла и проверка на открытие
    FILE *file = fopen(file_name, "r");
    if (!file){
        perror("Ошибка открытия файла");
        return;
    }

    //Переменные для строки со словом (буфер и счетчик)
    char line[1024];
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line_count++;

        //Поиск слова
        if (strstr(line, word)) {
            printf("%s:%d:%s", file_name, line_count, line);
        }
    }

    fclose(file);
}

void search_in_dir (const char *dir_path, const char *word) {
    //Открытие заданной директории
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Ошибка открытия директории");
        return;
    }

    struct dirent *entry;
    while ((entry=readdir(dir)) != NULL) {

        //Проверка на текущую и родительскую папку
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        //Сохраняем полный путь к файлу/директории
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        //Рекурсивный вызов, для проверки поддиректорий
        if (entry->d_type == DT_DIR) {
            search_in_dir(path, word);
        }//Обработка файлов
        else if (entry->d_type == DT_REG) {
            search_in_files(path, word);
        }

    }

    closedir(dir);
}

void help (char *prog_name) {
    printf("Использование: %s [ДИРЕКТОРИЯ] ИСКОМОЕ_СЛОВО [ОПЦИИ]\n", prog_name);
    printf("Опции:\n");
    printf("  -h, --help     Показать справку и выйти\n");
    printf("Описание:\n");
    printf("  Ищет СЛОВО во всех текстовых файлах в указанной ДИРЕКТОРИИ и её поддиректориях.\n");
    printf("  Если ДИРЕКТОРИЯ не указана, используется ~/files.\n");
}

int main(int argc, char *argv[]) {
    //Проверка на опцию help
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
            help(argv[0]);
            return EXIT_SUCCESS;
        }
    }

    // Получение домашней директории и проверка
    const char *home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "Ошибка: не удалось определить домашнюю директорию\n");
        return EXIT_FAILURE;
    }

    //Получение директории по умолчанию
    char default_dir[PATH_MAX];
    snprintf(default_dir, sizeof(default_dir), "%s/files", home_dir);

    //Обработка аргументов (путь к файлам и искомое слово)
    const char *dir_path = (argc > 2) ? argv[1] : default_dir;
    const char *word = (argc > 2) ? argv[2] : argv[1];

    //Проверка, указано ли искомое число
    if (!word) {
        fprintf(stderr, "Не указано искомое слово!\n", argv[0]);
        help(argv[0]);
        return EXIT_FAILURE;
    }

    search_in_dir(dir_path, word);
    return EXIT_SUCCESS;
}
