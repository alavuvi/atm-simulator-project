#ifndef MAINMENU_H
#define MAINMENU_H

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

private slots:
    void on_btnBalance_clicked();
    void on_btnTransactions_clicked();
    void on_btnWithdraw_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainMenu *ui;
    QByteArray myToken;
    QString accountid;
};

#endif
