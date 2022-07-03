#include "mainwindow.h"
#include "ui_mainwindow.h"

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

QString ip2Qstring (unsigned int ip){

    return QString::number(ip >> 24)+"."+QString::number((ip >> 16)&255)+"."+QString::number((ip >> 8)&255)+"."+QString::number(ip&255);

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    unsigned int mask = ( 4294967295  >> (32 - value) ) << (32 - value);
    QStringList ips = ui->lineEdit_IP->text().split(".");
    unsigned int ip = (ips[0].toInt() << 24) | (ips[0].toInt() << 16) | (ips[0].toInt() << 8) | (ips[0].toInt() );
    unsigned int network = ip&mask;

    ui->tableWidget_Info->item(0,0)->setText(ui->lineEdit_IP->text());
    //ui->tableWidget_Info->item(0,1)->setText(ip2Qstring(network));
    ui->lineEdit_CIDR->setText(QString::number(value));
    ui->lineEdit_Mask->setText(ip2Qstring(mask));
}

