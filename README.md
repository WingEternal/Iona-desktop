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
* `L2dTouchManager`类实现与鼠标跟踪：继承`QApplication`做全局鼠标跟踪
* 模型检测更换为`Cubism Native SDK`中的`HitTest()`：换了但是完全没换
* 代码结构整理归类
* 计算环的泛光效果（bloom）
* 注意，当前分支尚未在Ubuntu上测试

