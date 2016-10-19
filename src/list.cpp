#include "list.hpp"

using namespace std;

void List::unsafePushFront(Item *element) {
  element->kList = this;
  if (front != nullptr) {
    front->prev = element;
    element->next = front;
  }
  front = element;
  _size++;
}

void List::pushFront(Item *element) {
  lock_guard<mutex> guard(element->kMutex);

  unsafePushFront(element);
}

void List::unsafeRemove(Item *element) {
  if (element->kList != this) {
    return;
  }

  if (element->prev) {
    element->prev->next = element->next;
  }

  if (element->next) {
    element->next->prev = element->prev;
  }

  element->kList = nullptr;
  element->prev = nullptr;
  element->next = nullptr;
  _size--;
}

void List::remove(Item *element) {
  lock_guard<mutex> guard(element->kMutex);

  unsafeRemove(element);
}

void List::moveToFront(Item *element) {
  lock_guard<mutex> guard(element->kMutex);

  if (element->kList != this || front == element) {
    return;
  }

  unsafeRemove(element);
  unsafePushFront(element);
}
