#pragma once
#include <iostream>
#include <limits>
#include <memory>

template <typename T, typename U>
struct is_same
{
  static const bool value = false;
};

template <typename T>
struct is_same<T, T>
{
  static const bool value = true;
};

/**
 * С++03
 * Аллокатор работает с фиксированным количеством элементов.
 * Попытку выделить большее число элементов считать ошибкой.
 *
 * @param N кол-во элементов
 */
template <typename T, int N>
class MyAllocator03
{
public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template <typename U>
  struct rebind
  {
    typedef MyAllocator03<U, N> other;
  };
  /**
   * destruct allocator
   */
  ~MyAllocator03()
  {
    MyAllocator03<T, N>::m_next = 0;
  };
  /**
   * construct allocator
   */
  MyAllocator03()
  {
    MyAllocator03<T, N>::m_next = 0;
  };
  /**
   * construct allocator element
   * call allocate
   */
  template <typename U, int N1>
  MyAllocator03(const MyAllocator03<U, N1> &u)
  {
  }
  /**
   * allocate memory
   */
  pointer allocate(size_type n)
  {
    if (n > 1)
    {
      throw std::bad_alloc();
    }
    if (n > N || m_next + n > N)
    {
      throw std::bad_alloc();
    }
    pointer result = m_ptr + m_next;
    m_next += n;
    return result;
  }
  /**
   * deallocate memory
   */
  void deallocate(pointer p, size_type n)
  {
  };

  /**
   * call object constructor
   */
  void construct(pointer p, value_type const &val)
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

  size_type max_size() const throw()
  {
    return N;
  };

private:
  static value_type m_ptr[N];
  static int m_next;
};

template <class T, int N>
T MyAllocator03<T, N>::m_ptr[N] = {};
template <class T, int N>
int MyAllocator03<T, N>::m_next = 0;

template <class T, int N, class U, int N1>
bool operator==(MyAllocator03<T, N> const &, MyAllocator03<U, N1> const &)
{
  return N == N1 && is_same<T, U>::value;
}

template <class T, int N, class U, int N1>
bool operator!=(MyAllocator03<T, N> const &x, MyAllocator03<U, N1> const &y)
{
  return !(x == y);
}
