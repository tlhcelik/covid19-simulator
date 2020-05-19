#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPix.load(":/bg.jpg");
    imgWidth = myPix.width();
    imgHeight = myPix.height();

    ui->label->setPixmap(myPix);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ui_update()));
    timer->setInterval(24);
    timer->start();

    timerController = new QTimer(this);
    connect(timerController, SIGNAL(timeout()), this, SLOT(virusTransferController()));
    timerController->setInterval(1);
    timerController->start();

    ui->lcd_clear_human->display(totalClearHuman);
    ui->lcd_human->display(totalHuman);
    ui->lcd_infected->display(totalInfected);

    time = QTime::fromString("00:00:00");
    timerClock = new QTimer(this);
    connect(timerClock, &QTimer::timeout, this, &MainWindow::showTime);

    humanSize.setWidth(10);
    humanSize.setHeight(10);

    infectRadius =ui->spinBoxSocialDistance->value();

    ui->statusBar->setStyleSheet("color: green");
    ui->statusBar->showMessage("White : not infected, Red: diseased, Yellow : infected");

}
void MainWindow::showTime()
{
   time = time.addSecs(1);
    QString text = time.toString("hh:mm:ss");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcd_time->display(text);
}
MainWindow::~MainWindow()
{
    QString result;
    QTextStream(&result) << "Total Time " << time.toString("hh:mm:ss") <<endl;
    QTextStream(&result) << "Total Human : " << vlist.length()<<endl;
    QTextStream(&result) << "Total Infected : " << infectedList.length()<<endl;
    QTextStream(&result) << "Total Clear Human : " << clearList.length()<<endl;


    QMessageBox::information(this, "Results",result);
    is_run = false;
    future.cancel();
    futureMakeInfect.cancel();
    delete ui;
}

void MainWindow::addHuman(bool haveVirus, int name)
{
    if(!timerClock->isActive())
        timerClock->start(1000);

    int rndx =(QDateTime::currentMSecsSinceEpoch() - QDateTime(QDate::currentDate()).toMSecsSinceEpoch()) % imgWidth;
    rndx += (rndx+humanSize.width() >= imgWidth) ? -humanSize.width() : humanSize.width();
    Virus *v =new Virus(this,name, haveVirus);

    int rndy =(QDateTime::currentMSecsSinceEpoch() - QDateTime(QDate::currentDate()).toMSecsSinceEpoch()) % imgHeight - humanSize.height();
    rndy += (rndy+humanSize.height() >= imgHeight) ? -humanSize.height() : humanSize.height();

    v->setBorders(imgWidth, imgHeight);
    v->setCoord(rndx, rndy, humanSize.width(), humanSize.height());
    v->movePxRange = 1;

    v->setTimerInterval((rndx % 50)+10);
    vlist.append(v);

    if(haveVirus)
    {
        totalInfected += 1;
        v->oldVirusStatus = true;
        v->haveVirus =true;
        infectedList.append(v);
    }
    else
    {
        totalClearHuman +=1;
        clearList.append(v);
    }

    totalHuman +=1;


}

void MainWindow::ui_update()
{
    if(vlist.length() <=0 ) return;

    myPix.load(":/bg.jpg");
    ui->label->setPixmap(myPix);
    QPainter painter(&myPix);


    for (int i = 0; i < vlist.length(); ++i)
    {
        if(vlist[i]->haveVirus && vlist[i]->oldVirusStatus == true)
            painter.setBrush(Qt::red);
        if(vlist[i]->haveVirus && vlist[i]->oldVirusStatus == false )
            painter.setBrush(Qt::yellow);
        if(vlist[i]->haveVirus ==false )
            painter.setBrush(Qt::white);

//        QRect rect((vlist[i]->x - humanSize.width()/2)-infectRadius , (vlist[i]->y- humanSize.height()/2)-infectRadius,
//                   (humanSize.width() *2)+infectRadius , (humanSize.height()*2)+infectRadius);

        QRect rect(QPoint(vlist[i]->x - infectRadius, vlist[i]->y - infectRadius),
                   QPoint(vlist[i]->x + infectRadius, vlist[i]->y + infectRadius));

        painter.drawRect(vlist[i]->x, vlist[i]->y, humanSize.width(), humanSize.height());
        if(showSocialDistance)
            painter.fillRect(rect, QBrush(QColor(0, 255, 0, 20)));
    }

    ui->label->setPixmap(myPix);
}

void MainWindow::virusTransferController()
{
    //search algorithm
    this->setWindowTitle(QString("Infected list len :%1, Clear list len :%2").arg(infectedList.length()).arg(clearList.length()));

    if(infectedList.length() > clearList.length())
        for (int i = 0; i < infectedList.length(); ++i)
        {
            for (int j = 0; j < clearList.length(); ++j)
            {
                if(((infectedList.at(i)->x-infectRadius == clearList.at(j)->x) || (infectedList.at(i)->x+infectRadius == clearList.at(j)->x)) &&
                        ((infectedList.at(i)->y-infectRadius == clearList.at(j)->y) || (infectedList.at(i)->y+infectRadius == clearList.at(j)->y)))
                    //            if(((vlist[i]->x - infectRadius == clearList.at(j)->x-infectRadius) || (vlist[i]->x + infectRadius == clearList.at(j)->x+infectRadius)) &&
                    //                    ((vlist[i]->y - infectRadius == clearList.at(j)->y - infectRadius) || (vlist[i]->y + infectRadius == clearList.at(j)->y + infectRadius)))
                {
                    clearList[j]->haveVirus =true;
                    totalInfected +=1;
                    totalClearHuman -=1;
                    infectedList.append(clearList.at(j));
                    clearList.removeAt(j);
                }
            }
        }
    else
        for (int i = 0; i < infectedList.length(); ++i)
        {
            for (int j = 0; j < clearList.length(); ++j)
            {
                if(((infectedList.at(i)->x-infectRadius == clearList.at(j)->x) || (infectedList.at(i)->x+infectRadius == clearList.at(j)->x)) &&
                        ((infectedList.at(i)->y-infectRadius == clearList.at(j)->y) || (infectedList.at(i)->y+infectRadius == clearList.at(j)->y)))
                {
                    clearList[j]->haveVirus =true;
                    totalInfected +=1;
                    totalClearHuman -=1;
                    infectedList.append(clearList.at(j));
                    clearList.removeAt(j);
                }
            }
        }

    ui->lcd_clear_human->display(totalClearHuman);
    ui->lcd_human->display(totalHuman);
    ui->lcd_infected->display(totalInfected);

}

void MainWindow::on_speed_valueChanged(int value)
{
    for (int i = 0; i < vlist.length(); ++i)
    {
        //vlist[i]->setTimerInterval(value * 10);
    }
    this->timer->setInterval(value * 10);
}

void MainWindow::on_btnAdd_clicked()
{
    addHuman(false, vlist.length() +1);
}

void MainWindow::on_btn_add_virus_clicked()
{
    addHuman(true, vlist.length() +1);
}

void MainWindow::on_btnMakeInfect_clicked()
{
    if(vlist.length() > 0)
    {
        futureMakeInfect = QtConcurrent::run([=]() {
            for (int i = 0; i < vlist.length(); ++i)
            {
                if(!vlist[i]->haveVirus)
                {
                    vlist[i]->makeInfected();
                    infectedList.append(vlist[i]);
                    clearList.removeLast();
                    totalInfected +=1;
                    totalClearHuman -=1;
                    break;
                }
            }
        });
    }
}

void MainWindow::on_spinBoxSocialDistance_valueChanged(int arg1)
{
    infectRadius = arg1;
}

void MainWindow::on_chckShowSocialDistance_stateChanged(int arg1)
{
    showSocialDistance = arg1;

}

void MainWindow::on_btnReset_clicked()
{
    vlist.clear();
    infectedList.clear();
    clearList.clear();
    totalClearHuman = totalHuman = totalInfected = 0;
}
