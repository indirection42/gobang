//
//  GobangAI.cpp
//  Gobang
//
//  Created by Vain on 2017/6/3.
//  Copyright © 2017年 Vain. All rights reserved.
//
#include "gobangai.h"



int GobangAI::PatternScore[13] = 
    {
        
        560,    //L5        0
        280,     //L4        1
        280,     //D4 * 2    2
        280,     //D4 L3     3
        
        140,     //L3 * 2    4
        70,     //D3 L3     5
        60,     //D4        6
        50,     //L3        7
        40,     //L2 * 2    8
        30,     //D3        9
        20,     //L2        10
        10,     //D2        11
        0,      //Single piece      12
        
    };

void GobangAI::updateBoard(int pBoard[SIZE][SIZE])
{
    memcpy(boardCopy,pBoard,sizeof(boardCopy));
}


GobangAI::GobangAI(){
    memset(boardCopy,0,sizeof(boardCopy));
    level = 1;
    team = BLACK;
    //steps = 0;
    enemy = team==BLACK?WHITE:BLACK;
}

GobangAI::GobangAI(int t,int l)
{
    memset(boardCopy,0,sizeof(boardCopy));
    level = l;
    team = t;       //AI方
    //steps = 0;
    enemy = team==BLACK?WHITE:BLACK;
}

GobangAI::GobangAI(int AIcolor)
{
    memset(boardCopy,0,sizeof(boardCopy));
    level = 2;
    team = AIcolor;       //AI方
    //steps = 0;
    enemy = team==BLACK?WHITE:BLACK;
}

void GobangAI::setLevel(int l)
{
    if(l>0)
        level = l;
    else 
        level = 1;
}

int GobangAI::evaluate(int board[SIZE][SIZE],int teamColor)
{
    int dir[4][2] = {{0,1},{1,1},{1,0},{1,-1}};
    int patCount[13] = {0};
    for(int x = 0;x<SIZE;x++)
        for(int y = 0;y<SIZE;y++) 
        {
            if(board[x][y]==teamColor)
            {
                int pat = 100;
                for(int i =0;i<4;i++){
                    int getpat = getPattern(board,teamColor,x,y,dir[i][0],dir[i][1]);
                    if(getpat==-1) continue;
                    if(getpat<pat) pat = getpat;
                }//Win
                if(pat>=0)
                    patCount[pat]++;
            }
        }
        if(patCount[6]>=2)
        {
            patCount[2]++;
        }
        else if(patCount[6]>=1&&patCount[7]>=1)
        {
            patCount[3]++;
        }
        else if(patCount[7]>=2)
        {
            patCount[4]++;
        }
        else if(patCount[7]>=1&&patCount[9]>=1)
        {
            patCount[5]++;
        }
        else if(patCount[10]>=2)
        {
            patCount[8]++;
        }
    
        for(int i=0;i<13;i++)
            if(patCount[i]>0)
            {
                return PatternScore[i]; //Highest Score
            }
    return 0;
}


int GobangAI::getPattern(int board[SIZE][SIZE],int teamColor,int x,int y,int dx,int dy)
{
    bool live = true;
    int ENEMY = teamColor==BLACK?WHITE:BLACK;
    int tx = x - dx;
    int ty = y - dy;
    if(tx<0||ty<0||board[ty][tx]==ENEMY)
    {
        live = false;
    }
    if(board[tx][ty]==teamColor)
    {
        return -1; //Inside a pattern
    }
    // Head of the pattern
    int count=1;
    tx = x;
    ty = y;
    bool line = true;
    for(int i =1;i<5;i++)
    {
        tx += dx;
        ty += dy;
        if(tx>=SIZE||ty>=SIZE||(board[tx][ty]==ENEMY))
        {
            if(live==false) // two sides are both blocked
                return -1;
            if(board[tx-dx][ty-dy]==teamColor&&line==true)
                    live = false;

            // Reverse search
            tx = x;
            ty = y;
            for(int j = count;j<5;j++)
            {
                tx -= dx;
                ty -= dy;
                if(tx<0||ty<0||board[tx][ty]==ENEMY)
                    return -1;
            }

            break;
        }
        else if(board[tx][ty]==0)
            line = false;
        else if(board[tx][ty]==teamColor&&line==true)
        {
            count++;
        }
    }
    //Map to pattern
    if(live == false)
    {
        switch(count)
        {
            case 1: return 12;break;
            case 2: return 11;break;
            case 3: return 9;break;
            case 4: return 6;break;
            case 5: return 0;break;
        }
    }
    else 
    {
        switch(count)
        {
            case 1: return 12;break;
            case 2: return 10;break;
            case 3: return 7;break;
            case 4: return 1;break;
            case 5: return 0;break;
        }
    }
    
    return -1;
}




void GobangAI::getCandidatePos(int board[SIZE][SIZE],bool candi[SIZE][SIZE])
{
    //Scale posScale;
    const int range = 3;
    for(int i = 0;i<SIZE;i++)
        for(int j=0;j<SIZE;j++)
            candi[i][j] = false;

    for(int y =0;y<SIZE;y++)
        for(int x = 0;x<SIZE;x++)
        {
            if(board[x][y]!=0)
                for(int i = x-range;i<=x+range;i++)
                    for(int j = y-range;j<=y+range;j++)
                    {
                        /* Get the candidate position */
                        if(j>=0&&j<SIZE&&i>=0&&i<SIZE&&board[i][j]==0){
                            candi[i][j]=true;
                        }
                    }
        }
}


void GobangAI::makeDecision(int state,int player,int pBoard[SIZE][SIZE],QVector<int> record)
{
    updateBoard(pBoard);

    if(player!=team||state!=INGAME)
        return;

    if(record.size()==0&&team==BLACK)
    {
        emit aiRequestPlay(7,7);
        return;
    }

    vector<GBPoint> bestPos;

    alphaBeta(boardCopy,INFINITY,-INFINITY,team,level,bestPos);
    int randNumber = rand()%bestPos.size();
    int x = bestPos[randNumber].x;
    int y = bestPos[randNumber].y;
    emit aiRequestPlay(x,y);
}

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

int GobangAI::sortPos(int board[SIZE][SIZE],bool candidatePos[SIZE][SIZE],list<PosWithScore>& sortedCandi,int teamColor)
{
    int opponent = teamColor==BLACK?WHITE:BLACK;
    for(int x=0;x<SIZE;x++)
        for(int y=0;y<SIZE;y++)
        {
            if(candidatePos[x][y]==true)
            {
                board[x][y] = teamColor;
                int AttackScore = evaluate(board,teamColor);
                board[x][y] = opponent;
                int OpponentScore = evaluate(board,opponent);
                board[x][y] = 0;
                PosWithScore p(x,y,AttackScore+OpponentScore);
                list<PosWithScore>::iterator it = sortedCandi.begin();
                while(it!=sortedCandi.end())
                {
                    //cout<<it->score<<"  "<<p.score<<endl;
                    if(it->score <= p.score)
                        break;
                    it++;
                }
                sortedCandi.insert(it,PosWithScore(x,y,AttackScore+OpponentScore));
            }
        }
    return 0;
}


#define WIN 140
#define LOSE -7777777
int GobangAI::oneStep(int board[SIZE][SIZE],GBPoint p,int teamColor,bool& isattack)
{
    int opponent = teamColor==BLACK?WHITE:BLACK;
    board[p.x][p.y] = teamColor;
    int AttackScore = evaluate(board,teamColor);
    board[p.x][p.y] = opponent;
    int OpponentScore = evaluate(board,opponent);  
    board[p.x][p.y] = 0;
    if(AttackScore>=WIN&&OpponentScore<WIN)
    {
        isattack=true;
        return AttackScore;
    }
    else if(AttackScore<WIN&&OpponentScore>=WIN)
    {
        return OpponentScore;
    }
    else if(AttackScore>=WIN&&OpponentScore>=WIN)
    {
        if(AttackScore>=OpponentScore){
            isattack = true;
            return AttackScore;
        }
        else
        {
            return OpponentScore;
        }
    }
    return -INFINITY;

    
}
int GobangAI::isWin(int board[SIZE][SIZE],int teamColor,GBPoint p){
    int dir[4][2] = {{0,1},{1,1},{1,0},{1,-1}};
    int opponent = teamColor==BLACK?WHITE:BLACK;
    for(int i =0;i<4;i++){
        int getpat = getPattern(board,teamColor,p.x,p.y,dir[i][0],dir[i][1]);
        if(getpat==0) return PatternScore[0]-evaluate(board,opponent);
    }//Win
    return -1;
}


#define CUT -9999999

int GobangAI::alphaBeta(int board[SIZE][SIZE],int beta,int alpha,int teamColor,int depth,vector<GBPoint>& bestPos)
{
    //non-leaf node
    int opponent = teamColor==BLACK?WHITE:BLACK;
    if(depth==0)//Leaf or finished
    {   
        int val1 = evaluate(board,teamColor);
        int val2 = evaluate(board,opponent);
        return (val1-val2);
    }

    bool candidatePos[SIZE][SIZE];
    getCandidatePos(board,candidatePos);

    if(depth==level)
    {
        for(int i=0;i<SIZE;i++)
            for(int j = 0;j<SIZE;j++)   
            {
                if(candidatePos[i][j]==true)
                {
                    bool isattack = false;
                    GBPoint p(i,j);
                    int retval = oneStep(board,p,teamColor,isattack);
                    if(retval > alpha)
                    { 
                        alpha = retval;
                        bestPos.clear();
                        bestPos.push_back(p);
                    }
                    else if(isattack==true&&retval==alpha)
                    {
                        alpha = retval;
                        bestPos.clear();
                        bestPos.push_back(p);
                    }

                }
        }
    }
    
    list<PosWithScore> sortedPosSet;
    sortPos(board,candidatePos,sortedPosSet,teamColor);

    for(list<PosWithScore>::iterator sit = sortedPosSet.begin();sit!=sortedPosSet.end();sit++)
    {
        GBPoint p(sit->x,sit->y);
        board[p.x][p.y] = teamColor;
        int v;
        //if((v=isWin(board,teamColor,p))==-1)
            v = -alphaBeta(board,-alpha,-beta,opponent,depth-1,bestPos);
        board[p.x][p.y] = 0;
        if(v>=beta)
        {
            return beta;
        }
        if(v>alpha)
        {
            alpha=v;
            if(depth==level){
                bestPos.clear();
                bestPos.push_back(p);
            }
        }
    }
    return alpha;   
}
