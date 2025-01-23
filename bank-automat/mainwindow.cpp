#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCardnumber_clicked()
{
    Login *objLogin=new Login(this);
    QString cardNumber = ui->lineCardnumber->text();
    objLogin->setCardNumber(cardNumber);
    objLogin->open();
}
