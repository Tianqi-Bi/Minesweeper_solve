#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stack>
#include <vector>
using namespace std;

#define px first
#define py second
typedef pair<int, int> point;
struct node
{
    point head;
    point pre;
    point tail;
};

const int OFFSET[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

class Minesweeper
{
public:
    int mines, foundnum = 0;
    int X, Y;
    struct block
    {
        int num;
        bool found;
        bool flag;
    } map[100][100];

    Minesweeper(int ix, int iy, int n) // 生成
    {
        X = ix;
        Y = iy;
        mines = n;
        int x, y, sum;
        memset(map, 0, sizeof(map));
        // memset(ans, 0, sizeof(ans));
        /*
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
            if (ans[x][y] != 9)
            {
                ans[x][y] = 9;
                for (int j = 0; j < 8; j++) // 标记其他方格
                {
                    if (x + OFFSET[j][0] >= 0 && x + OFFSET[j][0] < X && y + OFFSET[j][1] >= 0 && y + OFFSET[j][1] < Y && ans[x + OFFSET[j][0]][y + OFFSET[j][1]] != 9)
                        ans[x + OFFSET[j][0]][y + OFFSET[j][1]]++;
                }
            }
            else
                i--;
        }
        */
    }
    int click(int x, int y) // 点击/检查
    {                       // return 0 正常 -1 失败 1 胜利
        if (map[x][y].found || map[x][y].flag)
            return 0;
        if (ans[x][y] == 9)
            return -1;
        map[x][y].found = true;
        map[x][y].num = ans[x][y];
        if (++foundnum == X * Y - mines)
            return 1;
        if (!ans[x][y]) // 若为空
        {
            for (int i = 0; i < 8; i++)
            {
                if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y)
                    click(x + OFFSET[i][0], y + OFFSET[i][1]);
            }
        }
        return 0;
    }
    int doubleclick(int x, int y) // 双击
    {
        if (!map[x][y].found || map[x][y].flag)
            return 0;
        int sum = 0, rt = 0;
        for (int i = 0; i < 8; i++)
        {
            if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y && map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag)
                sum++;
        }
        if (sum == ans[x][y])
        {
            for (int i = 0; i < 8; i++)
            {
                if (x + OFFSET[i][0] >= 0 && x + OFFSET[i][0] < X && y + OFFSET[i][1] >= 0 && y + OFFSET[i][1] < Y && !map[x + OFFSET[i][0]][y + OFFSET[i][1]].flag && !map[x + OFFSET[i][0]][y + OFFSET[i][1]].found)
                {
                    rt = click(x + OFFSET[i][0], y + OFFSET[i][1]);
                    if (rt == -1)
                        return -1;
                    else if (rt == 1)
                        return 1;
                }
            }
        }
        return 0;
    }
    void flag(int x, int y) // 标记
    {
        if (!map[x][y].found)
            map[x][y].flag = !map[x][y].flag;
        return;
    }
    void prt(int x = -1, int y = -1) // 输出
    {
        COORD pos = {0, 0}; // 移动光标
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);
        for (int i = 0; i < X; i++) // 显示
        {
            for (int j = 0; j < Y; j++)
            {
                if (i == x && j == y) // 当前位置
                {
                    if (map[i][j].found)
                    {
                        if (ans[i][j] == 9)
                            printf("\033[91m×\033[0m");
                        else
                            printf("\033[92m%d \033[0m", map[i][j].num);
                    }
                    else
                        printf("\033[92m■ \033[0m");
                }
                else
                {
                    if (map[i][j].found)
                        printf("%d ", map[i][j].num);
                    else if (map[i][j].flag)
                        printf("\033[96m■ \033[0m");
                    else
                        printf("■ ");
                }
            }
            printf("\n");
        }
        printf("\n");
        prtans();
        printf("\n");
        return;
    }

private:
    int ans[100][100] = {{0, 0, 1, 9, 0, 0}, {0, 0, 2, 0, 0, 0}, {1, 1, 2, 9, 0, 0}, {0, 9, 0, 0, 0, 9}, {0, 0, 0, 9, 3, 1}, {0, 0, 0, 9, 2, 0}};

    void prtans()
    {
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                printf("%d ", ans[i][j]);
            }
            printf("\n");
        }
        return;
    }
};

void prt(int out[][100], int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            printf("%d ", out[i][j]);
        }
        printf("\n");
    }
    return;
}
void prt(bool out[][100], int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            printf("%d ", out[i][j]);
        }
        printf("\n");
    }
    return;
}

int s1_solve(Minesweeper &Game, bool solved[][100], stack<point> list)
{
    int rt = 0; // return 0 无操作 1 有操作 2 胜利
    while (!list.empty())
    {
        point P = list.top();
        list.pop();
        if (solved[P.px][P.py] || !Game.map[P.px][P.py].found)
            continue;
        int flagnum = 0, unfoundnum = 0;
        for (int i = 0; i < 8; i++) // 统计未知格与旗帜数
        {
            if (P.px + OFFSET[i][0] >= 0 && P.px + OFFSET[i][0] < Game.X && P.py + OFFSET[i][1] >= 0 && P.py + OFFSET[i][1] < Game.Y)
            {
                if (!Game.map[P.px + OFFSET[i][0]][P.py + OFFSET[i][1]].found)
                    unfoundnum++;
                if (Game.map[P.px + OFFSET[i][0]][P.py + OFFSET[i][1]].flag)
                    flagnum++;
            }
        }
        if (unfoundnum != Game.map[P.px][P.py].num && flagnum != Game.map[P.px][P.py].num)
            continue;
        rt = 1;
        if (unfoundnum == Game.map[P.px][P.py].num) // 标记周围
        {
            for (int i = 0; i < 8; i++)
            {
                if (P.px + OFFSET[i][0] >= 0 && P.px + OFFSET[i][0] < Game.X && P.py + OFFSET[i][1] >= 0 && P.py + OFFSET[i][1] < Game.Y && !Game.map[P.px + OFFSET[i][0]][P.py + OFFSET[i][1]].found && !Game.map[P.px + OFFSET[i][0]][P.py + OFFSET[i][1]].flag)
                {
                    Game.flag(P.px + OFFSET[i][0], P.py + OFFSET[i][1]);
                }
            }
        }
        if (flagnum == Game.map[P.px][P.py].num)
        {
            if (Game.doubleclick(P.px, P.py) == 1)
                return 2;
        }
        solved[P.px][P.py] = true;
    }
    return rt;
}

point find_head(node chain[][100], point in)
{
    point head = chain[in.px][in.py].head;
    while (chain[head.px][head.py].head != head)
    {
        head = chain[head.px][head.py].head;
    }
    return head;
}

int s2_solve(Minesweeper &Game, bool solved[][100], stack<point> numlist)
{
    node chain[100][100];
    struct group_info
    {
        point tail;
        stack<point> members;
        int minmines;
        int expmines;
    };

    // vector<point, info> tails;
    int minetime[100][100];

    stack<point> tmp = numlist;
    for (int i = 0; i < Game.X; i++)
    {
        for (int j = 0; j < Game.Y; j++)
        {
            chain[i][j].head = make_pair(i, j);
            chain[i][j].pre= make_pair(i, j);
            chain[i][j].tail = make_pair(-1, -1);
        }
    }

    while (!numlist.empty())
    {
        point p = numlist.top();
        numlist.pop();
        chain[p.px][p.py].tail = make_pair(p.px, p.py);

        for (int i = 0; i < 9; i++)
        {
            if (p.px + OFFSET[i][0] >= 0 && p.px + OFFSET[i][0] < Game.X && p.py + OFFSET[i][1] >= 0 && p.py + OFFSET[i][1] < Game.Y && !Game.map[p.px + OFFSET[i][0]][p.py + OFFSET[i][1]].found)
            {
                // 搜索到未发现的格 定位
                for (int j = 0; j < 9; j++)
                {
                    if (p.px + OFFSET[i][0] + OFFSET[j][0] >= 0 && p.px + OFFSET[i][0] + OFFSET[j][0] < Game.X && p.py + OFFSET[i][1] + OFFSET[j][1] >= 0 && p.py + OFFSET[i][1] + OFFSET[j][1] < Game.Y && !(OFFSET[i][0] + OFFSET[j][0] == 0 && OFFSET[i][1] + OFFSET[j][1] == 0))
                    {
                        if (chain[p.px + OFFSET[i][0] + OFFSET[j][0]][p.py + OFFSET[i][1] + OFFSET[j][1]].tail != make_pair(-1, -1))
                        { // 未发现格旁的未解决数字格
                            // Game.prt(p.px + OFFSET[i][0] + OFFSET[j][0], p.py + OFFSET[i][1] + OFFSET[j][1]);
                            // prt(solved, 6, 6);
                            point p1 = p, p2 = make_pair(p.px + OFFSET[i][0] + OFFSET[j][0], p.py + OFFSET[i][1] + OFFSET[j][1]);
                            if (find_head(chain, p2) == find_head(chain, p1))
                                continue;
                            chain[find_head(chain, p2).px][find_head(chain, p2).py].pre = chain[find_head(chain, p1).px][find_head(chain, p1).py].tail;//将新链头连接旧链
                            chain[chain[find_head(chain, p1).px][find_head(chain, p1).py].tail.px][chain[find_head(chain, p1).px][find_head(chain, p1).py].tail.py].tail = p2;//将旧链的尾节点的尾标签更改
                            chain[find_head(chain, p1).px][find_head(chain, p1).py].tail = chain[find_head(chain, p2).px][find_head(chain, p2).py].tail;//将旧链的尾更改
                            chain[find_head(chain, p2).px][find_head(chain, p2).py].head = find_head(chain, p1);//将新链的头标签更新为旧链的头
                        }
                    }
                }
            }
        }
    }
    printf("head:\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%d,%d\t", chain[i][j].head.px, chain[i][j].head.py);
        }
        printf("\n");
    }
    printf("pre:\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%d,%d\t", chain[i][j].pre.px, chain[i][j].pre.py);
        }
        printf("\n");
    }
    printf("tail:\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (chain[i][j].tail.px == i && chain[i][j].tail.py == j && chain[i][j].tail == make_pair(i, j))
                printf("\033[92m%d,%d\t\033[0m", chain[i][j].tail.px, chain[i][j].tail.py);
            else
                printf("%d,%d\t", chain[i][j].tail.px, chain[i][j].tail.py);
        }
        printf("\n");
    }

    stack<group_info> groups;

    for (int i = 0; i < Game.X; i++)
    {
        for (int j = 0; j < Game.Y; j++)
        {
            if(chain[i][j].tail==make_pair(i,j)){
                group_info g;
                bool last_one=false;
                g.tail=chain[i][j].tail;
                point tmp_p;
                tmp_p=g.tail;
                while (true)
                {
                    if(chain[tmp_p.px][tmp_p.py].pre==tmp_p)
                        last_one=true;
                    g.members.push(tmp_p);
                    tmp_p=chain[tmp_p.px][tmp_p.py].pre;
                    if(last_one)
                     break;
                }
                

                groups.push(g);
            }
        }
        
    }
    while(!groups.top().members.empty()){
        printf("%d,%d\n",groups.top().members.top().px,groups.top().members.top().py);
        groups.top().members.pop();
    }


    return 1;
}

bool solve(Minesweeper &Game)
{
    bool solved[100][100];
    int signal;
    stack<point> toCheck;
    memset(solved, 0, sizeof(solved));

    // int rt = Game.click(Game.X / 2, Game.Y / 2);
    // if (rt == -1)
    //     return false;
    Game.click(0, 0);
    Game.click(5, 5);
    while (1)
    {
        signal = 0;
        while (!toCheck.empty())
        {
            toCheck.pop();
        }

        for (int i = 0; i < Game.X; i++)
        {
            for (int j = 0; j < Game.Y; j++)
            {
                if (Game.map[i][j].found && !solved[i][j])
                {
                    if (Game.map[i][j].num == 0)
                        solved[i][j] = true;
                    else
                        toCheck.push(make_pair(i, j));
                }
            }
        }
        signal = s1_solve(Game, solved, toCheck);
        // Game.prt();
        if (signal == 2)
            return true;
        else if (signal)
            continue;
        s2_solve(Game, solved, toCheck);

    }

    return 0;
}

int main()
{
    system("chcp 65001");
    system("cls");
    Minesweeper game(6, 6, 6);
    // game.prt();
    solve(game);
    system("pause");
    return 0;
}