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
  unique_lock<mutex> selfLock(element->kMutex);
  unique_lock<mutex> frontLock;

  if (front != nullptr) {
    frontLock = unique_lock<mutex>(front->kMutex, defer_lock);
    if (!frontLock.try_lock()) {
      selfLock.unlock();
      pushFront(element);
      return;
    }
  }

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
  unique_lock<mutex> selfLock(element->kMutex);
  unique_lock<mutex> nextLock, prevLock;

  if (element->next) {
    nextLock = unique_lock<mutex>(element->next->kMutex, defer_lock);
    if (!nextLock.try_lock()) {
      selfLock.unlock();
      remove(element);
      return;
    }
  }

  if (element->prev) {
    prevLock = unique_lock<mutex>(element->prev->kMutex, defer_lock);
    if (!prevLock.try_lock()) {
      if (nextLock.owns_lock()) {
        nextLock.unlock();
      }
      selfLock.unlock();
      remove(element);
      return;
    }
  }

  unsafeRemove(element);
}

void List::moveToFront(Item *element) {
  if (element->kList != this || front == element) {
    return;
  }

  remove(element);
  pushFront(element);
}
