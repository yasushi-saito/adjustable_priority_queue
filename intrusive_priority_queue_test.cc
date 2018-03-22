#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>

#include "intrusive_priority_queue.h"

namespace {
struct Elem {
  int value;
  int heap_index;
};

struct Ops {
  void SetIndex(int index, Elem* e) const {
    e->heap_index=index;
  }
  int GetIndex(Elem* e) const {
    return e->heap_index;
  }

  bool Less(Elem*e0, Elem*e1) const{
    return e0->value<e1->value;
  }
};

using Q = IntrusivePriorityQueue<Elem, Ops>;

}  // namespace

TEST(QTest, Basic) {
  Q q;

  Elem e0 = {1};
  Elem e1 = {10};
  q.Add(&e0);
  q.Add(&e1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
