#include "boardui.h"
#include "marcros.h"
BoardUi::BoardUi(QWidget *parent) : QWidget(parent)
{
     stateCopy = INGAME;
     playerCopy = BLACK;
     moveSignX=0;
     moveSignY=0;
     memset(boardCopy,0,sizeof(boardCopy));
     this->setMouseTracking(true);
}

BoardUi::~BoardUi(){}

void BoardUi::updateInformation(int state,int player,int board[SIZE][SIZE]){
    memcpy(boardCopy,board,sizeof(boardCopy));
    stateCopy=state;
    playerCopy=player;
    update();
}

void BoardUi::paintEvent(QPaintEvent *event)
{
    int currentWidth,currentHeight,widthSpace,heightSpace;
    currentWidth=this->width();
    currentHeight=this->height();
    widthSpace=currentWidth/(SIZE+1);
    heightSpace=currentHeight/(SIZE+1);
    QVector<QLine> verticalLines;
    QVector<QLine> horizontalLines;
    for(int i=0;i<=SIZE+1;i++){
        QLine verticalLine(widthSpace*i,0,widthSpace*i,currentHeight);
        QLine horizontalLine(0,heightSpace*i,currentWidth,heightSpace*i);
        verticalLines.append(verticalLine);
        horizontalLines.append(horizontalLine);
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBackground(Qt::yellow);
    QBrush brush(Qt::black);
    painter.setBrush(brush);
    painter.drawLines(verticalLines);
    painter.drawLines(horizontalLines);
    if (playerCopy==WHITE){
       brush.setColor(Qt::white);
       painter.setBrush(brush);
    }
    QPointF center(widthSpace*(moveSignX+1),heightSpace*(moveSignY+1));
    painter.drawEllipse(center,widthSpace/6,heightSpace/6);
    for(int i=0;i<SIZE;i++)
        for(int j=0;j<SIZE;j++){
            QPointF center(widthSpace*(i+1),heightSpace*(j+1));
            if(boardCopy[i][j]==BLACK){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(center,widthSpace/2,heightSpace/2);
            }
            else if(boardCopy[i][j]==WHITE){
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(center,widthSpace/2,heightSpace/2);
            }
            else
                continue;
        }
}

void BoardUi::mouseReleaseEvent(QMouseEvent *event)
{
    int currentWidth,currentHeight,widthSpace,heightSpace;
    currentWidth=this->width();
    currentHeight=this->height();
    widthSpace=currentWidth/(SIZE+1);
    heightSpace=currentHeight/(SIZE+1);
    QPoint pos=event->pos();
    if(event->button()==Qt::RightButton){
        emit requestRegret();
    }
    else{
        for(int i=0;i<SIZE;i++)
            for(int j=0;j<SIZE;j++){
                QRect rect(widthSpace*i+widthSpace/2,heightSpace*j+heightSpace/2,widthSpace,heightSpace);
                if(rect.contains(pos)){
                    emit requestPlay(i,j);
                    break;
                }
            }
    }
}

void BoardUi::mouseMoveEvent(QMouseEvent *event)
{
    int currentWidth,currentHeight,widthSpace,heightSpace;
    currentWidth=this->width();
    currentHeight=this->height();
    widthSpace=currentWidth/(SIZE+1);
    heightSpace=currentHeight/(SIZE+1);
    QPoint pos=event->pos();
    for(int i=0;i<SIZE;i++)
        for(int j=0;j<SIZE;j++){
            QRect rect(widthSpace*i+widthSpace/2,heightSpace*j+heightSpace/2,widthSpace,heightSpace);
            if(rect.contains(pos)){
                moveSignX=i;
                moveSignY=j;
                update();
            }
        }
}
