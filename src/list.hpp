#ifndef RETRIEVER_LIST_HPP
#define RETRIEVER_LIST_HPP

#include <atomic>
#include <mutex>

class List;

class Item {
public:
  Item() : prev(nullptr), next(nullptr), kList(nullptr){};
  Item *prev, *next;
  List *kList;

  std::mutex kMutex;
};

class List {
public:
  List() : front(nullptr), back(nullptr), _size(0){};

  void pushFront(Item *element);
  void remove(Item *element);
  void moveToFront(Item *element);

  bool empty() { return _size == 0; };
  int size() { return _size; };

  Item *front;
  Item *back;

private:
  void unsafePushFront(Item *element);
  void unsafeRemove(Item *element);
  std::atomic<int> _size;
};

#endif
