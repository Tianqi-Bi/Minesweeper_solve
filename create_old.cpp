#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <windows.h>
#include <conio.h>

const int FieldX = 10;
const int FieldY = 10;
int minenumber,  minefound,  localx,  localy;
bool end,  win;

struct lattice {
	int mine;
	bool found;
	bool flag;
}field[FieldX][FieldY];

void firstset();
void testout();
void out();
bool check(int, int);
void userui();

int main() {
	
	system("chcp 65001");
    system("cls");

	int in;
	//userui();
	firstset();
	//testout();
	//Sleep(3000);
	out();

	//printf("\033[0m");恢复
	//printf("\033[92m■");绿色
	//printf("\033[2J");清屏
	//printf("\033[1m■");亮
	//119 115 97 100 106 107 108
	/*
	for (int i = 0; i < 10; i++)
	{
		system("cls");
		testout();
		Sleep(1000);
	}
	*/

	while (!end) {
		in = getch();
		//printf("%d", in);
		switch (in)
		{
		case 119:
			if (localx > 0) {
				localx--;
				out();
			}
			break;
		case 115:
			if (localx < FieldX - 1) {
				localx++;
				out();
			}
			break;
		case 97:
			if (localy > 0) {
				localy--;
				out();
			}
			break;
		case 100:
			if (localy < FieldY - 1) {
				localy++;
				out();
			}
			break;
		case 106:
			if (!field[localx][localy].flag) {
				end = check(localx, localy);
			}
			break;
		case 107:
			field[localx][localy].flag = !field[localx][localy].flag;
			out();
			break;
		case 108:

			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}

void firstset() {
	int a, b, tag;
	srand((int)time(0));
	for (int i = 0; i < minenumber; i++) {
		a = rand() % FieldX;
		b = rand() % FieldY;
		if (field[a][b].mine != 9) {
			field[a][b].mine = 9;
		}
		else {
			i--;
		}
	}
	for (int i = 0; i < FieldX; i++) {
		for (int j = 0; j < FieldY; j++) {
			tag = 0;
			if (field[i][j].mine == 9)
				continue;
			if (i - 1 >= 0 && j - 1 >= 0 && field[i - 1][j - 1].mine == 9)
				tag++;
			if (j - 1 >= 0 && field[i][j - 1].mine == 9)
				tag++;
			if (i + 1 < FieldX && j - 1 >= 0 && field[i + 1][j - 1].mine == 9)
				tag++;
			if (i - 1 >= 0 && field[i - 1][j].mine == 9)
				tag++;
			if (i + 1 < FieldX && field[i + 1][j].mine == 9)
				tag++;
			if (i - 1 >= 0 && j + 1 < FieldY && field[i - 1][j + 1].mine == 9)
				tag++;
			if (j + 1 < FieldY && field[i][j + 1].mine == 9)
				tag++;
			if (i + 1 < FieldX && j + 1 < FieldY && field[i + 1][j + 1].mine == 9)
				tag++;
			field[i][j].mine = tag;
		}
	}
}

void testout() {
	for (int i = 0; i < FieldX; i++) {
		for (int j = 0; j < FieldY; j++) {
			printf("%d ", field[i][j].mine);
		}
		printf("\n");
	}
}
//■

void out() {
	//system("cls");
	printf("\033[2J\033[0;0H");
	for (int i = 0; i < FieldX; i++) {
		for (int j = 0; j < FieldY; j++) {
			if (i == localx && j == localy) {
				if (field[i][j].found) {
					if (field[i][j].mine == 9) {
						printf("\033[91m×");
						printf("\033[0m");
					}
					else {
						printf("\033[92m%d ", field[i][j].mine);
						printf("\033[0m");
					}
				}
				else {
					printf("\033[92m■");
					printf("\033[0m");
				}
			}
			else {
					if (field[i][j].found) {
						printf("%d ", field[i][j].mine);
					}
					else {
						printf("■");
					}
			}
		}
		printf("\n");
	}
}

bool check(int x, int y) {
	if (field[x][y].found) {
		return false;
	}
	if (field[x][y].mine == 9) {
		field[x][y].found = true;
		out();
		win = false;
		return true;
	}
	if (++minefound == FieldX * FieldY - minenumber) {
		field[x][y].found = true;
		out();
		win = true;
		return true;
	}
	if (field[x][y].mine) {
		field[x][y].found = true;
		out();
	}
	else {
		field[x][y].found = true;
		if (x - 1 >= 0) {
			check(x - 1, y);
		}
		if (x + 1 < FieldX) {
			check(x + 1, y);
		}
		if (y - 1 >= 0) {
			check(x, y - 1);
		}
		if (y + 1 < FieldY) {
			check(x, y + 1);
		}
	}
	return false;
}

void userui() {
	//printf("\033[92m");
	printf("\t\t扫雷\n\t\t\t1.1.0\n");
	printf("%d行\t%d列\n", FieldX, FieldY);
	//printf("\033[93m");
	printf("移动 W S A D 点击 J 标记 K 和弦 L\n");
	//printf("\033[0m");
	printf("地雷数:");
	scanf("%d", &minenumber);
	system("cls");
}