#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <iostream>
#include <string>

/* пример использования парсига структтуры по имени параметров */

struct MyStruct {
  std::string foo;
  double bar;
  std::string test;
  int a;
};

BOOST_FUSION_ADAPT_STRUCT(MyStruct, foo, bar, test, a)

namespace fuz = boost::fusion;
namespace mpl = boost::mpl;

int main(int argc, char* argv[]) {
  MyStruct dummy;
  dummy.foo = "foo_struct";
  dummy.bar = 12;
  dummy.test = "test_struct";
  dummy.a = 1;

  fuz::for_each(
      mpl::range_c<unsigned, 0, fuz::result_of::size<MyStruct>::value>(),
      [&](auto index) {
        std::cout << "Name: "
                  << fuz::extension::struct_member_name<MyStruct, index>::call()
                  << " Value: " << fuz::at_c<index>(dummy) << std::endl;
      });
}


/* Работает еще и в обратную сторону, очень удобно парсить настройки, по имени члена структуры */
fuz::for_each(
    mpl::range_c<unsigned, 0, fuz::result_of::size<MyStruct>::value>(),
    [&](auto index) {
      std::string key(
          fuz::extension::struct_member_name<MyStruct, index>::call());

      if (name == key) {
        fuz::at_c<index>(dummy) = value;
      }
    });
