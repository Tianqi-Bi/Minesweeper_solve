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
const int X_MAX = 100, Y_MAX = 100;

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
    } map[X_MAX][Y_MAX];

    Minesweeper(int ix, int iy, int n) // 生成
    {
        X = ix;
        Y = iy;
        mines = n;
        int x, y, sum;
        memset(map, 0, sizeof(map));
        ///*
        memset(ans, 0, sizeof(ans));

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
        //*/
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
    /*
    int ans[X_MAX][Y_MAX] = {
        {2, 9, 1, 0, 0, 0, 0, 0, 0, 0},
        {9, 2, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 1, 1, 2, 1, 1, 1, 9, 1},
        {0, 1, 2, 9, 2, 9, 3, 3, 2, 1},
        {0, 2, 9, 4, 3, 3, 9, 9, 1, 0},
        {0, 3, 9, 9, 2, 3, 9, 3, 2, 1},
        {0, 2, 9, 4, 9, 2, 1, 2, 3, 9},
        {0, 1, 1, 2, 2, 2, 2, 3, 9, 9},
        {0, 0, 0, 0, 1, 9, 2, 9, 9, 9},
    };
    */
    int ans[X_MAX][Y_MAX];
    void prtans()
    {
        for (int i = 0; i < X; i++)
        {
            for (int j = 0; j < Y; j++)
            {
                if (ans[i][j] == 9)
                    printf("\033[91m× \033[0m");
                else
                    printf("%d ", ans[i][j]);
            }
            printf("\n");
        }
        return;
    }
};

int s1_solve(Minesweeper &Game, bool solved[][Y_MAX], stack<point> list)
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

point find_head(node chain[][Y_MAX], point in)
{
    point head = chain[in.px][in.py].head;
    while (chain[head.px][head.py].head != head)
    {
        head = chain[head.px][head.py].head;
    }
    return head;
}

bool in_vector(vector<point> in, point p)
{
    for (vector<point>::iterator i = in.begin(); i != in.end(); i++)
    {
        if (*i == p)
            return true;
    }
    return false;
}

bool check(Minesweeper &Game, stack<point> num_blocks, bool draft[][Y_MAX])
{
    int num;
    while (!num_blocks.empty())
    {
        num = 0;
        for (int i = 0; i < 8; i++)
        {
            if (num_blocks.top().px + OFFSET[i][0] >= 0 && num_blocks.top().px + OFFSET[i][0] < Game.X && num_blocks.top().py + OFFSET[i][1] >= 0 && num_blocks.top().py + OFFSET[i][1] < Game.Y && draft[num_blocks.top().px + OFFSET[i][0]][num_blocks.top().py + OFFSET[i][1]])
                num++;
        }
        // printf("%d\n",Game.map[num_blocks.top().px][num_blocks.top().py].num);
        if (Game.map[num_blocks.top().px][num_blocks.top().py].num != num)
            return false;
        else
            num_blocks.pop();
    }
    return true;
}

int search(Minesweeper &Game, int block_num, int mine_num, vector<point> &unfounds, vector<point>::iterator present, int influence[][Y_MAX], bool draft[][Y_MAX], int solution[][Y_MAX], int step)
{
    if (block_num < mine_num || mine_num < 0)
        return 0;
    if (block_num == 0)
        return 1;
    // printf("%d\n",step);
    int tmp, s1 = 0, s2 = 0;
    // 储存可以检查的数字格
    stack<point> toCheck;
    for (int i = 0; i < 8; i++) // 更新influence 加载条件
    {
        if (present->px + OFFSET[i][0] >= 0 && present->px + OFFSET[i][0] < Game.X && present->py + OFFSET[i][1] >= 0 && present->py + OFFSET[i][1] < Game.Y && Game.map[present->px + OFFSET[i][0]][present->py + OFFSET[i][1]].found)
        {
            influence[present->px + OFFSET[i][0]][present->py + OFFSET[i][1]]--;
            if (influence[present->px + OFFSET[i][0]][present->py + OFFSET[i][1]] == 0)
            {
                toCheck.push(make_pair(present->px + OFFSET[i][0], present->py + OFFSET[i][1]));
            }
        }
    }
    if (check(Game, toCheck, draft)) // 无雷
        s1 = search(Game, block_num - 1, mine_num, unfounds, present + 1, influence, draft, solution, step + 1);

    draft[present->px][present->py] = true; // 标记

    if (check(Game, toCheck, draft)) // 有雷
        s2 = search(Game, block_num - 1, mine_num - 1, unfounds, present + 1, influence, draft, solution, step + 1);
    draft[present->px][present->py] = false; // 回溯
    for (int i = 0; i < 8; i++)
    {
        if (present->px + OFFSET[i][0] >= 0 && present->px + OFFSET[i][0] < Game.X && present->py + OFFSET[i][1] >= 0 && present->py + OFFSET[i][1] < Game.Y && Game.map[present->px + OFFSET[i][0]][present->py + OFFSET[i][1]].found)
            influence[present->px + OFFSET[i][0]][present->py + OFFSET[i][1]]++;
    }

    solution[present->px][present->py] += s2;
    return s1 + s2;
}

int s2_solve(Minesweeper &Game, bool solved[][Y_MAX], stack<point> numlist)
{
    node chain[X_MAX][Y_MAX];

    for (int i = 0; i < Game.X; i++)
    {
        for (int j = 0; j < Game.Y; j++)
        {
            chain[i][j].head = make_pair(i, j);
            chain[i][j].pre = make_pair(i, j);
            chain[i][j].tail = make_pair(-1, -1);
        }
    }

    while (!numlist.empty())
    {
        point p = numlist.top();
        numlist.pop();
        chain[p.px][p.py].tail = make_pair(p.px, p.py); // 自己加入链

        for (int i = 0; i < 8; i++)
        {
            if (p.px + OFFSET[i][0] >= 0 && p.px + OFFSET[i][0] < Game.X && p.py + OFFSET[i][1] >= 0 && p.py + OFFSET[i][1] < Game.Y && !Game.map[p.px + OFFSET[i][0]][p.py + OFFSET[i][1]].found && !Game.map[p.px + OFFSET[i][0]][p.py + OFFSET[i][1]].flag)
            {
                // 搜索到未发现的格 定位
                for (int j = 0; j < 8; j++)
                {
                    if (p.px + OFFSET[i][0] + OFFSET[j][0] >= 0 && p.px + OFFSET[i][0] + OFFSET[j][0] < Game.X && p.py + OFFSET[i][1] + OFFSET[j][1] >= 0 && p.py + OFFSET[i][1] + OFFSET[j][1] < Game.Y && !(OFFSET[i][0] + OFFSET[j][0] == 0 && OFFSET[i][1] + OFFSET[j][1] == 0))
                    {
                        if (chain[p.px + OFFSET[i][0] + OFFSET[j][0]][p.py + OFFSET[i][1] + OFFSET[j][1]].tail != make_pair(-1, -1)) // 未发现格旁的在链中未解决数字格
                        {
                            point p1 = p, p2 = make_pair(p.px + OFFSET[i][0] + OFFSET[j][0], p.py + OFFSET[i][1] + OFFSET[j][1]);
                            if (find_head(chain, p2) == find_head(chain, p1))
                                continue;
                            chain[find_head(chain, p2).px][find_head(chain, p2).py].pre = chain[find_head(chain, p1).px][find_head(chain, p1).py].tail;                        // 将新链头连接旧链
                            chain[chain[find_head(chain, p1).px][find_head(chain, p1).py].tail.px][chain[find_head(chain, p1).px][find_head(chain, p1).py].tail.py].tail = p2; // 将旧链的尾节点的尾标签更改
                            chain[find_head(chain, p1).px][find_head(chain, p1).py].tail = chain[find_head(chain, p2).px][find_head(chain, p2).py].tail;                       // 将旧链的尾更改
                            chain[find_head(chain, p2).px][find_head(chain, p2).py].head = find_head(chain, p1);                                                               // 将新链的头标签更新为旧链的头
                        }
                    }
                }
            }
        }
    }

    struct group_info
    {
        vector<point> unfounds;
        vector<point> founds;
        int minmines;
        float expmines;
    };
    vector<group_info> groups;
    int influence[X_MAX][Y_MAX], minetime[X_MAX][Y_MAX], flagnum = 0;
    float mineposibility[X_MAX][Y_MAX];
    bool draft[X_MAX][Y_MAX];
    for (int i = 0; i < Game.X; i++)
    {
        for (int j = 0; j < Game.Y; j++)
        {
            if (chain[i][j].tail == make_pair(i, j)) // 存入groups
            {
                group_info g;
                point tmp_p;
                bool last_one = false;
                tmp_p = chain[i][j].tail;
                while (true)
                {
                    if (chain[tmp_p.px][tmp_p.py].pre == tmp_p)
                        last_one = true;
                    g.founds.push_back(tmp_p);
                    tmp_p = chain[tmp_p.px][tmp_p.py].pre;
                    if (last_one)
                        break;
                }
                groups.push_back(g);
            }
            if (Game.map[i][j].flag)
                flagnum++;
            influence[i][j] = 0;
            draft[i][j] = false;
            if (Game.map[i][j].found && !solved[i][j]) // draft、influence初始化
            {
                for (int k = 0; k < 8; k++)
                {
                    if (i + OFFSET[k][0] >= 0 && i + OFFSET[k][0] < Game.X && j + OFFSET[k][1] >= 0 && j + OFFSET[k][1] < Game.Y)
                    {
                        if (!Game.map[i + OFFSET[k][0]][j + OFFSET[k][1]].found && !Game.map[i + OFFSET[k][0]][j + OFFSET[k][1]].flag)
                            influence[i][j]++;
                    }
                }
            }
            else if (Game.map[i][j].flag)
                draft[i][j] = true;
        }
    }

    for (vector<group_info>::iterator i = groups.begin(); i != groups.end(); i++) // 存入每个group邻近的未发现格
    {
        for (vector<point>::iterator j = i->founds.begin(); j != i->founds.end(); j++)
        {
            for (int k = 0; k < 8; k++)
            {
                if (j->px + OFFSET[k][0] >= 0 && j->px + OFFSET[k][0] < Game.X && j->py + OFFSET[k][1] >= 0 && j->py + OFFSET[k][1] < Game.Y)
                    if (!Game.map[j->px + OFFSET[k][0]][j->py + OFFSET[k][1]].found && !Game.map[j->px + OFFSET[k][0]][j->py + OFFSET[k][1]].flag && !in_vector(i->unfounds, make_pair(j->px + OFFSET[k][0], j->py + OFFSET[k][1])))
                    {
                        // printf("%d,%d\t",j->px+OFFSET[k][0],j->py+OFFSET[k][1]);
                        i->unfounds.push_back(make_pair(j->px + OFFSET[k][0], j->py + OFFSET[k][1]));
                    }
            }
        }
        // printf("\n");
    }

    // 预搜索，找到每个group的最小雷数
    int solution, min_mine_num, sum_min = 0;
    memset(minetime, 0, sizeof(minetime));
    for (vector<group_info>::iterator i = groups.begin(); i != groups.end(); i++)
    {
        solution = 0;
        min_mine_num = 0;
        do
        {
            min_mine_num++;
            solution = search(Game, i->unfounds.size(), min_mine_num, i->unfounds, i->unfounds.begin(), influence, draft, minetime, 1);
        } while (solution == 0);
        i->minmines = min_mine_num;
        sum_min += min_mine_num;
    }
    // 正式搜索
    int sum_mine_of_group, sum_solution_of_group, tmp_sol, mine;
    memset(minetime, 0, sizeof(minetime));
    memset(mineposibility, 0, sizeof(mineposibility));
    for (vector<group_info>::iterator i = groups.begin(); i != groups.end(); i++)
    {
        mine = i->minmines;
        sum_mine_of_group = 0;
        sum_solution_of_group = 0;
        do
        {
            tmp_sol = search(Game, i->unfounds.size(), mine, i->unfounds, i->unfounds.begin(), influence, draft, minetime, 1);
            sum_solution_of_group += tmp_sol;
            sum_mine_of_group += (mine * tmp_sol);
            mine++;
        } while (mine <= (i->unfounds.size() < (Game.mines - flagnum) ? i->unfounds.size() : (Game.mines - flagnum)));
        i->expmines = (float)sum_mine_of_group / sum_solution_of_group;
        for (vector<point>::iterator j = i->unfounds.begin(); j != i->unfounds.end(); j++)
        {
            mineposibility[j->px][j->py] = (float)minetime[j->px][j->py] / sum_solution_of_group;
        }
    }
    /*
    for (int i = 0; i < Game.X; i++)
    {
        for (int j = 0; j < Game.Y; j++)
        {
            printf("%d ", minetime[i][j]);
        }
        printf("\n");
    }
    */
    stack<point> toClick;
    for (vector<group_info>::iterator i = groups.begin(); i != groups.end(); i++)
    {
        for (vector<point>::iterator j = i->unfounds.begin(); j != i->unfounds.end(); j++)
        {
            if (!Game.map[j->px][j->py].found && !Game.map[j->px][j->py].flag && minetime[j->px][j->py] == 0)
            {
                toClick.push(make_pair(j->px, j->py));
            }
        }
    }
    if (!toClick.empty())
    {
        while (!toClick.empty())
        {
            if (Game.click(toClick.top().px, toClick.top().py) == 1)
                return 2;
            else
            {
                toClick.pop();
            }
        }
        return 1;
    }
    else
    {
        point min_p = make_pair(0, 0);
        float min_ = 1;
        int sum_expnum = 0;
        for (vector<group_info>::iterator i = groups.begin(); i != groups.end(); i++)
        {
            for (vector<point>::iterator j = i->unfounds.begin(); j != i->unfounds.end(); j++)
            {
                if (!Game.map[j->px][j->py].found && !Game.map[j->px][j->py].flag && mineposibility[j->px][j->py] < min_)
                {
                    min_ = mineposibility[j->px][j->py];
                    min_p = make_pair(j->px, j->py);
                }
            }
            sum_expnum += i->expmines;
        }
        if (min_ <= (float)(Game.mines - flagnum) / ((Game.X * Game.Y) - Game.foundnum - flagnum - sum_expnum))
        {
            return Game.click(min_p.px, min_p.py) + 1;
        }
        else
        {
            int randn = rand() % ((Game.X * Game.Y) - Game.foundnum);
            for (int i = 0; i < Game.X; i++)
            {
                for (int j = 0; j < Game.Y; j++)
                {
                    if (!Game.map[i][j].found && !Game.map[i][j].flag && minetime[i][j] == 0)
                    {
                        if (randn == 0)
                        {
                            return Game.click(i, j) + 1;
                        }
                        randn--;
                    }
                }
            }
        }
    }
    return 1;
}

bool solve(Minesweeper &Game)
{
    bool solved[X_MAX][Y_MAX];
    int signal;
    stack<point> toCheck;
    memset(solved, 0, sizeof(solved));

    int rt = Game.click(Game.X / 2, Game.Y / 2);
    if (rt == -1)
        return false;
    // Game.click(4, 5);
    // Game.click(3, 7);
    // Game.click(6, 4);
    while (1)
    {
        // Sleep(1000);
        signal = 0;
        while (!toCheck.empty())
        {
            toCheck.pop();
        }

        for (int i = 0; i < Game.X; i++)
        {
            for (int j = 0; j < Game.Y; j++)
            {
                if (!solved[i][j])
                {
                    if (Game.map[i][j].found)
                    {
                        if (Game.map[i][j].num == 0)
                            solved[i][j] = true;
                        else
                            toCheck.push(make_pair(i, j));
                    }
                    else
                    {
                        if (Game.map[i][j].flag)
                            solved[i][j] = true;
                    }
                }
            }
        }
        signal = s1_solve(Game, solved, toCheck);
        Game.prt();
        if (signal == 2)
            return true;
        else if (signal)
            continue;
        signal = s2_solve(Game, solved, toCheck);
        Game.prt();
        if (signal == 2)
            return true;
        else if (signal)
            continue;
        return false;
    }

    return 0;
}

int main()
{
    system("chcp 65001");
    system("cls");
    char c;
    while (true)
    {
        Minesweeper *Game = new Minesweeper(13, 25, 81);
        Game->prt();
        if (solve(*Game))
            printf("Solved!\n");
        else
            printf("Failed!\n");
        // Sleep(2000);
        c = getch();
        delete Game;
        system("cls");
    }
    system("pause");
    return 0;
}