#include "timermanager.h"
#include "qpushbutton.h"
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
    if (!window) {
        qDebug() << "Ei voida käynnistää ajastinta - ikkuna puuttuu";
        return;
    }

    if (currentWindow && currentWindow != window) {
        currentWindow->disconnect(this);
    }

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
    qDebug() << "Aloitettu" << timeout << "ms ajastin ikkunalle:"
             << window->metaObject()->className();
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
        QMessageBox msgBox;
        msgBox.setWindowTitle("Timeout");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Your session has timed out due to inactivity.</FONT>");
        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
        emit timerExpired();
        if (currentWindow) {
            currentWindow->close();
        }
    }
}
