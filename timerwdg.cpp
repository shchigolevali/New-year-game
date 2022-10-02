#include "timerwdg.h"
#include "QPaintEvent"
#include "QPainter"
#include "QDebug"

TimerWdg::TimerWdg(double timerValue, int delta, QWidget *parent):
    timerValue(timerValue),
    delta(delta)
{
    setGeometry(0,0,100,100);
    setProperty("answer", -1);//*вызов неправильный ответ?
}

void TimerWdg::start()
{
    currentTime = timerValue;//*через каакое-то время посылает сигнал стоп
    connect(&timer,SIGNAL(timeout()),this, SLOT(updateTime()));
    timer.start(delta);
}

void TimerWdg::start(int timerValue)
{
    this->timerValue = timerValue;
    start();
}

void TimerWdg::stop()
{
    timer.stop();
}

void TimerWdg::paintEvent(QPaintEvent *evt)
{
    qDebug() << "paint";
    Q_UNUSED(evt);
    QPainter painter(this);
    QRectF rectangle(-50, -50, 100.0, 100.0);
    int startAngle = 0* 16;
    int spanAngle = currentTime/timerValue*360 * 16;

    painter.translate(50,50);
    painter.rotate(-90);
    QPen pen(Qt::red);
    pen.setWidth(100);
    QBrush br(Qt::green);
    pen.setBrush(br);
    painter.setPen(pen);
    painter.setPen(QPen(Qt::red));
    painter.drawArc(rectangle,startAngle,spanAngle);
    painter.translate(-50,0);
    painter.rotate(90);
    painter.setPen(QPen(Qt::black));
    if(currentTime <=0)
    {
        return;
    }
    if(currentTime > 10000)
        painter.drawText(QPoint(-15,-45),QString::number(currentTime/1000));
    else
    {
        painter.drawText(QPoint(-8,-45),QString::number(currentTime/1000));
    }
}

void TimerWdg::updateTime()
{
    currentTime -= delta;//уменьшение текущего времени
    qDebug() << currentTime;
    if(currentTime <= 0)
    {
        timer.stop();
        emit timeout();
    }
    update();
}
