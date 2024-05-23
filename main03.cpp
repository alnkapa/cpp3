#include "my_allocator03.h"
#include <iostream>
#include <map>
#include <utility>

int factorial(int n) {
  if (n == 0) {
    return 1;
  } else {
    return n * factorial(n - 1);
  }
}
int main() {
  {
    // создание экземпляра std::map<int, int>
    std::map<int, int> stdMap;
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение -
    // факториал ключа
    for (int i = 0; i < 10; i++) {
      stdMap.insert(std::pair<int, int>(i, factorial(i)));
    }
  }
  {
    typedef std::map<int, int, std::less<int>, MyAllocator03<int> > map_t;
    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным
    // 10 элементами
    // MyAllocator03<int>
    map_t o3Map;
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение -
    // факториал ключа
    for (int i = 0; i < 10; i++) {
      o3Map.insert(std::pair<int, int>(i, factorial(i)));
    }
    for (map_t::const_iterator it = o3Map.begin(); it != o3Map.end(); it++) {
      std::cout << (*it).first << " " << (*it).second << "\n";
    }
  }
  return 0;
}