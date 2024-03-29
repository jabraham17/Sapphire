#ifndef SAPPHIRE_AST_NODE_ASTLIST_H_
#define SAPPHIRE_AST_NODE_ASTLIST_H_

#include <vector>

namespace ast {
namespace node {
class ASTNode;
using ASTList = std::vector<ASTNode*>;

template <class T = ASTNode> class ASTListIterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = const T*;
  using difference_type = ASTList::iterator::difference_type;
  using pointer = value_type*;
  using reference = value_type&;

private:
  ASTList::iterator it_;

public:
  ASTListIterator() = default;
  explicit ASTListIterator(ASTList::iterator it) : it_(it) {}
  ~ASTListIterator() = default;

  ASTListIterator<T>& operator=(const ASTListIterator<T>& it) = default;

  bool operator==(const ASTListIterator<T> rhs) const {
    return this->it_ == rhs.it_;
  }
  bool operator!=(const ASTListIterator<T> rhs) const {
    return this->it_ != rhs.it_;
  }

  T* operator*() const { return (T*)(*(this->it_)); }
  T* operator->() const { return (T*)(*(this->it_)); }

  ASTListIterator<T>& operator++() {
    ++this->it_;
    return *this;
  }
  ASTListIterator<T> operator++(int) {
    ASTListIterator<T> tmp = *this;
    ++this->it_;
    return tmp;
  }

  ASTListIterator<T>& operator--() {
    --this->it_;
    return *this;
  }
  ASTListIterator<T> operator--(int) {
    ASTListIterator<T> tmp = *this;
    --this->it_;
    return tmp;
  }

  ASTListIterator<T> operator+(const difference_type rhs) const {
    return ASTListIterator<T>(this->it_ + rhs);
  }
  ASTListIterator<T> operator-(const difference_type rhs) const {
    return ASTListIterator<T>(this->it_ - rhs);
  }
  difference_type operator-(const ASTListIterator<T> rhs) const {
    return this->it_ - rhs.it_;
  }

  bool operator<(const ASTListIterator<T> rhs) const {
    return this->it_ < rhs.it_;
  }
  bool operator>(const ASTListIterator<T> rhs) const {
    return this->it_ > rhs.it_;
  }
  bool operator<=(const ASTListIterator<T> rhs) const {
    return this->it_ <= rhs.it_;
  }
  bool operator>=(const ASTListIterator<T> rhs) const {
    return this->it_ >= rhs.it_;
  }

  const ASTListIterator<T>& operator+=(const difference_type& rhs) {
    this->it_ += rhs;
    return *this;
  }
  const ASTListIterator<T>& operator-=(const difference_type& rhs) {
    this->it_ -= rhs;
    return *this;
  }

  auto it() const { return this->it_; }
  auto it() { return const_cast<const ASTListIterator<T>*>(this)->it(); }
};

template <class T = ASTNode> struct ASTListIteratorPair {
  ASTListIterator<T> begin_;
  ASTListIterator<T> end_;

  ASTListIteratorPair(ASTList::iterator begin, ASTList::iterator end)
      : begin_(begin), end_(end) {}
  ASTListIteratorPair(ASTListIterator<T> begin, ASTListIterator<T> end)
      : begin_(begin), end_(end) {}
  ~ASTListIteratorPair() = default;

  ASTListIterator<T> begin() const { return begin_; }
  ASTListIterator<T> end() const { return end_; }
  ASTListIterator<T> get(std::size_t idx) const { return begin_ + idx; }
  T* operator[](std::size_t rhs) { return *get(rhs); }
  std::size_t size() const { return std::distance(begin_, end_); }
};

template <class T = ASTNode> class ASTSliceIterator;

template <class T = ASTNode> class ASTListSlice {
public:
  using index_type = std::size_t;
  using index_list_type = std::vector<index_type>;

private:
  const ASTList* backing_;
  index_list_type indicies_;

public:
  ASTListSlice() = default;
  template <typename... Ts>
  ASTListSlice(const ASTList& backing, Ts&&... indicies)
      : backing_(&backing), indicies_() {
    static_assert(
        std::conjunction<std::bool_constant<
            ((std::is_convertible_v<Ts, index_type>), ...)>>::value,
        "");
    (([&] { indicies_.push_back(indicies); }()), ...);
  }
  template <typename InputIt>
  ASTListSlice(
      const ASTList& backing,
      InputIt indiciesBegin,
      InputIt indiciesEnd)
      : backing_(&backing), indicies_() {
    static_assert(
        std::is_convertible_v<
            typename std::iterator_traits<InputIt>::value_type,
            index_type>,
        "");
    indicies_ = index_list_type(indiciesBegin, indiciesEnd);
  }
  ~ASTListSlice() = default;

  const ASTList& backing() { return *this->backing_; }
  index_list_type indicies() { return this->indicies_; }
  index_type backingIdx(index_type idx) { return this->indicies().at(idx); }
  T* get(index_type idx) { return (T*)this->backing().at(backingIdx(idx)); }
  T* operator[](index_type rhs) { return get(rhs); }

  ASTSliceIterator<T> begin() { return ASTSliceIterator<T>(this, 0); }
  ASTSliceIterator<T> end() {
    return ASTSliceIterator<T>(this, this->indicies().size());
  }
};

// non contiguous
template <class T> class ASTSliceIterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T*;
  using difference_type = ASTList::iterator::difference_type;
  using pointer = value_type*;
  using reference = value_type&;

  using index_type = std::size_t;
  using index_list_type = std::vector<index_type>;

private:
  ASTListSlice<T>* slice_;
  index_type currentIndex_;

public:
  ASTSliceIterator() = default;
  explicit ASTSliceIterator(ASTListSlice<T>* slice, index_type currentIndex)
      : slice_(slice), currentIndex_(currentIndex) {}
  ~ASTSliceIterator() = default;

  ASTSliceIterator<T>& operator=(const ASTSliceIterator<T>& it) = default;

  bool operator==(const ASTSliceIterator<T> rhs) const {
    return this->slice_ == rhs.slice_ &&
           this->currentIndex_ == rhs.currentIndex_;
  }
  bool operator!=(const ASTSliceIterator<T> rhs) const {
    return !(*this == rhs);
  }

  T* operator*() const { return (T*)this->slice_->get(this->currentIndex_); }
  T* operator->() const { return (T*)this->slice_->get(this->currentIndex_); }
  T* operator[](index_type rhs) { return (T*)this->slice_->get(rhs); }

  ASTSliceIterator<T>& operator++() {
    ++this->currentIndex_;
    return *this;
  }
  ASTSliceIterator<T> operator++(int) {
    ASTSliceIterator<T> tmp = *this;
    ++this->currentIndex_;
    return tmp;
  }

  ASTSliceIterator<T>& operator--() {
    --this->currentIndex_;
    return *this;
  }
  ASTSliceIterator<T> operator--(int) {
    ASTSliceIterator<T> tmp = *this;
    --this->currentIndex_;
    return tmp;
  }

  ASTSliceIterator<T> operator+(const difference_type rhs) const {
    return ASTSliceIterator<T>(slice_, this->currentIndex_ + rhs);
  }
  ASTSliceIterator<T> operator-(const difference_type rhs) const {
    return ASTSliceIterator<T>(slice_, this->currentIndex_ - rhs);
  }
  difference_type operator-(const ASTSliceIterator<T> rhs) const {
    return ASTSliceIterator<T>(
        slice_,

        this->currentIndex_ - rhs.it_);
  }

  bool operator<(const ASTSliceIterator<T> rhs) const {
    return this->slice_ == rhs.slice_ &&
           this->currentIndex_ < rhs.currentIndex_;
  }
  bool operator>(const ASTSliceIterator<T> rhs) const {
    return this->slice_ == rhs.slice_ &&
           this->currentIndex_ > rhs.currentIndex_;
  }
  bool operator<=(const ASTSliceIterator<T> rhs) const {
    return (*this < rhs) || (*this == rhs);
  }
  bool operator>=(const ASTSliceIterator<T> rhs) const {
    return (*this > rhs) || (*this == rhs);
  }

  const ASTSliceIterator<T>& operator+=(const difference_type& rhs) {
    this->currentIndex_ += rhs;
    return *this;
  }
  const ASTSliceIterator<T>& operator-=(const difference_type& rhs) {
    this->currentIndex_ -= rhs;
    return *this;
  }
};
} // namespace node
} // namespace ast
#endif
