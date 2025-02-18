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

void TimerManager::startTimer(QWidget* window, WindowType type)
{
    currentWindow = window;
    currentWindowType = type;

    int timeout;
    switch(type) {
    case WindowType::LOGIN:
        timeout = LOGIN_TIMEOUT;
        break;
    case WindowType::MAINMENU:
        timeout = MAINMENU_TIMEOUT;
        break;
    case WindowType::OPERATIONS:
        timeout = OPERATIONS_TIMEOUT;
        break;
    default:
        timeout = MAINMENU_TIMEOUT;
    }

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

    switch(currentWindowType) {
    case WindowType::LOGIN:
    case WindowType::MAINMENU:
        QMessageBox::warning(currentWindow, "Timeout",
                             "Your session has timed out due to inactivity.");
        emit timerExpired();
        break;

    case WindowType::OPERATIONS:
        emit returnToMainMenuRequested();
        break;
    }

    if (currentWindow) {
        currentWindow->close();
    }
}
