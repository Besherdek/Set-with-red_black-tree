#pragma once
#include <initializer_list>
#include <iterator>
/* 
  Your task here is to define a set of integers. Since this is not a template class 
  you must declare your functions in the header file and define them in .cpp file.
*/

struct Node {
  int value;
  Node* left;
  Node* right;
  Node* parent;
  char color;
};

class Set
{
private:
  Node* root;
  int size_;
public:
  //constructors
  Set();
  Set(std::initializer_list<int> init);
  Set(Set& other);

  //iterator start
  class iterator {
  private:
    Node* cur;
  public:
    using value_type = int;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = int*;
    using reference = int&;
    
    iterator(Node* other);
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
    const int& operator*();
  };

  //member functions
  void insert(const int value_to_insert);
  bool empty();
  int size();
  bool contains(int x) const;
  void erase(int x);
  void erase(iterator position);
  iterator begin();
  iterator end();
  iterator begin() const;
  iterator end() const;
  Set& operator=(Set other);
  bool operator==(const Set& other) const;
  bool operator!=(const Set& other) const;
  iterator find(int x) const;
  iterator upper_bound(const int i);
  iterator lower_bound(const int i);

  //Destructor
  ~Set();
};