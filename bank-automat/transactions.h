#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setCardnumber(const QString &newCardnumber);

    void setMyToken(const QByteArray &newMyToken);

private:
    Ui::Transactions *ui;
    QString cardnumber;
    QByteArray myToken;
};

#endif // TRANSACTIONS_H
