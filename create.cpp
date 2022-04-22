#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <windows.h>
#include <conio.h>

class Minesweeper
{
private:
    const int OFFSET[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int X, Y, mines, foundblock = 0;
    struct lattice
    {
        int mine;
        bool found;
        bool flag;
    } map[100][100];

public:
    Minesweeper(int ix, int iy, int n) //生成
    {
        system("chcp 65001");
        system("cls");
        X = ix;
        Y = iy;
        mines = n;
        int x, y, sum;
        memset(map, 0, sizeof(map));
        srand((int)time(0));
        for (int i = 0; i < mines; i++)
        {
            x = rand() % X;
            y = rand() % Y;
            if (map[x][y].mine != 9)
            {
                map[x][y].mine = 9;
                //标记其他方格
                for (int j = 0; j < 8; j++)
                {
                    if (x + OFFSET[j][0] >= 0 && x + OFFSET[j][0] < X && y + OFFSET[j][1] >= 0 && y + OFFSET[j][1] < Y && map[x + OFFSET[j][0]][y + OFFSET[j][1]].mine != 9)
                        map[x + OFFSET[j][0]][y + OFFSET[j][1]].mine++;
                }
            }
            else
                i--;
        }
    }
    int click(int x, int y) // 点击/检查
    {                       // return 0 正常 -1 失败 1 胜利
        if (map[x][y].found || map[x][y].flag)
            return 0;
        map[x][y].found = true;
        if (map[x][y].mine == 9)
            return -1;
        if (++foundblock == X * Y - mines)
            return 1;
        if (!map[x][y].mine) //若为空
        {
            for (int i = 0; i < 8; i++)
            {
                if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y)
                    click(x + OFFSET[i][0], y + OFFSET[i][1]);
            }
        }
        return 0;
    }
    int doubleclick(int &x, int &y) //双击
    {
        if (!map[x][y].found || map[x][y].flag)
            return 0;
        int sum = 0, rt = 0;
        for (int i = 0; i < 8; i++)
        {
            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y && map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag)
                sum++;
        }
        if (sum == map[x][y].mine)
        {
            for (int i = 0; i < 8; i++)
            {
                if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y && !map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag && !map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
                {
                    rt = click(x + OFFSET[i][0], y + OFFSET[i][1]);
                    if (rt == -1)
                    {
                        x += OFFSET[i][0];
                        y += OFFSET[i][1];
                        return -1;
                    }
                    else if (rt == 1)
                        return 1;
                }
            }
        }
        return 0;
    }
    void flag(int x, int y) //标记
    {
        if (map[x][y].flag)
            map[x][y].flag = false;
        else if (!map[x][y].found)
            map[x][y].flag = true;
        return;
    }
    void testprt() //输出
    {
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                printf("%d ", map[i][j].mine);
            }
            printf("\n");
        }
        return;
    }
    void prt(int x, int y) //输出
    {
        // system("cls");
        // printf("\033[2J\033[0;0H");

        COORD pos = {0, 0}; //移动光标
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);

        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                if (i == x && j == y) //当前位置
                {
                    if (map[i][j].found)
                    {
                        if (map[i][j].mine == 9)
                            printf("\033[91m×\033[0m");
                        else
                            printf("\033[92m%d \033[0m", map[i][j].mine);
                    }
                    else
                        printf("\033[92m■\033[0m");
                }
                else
                {
                    if (map[i][j].found)
                        printf("%d ", map[i][j].mine);
                    else if (map[i][j].flag)
                        printf("\033[96m■\033[0m");
                    else
                        printf("■");
                }
            }
            printf("\n");
        }
    }
};

int main()
{
    int X, Y, num;
    scanf("%d %d %d", &X, &Y, &num);
    system("cls");
    Minesweeper *a = new Minesweeper(X, Y, num);

    // a.testprt();
    //  a.click(0, 0);
    int x = 0, y = 0, in, end = 0;

    a->prt(0, 0);
    while (!end)
    {
        in = getch();
        // printf("%d", in);
        switch (in)
        {
        case 119:
            if (x > 0)
                a->prt(--x, y);
            break;
        case 115:
            if (x < X - 1)
                a->prt(++x, y);
            break;
        case 97:
            if (y > 0)
                a->prt(x, --y);
            break;
        case 100:
            if (y < Y - 1)
                a->prt(x, ++y);
            break;
        case 106:
            end = a->click(x, y);
            a->prt(x, y);
            break;
        case 107:
            a->flag(x, y);
            a->prt(x, y);
            break;
        case 108:
            end = a->doubleclick(x, y);
            a->prt(x, y);
            break;
        default:
            break;
        }
        if (end == -1)
        {
            printf("FAILED!\n");
            break;
        }
        else if (end == 1)
        {
            printf("WIN!\n");
            break;
        }
    }
    // a->testprt();
    delete a;
    system("pause");
    return 0;
}