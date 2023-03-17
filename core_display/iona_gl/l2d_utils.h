#ifndef IONA_DESKTOP_CORE_DISPLAY_L2D_UTILS_H
#define IONA_DESKTOP_CORE_DISPLAY_L2D_UTILS_H

#include "CubismFramework.hpp"
#include "ICubismAllocator.hpp"
#include <string>
#include <QElapsedTimer>
#include <cstdlib>

namespace IonaDesktop {
namespace CoreDisplay {
    class L2dAllocator : public Csm::ICubismAllocator
    {
        inline void* Allocate(const Csm::csmSizeType size)
        { return malloc(size); }
        inline void Deallocate(void* memory)
        { free(memory); }
        void* AllocateAligned(const Csm::csmSizeType size, const Csm::csmUint32 alignment);
        void DeallocateAligned(void* aligned_memory);
    };

    class L2dPal
    {
    public:
        static Csm::csmByte* LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize);
        static void ReleaseBytes(Csm::csmByte* byteData);

        static void PrintLog(const Csm::csmChar* format, ...);
        static void PrintMessage(const Csm::csmChar* message);
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_L2D_UTILS_H
