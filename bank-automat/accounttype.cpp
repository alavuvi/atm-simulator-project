#include "accounttype.h"

AccountType::AccountType(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    btnDebit = new QPushButton("Debit", this);
    btncredit = new QPushButton("Credit", this);

    connect(btnDebit, &QPushButton::clicked, this, &AccountType::onDebitClicked);
    connect(btncredit, &QPushButton::clicked, this, &AccountType::onCreditClicked);

    setWindowTitle("Valitse tili");
}

void AccountType::setAccounts(const QStringList &accounts)
{
    accountList = accounts;
    btnDebit->setEnabled(accounts.contains("debit"));
    btncredit->setEnabled(accounts.contains("credit"));
}

void AccountType::onDebitClicked()
{
    emit accountSelected("debit");
    close();
}

void AccountType::onCreditClicked()
{
    emit accountSelected("credit");
    close();
}
