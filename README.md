# 面向对象程序设计大程序文档
成员：
3150 范宏昌 
3150 方共凡
3150 杜众舒 
3140103103 郑济元 

## 总体概述及功能介绍
本五子棋游戏采用c++作为开发语言,使用了QT跨平台库，具有以下功能：
- 本机人人对战模式
	1. 该模式下黑白两方玩家轮流在一个棋盘上下棋
	2. 上一次落子的一方玩家可以进行悔棋
	3. 轮到自己下棋的一方玩家可以进行认输
	4. 游戏中途可以保存对局,可在以后选择加载对局
	5. 游戏结束可以保存游戏棋谱(带落子顺序标记)
- 本机人机对战模式
	1. 该模式下默认玩家执黑,电脑AI执白,轮流在一个棋盘上下棋
	2. 玩家可以进行悔棋
	3. 玩家可以进行认输
	4. 游戏中途可以保存对局，可在以后选择加载对局
	5. 游戏结束可以保存游戏棋谱(带落子顺序标记)
- $网络人人对战模式^*$
	1. 该模式下黑白两方玩家各自开一个客户端,连接上服务器后，分别在自己的棋盘上下棋
	2. 任一方玩家可以在轮到自己下和轮到对方下时申请进行悔棋，待对方同意后即可悔棋
	3. 任一方玩家可以在轮到自己下和轮到对方下时进行认输
	4. 游戏结束可以保存游戏棋谱(带落子顺序标记)

## 使用方法
- 开始游戏
	1. 启动Client客户端，打开菜单Game - New Game, 选择Local PvP或Local PvE即可开始一局单机的人人对战或人机对战游戏。
	2. 启动Server服务端，再打开两个Client客户端，分别选择Online PvP模式，即可开始一局联机的人人对战游戏。
- 保存/读取进度
	1. 在Client客户端中单机游戏开始后，打开菜单Game - Save Game，保存当前对弈的存档（任意设置存储位置与文件名）。
	2. 在Client客户端打开后，打开菜单Game - Load Game，即可载入之前保存的对弈，游戏模式以保存时的游戏模式为准（任意选择存档）。
- 保存棋谱
	1. 在任意一局比赛完成后，都可以选择保存当前比赛的棋谱————记录整局对弈过程的图片文件（任意设置存储位置与文件名）。
- 关闭游戏
	1. 点击窗口右上角红叉。
	2. 在Client客户端中可以打开菜单Game - Quit Application，退出应用。
	
## 组件（各类）间工作原理
- GobangBoard
	1. 最底层的棋盘类GobangBoard，储存有棋盘board、历史记录record、当前游戏状态state、当前执子人player、剩余时间blackTimer,whiteTimer等基础	的信息。
	2. 提供的接口能够访问到所有的成员变量，但是只有剩余时间与游戏状态能够被修改。
	3. 除了基础的棋盘状态判断功能check()、落子功能play()与放弃游戏等棋盘状态转换函数giveup()外，还提供了存档函数save()，能够将当前棋盘及其历史	    记录保存到文本文件当中。
	### 棋盘
		int board[SIZE][SIZE];
	1. 由二维数组保存
	### 落子功能 play()
		int play(int x, int y);
	
## 各类详细uml图
- GobangBoard(棋盘底层存储)
![gobangboard.png](./uml_png/gobangboard.png)
- GobangAI(电脑AI)
![gobangai.png](./uml_png/gobangai.png)
- BoardUi(棋盘界面)
![boardui.png](./uml_png/boardui.png)
- Client(客户端类)
![client.png](./uml_png/client.png)
- Dialog（带界面服务器类）
![server.png](./uml_png/server.png)
## 各源代码文件介绍
- 客户端部分主程序
`gobangclient/main.cpp`
- 定义了整个程序中用到的宏名
`gobangclient/marcos.h`
- 主窗口的头文件、cpp文件和ui设计文件
`gobangclient/mainwindow.h`，`gobangclient/mainwindow.cpp`，`gobangclient/mainwindow.ui`
- 棋盘界面类的头文件、cpp文件
`gobangclient/boardui.h`，`gobangclient/boardui.cpp`
- 客户端类的头文件、cpp文件
`gobangclient/client.h`，`gobangclient/client.cpp`
- 电脑AI类的头文件、cpp文件
`gobangclient/gobangai.h`，`gobangclient/gobangai.cpp`
- 棋盘底层存储类的头文件、cpp文件
`gobangclient/gobangboard.h`，`gobangclient/gobangboard.cpp`
- 服务器类头文件、cpp文件和ui设计文件
`gobangserver/dialog.h`，`gobangserver/dialog.cpp`，`gobangserver/dialog.ui`，
- 服务器端主程序
`gobangserver/main.cpp`

