# Changelog
## 20220508
  * `L2dTouchManager`类实现与鼠标跟踪：在Win10平台上使用`WH_MOUSE_LL`(hook)做全局鼠标跟踪
## 20220419
  * 添加拖动反馈，点击的动作需要K~~（就是还没做的意思）~~
  * 初始鼠标焦点需要在程序窗体内才能实现拖曳跟踪
## 20220405
  * 让伊欧娜动了起来，不再是静态贴图了
## 20220301
  * Windows部分的Cubism依赖库为32位(141-MSVC2017)，编译器版本15.9.28307.1800(x86)，QtCreator4.9.1，基于Qt5.12.3
  * Ubuntu部分的Cubism依赖库为64位，直接连接到libLive2DCubismCore.so，基于Qt5.12.3
## 20220127
  * 为了实现这三个计算环，代码基本重构了一遍。目前主要的绘制集中在`QOpenGLWidget`上用`OpenGL`完成
  * 异形窗口在`BaseWidget`上用了一个很粗糙的mask实现，反正每次改布局都得重新画一个，凑合先用
  * 把`ResourceMonitor`里的`HexagonWidget`整合到Core上，现为`HexagonBase`
## 20220118
  * ~~咕了是因为打别的游戏去了~~
  * 存档一下不卡的版本，去掉了六边形的百分比绘制
## 20220103    
  * `__int64`类型为VC++专用类型，改为`int32_t`以在Ubuntu上使用（GCC）
  * 图片素材降采样，加快重绘速度
  * 内存泄露原因还在找
## 20220102  
  * 去掉了所有widget的ui文件，本来就是纯代码实现的
  * 加了语音的字幕（`QLabel`实现），加了个淡入淡出效果，但是帧率很低
## 20211231
  * 加了读取当前CPU和内存使用率的小插件，目前在Win10上测试完成，能跑但是CPU利用率计算还有一些问题；Linux的代码也缝了一份，改天上Ubuntu测。还有三个悬浮窗位置空着，在思考要监控啥。
## 20211229
  * 整合了一下代码，把`plugin_manager`合进了`main_widget`，不然`plugin_manager`, `main_widget`, `json_config`三个玩意要互相访问数据会非常痛苦。
  * 写~~抄~~了个`QJson`读写函数，用于保存退出时各个窗体的状态，给系统托盘加了个重置位置和绘图大小的选项