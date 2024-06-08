#include "Set.h"
#include <algorithm>

Node* right_rotation(Node* root) {
  Node* _left = root->left;
  Node* _left_right = _left->right;
  if(root->parent) {
    if(root == root->parent->left) {
      root->parent->left = _left;
    }
    else {
      root->parent->right = _left;
    }
  }
  _left->parent = root->parent;
  root->parent = _left;
  root->left = _left_right;
  if(_left_right)
    _left_right->parent = root;
  _left->right = root;
  return root->parent;
}

Node* left_rotation(Node* root) {
  Node* _right = root->right;
  Node* _right_left = _right->left;
  if(root->parent) {
    if(root == root->parent->left) {
      root->parent->left = _right;
    }
    else {
      root->parent->right = _right;
    }
  }
  _right->parent = root->parent;
  root->parent = _right;
  root->right = _right_left;
  if(_right_left)
    _right_left->parent = root;
  _right->left = root;
  return root->parent;
}

Set::Set(): root(nullptr), size_(0) {}

Set::iterator::iterator(Node* other): cur(other) {}

void Set::insert(const int value_to_insert) {
  Node* current = root;
  
  while(current != nullptr) {
    
    if(value_to_insert == current->value) {
      break;
    }
    
    if(value_to_insert < current->value) {
      if(current->left == nullptr) {
        current->left = new Node{value_to_insert, nullptr, nullptr, current, 'r'};
        ++size_;
      }
      current = current->left;
    }
      
    else if(value_to_insert > current->value) {
      if(current->right == nullptr) {
        current->right = new Node{value_to_insert, nullptr, nullptr, current, 'r'};
        ++size_;
      }
      current = current->right;
    }
    
  }
  
  if(current == nullptr) {
    root = new Node{value_to_insert, nullptr, nullptr, nullptr, 'b'};
    ++size_;
    current = root;
  }

  //if current node violates the rules(red's parent is red)
  while((current->parent != nullptr) && (current->parent->color == 'r' && current->color == 'r')) {
    //if current's parent is the left child of current's grandparent
    if(current->parent == current->parent->parent->left) {
      //current's uncle is black
      if(current->parent->parent->right == nullptr || current->parent->parent->right->color == 'b') {
        //if current, parent and grandparent form a line
        if(current == current->parent->left) {
          current = right_rotation(current->parent->parent);
          current->right->color = 'r';
          current->color = 'b';
        }
        //if they form a triangle
        else {
          current = left_rotation(current->parent);
          current = right_rotation(current->parent);
          current->right->color = 'r';
          current->color = 'b';
        }
      }
      //if current's uncle is red
      else {
        current->parent->parent->right->color = 'b';
        current->parent->color = 'b';
        current->parent->parent->color = 'r';
        current = current->parent->parent;
      }
    }
    //if current's parent is the right child of current's grandparent
    else {
      //current's uncle is black
      if(current->parent->parent->left == nullptr || current->parent->parent->left->color == 'b') {
        //if current, parent and grandparent form a line
        if(current == current->parent->right) {
          current = left_rotation(current->parent->parent);
          current->left->color = 'r';
          current->color = 'b';
        }
        //if they form a triangle
        else {
          current = right_rotation(current->parent);
          current = left_rotation(current->parent);
          current->left->color = 'r';
          current->color = 'b';
        }
      }
      //if current's uncle is red
      else {
        current->parent->parent->left->color = 'b';
        current->parent->color = 'b';
        current->parent->parent->color = 'r';
        current = current->parent->parent;
      }
    }
  }

  while(current->parent) {
    current = current->parent;
  }
  root = current;
  
  root->color = 'b';
}

bool Set::empty() {
  return !size_;
}

int Set::size() {
  return size_;
}

bool Set::contains(int x) const{
  Node* cur = root;
  while(cur != nullptr) {
    if(x < cur->value) {
      cur = cur->left;
    }
    else if(x > cur->value) {
      cur = cur->right;
    }
    else {
      return true;
    }
  }
  return false;
}

Set::~Set() {
  Node* current = root;
  Node* temp;
  while(current != nullptr) {
    if(current->left) {
      current = current->left;
    }
    else if(current->right) {
      current = current->right;
    }
    else if(current->parent) {
      temp = current->parent;
      if(current == temp->left) {
        temp->left = nullptr;
      }
      else {
        temp->right = nullptr;
      }
      delete current;
      current = nullptr;
      current = temp;
    }
    else {
      delete current;
      current = nullptr;
    }
  }
  size_ = 0;
}

void Set::erase(int x) {
  Node* to_delete = root;
  //search node with that value
  while(to_delete != nullptr) {
    if(x == to_delete->value) {
      break;
    }
    else if(x < to_delete->value) {
      to_delete = to_delete->left;
    }
    else {
      to_delete = to_delete->right;
    }
  }
  if(to_delete == nullptr)
    return;
  else
    --size_;
  
  //find his predecessor
  Node* pred;
  if(to_delete->left) {
    pred = to_delete->left;
    while(true) {
      if(pred->right)
        pred = pred->right;
      else
        break;
    }
  }
  else
    pred = to_delete;

  //"swap" with predecessor
  to_delete->value = pred->value;
  to_delete = pred;

  //if the node we have to delete is the root
  if(!to_delete->parent) {
    root = root->right;
    if(root) { 
      root->color = 'b';
      root->parent = nullptr;
    }
    delete to_delete;
    return;
  }

  //case 1: if the predecessor is a red node
  if(to_delete->color == 'r') {
    //if pred. is the right child
    if(to_delete == to_delete->parent->right) {
      if(to_delete->left) {
        to_delete->parent->right = to_delete->left;
        to_delete->left->parent = to_delete->parent;
      }
      else {
        to_delete->parent->right = to_delete->right;
        if(to_delete->right)
          to_delete->right->parent = to_delete->parent;
      }
    }
    else {
      if(to_delete->left) { 
        to_delete->parent->left = to_delete->left;
        to_delete->left->parent = to_delete->parent;
      }
      else {
        to_delete->parent->left = to_delete->right;
        if(to_delete->right)
          to_delete->right->parent = to_delete->parent;
      }
    }
    delete to_delete;
    return;
  }

  //case 2: if the predecessor is a black node
  if(to_delete->color == 'b') {
    //case 2.1: if the child's color is red
    if(to_delete->left && to_delete->left->color == 'r') {
      to_delete->left->color = 'b';
      if(to_delete == to_delete->parent->right) {
        to_delete->parent->right = to_delete->left;
        to_delete->left->parent = to_delete->parent;
      }
      else {
        to_delete->parent->left = to_delete->left;
        to_delete->left->parent = to_delete->parent;
      }
      return;
    }
    else if(to_delete->right && to_delete->right->color == 'r') {
      to_delete->right->color = 'b';
      if(to_delete == to_delete->parent->right) {
        to_delete->parent->right = to_delete->right;
          to_delete->right->parent = to_delete->parent;
      }
      else {
        to_delete->parent->left = to_delete->right;
        to_delete->right->parent = to_delete->parent;
      }
      return;
    }
    
    //if we have not null child, delete current node, and set the child's color to double-black
    if(to_delete->left) {
      if(to_delete == to_delete->parent->left) {
        to_delete->parent->left = to_delete->left;
      }
      else {
        to_delete->parent->right = to_delete->left;
      }
      to_delete->left->parent = to_delete->parent;
      to_delete->left->color = 'd';
      Node* temp = to_delete->left;
      delete to_delete;
      to_delete = temp;
    }
    else if(to_delete->right) {
      if(to_delete == to_delete->parent->left) {
        to_delete->parent->left = to_delete->right;
      }
      else {
        to_delete->parent->right = to_delete->right;
      }
      to_delete->right->parent = to_delete->parent;
      to_delete->right->color = 'd';
      Node* temp = to_delete->right;
      delete to_delete;
      to_delete = temp;
    }
    //if we don't have a child, we ourself become double-black. we are deleted later
    else {
      to_delete->color = 't';
    }
    
    //if we are parent's left child
    if(to_delete == to_delete->parent->left) {
      while(to_delete->color == 'd' || to_delete->color == 't') {
        //case 2.2: the root is double-black node
        if(to_delete->parent == nullptr) {
          to_delete->color = 'b';
          break;
        }
        //case 2.3: the sibling is a red node
        else if(to_delete->parent->right->color == 'r') {
          to_delete->parent->right->color = 'b';
          to_delete->parent->color = 'r';
          left_rotation(to_delete->parent);
        }
        //case 2.4: sibling's right child is red
        else if(to_delete->parent->right->right != nullptr && to_delete->parent->right->right->color == 'r') {
          to_delete->parent->right->right->color = 'b';
          to_delete->parent->right->color = to_delete->parent->color;
          to_delete->parent->color = 'b';
          left_rotation(to_delete->parent);
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->left = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.5: siblings right child is black and left child is red
        else if(to_delete->parent->right->left != nullptr && to_delete->parent->right->left->color == 'r') {
          right_rotation(to_delete->parent->right);
          to_delete->parent->right->right->color = 'b';
          to_delete->parent->right->color = to_delete->parent->color;
          to_delete->parent->color = 'b';
          left_rotation(to_delete->parent);
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->left = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.6: parent is red and siblings both children are black
        else if(to_delete->parent->color == 'r') {
          to_delete->parent->color = 'b';
          to_delete->parent->right->color = 'r';
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->left = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.7: parent is black, sibling is black and siblings both children are black
        else {
          to_delete->parent->color = 'd';
          to_delete->parent->right->color = 'r';
          Node* temp = to_delete->parent;
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->left = nullptr;
            delete to_delete;
          }
          to_delete = temp;
        }
      }
    }
    //if we are parent's right child, we just left->right and right->left
    else {
      while(to_delete->color == 'd' || to_delete->color == 't') {
        //case 2.2: the root is double-black node
        if(to_delete->parent == nullptr) {
          to_delete->color = 'b';
          break;
        }
        //case 2.3: the sibling is a red node
        else if(to_delete->parent->left->color == 'r') {
          to_delete->parent->left->color = 'b';
          to_delete->parent->color = 'r';
          right_rotation(to_delete->parent);
        }
        //case 2.4: sibling's left child is red
        else if(to_delete->parent->left->left != nullptr && to_delete->parent->left->left->color == 'r') {
          to_delete->parent->left->left->color = 'b';
          to_delete->parent->left->color = to_delete->parent->color;
          to_delete->parent->color = 'b';
          right_rotation(to_delete->parent);
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->right = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.5: siblings left child is black and right child is red
        else if(to_delete->parent->left->right != nullptr && to_delete->parent->left->right->color == 'r') {
          right_rotation(to_delete->parent->left);
          to_delete->parent->left->left->color = 'b';
          to_delete->parent->left->color = to_delete->parent->color;
          to_delete->parent->color = 'b';
          left_rotation(to_delete->parent);
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->right = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.6: parent is red and siblings both children are black
        else if(to_delete->parent->color == 'r') {
          to_delete->parent->color = 'b';
          to_delete->parent->left->color = 'r';
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->right = nullptr;
            delete to_delete;
          }
          break;
        }
        //case 2.7: parent is black, sibling is black and siblings both children are black
        else {
          to_delete->parent->color = 'd';
          to_delete->parent->left->color = 'r';
          Node* temp = to_delete->parent;
          if(to_delete->color == 'd') {
            to_delete->color = 'b';
          }
          else {
            to_delete->parent->right = nullptr;
            delete to_delete;
          }
          to_delete = temp;
        }
      }
    }
  }
  while(root && root->parent) {
    root = root->parent;
  }
}

Set::Set(std::initializer_list<int> init): Set() {
  for(int i: init) {
    Set::insert(i);
  }
}

Set::Set(Set& other): Set() {
  for(auto it = other.begin(); it != other.end(); ++it) {
    Set::insert(*it);
  }
}

Set::iterator Set::begin() {
  Node* cur = root;
  while(cur && cur->left) {
    cur = cur->left;
  }
  return iterator{cur};
}

Set::iterator Set::end() {
  return iterator{nullptr};
}

Set::iterator Set::begin() const{
  Node* cur = root;
  while(cur->left) {
    cur = cur->left;
  }
  return iterator{cur};
}

Set::iterator Set::end() const{
  return iterator{nullptr};
}

Set::iterator& Set::iterator::operator++() {
  if(!cur)
    return *this;
  if(cur->right) {
    cur = cur->right;
    while(cur->left)
      cur = cur->left;
  }
  else if(cur->parent) {
    while(cur->parent && cur == cur->parent->right) {
      cur = cur->parent;
    }
    if(!cur->parent) {
      cur = nullptr;
      return *this;
    }
    if(cur == cur->parent->left)
      cur = cur->parent;
  }
  else {
    cur = nullptr;
  }
  return *this;
}

Set::iterator Set::iterator::operator++(int) {
  auto temp = *this;
  ++(*this);
  return temp;
}

bool Set::iterator::operator==(const iterator& other) const {
  return cur == other.cur;
}

bool Set::iterator::operator!=(const iterator& other) const {
  return cur != other.cur;
}

const int& Set::iterator::operator*() {
  return cur->value;
}

Set& Set::operator=(Set other) {
  size_ = other.size_;
  Node* temp = root;
  root = other.root;
  other.root = temp;
  return *this;
}

bool Set::operator==(const Set& other) const {
  if(size_ != other.size_)
    return false;
  for(auto it = begin(); it != end(); ++it) {
    if(!other.contains(*it)) {
      return false;
    }
  }
  return true;
}

bool Set::operator!=(const Set& other) const {
  return !(*this == other);
}

Set::iterator Set::find(int x) const {
  Node* cur = root;
  while(cur != nullptr) {
    if(x < cur->value)
      cur = cur->left;
    else if(x > cur->value)
      cur = cur->right;
    else 
      break;
  }
  return iterator{cur};
}

Set::iterator Set::lower_bound(const int i) {
  auto it = begin();
  for(; it != end(); ++it) {
    if(*it >= i)
      break;
  }
  return it;
}

Set::iterator Set::upper_bound(const int i) {
  auto it = begin();
  for(; it != end(); ++it) {
    if(*it > i)
      break;
  }
  return it;
}

void Set::erase(iterator position) {
  Set::erase(*position);
}