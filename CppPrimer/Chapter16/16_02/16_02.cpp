#include<iostream>

//比较两个输入参数的大小，第一个大于第二个返回1，小于则返回-1，相等则返回0
template<typename T>
int compare(const T &lhs, const T &rhs) {
    if(lhs < rhs) {
        return -1;
    }
    if(rhs < lhs) {
        return 1;
    }
    return 0;
}

int main( ) {
    int a = 1;
    int b = 2;
    std::cout << compare(1, 2) << std::endl;
    return 0;
}
