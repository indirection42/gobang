#include "boardui.h"
#include "marcros.h"
BoardUi::BoardUi(QWidget *parent) : QWidget(parent)
{
     stateCopy = IDLE;
     playerCopy=BLACK;
     moveSignX=0;
     moveSignY=0;
     addNumber=0;
     memset(boardCopy,0,sizeof(boardCopy));
     recordCopy={};
     this->setMouseTracking(true);
}

BoardUi::~BoardUi(){}

void BoardUi::newGame(int newGameMode){
    stateCopy = IDLE;
    playerCopy=BLACK;
    moveSignX=0;
    moveSignY=0;
    addNumber=0;
    memset(boardCopy,0,sizeof(boardCopy));
    recordCopy={};
    this->setMouseTracking(true);
    gameMode=newGameMode;
    if(gameMode==LOCALPVP){
        localPlayer=BOTH; //本机人人
        emit start();
    }
    else if(gameMode==LOCALPVC){
        localPlayer=BLACK;  //暂时默认人永远是黑，AI永远是白
        emit start();
    }
    else if(gameMode==ONLINEPVPBLACK){
        localPlayer=BLACK;  //本机执什么颜色根据某种规则指定
        emit start();
    }
    else if(gameMode==ONLINEPVPWHITE){
        localPlayer=WHITE;
        emit start();
    }
    update();
}


void BoardUi::save(void)
{
    emit requestSave(gameMode);
}

void BoardUi::updateInformation(int state,int player,int board[SIZE][SIZE], QVector<int> record){
    memcpy(boardCopy,board,sizeof(boardCopy));
    stateCopy=state;
    playerCopy=player;
    recordCopy=record;
    if((stateCopy == INGAME)|(stateCopy==IDLE)){
        addNumber = 0;
        update();
     }
    else if(stateCopy == OVER){
        addNumber = 1;
        gameOver(player);
    }
}

//when game over

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
        QLine verticalLine(widthSpace*i,0,widthSpace*i,heightSpace*(SIZE+1));
        QLine horizontalLine(0,heightSpace*i,widthSpace*(SIZE+1),heightSpace*i);
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
    if(stateCopy==IDLE)//游戏没开始则只画棋盘，不画移动标记和棋子
        return;
    if (playerCopy==WHITE){
       brush.setColor(Qt::white);
       painter.setBrush(brush);
    }
    if(stateCopy==INGAME){
        QPointF center(widthSpace*(moveSignX+1),heightSpace*(moveSignY+1));
        painter.drawEllipse(center,widthSpace/6,heightSpace/6);
    }
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
    if(addNumber)
    {
        for(int i = 0;i<recordCopy.size();i++)
        {
            if(i%2)
                painter.setPen(Qt::black);
            else
                painter.setPen(Qt::white);
            painter.drawText(QRect((recordCopy[i]/100 * widthSpace + widthSpace / 2),
                                   (recordCopy[i]%100 * heightSpace + heightSpace / 2),
                                   widthSpace,
                                   heightSpace),
                             Qt::AlignCenter,
                             QString::number(i + 1));
        }
    }
}

void BoardUi::mouseReleaseEvent(QMouseEvent *event)
{
    if(stateCopy!=INGAME){
        return;
    }
    if(event->button()==Qt::RightButton){ //右键可以请求悔棋
        emit requestRegret(localPlayer);
    }
    if(gameMode!=LOCALPVP){           //除去pvp模式,若当前玩家和本机玩家颜色不一致不允许下棋
        if(playerCopy!=localPlayer)
            return;
    }
    int currentWidth,currentHeight,widthSpace,heightSpace;
    currentWidth=this->width();
    currentHeight=this->height();
    widthSpace=currentWidth/(SIZE+1);
    heightSpace=currentHeight/(SIZE+1);
    QPoint pos=event->pos();
    if(event->button()==Qt::LeftButton){
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
    if(stateCopy!=INGAME){
        return;
    }
    if(gameMode!=LOCALPVP){            //除去pvp模式,若当前玩家和本机玩家颜色不一致就什么都不做
        if(playerCopy!=localPlayer)
            return;
    }
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

void BoardUi::gameOver(int winner)
{
    QString content;
    if(winner == BLACK)
        content = "The game is over and the winner is BLACK!\n Want to save the match?";
    else
        content = "The game is over and the winner is WHITE!\n Want to save the match?";
    if(QMessageBox::Yes == QMessageBox::information(NULL,
                             "Game Over",
                             content,
                             QMessageBox::Yes | QMessageBox::No,
                             QMessageBox::Yes))
    {
        //save the shot
        addNumber = 1;
        update();
        int currentWidth,currentHeight,widthSpace,heightSpace;
        currentWidth=this->width();
        currentHeight=this->height();
        widthSpace=currentWidth/(SIZE+1);
        heightSpace=currentHeight/(SIZE+1);

        QString path = QFileDialog::getSaveFileName(this,tr("Save Screen Shot"),QString(),tr("save files (*.png)"));
        if(path.length() == 0)
        {
            QMessageBox::information(NULL, tr("ERROR"), tr("Illegal file name."));
        } else
        {
            QPixmap p=this->grab(QRect(0,0,16 * widthSpace, 16 * heightSpace));
            p.save(path +".png","png");
        }
    }
    return;
}

void BoardUi::regretBinding()
{
    emit requestRegret(localPlayer);
}

void BoardUi::giveupBinding()
{
    emit requestGiveUp(localPlayer);
}
