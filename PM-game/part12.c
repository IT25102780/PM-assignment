#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Maximum number of players supported
#define MAX_PLAYERS 3
#define MAX_SIZE 10
#define LOG_FILE "game_log.text"  // File to store move logs

// Structure to represent a player
typedef struct {
	char symbol;      // Player's symbol 
	int isComputer;   // 1 if computer, 0 if human
} Player;

char **board;                  // Dynamic 2D array for the game oard
int N;                         // Board size (N * N)
Player players[MAX_PLAYERS];   // Array to store player information

// Function to initialize the game board with empty spaces
void initializeBoard() {
	board = (char **)malloc(N * sizeof(char *));
	for (int i = 0; i<N; i++) {
		board[i] = (char *)malloc(N * sizeof(char));
		for (int j = 0; j<N; j++) {
			board[i][j] = ' ';
		}
	}
}

// Function to display the current state of the board
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

// Function to check if a move is valid (within bounds and on an empty cell)
int isValidMove(int row, int col) {
	return row >= 0 && row < N && col >= 0 && col < N && board[row][col] == ' ';
}

// Function to update the board with the player's symbol
void updateBoard(int row, int col, char symbol) {
	board[row][col] = symbol;
}

// Function to log each move to a file
void logMove(int playerIndex, int row, int col) {
	FILE *fp = fopen(LOG_FILE, "a");
	if (fp != NULL) {
		fprintf(fp, "Player %d (%c) moved at (%d, %d)\n", playerIndex + 1, players[playerIndex].symbol, row, col);
		fclose(fp);
	}
}

// Function to check if a player has won the game
int checkWin(char symbol) {
	// Check rows and columns
	for (int i = 0; i < N; i++) {
		int rowWin = 1, colWin = 1;
		for (int j = 0; j < N; j++) {
			if (board[i][j] != symbol) rowWin = 0;
			if (board[j][i] != symbol) colWin = 0;
		}
		if (rowWin || colWin) return 1;
	}

	// Check diagonals
	int diag1 = 1, diag2 = 1;
	for (int i = 0; i < N; i++) {
		if (board[i][i] != symbol) diag1 = 0;
		if (board[i][N - i -1] != symbol) diag2 = 0;
	}
	return diag1 || diag2;
}

// Function to check if the game is a draw
int isDraw() {
	for (int i = 0; i<N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] == ' ') return 0;
	return 1;
}

// Function to get move input from a human player
void getUserMove(int playerIndex) {
	int row, col;
	do {
		printf("Player %d (%c), enter row and column: ", playerIndex + 1, players[playerIndex].symbol);
		scanf("%d %d", &row, &col);
	} while (!isValidMove(row, col));
	updateBoard(row, col, players[playerIndex].symbol);
	logMove(playerIndex, row, col);
}

// Function to generate a random valid move for a computer player
void getComputerMove(int playerIndex) {
	int row, col, attempts = 0;
	do {
		row = rand() % N;
		col = rand() % N;
		attempts++;
		if (attempts > N * N) break; //prevent infinite loop if board is full
	} while (!isValidMove(row, col));
	printf("Player %d (%c) moved at (%d, %d)\n", playerIndex + 1, players[playerIndex].symbol, row, col);
	updateBoard(row, col, players[playerIndex].symbol);
	logMove(playerIndex, row, col);
}

// Function to configure player roles (human or computer) based on game mode
void configurePlayers(int mode) {
	char symbols[MAX_PLAYERS] = {'X', '0', 'Z'};
	for (int i = 0; i < MAX_PLAYERS; i++) {
		players[i].symbol = symbols[i];
		if (mode == 1 && i < 2) {
			players[i].isComputer = 0;   // Mode 1: Two human players
		} else if (mode == 2 && i == 0) {
			players[i].isComputer = 0;   // Mode 2: Player 1 is human
		} else if (mode == 2 && i == 1) {
			players[i].isComputer = 1;   // Mode 2: Player 2 is computer
		} else {

			// Mode 3: Ask user to define each player's role
			printf("Is Player %d (%c) a computer? (1 = Yes, 0 = No): ", i + 1, symbols[i]);
			scanf("%d", &players[i].isComputer);
		}
	}
}

// Main game loop to handle turns, win/draw checks, and player actions
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

// Entry point of the program
int main() {
	srand(time(NULL)); // Initialize random seed for computer moves
	printf("Welcome to Tic-Tac-Toe\n");

	// Get board size from user
	printf("Enter board size N (3-10): ");
	scanf("%d", &N);
	if (N < 3 || N > 10) {
		printf("Invalid board size.\n");
		return 1;
	}

        // Get game mode from user
	int mode;
	printf("Choose mode:\n1. User vs User\n2. User vs Computer\n3. Multi-Player (3 players)\nEnter choice: ");
	scanf("%d", &mode);
	if (mode < 1 || mode > 3) {
		printf("Invalid mode.\n");
		return 1;
	}

	// Initialize board and clear log file
	initializeBoard();
	FILE *fp = fopen(LOG_FILE, "w"); 
	if (fp) fclose(fp);

	// Configure players and start the game
	configurePlayers(mode);
	playGame(mode);

	// Free dynamically allocated memory
	for (int i = 0; i<N; i++) free(board[i]);
	free(board);
	return 0;
}
