#include "l2d_utils.h"

using namespace IonaDesktop::CoreDisplay;
using namespace Csm;

void* L2dAllocator::AllocateAligned
    (const Csm::csmSizeType size, const Csm::csmUint32 alignment)
{
    size_t offset, shift, aligned_address;
    void* allocation;
    void** preamble;
    offset = alignment - 1 + sizeof(void*);
    allocation = Allocate(size + static_cast<csmUint32>(offset));
    aligned_address = reinterpret_cast<size_t>(allocation) + sizeof(void*);
    shift = aligned_address % alignment;
    if (shift)
        aligned_address += (alignment - shift);
    preamble = reinterpret_cast<void**>(aligned_address);
    preamble[-1] = allocation;
    return reinterpret_cast<void*>(aligned_address);
}

void L2dAllocator::DeallocateAligned(void *aligned_memory)
{
    void** preamble;
    preamble = static_cast<void**>(aligned_memory);
    Deallocate(preamble[-1]);
}
