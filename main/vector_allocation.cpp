#include <iostream>
#include <vector>

static std::size_t allocationCount = 0;

void* operator new(std::size_t sz) // no inline, required by [replacement.functions]/3
{
    std::cout << "Allocating " << sz << " bytes\n";

    allocationCount++;
    if (void* ptr = std::malloc(sz))
        return ptr;
    return nullptr;
    //throw std::bad_alloc{}; // required by [new.delete.single]/3
}
void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

int main()
{
    constexpr int iteration = 1000;
    std::vector<int> number;
    //number.reserve(iteration);
    for(int i = 0; i < iteration; i++)
    {
        number.push_back(rand());
    }
    std::cout << "Allocation count: " << allocationCount << '\n';
    return 0;
}