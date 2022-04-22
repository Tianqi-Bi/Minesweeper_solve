#include <cstdio>

class test
{
private:
    
public:
    test();
    ~test();
};

test::test()
{
}

test::~test()
{
}

int main()
{
    test *a = new test;
    int* b = new int;
    *b=3;
    delete b;
    //delete a;
    return 0;
}