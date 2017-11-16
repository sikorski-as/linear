#include <cstddef>
#include <cstdlib>
#include <string>

#include "Vector.h"
#include "LinkedList.h"
#include <iostream>

namespace
{

template <typename T>
using LinearCollection = aisdi::Vector<T>;

void perfomTest()
{
    LinearCollection<std::string> collection;
    collection.append("TODO");
}

} // namespace


int main(int argc, char** argv)
{
    std::cout << "APLIKACJA PROFILUJACA:\n";
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 100000;
    for (std::size_t i = 0; i < repeatCount; ++i)
        perfomTest();
    return 0;
}
