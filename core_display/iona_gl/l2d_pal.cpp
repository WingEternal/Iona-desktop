#include <QtGlobal>
#include "iona_gl/l2d_utils.h"
#include <cstdio>
#include <stdarg.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <QIODevice>
#include <QFile>
#include <Model/CubismMoc.hpp>
#include "iona_gl/l2d_config.h"

using std::endl;
using namespace Csm;
using namespace std;
using namespace IonaDesktop::CoreDisplay;

double L2dPal::s_currentFrame = 0.0;
double L2dPal::s_lastFrame = 0.0;
double L2dPal::s_deltaTime = 0.0;

QElapsedTimer L2dPal::timer;

csmByte* L2dPal::LoadFileAsBytes(const string filePath, csmSizeInt* outSize)
{
//    //filePath;//
//    const char* path = filePath.c_str();

//    int size = 0;
//    struct stat statBuf;
//    if (stat(path, &statBuf) == 0)
//    {
//        size = statBuf.st_size;
//    }

//    std::fstream file;
//    char* buf = new char[size];

//    file.open(path, std::ios::in | std::ios::binary);
//    if (!file.is_open())
//    {
//        if (L2dConfig::DebugLogEnable)
//        {
//            PrintLog("file open error");
//        }
//        return NULL;
//    }
//    file.read(buf, size);
//    file.close();

//    *outSize = size;
//    return reinterpret_cast<csmByte*>(buf);

    QFile file(QString(filePath.c_str()));
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray byteArray = file.readAll();
        *outSize = byteArray.size();
        char* buffer = new char[*outSize];
        memcpy(buffer, byteArray.data(), *outSize);
        return reinterpret_cast<csmByte*>(buffer);
    }
    else
    {
        *outSize = 0;
        if (L2dConfig::DebugLogEnable)
            PrintLog("file open error");
        return nullptr;
    }
}

void L2dPal::ReleaseBytes(csmByte* byteData)
{ delete[] byteData; }

csmFloat32  L2dPal::GetDeltaTime()
{  return static_cast<csmFloat32>(s_deltaTime); }

void L2dPal::UpdateTime()
{
    static bool flag_init = false;
    if(!flag_init) {
        timer.start();
        flag_init = true;
    }
    s_currentFrame = timer.elapsed();
    s_deltaTime = (s_currentFrame - s_lastFrame) / 1000;
    s_lastFrame = s_currentFrame;
}

void L2dPal::PrintLog(const csmChar* format, ...)
{
    va_list args;
    csmChar buf[256];
    va_start(args, format);
#ifdef Q_OS_WIN
    vsnprintf_s(buf, sizeof(buf), format, args); // 標準出力でレンダリング
#endif
#ifdef Q_OS_LINUX
    vsnprintf(buf, sizeof(buf), format, args);
#endif
#ifdef CSM_DEBUG_MEMORY_LEAKING
// メモリリークチェック時は大量の標準出力がはしり重いのでprintfを利用する
    std::printf(buf);
#else
    std::cerr << buf << std::endl;
#endif
    va_end(args);
}

void L2dPal::PrintMessage(const csmChar* message)
{
    PrintLog("%s", message);
}
