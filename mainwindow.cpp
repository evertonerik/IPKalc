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
    unsigned int ip = (ips[0].toInt() << 24) | (ips[1].toInt() << 16) | (ips[2].toInt() << 8) | (ips[3].toInt() );
    unsigned int network = ip&mask;
    unsigned int wild = ~mask;


    ui->lineEdit_CIDR->setText(QString::number(value));
    ui->lineEdit_Mask->setText(ip2Qstring(mask));

    ui->tableWidget_Info->item(0,0)->setText(ui->lineEdit_IP->text());
    ui->tableWidget_Info->item(1,0)->setText("Private");
    ui->tableWidget_Info->item(2,0)->setText("C");
    ui->tableWidget_Info->item(3,0)->setText(ip2Qstring(network));
    ui->tableWidget_Info->item(4,0)->setText(ip2Qstring(network|wild));
    ui->tableWidget_Info->item(5,0)->setText(ui->lineEdit_Mask->text());
    ui->tableWidget_Info->item(6,0)->setText(ip2Qstring(wild));
    ui->tableWidget_Info->item(7,0)->setText(QString::number(wild-1));
    ui->tableWidget_Info->item(8,0)->setText(ip2Qstring(network+1));
    ui->tableWidget_Info->item(9,0)->setText(ip2Qstring(network+1)+"-"+ip2Qstring((network|wild)-1));
    ui->tableWidget_Info->item(10,0)->setText(ip2Qstring(network+2)+"-"+ip2Qstring((network|wild)-1));

}

