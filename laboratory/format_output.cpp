#include <cstdio>
#include <cstdlib>
#include <windows.h>

void gotoxy(short x, short y)
{ //移动光标
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

int main()
{
    // system("clear");
    printf("\033[2J\033[0;0H");
    printf("test_output\n");
    // printf("\033[2J");
    for (int k = 0; k < 10; k++)
    {
        gotoxy(0, 0);
        for (int i = 0; i < 10; i++)
        {

            for (int j = 0; j < 10; j++)
            {
                printf("%d", k);
            }
            Sleep(1000);
            printf("\n");
        }
    }

    system("cls");
    system("pause");
    return 0;
}