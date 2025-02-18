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
    if (timer->isActive()) {
        timer->stop();
        qDebug() << "Ajastin pysäytetty";
    }
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

    if (currentWindowType == WindowType::OPERATIONS) {
        if (currentWindow) {
            currentWindow->close();
        }
        if (mainMenuWindow) {
            startTimer(mainMenuWindow, WindowType::MAINMENU);
        }
        emit returnToMainMenuRequested();
    } else {
        QMessageBox::warning(currentWindow, "Timeout",
                             "Your session has timed out due to inactivity.");
        emit timerExpired();
        if (currentWindow) {
            currentWindow->close();
        }
    }
}
