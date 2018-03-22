#ifndef ADJUSTABLE_PRIORITY_QUEUE_H_
#define ADJUSTABLE_PRIORITY_QUEUE_H_

#include <cstdint>
#include <vector>

// IntrusivePriorityQueue is a heap-based priority queue that allows the caller
// to modify the element in place. Thread compatible.
//
// The template parameter Ops is a class that provides the following methods:
//
//   void SetIndex(size_t index, T* e) const;
//   size_t GetIndex(T* e) const;
//   bool Less(T* e0, Elem* e1) const;
//
// SetIndex is called when "e" is going to be stored in the index'th position in
// the queue.  The SetIndex implementation must remember the value of index
// somewhere, so that a future call to GetIndex will return its value.
//
// Less should return true if *e0 < *e1. The priority queue will sort elements
// in the ascending order according to Less.
template <typename T, typename Ops>
class IntrusivePriorityQueue {
 public:
  static constexpr size_t kInvalidIndex = std::numeric_limits<size_t>::max();

  explicit IntrusivePriorityQueue(Ops ops = Ops()) : ops_(std::move(ops)) {}

  // Add a new element to the queue. "elem" remains owned by the caller. "elem"
  // must remain live until the caller removes it from the queue (e.g., using
  // DeleteMin), or until the queue is deleted.
  void Add(T* elem) {
    elems_.push_back(elem);
    ops_.SetIndex(elems_.size() - 1, elem);
    AdjustUp(elem);
  }

  // Return the number of elements in the queue.
  size_t Size() const { return elems_.size(); }

  // Return the smallest element.
  T* Min() const { return elems_[0]; }

  // Remove the smallest element and return it.
  T* DeleteMin() {
    T* min = elems_[0];
    ops_.SetIndex(kInvalidIndex, min);

    elems_[0] = elems_.back();
    ops_.SetIndex(0, elems_[0]);
    elems_.pop_back();
    if (elems_.empty()) return min;
    AdjustDown(elems_[0]);

    return min;
  }

  // AdjustDown must be called when caller has increased the value of "elem".
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
      ops_.SetIndex(i_parent, child);
      ops_.SetIndex(i_min, parent);
      i_parent = i_min;
    }
  }

  // AdjustDown must be called when caller has decreased the value of "elem".
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
