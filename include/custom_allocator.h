#pragma once

#include <cstdint>
#include <cstdlib>

inline void* alignForward(const void* address, std::uintptr_t alignment)
{
    return reinterpret_cast<void*>((reinterpret_cast<std::uintptr_t>(address) + alignment - 1) & (~(alignment - 1)));
}

inline std::uintptr_t alignForwardAdjustment(const void* address, std::uintptr_t alignment)
{
    const std::uintptr_t adjustment = alignment - (reinterpret_cast<std::uintptr_t>(address) & (alignment - 1));

    if (adjustment == alignment) return 0;

    //already aligned 
    return adjustment;
}

class Allocator
{
public:
    Allocator(void* rootPtr, std::size_t totalSize) : rootPtr_(rootPtr), totalSize_(totalSize){}
    virtual ~Allocator() = default;
    virtual void* Allocate(std::size_t allocationSize, std::size_t alignment) = 0;
    virtual void Deallocate(void* ptr) = 0;
    [[nodiscard]] std::size_t GetTotalSize() const { return totalSize_; }
    [[nodiscard]] std::size_t GetUsedMemory() const { return usedMemory_; }
    [[nodiscard]] std::size_t GetNumAllocations() const { return numAllocations_; }
protected:
    void* rootPtr_ = nullptr;
    std::size_t totalSize_ = 0;
    std::size_t usedMemory_ = 0;
    std::size_t numAllocations_ = 0;
};

class LinearAllocator final : public Allocator
{
public:
    LinearAllocator(void* rootPtr, std::size_t totalSize);
    void* Allocate(std::size_t allocationSize, std::size_t alignment) override;
    void Deallocate(void* ptr) override;
    void Clear();
private:
    void* currentPtr_ = nullptr;
};

class StackAllocator final : public Allocator
{
public:
    StackAllocator(void* rootPtr, std::size_t totalSize);
    void* Allocate(std::size_t allocationSize, std::size_t alignment) override;
    void Deallocate(void* ptr) override;

private:
    struct AllocationHeader
    {
        std::uint8_t adjustment = 0;
    };
    static std::uintptr_t alignForwardAdjustmentWithHeader(const void* address, std::uintptr_t alignment);
    void* currentPos_ = nullptr;
};

class FreeListAllocator final : public Allocator
{
public:
    FreeListAllocator(void* rootPtr, std::size_t totalSize);

    void* Allocate(std::size_t allocationSize, std::size_t alignment) override;
    void Deallocate(void* ptr) override;
private:
    struct AllocationHeader
    {
        std::size_t allocationSize = 0;
        std::uint8_t adjustment = 0;
    };
    struct FreeBlock
    {
        std::size_t freeSize = 0;
        FreeBlock* next = nullptr;
    };
    FreeBlock* freeBlocks_ = nullptr;
    static std::uintptr_t AlignForwardAdjustmentWithHeader(const void* address, std::uintptr_t alignment);
        
};