#include <cstdio>
#include<windows.h>

void gotoxy(short x, short y) {					//移动光标
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

int main()
{
    printf("test_output\n");
    //printf("\033[2J");
    gotoxy(0,0);
    for (int i = 0; i < 50; i++)
    {

        for (int j = 0; j < 10; j++)
        {
            printf("test_output\n");
        }
        gotoxy(0,0);
        
    }
    
    system("cls");
    system("pause");
    return 0;
}