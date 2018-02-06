#include "indefinite-stream.hpp"
#include <vector>
#include <algorithm>

int main()
{
  indefinite::IndefiniteStream<int> a(3);
  a.filter(
    [](int value) {
      return (value % 5 == 0);
    })
   .filter(
     [](int value) {
       return (value % 9 == 0);
     });

  std::cout << "One by one: " << '\n';
  std::cout << a.top() << '\n';
  std::cout << a.pop() << '\n';
  std::cout << a.pop() << '\n';
  std::cout << a.pop() << '\n';
  std::cout << a.pop() << '\n';
  std::cout << '\n';
  
  int size = 10;
  std::cout << "Get " << size << " numbers: " << '\n';
  indefinite::IndefiniteStream<int> b(2, 5);
  std::vector<int> v = b.filter(
    [](int value) {
      return (value % 7 == 0);
    })
   .filter(
     [](int value) {
       return (value % 11 == 0);
     })
   .limit(size);
  std::for_each(v.begin(), v.end(), [](int value){ std::cout << value << ' '; });

  return 0;
}

