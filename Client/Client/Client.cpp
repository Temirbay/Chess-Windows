/*
	Client file
	Coded by: Miras Temirbay
	All rights copyrighted (c)
*/
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

# define N 8

int board[9][9];
int whoIsFirst;

SOCKET connection;

bool IsAlpha(char c) {
	int num = c;
	return (97 <= num && num <= 122) || (65 <= num && num <= 90);
}

bool IsDigit(char c) {
	int num = c;
	return 48 <= num && num <= 57;
}


int differs(char a, char b) {
	int num1 = a;
	int num2 = b;
	int case1, case2;
	if (65 <= num1 && num1 <= 90) case1 = 1;
	else case1 = 2;

	if (65 <= num2 && num2 <= 90) case2 = 1;
	else case2 = 2;

	return case1 + case2 == 3;
}


void printInstructions() {
	printf("P - Player 1 Pawn\n");
	printf("R - Player 1 Rook\n");
	printf("K - Player 1 Knight\n");
	printf("B - Player 1 Bishop\n");
	printf("Q - Player 1 Queen\n");
	printf("W - Player 1 King\n");

	printf("p - Player 2 Pawn\n");
	printf("r - Player 2 Rook\n");
	printf("k - Player 2 Knight\n");
	printf("b - Player 2 Bishop\n");
	printf("q - Player 2 Queen\n");
	printf("w - Player 2 King\n\n");

	printf("Player should enter initial and final\ncoordinates of the figure\n");
	printf("Like - H1-G3\n\n");
}

void printMenu() {
	printf("Press 1 - Start a game\n");
	printf("Press 2 - Print instructions\n");
	printf("Press 3 - Exit\n");

	int choose;
	cin >> choose;

	if (choose == 1) return;
	if (choose == 2) {
		printInstructions();
		printMenu();
	}
	if (choose == 3) {
		printf("Good Bye!!!\n");
		exit(0);
	}
}


int checkForCheck() {
	int king_row = 8, king_column = 8;
	int ok = 1;
	for (int i = 1; i <= 8; ++i) {
		for (int j = 1; j <= 8; ++j)
			if (board[i][j] == 'W' && whoIsFirst == 0) {
				king_row = i;
				king_column = j;
				ok = 0;
				break;
			}
			else if (board[i][j] == 'w' && whoIsFirst == 1) {
				king_row = i;
				king_column = j;
				ok = 0;
				break;
			}
			if (ok == 0) break;
	}

	int k;
	int dxx[8] = { 2, 2, 1, 1, -1, -1, -2, -2 };
	int dyy[8] = { 1, -1, 2, -2, -2, 2, 1, -1 };
	for (k = 0; k < 8; ++k) {
		int x = king_row + dxx[k];
		int y = king_column + dyy[k];
		if (1 > x || x > 8 || 1 > y || y > 8) continue;
		if (board[x][y] == 'K' || board[x][y] == 'k')
			if (differs(board[x][y], board[king_row][king_column]) == 1)
				return 1;

	}


	int dx[8] = { 1, 1, -1, -1, 1, -1, 0, 0 };
	int dy[8] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	for (k = 0; k < 8; ++k) {
		int x = king_row, y = king_column;

		for (int i = 1; i <= 8; ++i) {
			int xx = x + dx[k] * i;
			int yy = y + dy[k] * i;

			if (1 > xx || xx > 8 || 1 > yy || yy > 8)
				break;


			if (board[xx][yy] != '.' && differs(board[xx][yy], board[x][y]) == 0) break;

			if (xx == king_row || yy == king_column) {
				if (board[xx][yy] == 'Q' || board[xx][yy] == 'q')
					if (differs(board[xx][yy], board[x][y]) == 1)
						return 1;

				if (board[xx][yy] == 'R' || board[xx][yy] == 'r')
					if (differs(board[xx][yy], board[x][y]) == 1)
						return 1;

				if (board[xx][yy] == 'W' || board[xx][yy] == 'w')
					if (differs(board[xx][yy], board[x][y]) == 1 && i == 1)
						return 1;
			}

			if (abs(dx[k]) + abs(dy[k]) == 2) {
				if (board[xx][yy] == 'B' || board[xx][yy] == 'b')
					if (differs(board[xx][yy], board[x][y]) == 1)
						return 1;

				if (board[xx][yy] == 'Q' || board[xx][yy] == 'q')
					if (differs(board[xx][yy], board[x][y]) == 1)
						return 1;

				if (board[xx][yy] == 'W' || board[xx][yy] == 'w')
					if (differs(board[xx][yy], board[x][y]) == 1 && i == 1)
						return 1;

				if (whoIsFirst == 1 && xx > x && i == 1) {
					if (board[xx][yy] == 'P' || board[xx][yy] == 'p')
						if (differs(board[xx][yy], board[x][y]) == 1)
							return 1;
				}

				if (whoIsFirst == 0 && xx < x && i == 1) {
					if (board[xx][yy] == 'P' || board[xx][yy] == 'p')
						if (differs(board[xx][yy], board[x][y]) == 1)
							return 1;
				}

				int dxx[8] = { 2, 2, 1, 1, -1, -1, -2, -2 };
				int dyy[8] = { 1, -1, 2, -2, 2, -2, 1, -1 };

				if (board[xx][yy] != '.') break;
			}
		}
	}

	return 0;
}

int checkForMate() {
	int king_row, king_column;
	int ok = 1, i, j;
	for (i = 1; i <= 8; ++i) {
		for (j = 1; j <= 8; ++j)
			if (board[i][j] == 'W' && whoIsFirst == 1) {
				king_row = i;
				king_column = j;
				ok = 0;
				break;
			}
			else if (board[i][j] == 'w' && whoIsFirst == 0) {
				king_row = i;
				king_column = j;
				ok = 0;
				break;
			}
			if (ok == 0) break;
	}

	int dx[8] = { 1, 1, -1, -1, 1, -1, 0, 0 };
	int dy[8] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	int k;
	for (k = 0; k < 8; ++k) {
		int x = king_row + dx[k];
		int y = king_column + dy[k];
		if (1 > x || x > 8 || 1 > y || y > 8) continue;
		if (board[x][y] != '.') continue;
		char old_figure = board[x][y];
		board[x][y] = board[king_row][king_column];
		board[king_row][king_column] = '.';

		if (checkForCheck() == 0) {
			board[king_row][king_column] = board[x][y];
			board[x][y] = old_figure;
			return 0;
		}

		board[king_row][king_column] = board[x][y];
		board[x][y] = old_figure;
	}

	return 1;
}

int validate(char event[100]) {
	int from_row, from_column, to_row, to_column;
	int first_alpha = 0, first_digit = 0;

	for (int i = 0; i < 5; ++i) {
		if (IsAlpha(event[i])) {
			if (first_alpha == 0) { from_column = (int)event[i] - 96; first_alpha = 1; }
			if (first_alpha == 1) { to_column = (int)event[i] - 96; }
		}
		else if (IsDigit(event[i])) {
			if (first_digit == 0) { from_row = (int)event[i] - 48; first_digit = 1; }
			if (first_digit == 1) { to_row = (int)event[i] - 48; }
		}
	}
	char ch = board[from_row][from_column];

	int team = 0;
	if (65 <= (int)ch && (int)ch <= 90) team = 1;
	if (97 <= (int)ch && (int)ch <= 122) team = 2;
	if (team == 0) {
		printf("You need to choose a figure\n");
		return 0;
	}

	if (team == 1) {
		if (whoIsFirst % 2 == 1) {
			printf("It is not your figure\n");
			return 0;
		}
	}

	if (team == 2) {
		if (whoIsFirst % 2 == 0) {
			printf("It is not your figure\n");
			return 0;
		}
	}

	int copyBoard[9][9], i, j;
	for (i = 1; i <= 8; ++i)
		for (j = 1; j <= 8; ++j)
			copyBoard[i][j] = 0;

	if (ch == 'k' || ch == 'K') {
		int dx[8] = { 1, 2, -1, 2, 1, -2, -1, -2 };
		int dy[8] = { 2, 1, 2, -1, -2, 1, -2, -1 };
		int k;
		for (k = 0; k <= 8; ++k) {
			int x = from_row + dx[k];
			int y = from_column + dy[k];
			if (1 > x || x > 8 || 1 > y || y > 8) continue;
			if (differs(board[from_row][from_column], board[x][y]) || board[x][y] == '.')
				copyBoard[x][y] = 1;
		}
		if (copyBoard[to_row][to_column] != 1) {
			printf("Knight's step is incorrect\n");
			return 0;
		}
	}

	if (ch == 'P' || ch == 'p') {


		int dx_team1[3] = { 1, 1, 1 };
		int dy_team1[3] = { 0, 1, -1 };

		int dx_team2[3] = { -1, -1, -1 };
		int dy_team2[3] = { 0, 1, -1 };

		if (team == 1) {
			int k;
			for (k = 0; k < 3; ++k) {
				int x = from_row + dx_team1[k];
				int y = from_column + dy_team1[k];
				if (1 > x || x > 8 || 1 > y || y > 8) continue;
				if (k == 0) {
					if (board[x][y] == '.') copyBoard[x][y] = 1;
				}
				else {
					if (board[x][y] != '.' && differs(board[from_row][from_column], board[x][y])) copyBoard[x][y] = 1;
				}

			}

			if (from_row == 2 && board[from_row + 1][from_column] == '.')
				copyBoard[from_row + 2][from_column] = 1;
		}
		else {
			int k;
			for (k = 0; k < 3; ++k) {
				int x = from_row + dx_team2[k];
				int y = from_column + dy_team2[k];
				if (1 > x || x > 8 || 1 > y || y > 8) continue;
				if (k == 0) {
					if (board[x][y] == '.') copyBoard[x][y] = 1;
				}
				else {
					if (board[x][y] != '.' && differs(board[from_row][from_column], board[x][y])) copyBoard[x][y] = 1;
				}

			}

			if (from_row == 7 && board[from_row - 1][from_column] == '.')
				copyBoard[from_row - 2][from_column] = 1;
		}

		if (copyBoard[to_row][to_column] != 1) {
			printf("Pawn's step is incorrect\n");
			return 0;
		}
	}
	if (ch == 'r' || ch == 'R') {
		for (int i = from_row + 1; i <= 8; ++i) {
			if (board[i][from_column] != '.') {
				if (differs(board[from_row][from_column], board[i][from_column]))
					copyBoard[i][from_column] = 1;
				break;
			}
			else copyBoard[i][from_column] = 1;
		}

		for (int i = from_row - 1; i >= 1; --i) {
			if (board[i][from_column] != '.') {
				if (differs(board[from_row][from_column], board[i][from_column]))
					copyBoard[i][from_column] = 1;
				break;
			}
			else copyBoard[i][from_column] = 1;
		}

		for (int j = from_column + 1; j <= 8; ++j) {
			if (board[from_row][j] != '.') {
				if (differs(board[from_row][from_column], board[from_row][j]))
					copyBoard[from_row][j] = 1;
				break;
			}
			else copyBoard[from_row][j] = 1;
		}

		for (int j = from_column - 1; j >= 1; --j) {
			if (board[from_row][j] != '.') {
				if (differs(board[from_row][from_column], board[from_row][j]))
					copyBoard[from_row][j] = 1;
				break;
			}
			else copyBoard[from_row][j] = 1;
		}

		if (copyBoard[to_row][to_column] == 0) {
			printf("Rock's step is incorrect\n");
			return 0;
		}
	}

	if (ch == 'b' || ch == 'B') {
		int dx[4] = { 1, 1, -1, -1 };
		int dy[4] = { 1, -1, 1, -1 };
		for (int k = 0; k <= 4; ++k) {
			int ii = from_row, jj = from_column;
			while (1) {
				int x = ii + dx[k];
				int y = jj + dy[k];
				if (1 > x || x > 8 || 1 > y || y > 8) break;
				if (board[x][y] != '.') {
					if (differs(board[from_row][from_column], board[x][y]))
						copyBoard[x][y] = 1;
					break;
				}
				copyBoard[x][y] = 1;
				ii = x; jj = y;
			}
		}

		if (copyBoard[to_row][to_column] == 0) {
			printf("Bishop's step is incorrect\n");
			return 0;
		}
	}

	if (ch == 'q' || ch == 'Q') {

		int dx[4] = { 1, 1, -1, -1 };
		int dy[4] = { 1, -1, 1, -1 };
		for (int k = 0; k <= 4; ++k) {
			int ii = from_row, jj = from_column;
			while (1) {
				int x = ii + dx[k];
				int y = jj + dy[k];
				if (1 > x || x > 8 || 1 > y || y > 8) break;
				if (board[x][y] != '.') {
					if (differs(board[from_row][from_column], board[x][y]))
						copyBoard[x][y] = 1;
					break;
				}
				copyBoard[x][y] = 1;
				ii = x; jj = y;
			}
		}

		for (int i = from_row + 1; i <= 8; ++i) {
			if (board[i][from_column] != '.') {
				if (differs(board[from_row][from_column], board[i][from_column]))
					copyBoard[i][from_column] = 1;
				break;
			}
			else copyBoard[i][from_column] = 1;
		}

		for (int i = from_row - 1; i >= 1; --i) {
			if (board[i][from_column] != '.') {
				if (differs(board[from_row][from_column], board[i][from_column]))
					copyBoard[i][from_column] = 1;
				break;
			}
			else copyBoard[i][from_column] = 1;
		}

		for (int j = from_column + 1; j <= 8; ++j) {
			if (board[from_row][j] != '.') {
				if (differs(board[from_row][from_column], board[from_row][j]))
					copyBoard[from_row][j] = 1;
				break;
			}
			else copyBoard[from_row][j] = 1;
		}

		for (int j = from_column - 1; j >= 1; --j) {
			if (board[from_row][j] != '.') {
				if (differs(board[from_row][from_column], board[from_row][j]))
					copyBoard[from_row][j] = 1;
				break;
			}
			else copyBoard[from_row][j] = 1;
		}

		if (copyBoard[to_row][to_column] == 0) {
			printf("Queen's step is incorrect\n");
			return 0;
		}
	}

	if (ch == 'W' || ch == 'w') {
		int dx[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
		int dy[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
		for (int k = 0; k < 8; ++k) {
			int x = from_row + dx[k];
			int y = from_column + dy[k];
			if (1 > x || x > 8 || 1 > y || y > 8) continue;
			if (board[x][y] != '.') {
				if (differs(board[from_row][from_column], board[x][y]))
					copyBoard[x][y] = 1;
			}
			else copyBoard[x][y] = 1;
		}

		if (copyBoard[to_row][to_column] == 0) {
			printf("King's step is incorrect\n");
			return 0;
		}
	}

	char newFigure = board[from_row][from_column];
	char oldFigure = board[to_row][to_column];

	board[to_row][to_column] = newFigure;
	board[from_row][from_column] = '.';

	if (checkForCheck() == 1) {
		board[to_row][to_column] = oldFigure;
		board[from_row][from_column] = newFigure;
		printf("King is in danger zone\n");
		return 0;
	}

	board[to_row][to_column] = oldFigure;
	board[from_row][from_column] = newFigure;

	return 1;
}

void changeBoard(char event[100]) {
	int from_row, from_column, to_row, to_column;
	int first_alpha = 0, first_digit = 0;
	for (int i = 0; i < 5; ++i) {
		if (IsAlpha(event[i])) {
			if (first_alpha == 0) { from_column = (int)event[i] - 96; first_alpha = 1; }
			if (first_alpha == 1) { to_column = (int)event[i] - 96; }
		}
		else if (IsDigit(event[i])) {
			if (first_digit == 0) { from_row = (int)event[i] - 48; first_digit = 1; }
			if (first_digit == 1) { to_row = (int)event[i] - 48; }
		}
	}


	char ch = board[from_row][from_column];
	board[from_row][from_column] = '.';
	board[to_row][to_column] = ch;
}

void printBoard() {
	if (whoIsFirst == 1) {
		for (int i = 1; i <= N; ++i)
			for (int j = 1; j <= N; ++j) {
				printf("%c ", board[i][j]);
				if (j == N) printf(" %d\n", i);
			}
	}
	else {
		for (int i = N; i >= 1; --i)
			for (int j = 1; j <= N; ++j) {
				printf("%c ", board[i][j]);
				if (j == N) printf(" %d\n", i);
			}
	}
	printf("\n");
	for (int i = 1; i <= N; ++i) {
		char ch = i + 64;
		printf("%c ", ch);
	}

	printf("\n\n");
	printf("--------------------------------------\n");
}

void initBoard () {
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= N; ++j)
			board[i][j] = '.';

	board[1][1] = board[1][8] = 'R';
	board[1][2] = board[1][7] = 'K';
	board[1][3] = board[1][6] = 'B';
	board[1][4] = 'W';
	board[1][5] = 'Q';


	board[8][1] = board[8][8] = 'r';
	board[8][2] = board[8][7] = 'k';
	board[8][3] = board[8][6] = 'b';
	board[8][4] = 'w';
	board[8][5] = 'q';

	for (int i = 1; i <= N; ++i) {
		board[2][i] = 'P';
		board[7][i] = 'p';
	}
}

int main()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) 
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connection == INVALID_SOCKET)
	{
		MessageBoxA(NULL, "Failed to create socket", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (connect(connection, (SOCKADDR*)&addr, addrlen))
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	cout << "Connected. " << endl;
	
	initBoard();

	char buffer[256];
	recv(connection, buffer, 256, NULL);
	whoIsFirst = buffer[strlen(buffer) - 1] == '.';
	cout << buffer << endl;

	bool firstStarted = false, secondStarted = false;

	if (whoIsFirst == 1)
	{
		while (1)
		{
			if (firstStarted == false) {
				printMenu();
				firstStarted = true;
			}

			printBoard(); 
			do {
				printf("Enter an event:\n");
				cin >> buffer;
			} while (validate(buffer) == 0);
			
			changeBoard(buffer);

			send(connection, buffer, 1024, 0);

			if (strcmp(buffer, ":exit") == 0)
			{
				closesocket(connection);
				cout << "Disconnected from server " << endl; 
				return 0;
			}
			if (recv(connection, buffer, 1024, 0) < 0)
			{
				cout << "Error in receiving data" << endl;
			}
			else {
				cout << "Opponent: " << buffer << endl;
				changeBoard(buffer);
			}
		}
	}
	else {
		while (1)
		{
			if (secondStarted == false) {
				printMenu();
				secondStarted = true;
			}

			if (recv(connection, buffer, 1024, 0) < 0) {       
				cout << "Error in receiving data." << endl;
			}
			else {
				cout << "Opponent: " << buffer << endl;
				changeBoard(buffer);
			}


			printBoard();

			do {
				printf("Enter an event:\n");
				cin >> buffer;
			} while (validate(buffer) == 0);

			changeBoard(buffer);

			send(connection, buffer, 1024, 0);

			if (strcmp(buffer, ":exit") == 0) {
				closesocket(connection);
				cout << "Disconnected from server." << endl;
				return 0;
			}
		}
	}

	system("pause");
    return 0;
}

