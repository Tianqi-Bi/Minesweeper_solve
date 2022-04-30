/**
 * @file solve.cpp
 * @author TianqiBi (TianqiBi@outlook.com)
 * @brief Minesweeper solve
 * @version 0.2
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
bool solved[100][100], draft[100][100];
int note[100][100], influence[100][100];

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
        srand((int)time(0));
        if (mines >= X * Y)
        {
            printf("Error\n");
            exit(0);
        }
        for (int i = 0; i < mines; i++)
        {
            x = rand() % X;
            y = rand() % Y;
            if (map[x][y].mine != 9)
            {
                map[x][y].mine = 9;
                for (int j = 0; j < 8; j++) //标记其他方格
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
        if (map[x][y].mine == 9)
            return -1;
        map[x][y].found = true;
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
        /*
        printf("map:\n");
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                printf("%d ", draft[i][j]);
            }
            printf("\n");
        }
        printf("influence:\n");
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                printf("%d ", influence[i][j]);
            }
            printf("\n");
        }
        */
        printf("\nMine times:\n");
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                if (map[i][j].mine == 9)
                    printf("\033[91m%d \033[0m", note[i][j]);
                else
                    printf("%d ", note[i][j]);
            }
            printf("\n");
        }
        printf("\n");
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

int check(Minesweeper &a, int x, int y) // return 0 无变化 1 有变化 2 胜利
{
    if (solved[x][y] || !a.map[x][y].found || a.map[x][y].mine == 0)
        return 0;

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
        return 0;
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
        if (a.doubleclick(x, y) == 1)
            return 2;
    }
    solved[x][y] = true;
    return 1;
}

void load(int x, int y, stack<int> &inx, stack<int> &iny, Minesweeper &a, bool islarge)
{
    if (solved[x][y] || !a.map[x][y].found)
        return;

    if (!a.map[x][y].mine) //空格
    {
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

bool primary_solve(int startx, int starty, Minesweeper &a)
{
    stack<int> qx, qy;
    int tx, ty, rt;
    load(startx, starty, qx, qy, a, true); //首次存入
    while (!qx.empty())
    {
        tx = qx.top();
        ty = qy.top();
        qx.pop();
        qy.pop();
        rt = check(a, tx, ty);
        if (rt == 2)
            return true;
        else if (rt)
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
    return false;
}

bool a_check(stack<int> tx, stack<int> ty, Minesweeper &a)
{
    int num;
    while (!tx.empty())
    {
        num = 0;
        for (int i = 0; i < 8; i++)
        {
            if (draft[tx.top() + OFFSET[i][0]][ty.top() + OFFSET[i][1]])
                num++;
        }
        if (a.map[tx.top()][ty.top()].mine != num)
            return false;
        else
        {
            tx.pop();
            ty.pop();
        }
    }
    return true;
}

int build(int m, int n, int x, int y, stack<int> mx, stack<int> my, Minesweeper &a)
{
    if (m < n || n < 0)
        return 0;
    if (m == 0)
        return 1;
    stack<int> tx, ty;
    int tmp, s1 = 0, s2 = 0, nextx = x, nexty = y;

    for (int i = 0; i < 8; i++) //更新influence 加载条件
    {
        if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
        {
            influence[x + OFFSET[i][0]][y + OFFSET[i][1]]--;
            if (influence[x + OFFSET[i][0]][y + OFFSET[i][1]] == 0)
            {
                tx.push(x + OFFSET[i][0]);
                ty.push(y + OFFSET[i][1]);
            }
        }
    }
    mx.pop();
    my.pop();
    if (!mx.empty()) //下一格
    {
        nextx = mx.top();
        nexty = my.top();
    }
    if (a_check(tx, ty, a)) //无雷
        s1 = build(m - 1, n, nextx, nexty, mx, my, a);

    draft[x][y] = true;     //标记
    if (a_check(tx, ty, a)) //有雷
        s2 = build(m - 1, n - 1, nextx, nexty, mx, my, a);

    draft[x][y] = false; //回溯
    for (int i = 0; i < 8; i++)
    {
        if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a.X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a.Y && a.map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
        {
            influence[x + OFFSET[i][0]][y + OFFSET[i][1]]++;
        }
    }

    note[x][y] += s2;
    return s1 + s2;
}

int advanced_solve(stack<int> &outx, stack<int> &outy, Minesweeper &a) // 0 有解 -2 可能有 2 可能无 -1 输 1 赢
{
    stack<int> mx, my;
    int lattice_num, mines = 1, solution = 0, mostpx, mostpy, rt = 0;
    double chance = 0.5;
    memset(draft, 0, sizeof(draft));
    memset(note, 0, sizeof(note));
    memset(influence, 0, sizeof(influence));

    for (int i = 0; i < a.X; i++)
    {
        for (int j = 0; j < a.Y; j++)
        {
            if (a.map[i][j].found && !solved[i][j]) //条件、influence初始化
            {
                for (int k = 0; k < 8; k++)
                {
                    if (i + OFFSET[k][0] >= 0 && i + OFFSET[k][0] < a.X && j + OFFSET[k][1] >= 0 && j + OFFSET[k][1] < a.Y)
                    {
                        if (!a.map[i + OFFSET[k][0]][j + OFFSET[k][1]].found && !a.map[i + OFFSET[k][0]][j + OFFSET[k][1]].flag)
                        {
                            influence[i][j]++;
                            if (dif(mx, my, i + OFFSET[k][0], j + OFFSET[k][1]))
                            {
                                mx.push(i + OFFSET[k][0]);
                                my.push(j + OFFSET[k][1]);
                            }
                        }
                    }
                }
            }
            else if (a.map[i][j].flag) // draft初始化
                draft[i][j] = true;
        }
    }

    //qx空？

    lattice_num = mx.size();
    while (mines <= lattice_num)
    {
        solution += build(lattice_num, mines, mx.top(), my.top(), mx, my, a);
        mines++;
        // a.prt(0, 0);
    }
    a.prt(0, 0);
    printf("\nSolutions:%d   \n", solution);
    while (!mx.empty())
    {
        if (note[mx.top()][my.top()] == solution) //有雷
        {
            a.flag(mx.top(), my.top());
            for (int i = 0; i < 8; i++)
            {
                if (mx.top() + OFFSET[i][0] >= 0 && mx.top() + OFFSET[i][0] < a.X && my.top() + OFFSET[i][1] >= 0 && my.top() + OFFSET[i][1] < a.Y && a.map[mx.top() + OFFSET[i][0]][my.top() + OFFSET[i][1]].found && !solved[mx.top() + OFFSET[i][0]][my.top() + OFFSET[i][1]])
                {
                    outx.push(mx.top() + OFFSET[i][0]);
                    outy.push(my.top() + OFFSET[i][1]);
                }
            }
        }
        else if (note[mx.top()][my.top()] == 0) //无雷
        {
            rt = a.click(mx.top(), my.top());
            outx.push(mx.top());
            outy.push(my.top());
            if (rt == -1 || rt == 1)
                return rt;
        }
        else
        {
            if ((double)note[mx.top()][my.top()] / solution >= chance) //可能有
            {
                mostpx = mx.top();
                mostpy = my.top();
                chance = (double)note[mx.top()][my.top()] / solution;
                rt = -2;
            }
            else if (1 - (double)note[mx.top()][my.top()] / solution >= chance) //可能无
            {
                mostpx = mx.top();
                mostpy = my.top();
                chance = 1 - (double)note[mx.top()][my.top()] / solution;
                rt = 2;
            }
        }
        mx.pop();
        my.pop();
    }
    if (outx.empty())
    {
        outx.push(mostpx);
        outy.push(mostpy);
        return rt;
    }
    else
        return 0;
}

int main()
{
    system("chcp 65001");
    system("cls");
    // printf("\033[2J\033[0;0H");
    stack<int> startx, starty;
    char c;
    bool end, first;
    int status, x, y;
    while (true)
    {
        c = getch();
        system("cls");
        Minesweeper *a = new Minesweeper(10, 10, 20);
        memset(solved, 0, sizeof(solved));
        first = true, end = false;

        if (c == 'q')
            break;
        while (!end)
        {
            a->prt(0, 0);
            if (first)
            {
                if (a->click(0, 0) == -1)
                {
                    a->prt(0, 0);
                    printf("Failed!\n");
                    end = true;
                }
                else
                {
                    startx.push(0);
                    starty.push(0);
                    first = false;
                }
            }
            else
            {
                while (!startx.empty())
                {
                    if (primary_solve(startx.top(), starty.top(), *a))
                    {
                        a->prt(0, 0);
                        end = true;
                        printf("Solved!\n");
                    }
                    else
                    {
                        startx.pop();
                        starty.pop();
                    }
                }
                a->prt(0, 0);
                status = advanced_solve(startx, starty, *a);
                x = startx.top();
                y = starty.top();
                startx.pop();
                starty.pop();
                switch (status)
                {
                case 1:
                    a->prt(0, 0);
                    end = true;
                    printf("Solved!\n");
                    break;
                case 2:
                    if (a->click(x, y) == -1)
                    {
                        a->prt(x, y);
                        end = true;
                        printf("Failed!\n");
                    }
                    else
                    {
                        for (int i = 0; i < 8; i++)
                        {
                            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a->X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a->Y && a->map[x + OFFSET[i][0]][y + OFFSET[i][1]].found && !solved[x + OFFSET[i][0]][y + OFFSET[i][1]])
                            {
                                startx.push(x + OFFSET[i][0]);
                                starty.push(y + OFFSET[i][1]);
                            }
                        }
                    }
                    break;
                case -2:
                    a->flag(x, y);
                    for (int i = 0; i < 8; i++)
                    {
                        if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < a->X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < a->Y && a->map[x + OFFSET[i][0]][y + OFFSET[i][1]].found && !solved[x + OFFSET[i][0]][y + OFFSET[i][1]])
                        {
                            startx.push(x + OFFSET[i][0]);
                            starty.push(y + OFFSET[i][1]);
                        }
                    }
                    break;
                case -1:
                    a->prt(x, y);
                    end = true;
                    printf("Failed!\n");
                    break;
                default:
                    a->prt(x, y);
                    break;
                }
            }
        }
        Sleep(20);
        delete a;
    }

    system("pause");
    return 0;
}