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
    static constexpr int OPERATIONS_TIMEOUT = 10000;
    static constexpr int MAINMENU_TIMEOUT = 30000;

    enum class WindowType {
        LOGIN,
        MAINMENU,
        OPERATIONS
    };

    static TimerManager& getInstance();
    void startTimer(QWidget* window, WindowType type);
    void stopTimer();
    void resetTimer();
    void returnToMainMenu();

signals:
    void timerExpired();
    void returnToMainMenuRequested();

private:
    TimerManager(QObject* parent = nullptr);
    ~TimerManager();
    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;

    QTimer* timer;
    QWidget* currentWindow;
    WindowType currentWindowType;

private slots:
    void handleTimeout();

};

#endif // TIMERMANAGER_H
