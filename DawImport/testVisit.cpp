#include <string>
#include <vector>
#include <variant>
#include <iostream>

struct letter{

};
struct a:letter{
  std::string name = "a\n";
};
struct b:letter{
  std::string name = "b\n";
};

struct loadClip{
  std::string avariable;
  void operator()(a j){

    std::cout<< avariable;
    std::cout<< "a";
  }
  void operator()(b j){
    std::cout<< avariable;
    std::cout<< "b";
  }
};

struct aoub{
  std::variant<a,b> the_aoub;
};



int main()
{
    aoub d;
    b f{};
    d.the_aoub = f;
    loadClip clipLoader;
    std::visit(clipLoader, f);

}
