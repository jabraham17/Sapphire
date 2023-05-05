#include <iostream>
#include <type_traits>
struct A {
  void print() const {
    std::cout <<"test\n";
  }
  void print() {
    std::cout << "non const\n";
    using ClassType = std::add_pointer_t<std::add_const_t<std::remove_pointer_t<decltype(this)>>>;
    static_assert(std::is_same_v<T, const A*>, "");
    const_cast<T>(this)->print();
  }
};

int main() {
  A a;
  a.print();
  return 0;
}
