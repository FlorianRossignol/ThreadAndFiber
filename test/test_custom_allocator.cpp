#include <custom_allocator.h>
#include <gtest/gtest.h>
#include <array>

TEST(CustomAllocator, Alignement)
{
    void* address;
    constexpr std::uintptr_t alignment = 8;
    void* alignedAddress = reinterpret_cast<void*>(alignment);
    for (std::uintptr_t i = 0; i < 8; i++)
    {
        address = reinterpret_cast<void*>(i);
        if(i > 0)
        {
            EXPECT_EQ(alignedAddress, alignedAddress);
        }
        EXPECT_EQ(alignForwardAdjustment(address, alignment), (alignment-i)%alignment);
    }
}

TEST(CustomAllocator, LinearAllocator)
{
    std::array<char, 1000> data{};
    LinearAllocator allocator(data.data(), data.size());
    for (std::size_t i = 0; i < data.size(); i++)
    {
        void* address = allocator.Allocate(1, 1);
        EXPECT_EQ(address, data.data() + i);
    }
    void* nullAddress = allocator.Allocate(1, 1);
    EXPECT_EQ(nullAddress, nullptr);
    allocator.Clear();

    const auto adjustment = alignForwardAdjustment(data.data()+1, alignof(int));
    allocator.Allocate(1, 1);
    for (std::size_t i = 0; i < (data.size()-1) / 4 ; i++)
    {
        void* address = allocator.Allocate(4, 4);
        EXPECT_EQ(address, data.data() + 1 + 4 * i + adjustment);
    }
    if(adjustment == 0)
    {
        EXPECT_NE(allocator.Allocate(4, 4), nullptr);
    }
    nullAddress = allocator.Allocate(4, 4);
    EXPECT_EQ(nullAddress, nullptr);
    
}


TEST(CustomAllocator, StackAllocator)
{
    std::array<char, 1000> data{};
    {
        StackAllocator allocator(data.data(), data.size());
        for (std::size_t i = 0; i < data.size()/2; i++)
        {
            void* address = allocator.Allocate(1, 1);
            EXPECT_EQ(address, data.data() + 2*i+1);
        }
        void* nullAddress = allocator.Allocate(1, 1);
        EXPECT_EQ(nullAddress, nullptr);
    }
    {
        struct ReverseLinkedListNode
        {
            ReverseLinkedListNode* previousNode = nullptr;
        };
        StackAllocator allocator(data.data(), data.size());
        ReverseLinkedListNode lastNode;
        for(std::size_t i = 0; i < data.size()/sizeof(ReverseLinkedListNode) / 2; i++)
        {
            void* address = allocator.Allocate(sizeof(ReverseLinkedListNode), alignof(ReverseLinkedListNode));
            auto* currentNode = static_cast<ReverseLinkedListNode*>(address);
            *currentNode = lastNode;
            lastNode.previousNode = static_cast<ReverseLinkedListNode*>(address);
        }

        

        while(lastNode.previousNode != nullptr)
        {
            ReverseLinkedListNode* currentLastNode = lastNode.previousNode;
            ReverseLinkedListNode* preNode = currentLastNode->previousNode;
            allocator.Deallocate(currentLastNode);
            lastNode.previousNode = preNode;
        }
    }

}


TEST(CustomAllocator, FreeListAllocator)
{
    std::array<char, 1000> data{};
    {
        constexpr auto allocationSize = 250ul;
        FreeListAllocator allocator(data.data(), data.size());
        auto* address1 = allocator.Allocate(allocationSize, 8);
        auto* address2 = allocator.Allocate(allocationSize, 8);
        auto* address3 = allocator.Allocate(allocationSize, 8);
        EXPECT_GE(reinterpret_cast<std::uintptr_t>(address2) - reinterpret_cast<std::uintptr_t>(address1), 
            allocationSize);
        allocator.Deallocate(address2);
        allocator.Deallocate(address3);
        allocator.Deallocate(address1);
    }

    {
        constexpr auto allocationSize = 250ul;
        FreeListAllocator allocator(data.data(), data.size());
        auto* address1 = allocator.Allocate(allocationSize, 8);
        auto* address2 = allocator.Allocate(allocationSize, 8);
        auto* address3 = allocator.Allocate(allocationSize, 8);
        EXPECT_GE(reinterpret_cast<std::uintptr_t>(address2) - reinterpret_cast<std::uintptr_t>(address1),
            allocationSize);
        allocator.Deallocate(address1);
        allocator.Deallocate(address2);
        allocator.Deallocate(address3);
    }

}