#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main() {
  int a, b;
  cin >> a >> b;
  cout << static_cast<double>(a) / b << endl;
  return 0;
}
