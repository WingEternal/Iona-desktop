#ifndef IONA_DESKTOP_CORE_EX1_L2D_ALLOCATOR_H
#define IONA_DESKTOP_CORE_EX1_L2D_ALLOCATOR_H

#include "CubismFramework.hpp"
#include  "ICubismAllocator.hpp"
#include <cstdlib>

namespace IonaDesktop {
namespace CoreEx1 {
    class L2dAllocator : public Csm::ICubismAllocator
    {
        inline void* Allocate(const Csm::csmSizeType size)
        { return malloc(size); }
        inline void Deallocate(void* memory)
        { free(memory); }
        void* AllocateAligned(const Csm::csmSizeType size, const Csm::csmUint32 alignment);
        void DeallocateAligned(void* aligned_memory);
    };
}
}

#endif
