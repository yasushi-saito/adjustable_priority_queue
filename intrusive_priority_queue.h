#ifndef ADJUSTABLE_PRIORITY_QUEUE_H_
#define ADJUSTABLE_PRIORITY_QUEUE_H_

#include <cstdint>
#include <vector>

template <typename T, typename Ops>
class IntrusivePriorityQueue {
 public:
  explicit IntrusivePriorityQueue(Ops ops = Ops()) : ops_(std::move(ops)) {}

  void Add(T* elem) {
    elems_.push_back(elem);
    ops_.SetIndex(elems_.size() - 1, elem);
    AdjustUp(elem);
  }

  static constexpr size_t kInvalidIndex = std::numeric_limits<size_t>::max();

  size_t Size() const { return elems_.size(); }

  T* Min() const { return elems_[0]; }

  T* DeleteMin() {
    T* min = elems_[0];
    elems_[0] = elems_.back();
    ops_.SetIndex(0, elems_[0]);
    elems_.pop_back();
    if (elems_.empty()) return;
    AdjustDown(elems_[0]);

    ops_.SetIndex(min, kInvalidIndex);
    return min;
  }

  void AdjustDown(T* elem) {
    size_t i_parent = ops_.GetIndex(elem);
    for (;;) {
      size_t i_lc = LeftChild(i_parent);
      size_t i_rc = RightChild(i_parent);

      T*& parent = elems_[i_parent];
      if (i_lc >= elems_.size()) {  // elem is a leaf
        return;
      }
      int i_min = i_lc;

      if (i_rc < elems_.size()) {  // right child doesn't exist.
        i_min = (ops_.Less(elems_[i_lc], elems_[i_rc]) ? i_lc : i_rc);
      }
      T*& child = elems_[i_min];
      if (!ops_.Less(child, parent)) break;
      std::swap(child, parent);
      ops_.SetIndex(child, i_parent);
      ops_.SetIndex(parent, i_min);
      i_parent = i_min;
    }
  }

  void AdjustUp(T* elem) {
    size_t i = ops_.GetIndex(elem);
    while (i != 0) {
      const size_t pi = Parent(i);
      T*& child = elems_[i];
      T*& parent = elems_[pi];
      if (!ops_.Less(child, parent)) break;

      std::swap(child, parent);
      ops_.SetIndex(pi, child);
      ops_.SetIndex(i, parent);
      i = pi;
    }
  }

 private:
  static size_t Parent(size_t i) { return (i - 1) / 2; }
  static size_t LeftChild(size_t i) { return i * 2 + 1; }
  static size_t RightChild(size_t i) { return i * 2 + 2; }

  const Ops ops_;
  std::vector<T*> elems_;
};

#endif  // ADJUSTABLE_PRIORITY_QUEUE_H_
