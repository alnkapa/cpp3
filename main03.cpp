#include "my_allocator03.h"
#include <exception>
#include <iostream>
#include <map>
#include <utility>

int factorial(int n)
{
  if (n == 0)
  {
    return 1;
  }
  else
  {
    return n * factorial(n - 1);
  }
}
int main()
{

  typedef std::pair<const int, int> pair_t;
  {
    // 1) создание экземпляра std::map<int, int>
    std::map<int, int> stdMap;
    // 2)  заполнение 10 элементами, где ключ - это число от 0 до 9, а значение
    // - факториал ключа
    for (int i = 0; i < 10; i++)
    {
      stdMap.insert(pair_t(i, factorial(i)));
    }
  }
  {
    // 3) создание экземпляра std::map<int, int> с новым аллокатором,
    // ограниченным 10 элементами
    //
    // 4) заполнение 10 элементами, где ключ - это число от 0 до 9, а значение -
    // факториал ключа
    typedef std::map<int, int, std::less<int>, MyAllocator03<pair_t, 10> > map_t;
    map_t o3Map;
    for (int i = 0; i < 3; i++)
    {
      try
      {
        o3Map.insert(pair_t(i, factorial(i)));
      }
      catch (...)
      {
        std::cout << "err" << std::endl;
      };
    }
    // 5) вывод на экран всех значений (ключ и значение разделены пробелом)
    // хранящихся в контейнере
    for (map_t::const_iterator it = o3Map.begin(); it != o3Map.end(); it++)
    {
      std::cout << (*it).first << " " << (*it).second << "\n";
    }
  }
  return 0;
}