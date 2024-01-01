#include <stdarg.h>
#include <sys/stat.h>

#include <Model/CubismMoc.hpp>
#include <QFile>
#include <QIODevice>
#include <QtGlobal>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "iona_gl/l2d_config.h"
#include "iona_gl/l2d_utils.h"

using std::endl;
using namespace Csm;
using namespace std;
using namespace IonaDesktop::CoreDisplay;

csmByte* L2dPal::LoadFileAsBytes(const string filePath, csmSizeInt* outSize) {
  QFile file(QString(filePath.c_str()));
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray byteArray = file.readAll();
    *outSize = byteArray.size();
    char* buffer = new char[*outSize];
    memcpy(buffer, byteArray.data(), *outSize);
    return reinterpret_cast<csmByte*>(buffer);
  } else {
    *outSize = 0;
    if (L2dConfig::DebugLogEnable) PrintLog("file open error");
    return nullptr;
  }
}

void L2dPal::ReleaseBytes(csmByte* byteData) { delete[] byteData; }

void L2dPal::PrintLog(const csmChar* format, ...) {
  va_list args;
  csmChar buf[256];
  va_start(args, format);
#ifdef Q_OS_WIN
  vsnprintf_s(buf, sizeof(buf), format, args);  // 標準出力でレンダリング
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

void L2dPal::PrintMessage(const csmChar* message) { PrintLog("%s", message); }
