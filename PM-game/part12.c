#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PLAYERS 3
#define MAX_SIZE 10
#define LOG_FILE "game_log.text"

typedef struct {
	char symbol;
	int isComputer;
} Player;

char **board;
int N;
Player players[MAX_PLAYERS];

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
	printf("\nCurrent board:\n");
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

void logMove(int playerIndex, int row, int col) {
	FILE *fp = fopen(LOG_FILE, "a");
	if (fp != NULL) {
		fprintf(fp, "Player %d (%c) moved at (%d, %d)\n", playerIndex + 1, players[playerIndex].symbol, row, col);
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

void getUserMove(int playerIndex) {
	int row, col;
	do {
		printf("Player %d (%c), enter row and column: ", playerIndex + 1, players[playerIndex].symbol);
		scanf("%d %d", &row, &col);
	} while (!isValidMove(row, col));
	updateBoard(row, col, players[playerIndex].symbol);
	logMove(playerIndex, row, col);
}

void getComputerMove(int playerIndex) {
	int row, col;
	srand(time(NULL) + playerIndex);
	do {
		row = rand() % N;
		col = rand() % N;
	} while (!isValidMove(row, col));
	printf("Player %d (%c) moved at (%d, %d)\n", playerIndex + 1, players[playerIndex].symbol, row, col);
	updateBoard(row, col, players[playerIndex].symbol);
	logMove(playerIndex, row, col);
}

void configurePlayers(int mode) {
	char symbols[MAX_PLAYERS] = {'X', '0', 'Z'};
	for (int i = 0; i < MAX_PLAYERS; i++) {
		players[i].symbol = symbols[i];
		if (mode == 1 && i < 2) {
			players[i].isComputer = 0;
		} else if (mode == 2 && i == 0) {
			players[i].isComputer = 0;
		} else if (mode == 2 && i == 1) {
			players[i].isComputer = 1;
		} else {
			printf("Is Player %d (%c) a computer? (1 = Yes, 0 = No): ", i + 1, symbols[i]);
			scanf("%d", &players[i].isComputer);
		}
	}
}

void playGame(int mode) {
	int turn = 0;
	int totalPlayers = (mode == 3) ? 3 : 2 ;

	while (1) {
		displayBoard();
		int currentPlayer = turn % totalPlayers;
		if (players[currentPlayer].isComputer)
			getComputerMove(currentPlayer);
	       	else 
			getUserMove(currentPlayer);

		if (checkWin(players[currentPlayer].symbol)) {
			displayBoard();
			printf("Player %d (%c) wins!\n", currentPlayer + 1, players[currentPlayer].symbol);
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
	printf("Welcome to Tic-Tac-Toe\n");
	printf("Enter board size N (3-10): ");
	scanf("%d", &N);
	if (N < 3 || N > 10) {
		printf("Invalid board size.\n");
		return 1;
	}

	int mode;
	printf("Choose mode:\n1. User vs User\n2. User vs Computer\n3. Multi-Player (3 players)\nEnter choice: ");
	scanf("%d", &mode);
	if (mode < 1 || mode > 3) {
		printf("Invalid mode.\n");
		return 1;
	}

	initializeBoard();
	FILE *fp = fopen(LOG_FILE, "w"); // clear log file
	if (fp) fclose(fp);

	configurePlayers(mode);
	playGame(mode);

	for (int i = 0; i<N; i++) free(board[i]);
	free(board);
	return 0;
}
