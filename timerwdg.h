#ifndef TIMERWDG_H
#define TIMERWDG_H

#include <QWidget>
#include "QTimer"
class TimerWdg : public QWidget
{
    Q_OBJECT
    double timerValue;
    double currentTime;
    int delta;
    QTimer timer;
public:
   explicit TimerWdg(double timerValue = 200000, int delta = 10, QWidget *parent = nullptr);
    void start();
    void start(int timerValue);
    void stop();
signals:
    void timeout();
protected:
    void paintEvent(QPaintEvent* evt);
public slots:
    void updateTime();
};

#endif // TIMERWDG_H
