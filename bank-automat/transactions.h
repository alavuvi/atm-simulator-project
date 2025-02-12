#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setAccountId(const QString &newAccountId);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);
    void on_btnBack_clicked();
    void on_btn_older_clicked();

private:
    Ui::Transactions *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
    QByteArray response_data;
    QNetworkReply *reply;
};

#endif // TRANSACTIONS_H
