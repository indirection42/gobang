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

## 各文件详细介绍
源代码:
`gobangclient/marcos.h`
`gobangclient/mainwindow.h`
`gobangclient/mainwindow.cpp`
`gobangclient/mainwindow.ui`
`gobangclient/boardui.h`
`gobangclient/boardui.cpp`
`gobangclient/client.h`
`gobangclient/client.cpp`
`gobangclient/gobangai.h`
`gobangclient/gobangai.cpp`
`gobangclient/gobangboard.h`
`gobangclient/gobangboard.cpp`
`gobangserver/dialog.h`
`gobangserver/dialog.cpp`
`gobangserver/dialog.ui`
`gobangserver/main.cpp`

