#ifndef IONA_DESKTOP_CORE_EX1_L2D_PAL_H
#define IONA_DESKTOP_CORE_EX1_L2D_PAL_H

#include <CubismFramework.hpp>
#include <string>
#include <QElapsedTimer>

namespace IonaDesktop {
namespace CoreEx1 {
    class L2dPal
    {
    public:
        static Csm::csmByte* LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize);
        static void ReleaseBytes(Csm::csmByte* byteData);

        static void PrintLog(const Csm::csmChar* format, ...);
        static void PrintMessage(const Csm::csmChar* message);

        static Csm::csmFloat32 GetDeltaTime();
        static void UpdateTime();
        static double s_currentFrame;
        static double s_lastFrame;
        static double s_deltaTime;
    private:
        static QElapsedTimer timer;
    };
}
}

#endif // IONA_DESKTOP_CORE_EX1_L2D_PAL_H
