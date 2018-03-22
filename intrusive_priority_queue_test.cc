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
  size_t heap_index;
};

struct Ops {
  void SetIndex(size_t index, Elem* e) const { e->heap_index = index; }
  size_t GetIndex(Elem* e) const { return e->heap_index; }

  bool Less(Elem* e0, Elem* e1) const { return e0->value < e1->value; }
};

struct ElemLess {
  bool operator()(Elem* e0, Elem* e1) const { return e0->value < e1->value; }
};

using Q = IntrusivePriorityQueue<Elem, Ops>;

}  // namespace

TEST(QTest, Basic) {
  Q q;

  ASSERT_EQ(q.Size(), 0);
  Elem e[] = {{0}, {30}, {4}, {5}};
  for (int i = 0; i < 4; i++) {
    q.Add(&e[i]);
  }
  ASSERT_EQ(q.Size(), 4);
  ASSERT_EQ(q.DeleteMin()->value, 0);
  ASSERT_EQ(q.DeleteMin()->value, 4);
  ASSERT_EQ(q.DeleteMin()->value, 5);
  ASSERT_EQ(q.DeleteMin()->value, 30);
  ASSERT_EQ(q.Size(), 0);
}

void DoRandomTest(int seed) {
  using Model = std::multiset<Elem*, ElemLess>;
  std::mt19937 rand(seed);
  Q q;
  Model m;

  for (int round = 0; round < 1000; round++) {
    const int op = std::uniform_int_distribution<>(0, 19)(rand);
    if (op < 11) {
      Elem* elem = new Elem;
      elem->value = rand() % 10000;
      q.Add(elem);
      m.insert(elem);
    } else if (q.Size() > 0) {
      Elem* e0 = q.DeleteMin();
      Elem* e1 = *m.begin();
      m.erase(m.begin());
      ASSERT_TRUE(e0 == e1);
      delete e0;
    }

    ASSERT_EQ(m.size(), q.Size());
    if (q.Size() > 0) {
      Elem* e0 = q.Min();
      Elem* e1 = *m.begin();
      ASSERT_TRUE(e0 == e1);
    }
  }
}

TEST(QTest, Random0) { DoRandomTest(0); }
TEST(QTest, Random1) { DoRandomTest(1); }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
