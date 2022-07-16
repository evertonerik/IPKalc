#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmath"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QActionGroup *BaseGroup = new QActionGroup(this);
    BaseGroup->addAction(ui->actionBinary);
    BaseGroup->addAction(ui->actionOctal);
    BaseGroup->addAction(ui->actionDecimal);
    BaseGroup->addAction(ui->actionHexadecimal);
    BaseGroup->setExclusive(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString ip2Qstring (unsigned int ip,int base = 10){


    return (QString::number(ip >> 24,base)+"."+QString::number((ip >> 16)&255,base)+"."+QString::number((ip >> 8)&255,base)+"."+QString::number(ip&255,base)).toUpper();

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{

 //=======INFO TAB
    unsigned int mask = ( 4294967295  >> (32 - value) ) << (32 - value); // 4294967295 = 255.255.255.255
    QStringList ips = ui->lineEdit_IP->text().split(".");
    unsigned int ip = (ips[0].toInt() << 24) | (ips[1].toInt() << 16) | (ips[2].toInt() << 8) | (ips[3].toInt() ); //Remap a Strings to a binary Int
    unsigned int network = ip&mask;
    unsigned int wild = ~mask;
    QString ipclass,iptype = "Public";
    int base = 10;

    //base calculation
    if (ui->actionBinary->isChecked()) base = 2;
    else if (ui->actionOctal->isChecked()) base = 8;
    else if (ui->actionDecimal->isChecked()) base = 10;
    else if (ui->actionHexadecimal->isChecked()) base = 16;

    // Verify IP Type and Class
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

    ui->tableWidget_Info->item(0,0)->setText(ip2Qstring(ip,base));
    ui->tableWidget_Info->item(1,0)->setText(ip2Qstring(network,base));
    ui->tableWidget_Info->item(2,0)->setText(ip2Qstring(mask,base));
    ui->tableWidget_Info->item(3,0)->setText(ip2Qstring(wild,base));
    ui->tableWidget_Info->item(4,0)->setText("/"+ui->lineEdit_CIDR->text());
    ui->tableWidget_Info->item(5,0)->setText(ip2Qstring(network+1,base)+"-"+ip2Qstring((network|wild)-1,base));
    ui->tableWidget_Info->item(6,0)->setText(ip2Qstring(network+2,base)+"-"+ip2Qstring((network|wild)-1,base));
    ui->tableWidget_Info->item(7,0)->setText(ip2Qstring(network|wild,base));
    ui->tableWidget_Info->item(8,0)->setText(ip2Qstring(network+1,base));
    ui->tableWidget_Info->item(9,0)->setText(QString::number(wild-1));
    ui->tableWidget_Info->item(10,0)->setText(iptype);
    ui->tableWidget_Info->item(11,0)->setText(ipclass);


    //=======FLSM TAB
    int nFLSM = pow(2,value % 8);
    if (nFLSM == 1){nFLSM = 256;}

    unsigned int netFLSM = ((value-1)/8)*8;
    if (value == 0){
        netFLSM = 0;
        nFLSM = 1;
    }

    unsigned int maskFLSM = ( 4294967295  >> (32 - netFLSM) ) << (32 - netFLSM);
    unsigned int hostsFLSM = (256/nFLSM) << (24 - netFLSM);
    unsigned int networkFLSM = 0;
    netFLSM = ip&maskFLSM;

    ui->tableWidget_FLSM->clearContents();
    ui->tableWidget_FLSM->setRowCount(nFLSM);

    for (int i = 0; i < nFLSM ; i++){
        networkFLSM = netFLSM|(hostsFLSM*i);
        ui->tableWidget_FLSM->setItem(i,0, new QTableWidgetItem(ip2Qstring(networkFLSM,base)));
        ui->tableWidget_FLSM->setItem(i,1, new QTableWidgetItem(ip2Qstring(networkFLSM+1,base)+"-"+ip2Qstring((networkFLSM|wild)-1,base)));
        ui->tableWidget_FLSM->setItem(i,2, new QTableWidgetItem(ip2Qstring(networkFLSM|wild,base)));
    }
}


void MainWindow::on_lineEdit_Mask_editingFinished()
{
    QStringList mask = ui->lineEdit_Mask->text().split(".");
    int cidr = 0;
    for (int i = 0; i < mask.count() ; i++){
        if (mask[i].toInt() > 255){
            mask[i] = "255";
        }
        cidr += mask[i].setNum(mask[i].toInt(),2).count('1');
    }
    ui->horizontalSlider->setValue(cidr);
}


void MainWindow::on_lineEdit_IP_editingFinished()
{
    QStringList ips = ui->lineEdit_IP->text().split(".");

    for (int i = 0; i < ips.count() ; i++){
        if (ips[i].toInt() > 255) ips[i] = "255";
    }

    ui->lineEdit_IP->setText(ips.join("."));
    on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}


void MainWindow::on_lineEdit_CIDR_editingFinished()
{
    if (ui->lineEdit_CIDR->text().toInt() > 32) ui->lineEdit_CIDR->setText("32");
    ui->horizontalSlider->setValue(ui->lineEdit_CIDR->text().toInt());
}





