#include "custom_allocator.h"
#include "custom_allocator.h"
#include <algorithm>
#include <iostream>

LinearAllocator::LinearAllocator(void* rootPtr, std::size_t totalSize) :
    Allocator(rootPtr, totalSize),
    currentPtr_(rootPtr)
{
}

void* LinearAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    const auto adjustment = alignForwardAdjustment(currentPtr_, alignment);
    if (usedMemory_ + adjustment + allocationSize > totalSize_) [[unlikely]]
        return nullptr;

    const auto alignedAddress = reinterpret_cast<std::uintptr_t>(currentPtr_) + adjustment;
    currentPtr_ = reinterpret_cast<void*>(alignedAddress + allocationSize);
    usedMemory_ += allocationSize + adjustment;
    numAllocations_++;
    return reinterpret_cast<void*>(alignedAddress);
}

void LinearAllocator::Deallocate(void* ptr)
{
}


void LinearAllocator::Clear()
{
    currentPtr_ = rootPtr_;
    numAllocations_ = 0;
    usedMemory_ = 0;
}

StackAllocator::StackAllocator(void* rootPtr, std::size_t totalSize) :
    Allocator(rootPtr, totalSize),
    currentPos_(rootPtr)
{
}

void* StackAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    const auto adjustment = alignForwardAdjustmentWithHeader(currentPos_, alignment);
    if (usedMemory_ + adjustment + allocationSize > totalSize_) [[unlikely]]
        return nullptr;
    const std::uintptr_t alignedAddress = reinterpret_cast<std::uintptr_t>(currentPos_) + adjustment;
    auto* header = reinterpret_cast<AllocationHeader*>(alignedAddress - sizeof(AllocationHeader));
    header->adjustment = adjustment;

    currentPos_ = reinterpret_cast<void*>(alignedAddress + allocationSize);
    usedMemory_ += allocationSize + adjustment;
    numAllocations_++;
    return reinterpret_cast<void*>(alignedAddress);
}

void StackAllocator::Deallocate(void* ptr)
{
    const auto* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uintptr_t>(ptr) - sizeof(AllocationHeader));
    usedMemory_ -= reinterpret_cast<std::uintptr_t>(currentPos_) - reinterpret_cast<std::uintptr_t>(ptr) + header->adjustment;
    currentPos_ = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(ptr)- header->adjustment);
}

std::uintptr_t StackAllocator::alignForwardAdjustmentWithHeader(const void* address, std::uintptr_t alignment)
{
    auto adjustment = alignForwardAdjustment(address, alignment);
    if(adjustment == 0)
    {
        adjustment += alignment;
    }
    return adjustment;
}

FreeListAllocator::FreeListAllocator(void* rootPtr, std::size_t totalSize) : Allocator(rootPtr, totalSize)
{
    const auto adjustment = alignForwardAdjustment(rootPtr, alignof(AllocationHeader));
    freeBlocks_ = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(rootPtr)+adjustment);
    freeBlocks_->freeSize = totalSize - adjustment;
    freeBlocks_->next = nullptr;

}

void* FreeListAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks_;

    while (freeBlock != nullptr)
    {
        //Calculate adjustment needed to keep object correctly aligned 
        const auto adjustment = AlignForwardAdjustmentWithHeader(freeBlock, alignment);
        size_t total_size = allocationSize + adjustment;

        //If allocation doesn't fit in this FreeBlock, try the next 
        if (freeBlock->freeSize < total_size)
        {
            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
            continue;
        }

        static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

        //If allocations in the remaining memory will be impossible 
        if (freeBlock->freeSize - total_size <= sizeof(AllocationHeader))
        {
            //Increase allocation size instead of creating a new FreeBlock 
            total_size = freeBlock->freeSize;

            if (prevFreeBlock != nullptr)
                prevFreeBlock->next = freeBlock->next;
            else
                freeBlocks_ = freeBlock->next;
        }
        else
        {
            //Else create a new FreeBlock containing remaining memory 
            auto* next_block = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(freeBlock) + total_size);

            next_block->freeSize = freeBlock->freeSize - total_size;
            next_block->next = freeBlock->next;

            if (prevFreeBlock != nullptr)
                prevFreeBlock->next = next_block;
            else
                freeBlocks_ = next_block;
        }

        const auto aligned_address = reinterpret_cast<std::uintptr_t>(freeBlock) + adjustment;
        auto* header = reinterpret_cast<AllocationHeader*>(aligned_address - sizeof(AllocationHeader));
        header->allocationSize = total_size;
        header->adjustment = adjustment;
        usedMemory_ += total_size;
        numAllocations_++;

        return reinterpret_cast<void*>(aligned_address);
    }

    //ASSERT(false && "Couldn't find free block large enough!"); 
    return nullptr;
}

void FreeListAllocator::Deallocate(void* ptr)
{
    const auto* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uintptr_t>(ptr)- sizeof(AllocationHeader));
    const std::uintptr_t blockStart = reinterpret_cast<std::uintptr_t>(ptr) - header->adjustment;
    const std::size_t blockSize = header->allocationSize;
    const std::uintptr_t blockEnd = blockStart + blockSize;
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks_;

    while (freeBlock != nullptr)
    {
        if (reinterpret_cast<std::uintptr_t>(freeBlock) >= blockEnd) break;
        prevFreeBlock = freeBlock;
        freeBlock = freeBlock->next;
    }

    if (prevFreeBlock == nullptr)
    {
        prevFreeBlock = reinterpret_cast<FreeBlock*>(blockStart);
        prevFreeBlock->freeSize = blockSize;
        prevFreeBlock->next = freeBlocks_;
        freeBlocks_ = prevFreeBlock;
    }
    else if (reinterpret_cast<std::uintptr_t>(prevFreeBlock) + prevFreeBlock->freeSize == blockStart)
    {
        prevFreeBlock->freeSize += blockSize;
    }
    else
    {
        auto* temp = reinterpret_cast<FreeBlock*>(blockStart);
        temp->freeSize = blockSize;
        temp->next = prevFreeBlock->next;
        prevFreeBlock->next = temp;
        prevFreeBlock = temp;
    }

    if (freeBlock != nullptr && reinterpret_cast<std::uintptr_t>(freeBlock) == blockEnd)
    {
        prevFreeBlock->freeSize += freeBlock->freeSize;
        prevFreeBlock->next = freeBlock->next;
    }

    numAllocations_--;
    usedMemory_ -= blockSize;
}

std::uintptr_t FreeListAllocator::AlignForwardAdjustmentWithHeader(const void* address, std::uintptr_t alignment)
{
    const auto actualAlignment = std::max(alignment, alignof(AllocationHeader));
    auto adjustment = alignForwardAdjustment(address, actualAlignment);
    constexpr auto neededSpace = sizeof(AllocationHeader);
    while (adjustment < neededSpace)
    {
        adjustment += alignment;
    }
    return adjustment;
}
