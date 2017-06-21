//
//  GobangAI.hpp
//  Gobang
//
//  Created by Vain on 2017/6/3.
//  Copyright © 2017年 Vain. All rights reserved.
//

#ifndef GobangAI_hpp
#define GobangAI_hpp
#include "marcros.h"
#include <QVector>
#include <QObject>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;
#define INFINITY 1000000

class PosWithScore
{
public:
    int x;
    int y;
    int score;
    PosWithScore(int x,int y,int score):x(x),y(y),score(score){};
    bool operator>(const PosWithScore& p) const
    {
        return score>p.score;
    }
    bool operator<(const PosWithScore& p) const
    {
        return score<p.score;
    }
};


/* A chess piece */
struct GBPoint
{
public:
    int x;
    int y;
    GBPoint(int gx,int gy):x(gx),y(gy){};
};


class GobangAI:public QObject
{
    Q_OBJECT
public:
signals:
    void aiRequestPlay(int x,int y);
public slots:
    void makeDecision(int state,int player,int pBoard[SIZE][SIZE],QVector<int> record); //对当前盘面做出决策
public:
    GobangAI(int c,int l);
    GobangAI();
    GobangAI(int AIcolor);
    void setLevel(int l);

    /* Make Decision*/
private:
    int getPattern(int teamColor,int x,int y,int dx,int dy);
    int alphaBeta(int beta,int alpha,int teamColor,int depth,vector<GBPoint>& bestPos);
    int oneStep(GBPoint p,int teamColor,bool& isattack);
    int sortPos(bool candidatePos[SIZE][SIZE],list<PosWithScore>& sortedCandi,int teamColor);
    void getCandidatePos(bool candi[SIZE][SIZE]);     //获得候选落子点
    int evaluate(int teamColor);                      //评估函数，对盘面评分
    int isWin(int teamColor,GBPoint p);
    void updateBoard(int pBoard[SIZE][SIZE]);
private:
    int boardCopy[SIZE][SIZE];
    int level;
    //int steps;
    //int color;

    int team;
    int enemy;
    static int PatternScore[13];
};


/*

static int PatternScore[13] = 
    {
        100,    //L5        0
        90,     //L4        1
        90,     //D4 * 2    2
        90,     //D4 L3     3
 
        80,     //L3 * 2    4
        70,     //D3 L3     5
        60,     //D4        6
        50,     //L3        7
        40,     //L2 * 2    8
        30,     //D3        9
        20,     //L2        10
        10,     //D2        11  
        0,      //Single piece      12
    };
*/
#endif /* GobangAI_hpp */
