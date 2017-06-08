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

void BoardUi::updateInformation(int state,int player,int board[SIZE][SIZE], QVector<int> r){
    memcpy(boardCopy,board,sizeof(boardCopy));
    stateCopy=state;
    playerCopy=player;
    record.push_back(r[r.size() - 1]);
    update();
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
    if(addNumber)
    {
        for(int i = 0;i<record.size();i++)
        {
            if(i%2)
                painter.setPen(Qt::black);
            else
                painter.setPen(Qt::white);
            painter.drawText(QRect((record[i]/100 * widthSpace + widthSpace / 2),
                                   (record[i]%100 * heightSpace + heightSpace / 2),
                                   widthSpace,
                                   heightSpace),
                             Qt::AlignCenter,
                             QString::number(i + 1));
        }
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

void BoardUi::gameover(void)
{
    QString content;
    if(playerCopy == BLACK)
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
        bool ok;
        QString path = QInputDialog::getText(this,
                            tr( "Save Name" ),
                            tr( "Please enter your save name" ),
                            QLineEdit::Normal, QString::null, &ok);
        if(ok == 0 || path.length() == 0)
        {
            QMessageBox::information(NULL, tr("ERROR"), tr("Illegal file name."));
        } else
        {
            QPixmap p=this->grab(QRect(0,0,16 * widthSpace, 16 * heightSpace));
            p.save(path +".png","png");
        }
    }
    else
    {
        //clean the board
        //restart
    }
}
