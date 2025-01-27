#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Numeropainikkeet
    connect(ui->button00, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button01, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button02, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button03, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button04, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button05, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button06, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button07, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button08, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(ui->button09, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);

    connect(ui->buttonOk, &QPushButton::clicked, this, &MainWindow::onOkButtonClicked);
    connect(ui->buttonBack, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);

}

// Slotti numeronapeille
void MainWindow::onNumberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString buttonText = button->text();
        QString cardNumber = ui->lineCardnumber->text();
        cardNumber.append(buttonText);
        ui->lineCardnumber->setText(cardNumber);
    }
}

// Slot backspace napille
void MainWindow::onBackButtonClicked()
{
    QString cardnumber = ui->lineCardnumber->text();
    cardnumber.chop(1);
    ui->lineCardnumber->setText(cardnumber);
}

// Slot OK napille
void MainWindow::onOkButtonClicked()
{
    Login *objLogin=new Login(this);
    QString cardNumber = ui->lineCardnumber->text();
    objLogin->setCardNumber(cardNumber);
    ui->lineCardnumber->clear();
    objLogin->open();
}

MainWindow::~MainWindow()
{
    delete ui;
}
