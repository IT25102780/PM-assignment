#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10
#define LOG_FILE "game_log.text"

char **board;
int N;

void initializeBoard() {
	board = (char **)malloc(N * sizeof(char *));
	for (int i = 0; i<N; i++) {
		board[i] = (char *)malloc(N * sizeof(char));
		for (int j = 0; j<N; j++) {
			board[i][j] = ' ';
		}
	}
}

void displayBoard() {
	printf("\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j<N; j++) {
			printf(" %c ", board[i][j]);
			if (j< N - 1) printf("|");
		}
		printf("\n");
		if (i < N - 1) {
			for (int k = 0; k < N; k++) {
				printf("---");
				if (k < N - 1) printf("+");
			}
			printf("\n");
		}
	}
	printf("\n");
}

int isValidMove(int row, int col) {
	return row >= 0 && row < N && col >= 0 && col < N && board[row][col] == ' ';
}

void updateBoard(int row, int col, char symbol) {
	board[row][col] = symbol;
}

void logMove(int player, int row, int col) {
	FILE *fp = fopen(LOG_FILE, "a");
	if (fp != NULL) {
		fprintf(fp, "Player %d moved at (%d, %d)\n", player, row, col);
		fclose(fp);
	}
}

int checkWin(char symbol) {
	// Rows and Columns
	for (int i = 0; i < N; i++) {
		int rowWin = 1, colWin = 1;
		for (int j = 0; j < N; j++) {
			if (board[i][j] != symbol) rowWin = 0;
			if (board[j][i] != symbol) colWin = 0;
		}
		if (rowWin || colWin) return 1;
	}

	// Diagonals
	int diag1 = 1, diag2 = 1;
	for (int i = 0; i < N; i++) {
		if (board[i][i] != symbol) diag1 = 0;
		if (board[i][N - i -1] != symbol) diag2 = 0;
	}
	return diag1 || diag2;
}

int isDraw() {
	for (int i = 0; i<N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] == ' ') return 0;
	return 1;
}

void getUserMove(int player, char symbol) {
	int row, col;
	do {
		printf("Player %d (%c), enter row and column: ", player, symbol);
		scanf("%d %d", &row, &col);
	} while (!isValidMove(row, col));
	updateBoard(row, col, symbol);
	logMove(player, row, col);
}

void getComputerMove(char symbol) {
	int row, col;
	srand(time(NULL));
	do {
		row = rand() % N;
		col = rand() % N;
	} while (!isValidMove(row, col));
	printf("Computer (%c) moved at (%d, %d)\n", symbol, row, col);
	updateBoard(row, col, symbol);
	logMove(2, row, col);
}

void playGame(int mode) {
	int turn = 0;
	char symbols[2] = {'X', '0'};
	while (1) {
		displayBoard();
		if (mode == 1 || (mode ==2 && turn % 2 == 0)) {
			getUserMove(turn % 2 + 1, symbols[turn % 2]);
		} else {
			getComputerMove(symbols[1]);
		}

		if (checkWin (symbols[turn % 2])) {
			displayBoard();
			printf("Player %d (%c) wins!\n", turn % 2 + 1, symbols[turn % 2]);
			break;
		}
		if (isDraw()) {
			displayBoard();
			printf("Game is a draw!\n");
			break;
		}
		turn++;
	}
}

int main() {
	printf("Enter board size N (3-10): ");
	scanf("%d", &N);
	if (N < 3 || N > 10) {
		printf("Invalid board size.\n");
		return 1;
	}

	int mode;
	printf("Choose mode:\n1. User vs User\n2. User vs Computer\nEnter choice: ");
	scanf("%d", &mode);

	initializeBoard();
	FILE *fp = fopen(LOG_FILE, "w"); // clear log file
	if (fp) fclose(fp);

	playGame(mode);

	for (int i = 0; i<N; i++) free(board[i]);
	free(board);
	return 0;
}
