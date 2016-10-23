#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "retriever/cache.hpp"
#include <iostream>

TEST_CASE("Int to int", "[int:int]") {
  Cache<int, int> cache(1024);

  cache.set(0, 1024);
  cache.set(1, 1024);
  REQUIRE(cache.get(0) == 1024);

  cache.set(0, 2048);
  REQUIRE(cache.get(0) == 2048);
}
