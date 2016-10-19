#ifndef RETRIEVER_CACHE_HPP
#define RETRIEVER_CACHE_HPP

#include "list.hpp"
#include <memory>
#include <shared_mutex>
#include <unordered_map>

template <typename K, typename V> class CacheItem : public Item {
public:
  CacheItem(K k, V v) : key(k), value(v){};
  K key;
  V value;
};

template <typename K, typename V> class Cache {
public:
  explicit Cache(const int capacity) : capacity(capacity) {
    lookUpTable =
        std::unordered_map<K, std::unique_ptr<CacheItem<K, V>>>(capacity);
  };

  void set(const K key, const V value) {
    std::unique_lock<std::shared_mutex> lock(tableSMutex);

    auto mapIt = lookUpTable.find(key);
    if (mapIt == lookUpTable.end()) {
      std::unique_ptr<CacheItem<K, V>> item(new CacheItem<K, V>(key, value));
      if (lookUpTable.size() == capacity) {
        auto toRemove = kList.back;
        kList.remove(toRemove);
        lookUpTable.erase(
            lookUpTable.find(dynamic_cast<CacheItem<K, V> *>(toRemove)->key));
      }
      kList.pushFront(dynamic_cast<Item *>(item.get()));
    } else {
      auto item = mapIt->second;
      item->value = value;
      kList.moveToFront(dynamic_cast<Item *>(item.get()));
    }
  };

  V get(const K key) {
    std::shared_lock<std::shared_mutex> lock(tableSMutex);

    auto mapIt = lookUpTable.find(key);
    if (mapIt == lookUpTable.end()) {
      return V();
    }
    kList.moveToFront(dynamic_cast<Item *>(mapIt->second->get()));
    return mapIt->second->value;
  };

private:
  Cache(){};

  int capacity;
  std::unordered_map<K, std::unique_ptr<CacheItem<K, V>>> lookUpTable;
  std::shared_mutex tableSMutex;
  List kList;
};

#endif
