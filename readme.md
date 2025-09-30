## 前言
这本来是一个200行代码就能完成的作业，我却冒着被老师认为是AI写的的风险去尝试学习**优雅的工程实践**。
既然是个优雅的工程实践，那就必须要有文档（对吧？）。

这是一个贪吃蛇游戏（显而易见），游戏机制和UI风格方面尝试**复刻**我小时候玩的贪吃蛇版本，游戏素材来自别人的逆向工程。（[素材来源](https://store.bananahackers.net/#snek)）
<img width="1202" height="803" alt="IMG-20250930110845946" src="https://github.com/user-attachments/assets/39218d0f-ae3f-48d4-b36e-9a5bb794dc59" />

哦对了，去tmd EGE(Easy Graphics Engine)。一点都不easy，配环境就花了我不止6h。谁用谁倒霉（我是我们老师强制使用的）。
## 游戏机制
机制基本是摁着我回忆中的原版本写的。

同：
- 吃5/10个苹果之后出现一个定时消失的红圈，吃得越快得分越多。
- 有经典地图和轨道地图

不同：
- 新增了左右手互搏版本（双蛇游戏）
- 取消了加速机制（我不太喜欢原版一开始的龟速）
- 没做排行榜
- 开始游戏和结束游戏的UI设计没有原版做得好（这主要怪EGE）

## 工程设计
主要思想：
- 头文件和实现文件分开，需要修改项目可以直接到头文件中看架构。
- 遵守类和对象的封装，在合理的情况下尽量代码复用。
- 对于游戏的各个阶段，使用`GameState`一个基类和多个`...State`子类，通过改变`changeState`来将各个状态分开。
- EGE前端的东西全部交由Render处理，前端参数对后端不可见。
- 使用枚举类`enum class`消除Magic Number，使用自定义类型`struct`和重载运算符获得更高的可读性。（比如`head.x`而不是`head.first`(如果使用`<pair>`))
- 主要游戏逻辑放在`Game`类中，`Reward`类和`Snake`类仅完成它们应该做的事。

## 改进空间
- 做一个页面更大的版本
- 将第二条蛇的颜色换一下

## 实现效果
<img width="476" height="638" alt="IMG-20250930113116885" src="https://github.com/user-attachments/assets/8a1238a6-13da-4dec-9aba-e7cd4d33d303" />
<img width="469" height="636" alt="IMG-20250930113140362" src="https://github.com/user-attachments/assets/4c515200-0567-4bc1-bb99-c38f33cce0ec" />

