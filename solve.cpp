/**
 * @file solve.cpp
 * @author TianqiBi (TianqiBi@outlook.com)
 * @brief Minesweeper solve
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stack>
using namespace std;

const int OFFSET[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
bool solved[100][100];

struct lattice_s
{
    int solve;
    int mine_times;
} note[100][100];

class Minesweeper
{
public:
    int mines, foundblock = 0;
    int X, Y;
    struct lattice
    {
        int mine;
        bool found;
        bool flag;
    } map[100][100];

    Minesweeper(int ix, int iy, int n) //生成
    {
        X = ix;
        Y = iy;
        mines = n;
        int x, y, sum;
        memset(map, 0, sizeof(map));
        // srand((int)time(0));
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
                if (solved[i][j])
                    printf("\033[92m%d \033[0m", map[i][j].mine);
                else
                    printf("%d ", map[i][j].mine);
            }
            printf("\n");
        }
        printf("                                               \n");
        return;
    }
    void solvedprt()
    {
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                printf("%d ", solved[i][j]);
            }
            printf("\n");
        }
        printf("                                               \n");
        return;
    }
    void prt(int x, int y) //输出
    {
        // system("cls");
        // printf("\033[2J\033[0;0H");

        COORD pos = {0, 0}; //移动光标
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);

        for (int i = 0; i < X; i++) //显示
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
        printf("\n");
        testprt();
        return;
    }
};

void stackprt(stack<int> inx, stack<int> iny)
{
    while (!inx.empty())
    {
        printf("%d %d\t", inx.top(), iny.top());
        inx.pop();
        iny.pop();
    }
    printf("                \n");
    return;
}

bool dif(stack<int> inx, stack<int> iny, int x, int y)
{
    while (!inx.empty())
    {
        if (inx.top() == x && iny.top() == y)
            return false;
        inx.pop();
        iny.pop();
    }
    return true;
}

bool check(Minesweeper &a, int x, int y)
{
    // printf("%d %d\n", x, y);
    if (solved[x][y] || !a.map[x][y].found || a.map[x][y].mine == 0)
        return false;

    int flagnum = 0, unfoundnum = 0;
    for (int i = 0; i < 8; i++) //统计未知格与旗帜数
    {
        if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y)
        {
            if (!a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
                unfoundnum++;
            if (a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag)
                flagnum++;
        }
    }
    if (unfoundnum != a.map[x][y].mine && flagnum != a.map[x][y].mine)
        return false;
    if (unfoundnum == a.map[x][y].mine) //标记周围
    {
        for (int i = 0; i < 8; i++)
        {
            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && !a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found && !a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag)
            {
                a.flag(x + OFFSET[i][0], y + OFFSET[i][1]);
            }
        }
    }
    if (flagnum == a.map[x][y].mine)
    {
        a.doubleclick(x, y);
    }
    solved[x][y] = true;
    return true;
}

void load(int x, int y, stack<int> &inx, stack<int> &iny, Minesweeper &a, bool islarge)
{
    // a.prt(x,y);
    // stackprt(inx,iny);

    if (solved[x][y] || !a.map[x][y].found)
        return;

    if (!a.map[x][y].mine) //空格
    {
        // printf("location %d %d 0      \n",x,y);
        solved[x][y] = true;
        for (int i = 0; i < 8; i++)
        {
            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
            {
                load(x + OFFSET[i][0], y + OFFSET[i][1], inx, iny, a, false);
            }
        }
    }
    else //数字
    {
        if (islarge) //存入25格数字
        {
            for (int i = 0; i < 7; i += 2)
            {
                if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        load(x + OFFSET[j][0], y + OFFSET[j][1], inx, iny, a, false);
                    }
                }
            }
        }
        if (dif(inx, iny, x, y)) //存入9格数字
        {
            inx.push(x);
            iny.push(y);
        }
    }
    return;
}

void load(stack<int> indexx, stack<int> indexy, stack<int> &outx, stack<int> &outy, Minesweeper &a)
{
    int x, y;
    while (!indexx.empty())
    {
        x = indexx.top();
        y = indexy.top();
        indexx.pop();
        indexy.pop();
        for (int i = 0; i < 8; i++)
        {
            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && !a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found && !a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag)
            {
                if (dif(outx, outy, x + OFFSET[i][0], y + OFFSET[i][1]))
                {
                    outx.push(x + OFFSET[i][0]);
                    outy.push(y + OFFSET[i][1]);
                }
            }
        }
    }
    return;
}

void primary_solve(Minesweeper &a)
{
    stack<int> qx, qy;
    int tx, ty;
    load(0, 0, qx, qy, a, true); //首次存入
    while (!qx.empty())
    {
        tx = qx.top();
        ty = qy.top();
        qx.pop();
        qy.pop();
        if (check(a, tx, ty))
        {
            solved[tx][ty] = true;
            for (int i = 0; i < 8; i++)
            {
                if (tx + OFFSET[i][0] >= 0 && tx + OFFSET[i][0] < a.X && ty + OFFSET[i][1] >= 0 && ty + OFFSET[i][1] < a.Y && a.map[tx + OFFSET[i][0]][ty + OFFSET[i][1]].found)
                {
                    load(tx + OFFSET[i][0], ty + OFFSET[i][1], qx, qy, a, true);
                }
            }
        }
        // a.prt(tx, ty);
        // stackprt(qx, qy);
    }
    return;
}

void build(int m, int n, stack<int> mx, stack<int> my, stack<int> mine_x, stack<int> mine_y, stack<int> qx, stack<int> qy, Minesweeper &a)
{
    if (m <= n || n == 0)
    {
        int map[100][100];
        while (!mine_x.empty()) //标记
        {
            map[mine_x.top()][mine_y.top()] = 9;
            mine_x.pop();
            mine_y.pop();
        }
        for (int x = 0; x < a.X; x++)
        {
            for (int y = 0; y < a.Y; y++)
            {
                if (a.map[x][y].flag)
                    map[x][y] = 9;
            }
        }
        for (int x = 0; x < a.X; x++) //标数
        {
            for (int y = 0; y < a.Y; y++)
            {
                if (map[x][y] == 9)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && map[x + OFFSET[i][0]][y + OFFSET[i][1]] != 9)
                            map[x + OFFSET[i][0]][y + OFFSET[i][1]]++;
                    }
                }
            }
        }
        while (!qx.empty())
        {
            if (a.map[qx.top()][qy.top()].mine != map[qx.top()][qy.top()])
                return;
            else
            {
                qx.pop();
                qy.pop();
            }
        }
        while (!mx.empty())
        {
            note[mx.top()][my.top()].solve++;
            if (map[mx.top()][my.top()] == 9)
                note[mx.top()][my.top()].mine_times++;
            mx.pop();
            my.pop();
        }
        return;
    }
    build(m - 1, n, mx, my, mine_x, mine_y, qx, qy, a);
    mine_x.push(mx.top());
    mine_y.push(my.top());
    mx.pop();
    my.pop();
    build(m, n - 1, mx, my, mine_x, mine_y, qx, qy, a);
    return;
}

void intermediate_solve(Minesweeper a)
{
    stack<int> qx, qy, mx, my, mine_x, mine_y;

    int m_num, M = 0;
    for (int i = 0; i < a.X; i++)
    {
        for (int j = 0; j < a.Y; j++)
        {
            if (a.map[i][j].found && !solved[i][j])
            {
                qx.push(i);
                qy.push(j);
            }
        }
    }
    load(qx, qy, mx, my, a);
    m_num = mx.size();

    stackprt(mx, my);

    while (M <= m_num)
    {
        build(m_num, M, mx, my, mine_x, mine_y, qx, qy, a);
        M++;
        a.prt(0, 0);
        for (int i = 0; i < a.X; i++)
        {
            for (int j = 0; j < a.Y; j++)
            {
                printf("%d ", note[i][j].mine_times);
            }
            printf("\n");
        }
        printf("\n");
    }

    for (int i = 0; i < a.X; i++)
    {
        for (int j = 0; j < a.Y; j++)
        {
            printf("%d ", note[i][j].mine_times);
        }
        printf("\n");
    }
    printf("\n");

    return;
}

int main()
{
    system("chcp 65001");
    //system("cls");
    printf("\033[2J\033[0;0H");
    char c;
    while (true)
    {
        memset(solved, 0, sizeof(solved));
        // c = getch();
        Minesweeper *a = new Minesweeper(15, 15, 35);
        // a->prt(0, 0);
        if (a->click(0, 0) == -1)
        {
            printf("Fail!\n");
        }
        else
        {
            a->prt(0, 0);
            primary_solve(*a);
            a->prt(0, 0);
            // printf("Solved!\n");
            if (a->foundblock + a->mines != a->X * a->Y)
            {
                intermediate_solve(*a);
            }
        }
        Sleep(20);
        delete a;
    }

    system("pause");
    return 0;
}