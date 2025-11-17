#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define SIZE 7
#define BLACK 0.3
#define NUMBER 0.6

char board[SIZE][SIZE];

void initializeBoard();
void printBoard();
int isValidBoard();
void saveToFile();
void generateGameBoard();

int main() {
    setlocale(LC_CTYPE, "RUS");
    srand(time(NULL));

    int choice = 1;
    do {
        generateGameBoard();
        printBoard();
        saveToFile();

        printf("\nНачать новую генерацию поля (1 - да, 0 - нет)?\n");
        scanf("%d", &choice);
    } while (choice != 0);

    return 0;
}

void initializeBoard() {
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

void printBoard() {
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int isValidBoard() {
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


void saveToFile() {
    FILE* file = fopen("lightup_board.txt", "w");
    if (file == NULL) {
        printf("Ошибка создания файла!\n");
        return;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\nПоле сохранено в файл: lightup_board.txt\n");
}

void generateGameBoard() {
    do {
        initializeBoard();
    } while (!isValidBoard());
}