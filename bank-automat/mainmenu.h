#ifndef MAINMENU_H
#define MAINMENU_H

#include "qlabel.h"
#include <QNetworkAccessManager>
#include <QDialog>
#include <QDebug>


namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    void setMyToken(const QByteArray &newMyToken);
    void setAccountId(const QString &newAccountId);
    void setupTimerConnections();

private slots:
    void handleCustomerInfo(QNetworkReply *reply);
    void on_btnBalance_clicked();
    void on_btnTransactions_clicked();
    void on_btnWithdraw_clicked();
    void on_btnLogout_clicked();  

private:
    Ui::MainMenu *ui;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QByteArray myToken;
    QString accountid;
    QLabel *labelName;
    void getCustomerInfo();
    void handleTimerExpired();

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif
