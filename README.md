# Project Iona-desktop

* 基于Qt实现的跨平台方案，放一只伊欧娜在桌面上
* 无技术力警告，目前本质上是优先层最高的图片展示器x
* 素材来源：[萌娘百科-イオナ](https://zh.moegirl.org.cn/%E4%BC%8A%E6%AC%A7%E5%A8%9C)
## 可能咕咕的功能
* ~~监测系统资源占用~~（DONE）
* 搞一台服务器玩文件备份和同步，悬浮窗展示服务器连接状态
* 监控指定文件夹git状态
* 提醒喝水~~提肛~~小助手
* 一些动画效果，比如伊欧娜的计算环，401的外部灯光，可能试试Qt的2D/3D混合，~~也可能直接在别的软件里渲染完直接拖Qt里当图片播放器~~
* 看看Live2D或者MMD能不能整起来，~~L2D可以自己试试，但是建模实在做不到了~~
* 外接一个Stm32或者Arduino，DIY能在电脑上控制的台灯
* [Ubuntu] 查询roscore状态，快捷启动菜单
* ~~`ResourceMonitor`里的`HexagonWidget`估计之后很多插件会用得上（做悬浮窗状态），考虑把它整合到Core上，或打包成插件~~（DEPRECATED）
## 正在施工的项目(TODO)
* `QMenu`菜单美化
* `RecoureMonitor` Ticked的时候会导致窗口卡一下，按理说已经在新线程上跑了；
* 而且开久了会卡主线程（GUI），目前的测试结果是，每次`ResourceMonitor`更新都会调用重绘，久了之后`main_widget`的`paintEvent`跟不上了，从而主线程事件循环出现问题。
  * 临时解决方案：把所有图都降采样了一遍，而且载入过程改到`widget`构造函数里执行，`ResourceMonitor`刷新周期降低到1hz。~~改完之后字幕的动画效果也不卡了~~
  * 在test_branch上测试，与插件加载无关，是资源监视器内部导致的事件问题
  * 和CPU和MEM获取数据的实现无关，问题似乎出现在事件回调的过程上
  * 定位问题在`hexagon_progress_widget`的`paintEvent`上，把绘制六边形的过程全部注释掉，只保留`QLabel`就不卡了
  * ~~怀疑是`hexagon_progress_widget`的`slot`被异步调用，但是数据没写保护~~并不是
  * ~~改用`QProgressBar`算了~~RM直接编译为独立的应用程序，百分比占用的UI实现改为`QProgressBar`
* `iona_widget`继承于`QOpenGLWidget`实现
## 流水账

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

