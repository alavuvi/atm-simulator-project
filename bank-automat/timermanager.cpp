#include "timermanager.h"
#include <QDebug>
#include <QMessageBox>

TimerManager::TimerManager(QObject* parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerManager::handleTimeout);
    currentWindow = nullptr;
}

TimerManager::~TimerManager()
{
    delete timer;
}

TimerManager& TimerManager:: getInstance()
{
    static TimerManager instance;
    return instance;
}

void TimerManager::startTimer(QWidget* window, int timeout)
{
    currentWindow = window;
    timer->start(timeout);
    qDebug() << "Aloitettu" << timeout << "ms ajastin";
}

void TimerManager::stopTimer()
{
    timer->stop();
    qDebug() << "Ajastin pysäytetty";
}

void TimerManager::resetTimer()
{
    if(timer->isActive()) {
        timer->start();
        qDebug() << "Ajastin käynnistetty uudelleen";
    }
}

void TimerManager::handleTimeout()
{
    stopTimer();
    QMessageBox::warning(currentWindow, "Timeout",
                         "Your session has timed out due to inactivity.");
    emit timerExpired();

    if (currentWindow) {
        currentWindow->close();
    }
}
