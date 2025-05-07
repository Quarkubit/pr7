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

void help (char *prog_name) {
    printf("Использование: %s [ДИРЕКТОРИЯ] СЛОВО\n", prog_name);
    printf("Опции:\n");
    printf("  -h, --help     Показать справку и выйти\n");
    printf("Описание:\n");
    printf("  Ищет СЛОВО во всех текстовых файлах в указанной ДИРЕКТОРИИ и её поддиректориях.\n");
    printf("  Если ДИРЕКТОРИЯ не указана, используется ~/files.\n");
}

int main(int argc, char *argv[]) {
    printf("\n\tStart working!\n\n");

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            help(argv[0]);
            return EXIT_SUCCESS;
        }
    }
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

    if (!word) {
        fprintf(stderr, "Не указано искомое слово!\n", argv[0]);
        help(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
