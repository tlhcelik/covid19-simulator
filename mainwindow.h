#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "virus.h"
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QFuture>
#include <QtConcurrent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addHuman(bool haveVirus, int name);

private slots:
    void virusTransferController();

    void ui_update();

    void showTime();

    void on_speed_valueChanged(int value);

    void on_btnAdd_clicked();

    void on_btn_add_virus_clicked();

    void on_btnMakeInfect_clicked();

    void on_spinBoxSocialDistance_valueChanged(int arg1);

    void on_chckShowSocialDistance_stateChanged(int arg1);

    void on_btnReset_clicked();

private:
    Ui::MainWindow *ui;

    QPixmap myPix;

    QTime time;

    QList<Virus*> vlist;
    QList<Virus*> infectedList;
    QList<Virus*> clearList;

    QSize humanSize;

    int infectRadius =10; // px
    int showSocialDistance =1;

    QTimer *timer;
    QTimer *timerController;
    QTimer *timerClock;

    int imgWidth =100;//590;
    int imgHeight =100;//332;

    bool is_run =true;
    QFuture<void> future;
    QFuture<void> futureMakeInfect;

    int totalInfected = 0;
    int totalClearHuman = 0;
    int totalHuman = 0;
};

#endif // MAINWINDOW_H
