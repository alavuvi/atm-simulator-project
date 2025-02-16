#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <QObject>
#include <QTimer>
#include <QString>

class TimerManager : public QObject
{
    Q_OBJECT

public:
    static constexpr int LOGIN_TIMEOUT = 10000;
    static constexpr int DEFAULT_TIMEOUT = 30000;

    static TimerManager& getInstance();
    void startTimer(QWidget* window, int timeout = DEFAULT_TIMEOUT);
    void stopTimer();
    void resetTimer();

signals:
    void timerExpired();

private:
    TimerManager(QObject* parent = nullptr);
    ~TimerManager();
    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;

    QTimer* timer;
    QWidget* currentWindow;

private slots:
    void handleTimeout();

};

#endif // TIMERMANAGER_H
