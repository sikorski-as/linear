#include <cstddef>
#include <cstdlib>
#include <string>

#include <iostream>
#include <chrono>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinkedList = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

void performLinkedListTest(std::size_t n)
{

  // appending tests

  std::chrono::time_point<std::chrono::system_clock> start, end;
  LinkedList<std::string> collection;
  std::cout<<"LinkedList:\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection.append("ELEMENCIK");
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> timeTaken = end-start;
  std::cout << "Appending "<<n<<" elements takes: " << timeTaken.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection.erase(collection.begin());
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Erasing "<<n<<" elements from the beginning takes: " << timeTaken.count() << "s\n";



  // prepending tests:



  LinkedList<std::string> collection2;
  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection2.prepend("ELEMENCIK");
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Prepending "<<n<<" elements takes: " << timeTaken.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n-1; ++i)
  {
    collection2.erase(collection2.end()-1);
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Erasing "<<n<<" elements from the end takes: " << timeTaken.count() << "s\n";
}

void performVectorTest(std::size_t n)
{
  // appending tests

  std::chrono::time_point<std::chrono::system_clock> start, end;
  Vector<std::string> collection;
  std::cout<<"Vector:\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection.append("ELEMENCIK");
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> timeTaken = end-start;
  std::cout << "Appending "<<n<<" elements takes: " << timeTaken.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection.erase(collection.begin());
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Erasing "<<n<<" elements from the beginning takes: " << timeTaken.count() << "s\n";

   // prepending tests:

  Vector<std::string> collection2;
  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection2.prepend("ELEMENCIK");
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Prepending "<<n<<" elements takes: " << timeTaken.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
  {
    collection2.erase(collection2.end()-1);
  }
  end = std::chrono::system_clock::now();
  timeTaken = end-start;
  std::cout << "Erasing "<<n<<" elements from the end takes: " << timeTaken.count() << "s\n";
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  std::cout<<"\n\n-------------------- AISDI Linear time tests --------------------" << std::endl;
  performLinkedListTest(repeatCount);
  std::cout<<"----------------------------------------\n";
  performVectorTest(repeatCount);
  std::cout<<"-------------------- AISDI Linear time tests end ----------------\n" << std::endl;
  return 0;
}
