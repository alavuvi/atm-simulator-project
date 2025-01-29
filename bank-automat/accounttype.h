#ifndef ACCOUNTTYPE_H
#define ACCOUNTTYPE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class AccountType : public QWidget
{
    Q_OBJECT
public:
    explicit AccountType(QWidget *parent = nullptr);
    void setAccounts(const QStringList &accounts);

signals:
    void accountSelected(const QString &type);

private slots:
    void onDebitClicked();
    void onCreditClicked();

private:
    QPushButton *btnDebit;
    QPushButton *btncredit;
    QStringList accountList;
};

#endif
