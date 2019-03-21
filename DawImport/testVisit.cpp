#include <string>
#include <vector>
#include <variant>
#include <iostream>

struct a{
  std::string name = "a";
};
struct b{
  std::string name = "b";
};

struct loadClip{
  void operator()(a j){
    std::cout<< "a";
  }
  void operator()(b j){
    std::cout<< "b";
  }
};

struct aoub{
  std::variant<a,b> the_aoub;
};

int main()
{
    aoub d;
    a f{};
    d.the_aoub = f;
    loadClip clipLoader;
    std::visit(clipLoader, d.the_aoub);

}
