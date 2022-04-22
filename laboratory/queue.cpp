#include<cstdio>
#include<queue>
#include<windows.h>
using namespace std;

int main(){
    queue<int> q;
    q.push(10);
    printf("%d %d\n",q.front(),q.empty());
    q.pop();
    printf("%d\n",q.empty());
    system("pause");
    return 0;
}