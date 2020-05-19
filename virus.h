#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QTimer>
#include <QMouseEvent>
#include <QWidget>

class Virus: public QObject
{
    Q_OBJECT
public:
    explicit Virus(QObject *parent = nullptr, int virusName =-1, bool haveVirus =false);

    int x =0;
    int y =0;

    bool haveVirus = false;
    bool oldVirusStatus =false;

    QTimer *timer;
    int virusName;

    int xBorder = 100;
    int yBorder = 100;

    int xdirection =1;
    int ydirection =1;

    int width , height =10;

    int movePxRange =10;

    void setBorders(int xB, int yB);
    void setCoord(int x, int y , int width, int height);
    void setTimerInterval(int ms);
    void start(int ms);
    void makeInfected();

public slots:
    void updatePos();
};

#endif // VIRUS_H
