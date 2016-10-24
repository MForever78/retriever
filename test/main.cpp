#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "retriever/cache.hpp"
#include <thread>

using namespace std;

TEST_CASE("can handle basic type with single thread", "[basic]") {

  SECTION("int to int") {
    Cache<int, int> cache(1024);

    cache.set(0, 1024);
    cache.set(1, 1024);
    REQUIRE(cache.get(0) == 1024);

    cache.set(0, 2048);
    REQUIRE(cache.get(0) == 2048);
  }

  SECTION("string to int") {
    Cache<string, int> cache(1024);
    cache.set("foo", 1024);
    cache.set("bar", 1024);
    REQUIRE(cache.get("foo") == 1024);

    cache.set("foo", 2048);
    REQUIRE(cache.get("foo") == 2048);
  }

  SECTION("string to string") {
    Cache<string, string> cache(1024);
    cache.set("foo", "lorem");
    cache.set("bar", "ipsum");
    REQUIRE(cache.get("foo") == "lorem");

    cache.set("foo", "long");
    REQUIRE(cache.get("foo") == "long");
  }
}

/*
class Custom {
public:
  Custom(string _key, int _id) : key(_key), id(_id){};
  string key;
  int id;

  bool operator==(const Custom &other) const {
    return (key == other.key && id == other.id);
  }
};

template <> struct hash<Custom> {
  size_t operator()(const Custom &c) {
    return hash<string>()(c.key) ^ hash<int>()(c.id);
  }
};

TEST_CASE("can handle custom type with single thread", "[custom]") {
  SECTION("automatically") { Cache<Custom, int> cache(1024); }

  SECTION("manually") {
    struct CustomHasher {
      size_t operator()(const Custom &c) const {
        return hash<string>()(c.key) ^ hash<int>()(c.id);
      }
    };

    Cache<Custom, int, CustomHasher> cache(1024);
    Custom a("foo", 0);
    Custom b("bar", 1);

    cache.set(a, 1024);
    cache.set(b, 2048);
    REQUIRE(cache.get(a) == 1024);

    cache.set(a, 2048);
    REQUIRE(cache.get(a) == 2048);
  }
}
*/

void runner(Cache<int, int> &cache, int l, int r) {
  for (int i = l; i < r; ++i) {
    cache.set(i, i);
  }

  for (int i = l; i < r; ++i) {
    REQUIRE(cache.get(i) == i);
  }
}

TEST_CASE("can handle multithreading", "[multithreading]") {
  Cache<int, int> cache(2048);

  thread t1(runner, ref(cache), 0, 1000);
  thread t2(runner, ref(cache), 1000, 2000);
  t1.join();
  t2.join();
}
