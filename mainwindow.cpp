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
    QString ipclass,iptype = "Public";

    int ipc = ip>>24;
    if (ipc < 128 ) {
        ipclass = "A";
        if (ipc==0) iptype = "Reserved"; //10.X.X.X
        if (ipc==10) iptype = "Private"; //10.X.X.X
        if (ipc==127) iptype = "Loopback"; //10.X.X.X
        if ((ip >= 1681915904)&&(ip<=1686110207)) iptype = "CGNAT"; //100.64-127.X.X
    }else if (ipc < 192 ) {
        ipclass = "B";
        if ((ip >= 2851995648)&&(ip<=2852061183)) iptype = "APIPA"; //169.254.X.X
        if ((ip >= 2886729728)&&(ip<=2887843839)) iptype = "Private"; //172.16-32.X.X
    }else if (ipc < 224 ) {
        ipclass = "C";
        if ((ip >= 3232235520)&&(ip<=3232301055)) iptype = "Private"; //172.16-32.X.X
    } else if (ipc < 224 ) {
        ipclass = "D";
        iptype = "Multicast";
    } else {
        ipclass = "E";
        iptype = "Experimental";
    }



    ui->lineEdit_CIDR->setText(QString::number(value));
    ui->lineEdit_Mask->setText(ip2Qstring(mask));

    ui->tableWidget_Info->item(0,0)->setText(ui->lineEdit_IP->text());
    ui->tableWidget_Info->item(1,0)->setText(ip2Qstring(network));
    ui->tableWidget_Info->item(2,0)->setText(ip2Qstring(network+1)+"-"+ip2Qstring((network|wild)-1));
    ui->tableWidget_Info->item(3,0)->setText(ip2Qstring(network+2)+"-"+ip2Qstring((network|wild)-1));
    ui->tableWidget_Info->item(4,0)->setText(ip2Qstring(network|wild));
    ui->tableWidget_Info->item(5,0)->setText(ip2Qstring(network+1));
    ui->tableWidget_Info->item(6,0)->setText(QString::number(wild-1));
    ui->tableWidget_Info->item(7,0)->setText(ui->lineEdit_Mask->text());
    ui->tableWidget_Info->item(8,0)->setText(ip2Qstring(wild));
    ui->tableWidget_Info->item(9,0)->setText("/"+ui->lineEdit_CIDR->text());
    ui->tableWidget_Info->item(10,0)->setText(iptype);
    ui->tableWidget_Info->item(11,0)->setText(ipclass);

}

