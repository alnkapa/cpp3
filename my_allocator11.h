#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

/**
 * С++17
 * Аллокатор работает с фиксированным количеством элементов.
 * При попытке выделить число элементов,
 * которое превышает текущее зарезервированное количество,
 * аллокатор расширяет зарезервированную память.
 * реализовать поэлементное освобождение
 * @param N кол-во элементов
 */
template <typename T, int N>
class MyAllocator17
{
public:
  using value_type = T;
  using pointer = T *;
  using size_type = std::size_t;
  template <typename U>
  struct rebind
  {
    typedef MyAllocator17<U, N> other;
  };
  /**
   * allocator can be move
   */
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  /**
   * allocator can be copy
   */
  using propagate_on_container_copy_assignment = std::true_type; // select_on_container_copy_construction ??

  /**
   * construct allocator
   */
  MyAllocator17()
  {
    m_store.reserve(N);
  };
  template <typename U, int N1>
  MyAllocator17(const MyAllocator17<U, N1> &u)
  {
  }
  /**
   * destruct allocator
   */
  ~MyAllocator17()
  {
    m_free.clear();
    m_store.clear();
  };

  /**
   * allocate memory
   */
  pointer allocate(size_type n, const void *hint = 0)
  {
    if (n > 1)
    {
      throw std::bad_alloc();
    }
    if (!m_free.empty())
    {
      auto index = m_free.back();
      m_free.pop_back();
      return &m_store[index];
    }
    else
    {
      m_store.emplace_back();
      return &m_store.back();
    }
  }
  /**
   * deallocate memory
   */
  void deallocate(pointer p, size_type n)
  {
    if (n > 1)
    {
      throw std::bad_alloc();
    }
    for (size_t i = 0; i < m_store.size(); i++)
    {
      if (&m_store[i] == p)
      {
        m_free.push_back(i);
        break;
      }
    }
  };
  /**
   * call object constructor
   */
  void construct(pointer p, const value_type &val)
  {
    ::new (p) value_type(val);
  }
  /**
   * call object destructor
   */
  void destroy(pointer p)
  {
    p->~value_type();
  };

private:
  std::vector<int> m_free{};
  std::vector<value_type> m_store{};
};

template <class T, int N, class U, int N1>
bool operator==(MyAllocator17<T, N> const &, MyAllocator17<U, N1> const &)
{
  return (N == N1 && std::is_same<T, U>::value);
}

template <class T, int N, class U, int N1>
bool operator!=(MyAllocator17<T, N> const &x, MyAllocator17<U, N1> const &y)
{
  return !(N == N1 && x == y);
}
