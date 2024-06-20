#pragma once
#include <memory>

/**
 * Реализовать свой контейнер, который по аналогии с контейнерами stl
 * параметризуется аллокатором. Контейнер должен иметь две возможности -
 * добавить новый элемент и обойти контейнер в одном направлении. Реализовать
 * совместимость с контейнерами stl – итераторы, вспомогательные методы size,
 * empty и т.д.
 */
template <typename T, class Allocator = std::allocator<T>>
class MyList
{
public:
  struct Node
  {
    T value;
    Node *next;
    Node() : value(), next(nullptr) {}
    Node(const T &value) : value(value), next(nullptr) {}
  };
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using allocator_type = Allocator;
  using node_allocator_type = typename Allocator::template rebind<Node>::other;

  class iterator
  {
  public:
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    iterator(Node *node) : m_node(node) {}

    reference operator*() const { return m_node->value; }

    pointer operator->() const { return &(m_node->value); }

    iterator &operator++()
    {
      m_node = m_node->next;
      return *this;
    }

    iterator operator++(int)
    {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const iterator &other) const
    {
      return m_node == other.m_node;
    }

    bool operator!=(const iterator &other) const { return !(*this == other); }

  private:
    Node *m_node;
  };
  using const_iterator = const iterator;

  MyList() : m_head(nullptr), m_tail(nullptr), m_size(0), m_node_allocator() {}

  MyList(const MyList &other)
  {
    for (Node *node = other.m_head; node != nullptr; node = node->next)
    {
      push_back(node->value);
    }
  }
  MyList &operator=(const MyList &other)
  {
    if (this != &other)
    {
      clear();
      for (Node *node = other.m_head; node != nullptr; node = node->next)
      {
        push_back(node->value);
      }
    }
    return *this;
  }

  ~MyList() { clear(); }

  MyList(MyList &&other) noexcept
      : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size),
        m_node_allocator(std::move(other.m_node_allocator))
  {
    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
  }
  MyList &operator=(MyList &&other) noexcept
  {
    if (this != &other)
    {
      clear();
      m_head = other.m_head;
      m_tail = other.m_tail;
      m_size = other.m_size;
      m_node_allocator = std::move(other.m_node_allocator);
      other.m_head = nullptr;
      other.m_tail = nullptr;
      other.m_size = 0;
    }
    return *this;
  }

  size_type max_size() const { return std::numeric_limits<size_type>::max(); }
  size_type size() const { return m_size; }

  bool empty() const { return m_size == 0; }

  iterator begin() { return iterator(m_head); }
  const_iterator begin() const { return const_iterator(m_head); }
  iterator end() { return iterator(nullptr); }
  const_iterator end() const { return const_iterator(nullptr); }

  reference front()
  {
    if (!m_head)
      throw std::out_of_range("List is empty");
    return m_head->value;
  }
  const_reference front() const
  {
    if (!m_head)
      throw std::out_of_range("List is empty");
    return m_head->value;
  }
  reference back()
  {
    if (!m_head)
      throw std::out_of_range("List is empty");
    return m_tail->value;
  }
  const_reference back() const
  {
    if (!m_head)
      throw std::out_of_range("List is empty");
    return m_tail->value;
  }

  void push_back(const T &value)
  {
    Node *new_node = m_node_allocator.allocate(1);
    try
    {
      m_node_allocator.construct(new_node, value);
    }
    catch (...)
    {
      m_node_allocator.deallocate(new_node, 1);
      throw;
    }
    new_node->next = nullptr;
    if (m_tail)
    {
      m_tail->next = new_node;
    }
    else
    {
      m_head = new_node;
    }
    m_tail = new_node;
    ++m_size;
  };

  template <typename... Args>
  void emplace_back(Args &&...args)
  {
    Node *new_node = m_node_allocator.allocate(1);
    try
    {
      m_node_allocator.construct(new_node, std::forward<Args>(args)...);
    }
    catch (...)
    {
      m_node_allocator.deallocate(new_node, 1);
      throw;
    }
    new_node->next = nullptr;
    if (m_tail)
    {
      m_tail->next = new_node;
    }
    else
    {
      m_head = new_node;
    }
    m_tail = new_node;
    ++m_size;
  }

  void pop_back()
  {
    if (!m_head)
      throw std::out_of_range("List is empty");

    if (m_head == m_tail)
    {
      m_node_allocator.destroy(m_head);
      m_node_allocator.deallocate(m_head, 1);
      m_head = m_tail = nullptr;
    }
    else
    {
      Node *current = m_head;
      while (current->next != m_tail)
      {
        current = current->next;
      }
      m_node_allocator.destroy(m_tail);
      m_node_allocator.deallocate(m_tail, 1);
      m_tail = current;
      m_tail->next = nullptr;
    }
    --m_size;
  }

  void clear()
  {
    while (m_head)
    {
      Node *next = m_head->next;
      m_node_allocator.destroy(m_head);
      m_node_allocator.deallocate(m_head, 1);
      m_head = next;
    }
    m_tail = nullptr;
    m_size = 0;
  }

  iterator insert(iterator pos, const T &value)
  {
    if (pos == end())
    {
      push_back(value);
      return iterator(m_tail);
    }
    Node *new_node = m_node_allocator.allocate(1);
    try
    {
      m_node_allocator.construct(new_node, value);
    }
    catch (...)
    {
      m_node_allocator.deallocate(new_node, 1);
      throw;
    }
    Node *current = pos.m_node;
    if (current == m_head)
    {
      new_node->next = m_head;
      m_head = new_node;
    }
    else
    {
      Node *prev = m_head;
      while (prev->next != current)
      {
        prev = prev->next;
      }
      prev->next = new_node;
      new_node->next = current;
    }
    ++m_size;
    return iterator(new_node);
  }

  iterator erase(iterator pos)
  {
    if (pos == end())
      return end();
    Node *current = pos.m_node;
    if (current == m_head)
    {
      m_head = m_head->next;
      if (m_head == nullptr)
        m_tail = nullptr;
    }
    else
    {
      Node *prev = m_head;
      while (prev->next != current)
      {
        prev = prev->next;
      }
      prev->next = current->next;
      if (current == m_tail)
        m_tail = prev;
    }
    m_node_allocator.destroy(current);
    m_node_allocator.deallocate(current, 1);
    --m_size;
    return iterator(current->next);
  }

private:
  Node *m_head{nullptr};
  Node *m_tail{nullptr};
  size_type m_size{0};
  allocator_type m_allocator;
  node_allocator_type m_node_allocator;
};
