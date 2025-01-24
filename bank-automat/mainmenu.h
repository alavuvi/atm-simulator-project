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

    void setCardnumber(const QString &newCardnumber);

    void setMyToken(const QByteArray &newMyToken);

private:
    Ui::MainMenu *ui;
    QString cardnumber;
    QByteArray myToken;
};

#endif // MAINMENU_H
