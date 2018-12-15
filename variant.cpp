
#include <variant>
#include <iostream>



struct A { void foo() { std::cout << "A" << std::endl; }
};

struct B { void foo() { std::cout << "B" << std::endl; }
};

int main() {

  std::variant<A, B> v = B{};

  v = B{};
  std::visit([](auto&& x) { x.foo(); }, v);
}
