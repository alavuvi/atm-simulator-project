#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

    void setAccountid(const QString &newAccountid);
    void setMyToken(const QByteArray &newMyToken);


private slots:
    void on_btnBalance_clicked();
    void on_btnTransactions_clicked();
    void on_btnWithdraw_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainMenu *ui;
    QString accountid;
    QByteArray myToken;
};

#endif // MAINMENU_H
