#include "my_allocator11.h"
#include "my_list.h"
#include <exception>
#include <iostream>
#include <map>
#include <utility>

int main()
{
  {
    // 6) создание экземпляра своего контейнера для хранения значений типа int
    MyList<int> list;
    // 7) заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; i++)
    {
      try
      {
        list.push_back(i);
      }
      catch (const std::exception &e)
      {
        std::cout << "exception:" << e.what() << "\n";
      };
    }
  }
  {
    // 8) создание экземпляра своего контейнера для хранения значений типа int с
    // новым аллокатором, ограниченным 10 элементами
    MyList<int, MyAllocator17<int, 10>> list1;
    // 9) заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; i++)
    {
      try
      {
        list1.emplace_back(i);
      }
      catch (const std::exception &e)
      {
        std::cout << "exception:" << e.what() << "\n";
      };
    }
    // 10) вывод на экран всех значений, хранящихся в контейнере
    for (auto it = list1.begin(); it != list1.end(); it++)
    {
      std::cout << (*it) << "\n";
    }
  }
  return 0;
}