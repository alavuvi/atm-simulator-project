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
    qDebug() << "Timer started with" << timeout << "ms timeout";
}

void TimerManager::stopTimer()
{
    timer->stop();
    qDebug() << "Timer stopped";
}

void TimerManager::resetTimer()
{
    if(timer->isActive()) {
        timer->start();
        qDebug() << "Timer reset";
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
