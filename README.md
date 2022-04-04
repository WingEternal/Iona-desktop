# Project Iona-desktop

* 基于Qt实现的跨平台方案，放一只伊欧娜在桌面上
* 无技术力警告，目前本质上是优先层最高的~~图片展示器~~现在是OpenGL展示器了
* 素材来源：[萌娘百科-イオナ](https://zh.moegirl.org.cn/%E4%BC%8A%E6%AC%A7%E5%A8%9C)
* fragment_shader_mix2: formula from [fifbro](https://blog.csdn.net/qq_21950929/article/details/78989215)
## 可能咕咕的功能
* ~~监测系统资源占用~~（DONE）
* 搞一台服务器玩文件备份和同步，悬浮窗展示服务器连接状态，共享剪贴板
* 监控指定文件夹git状态
* 提醒喝水~~提肛~~小助手
* 一些动画效果，比如伊欧娜的数据环，401的外部灯光（DONE 1/2）
* 数据环的一些动效，比如鼠标悬浮时扩大并加快转速
* 外接一个Stm32或者Arduino，DIY能在电脑上控制的台灯
* [Ubuntu] 查询roscore状态，快捷启动菜单
## 正在施工的项目(TODO)
* `QMenu`菜单美化
* 伊欧娜的Live2D模型迭代
* `L2dTouchManager`类实现与鼠标跟踪
* 模型检测更换为`Cubism Native SDK`中的`HitTest()`
* 代码结构整理归类
* FBX模型加载器，把I-401重新整回来做背景
* 计算环的泛光效果（bloom）
## 流水账

* 20220405
  * 让伊欧娜动了起来，不再是静态贴图了
* 20220301
  * Windows部分的Cubism依赖库为32位(141-MSVC2017)，编译器版本15.9.28307.1800(x86)，QtCreator4.9.1，基于Qt5.12.3
  * Ubuntu部分的Cubism依赖库为64位，直接连接到libLive2DCubismCore.so，基于Qt5.12.3
* 20220127
  * 为了实现这三个计算环，代码基本重构了一遍。目前主要的绘制集中在`QOpenGLWidget`上用`OpenGL`完成
  * 异形窗口在`BaseWidget`上用了一个很粗糙的mask实现，反正每次改布局都得重新画一个，凑合先用
  * 把`ResourceMonitor`里的`HexagonWidget`整合到Core上，现为`HexagonBase`
* 20220118
  * ~~咕了是因为打别的游戏去了~~
  * 存档一下不卡的版本，去掉了六边形的百分比绘制
* 20220103    
  * `__int64`类型为VC++专用类型，改为`int32_t`以在Ubuntu上使用（GCC）
  * 图片素材降采样，加快重绘速度
  * 内存泄露原因还在找
* 20220102  
  * 去掉了所有widget的ui文件，本来就是纯代码实现的
  * 加了语音的字幕（`QLabel`实现），加了个淡入淡出效果，但是帧率很低
* 20211231
  * 加了读取当前CPU和内存使用率的小插件，目前在Win10上测试完成，能跑但是CPU利用率计算还有一些问题；Linux的代码也缝了一份，改天上Ubuntu测。还有三个悬浮窗位置空着，在思考要监控啥。
* 20211229
  * 整合了一下代码，把`plugin_manager`合进了`main_widget`，不然`plugin_manager`, `main_widget`, `json_config`三个玩意要互相访问数据会非常痛苦。
  * 写~~抄~~了个QJson读写函数，用于保存退出时各个窗体的状态，给系统托盘加了个重置位置和绘图大小的选项

  * 写~~抄~~了个`QJson`读写函数，用于保存退出时各个窗体的状态，给系统托盘加了个重置位置和绘图大小的选项

