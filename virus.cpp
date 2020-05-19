#include "virus.h"
#include <QDebug>
#include <QDateTime>
Virus::Virus(QObject *parent, int virusName, bool haveVirus) : QObject(parent)
{
    this->virusName = virusName;
    this->haveVirus =haveVirus;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePos()));
    timer->start(100);

    QString status =haveVirus ? "INFECTED" : "CLEAR";
    qDebug()<<"My name is :" << virusName << " I am " + status;

    if((QDateTime::currentMSecsSinceEpoch() - QDateTime(QDate::currentDate()).toMSecsSinceEpoch()) % 2 ==0)
    {
        xdirection *= -1;
    }


}

void Virus::updatePos()
{
    if(this->x +this->width > xBorder)
        xdirection *= -1;
    else if (this->x < 0)
        xdirection *= -1;

    if(this->y + this->height > yBorder)
        ydirection *= -1;
    else if (this->y < 0)
        ydirection *= -1;

    this->x += movePxRange * xdirection;
    this->y += movePxRange * ydirection;

//    qDebug()<<"My name is :" << this->virusName << " X: "<<this->x << " Y: "<<this->y;

}

void Virus::setBorders(int xB, int yB)
{
    this->xBorder = xB;
    this->yBorder =yB;
}

void Virus::setCoord(int x, int y, int w, int h)
{
    this->x =x;
    this->y =y;
    this->width = w;
    this->height =h;
}

void Virus::setTimerInterval(int ms)
{
    this->timer->setInterval(ms);
}

void Virus::start(int ms)
{
    setTimerInterval(ms);
    timer->start();
}

void Virus::makeInfected()
{
    this->haveVirus =true;
    QString status =haveVirus ? "INFECTED" : "CLEAR";
    qDebug()<<"My name is :" << virusName << "I am " + status;
}

