#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define SIZE 7
#define BLACK 0.3
#define NUMBER 0.6

void initializeBoard(char* board[SIZE][SIZE]);
void printBoard(char* board[SIZE][SIZE]);
int isValidBoard(char* board[SIZE][SIZE]);
int saveToFile(char* board[SIZE][SIZE], char* fileName);
void log_error(char* message);

int main() {
    setlocale(LC_CTYPE, "RUS");
    srand(time(NULL));

    char* board[SIZE][SIZE];
    char* fileName[200];
    
    int choice = 0;
    printf("[-------- Game Light Up --------]\n");
    printf("| 1) Сгенерировать поле         |\n");
    printf("| 2) Сохранить поле в файл      |\n");
    printf("| 3) Узнать правила игры        |\n");
    printf("| 4) Выход                      |\n");
    printf("[-------------------------------]\n");

    while (1 > 0) {
        printf("\nВведите значение: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                do {
                    initializeBoard(board);
                } while (!isValidBoard(board));

                printBoard(board);

                break;
            case 2:
                printf("Введите название файла (ex: map.txt): ");
                scanf("%s", &fileName);

                int res = saveToFile(board, fileName);
                if (res == 0)
                    printf("Поле сохранено в файл: %s\n", fileName);
                else {
                    printf("ОШИБКА! Посмотрите файл ошибок\n");
                    log_error("Файл для сохранения поля не был создан");
                }

                break;
            case 3:
                printf("[------------------------ Rule Light Up ------------------------]\n");
                printf("| > Размещайте ламы на белых клетках, чтобы осветить все поле   |\n");
                printf("| > Лампы светят по горизонтали и вертикали до черных клеток    |\n");
                printf("| > Лампы не должны светить друг на друга                       |\n");
                printf("| > Числа в черных клетках = количеству соседних ламп           |\n");
                printf("[---------------------------------------------------------------]\n");

                break;
            case 4:
                exit(0);
                break;
            default:
                printf("Команда `%d` не найдена\n", choice);
                break;
        }
    }

    return 0;
}

/**
* Заполнение поля значениями
* @param board - поле для заполнения
*/
void initializeBoard(char* board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((double)rand() / RAND_MAX < BLACK) {
                int nearNumber = 0;

                if (i > 0 && board[i - 1][j] >= '1' && board[i - 1][j] <= '3') nearNumber = 1;
                if (j > 0 && board[i][j - 1] >= '1' && board[i][j - 1] <= '3') nearNumber = 1;

                if (!nearNumber && (double)rand() / RAND_MAX < NUMBER) {
                    board[i][j] = '1' + rand() % 3;
                }
                else {
                    board[i][j] = 'B';
                }
            }
            else {
                board[i][j] = '.';
            }
        }
    }
}

/**
* Печать поля
* @param board - поле для печати
*/
void printBoard(char* board[SIZE][SIZE]) {
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

/**
* Проверка значений поля на корректность
* @param board - проверяемое поле
* @return успешность генерации (0 - ошибка, 1 - успешно)
*/
int isValidBoard(char* board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] >= '1' && board[i][j] <= '3') {
                int required = board[i][j] - '0';
                int spaces = 0;

                if (i > 0 && board[i - 1][j] == '.') spaces++;
                if (i < SIZE - 1 && board[i + 1][j] == '.') spaces++;
                if (j > 0 && board[i][j - 1] == '.') spaces++;
                if (j < SIZE - 1 && board[i][j + 1] == '.') spaces++;

                if (required > spaces || spaces == 0) {
                    return 0;
                }
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == '.') {
                int neighbors = 0;
                if (i > 0 && board[i - 1][j] != 'B' && !(board[i - 1][j] >= '1' && board[i - 1][j] <= '3')) neighbors++;
                if (i < SIZE - 1 && board[i + 1][j] != 'B' && !(board[i + 1][j] >= '1' && board[i + 1][j] <= '3')) neighbors++;
                if (j > 0 && board[i][j - 1] != 'B' && !(board[i][j - 1] >= '1' && board[i][j - 1] <= '3')) neighbors++;
                if (j < SIZE - 1 && board[i][j + 1] != 'B' && !(board[i][j + 1] >= '1' && board[i][j + 1] <= '3')) neighbors++;

                if (neighbors == 0) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

/**
* Сохранение поля в файл
* @param board - поле для сохранения
* @param fileName - имя файла для сохранения
* @return успешность сохранения (-1 - ошибка, 0 - успешно)
*/
int saveToFile(char* board[SIZE][SIZE], char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (file == NULL) return -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) fprintf(file, "%c", board[i][j]);
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

/**
* Сохранение ошибки в файл
* @param message - сообщение ошибки
*/
void log_error(char* message) {
    FILE* log_file = fopen("errors.txt", "a");
    if (log_file) {
        fprintf(log_file, "ERROR: %s\n", message);
        fclose(log_file);
    }
}