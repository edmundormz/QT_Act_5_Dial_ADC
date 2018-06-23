#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
#include <QScreen>
#include <QMetaEnum>
#include "qcustomplot.h"

QByteArray databuf;
unsigned char pot = 49;
//int pot = 49;
int foo = 0;
int segundero=0;
QVector<double> xx(51), yy(51);
int TiempoMuestreo = 500; //ms

void MainWindow::fTimer(){
    QCustomPlot *ObjetoGrafica = ui->Grafica;
    segundero++;
    if(segundero>50){ segundero=0;
     for(int i=0; i<=50; i++){
         xx[i] = 0;
         yy[i] = 0;
     }
    }
    ui->lcdNumber_3->display(segundero);
    if(segundero == 0) xx[segundero] = segundero;
    else if(segundero > 0){
      xx[segundero] = segundero*((float)TiempoMuestreo/1000);
}
  if(hw_is_available){
      if(hw->isWritable()){

//          //if (pot != foo){
//              QString tmp = qPrintable(QString::number(pot));
//              //tmp += "\n";
//              hw->write(qPrintable(tmp));
//              //foo = pot;
//              qDebug() << "pot = " << tmp;
//          //}

            int dialV = ui->dial->value();
            ///qInfo()<<dialV;
            char cValue = dialV;
            qDebug()<<cValue;
            char *pCValue = &cValue;
            //qDebug() << pCValue;
            hw->write(pCValue);

          hw->write("a");
          if(hw->isReadable()){
              QByteArray datosLeidos = hw->read(2);
              int ADC_Digital = datosLeidos.toHex().toInt(0,16);
              float voltaje = (5*(float)ADC_Digital/1023);
              ui->progressBar->setValue(ADC_Digital);
              ui->lcdNumber->display(voltaje);
              yy[segundero] = voltaje;

          }
      }

      else yy[segundero] = -1;

      ObjetoGrafica->addGraph();
      ObjetoGrafica->graph(0)->setData(xx, yy);

      ObjetoGrafica->xAxis->setLabel("Tiempo (s)");
       ObjetoGrafica->yAxis->setLabel("Voltaje (V)");
       ObjetoGrafica->xAxis->setRange(0, TiempoMuestreo*50/1000);
        ObjetoGrafica->yAxis->setRange(0, 5);
      ui->Grafica->replot();

  }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //QTimer *cronometro=new QTimer(this);
     connect(cronometro, SIGNAL(timeout()), this, SLOT(fTimer()));
     cronometro->start(TiempoMuestreo);

     ui->dial->setValue(49);
     ui->lcdNumber_4->display(49);

    hw_is_available = false;
    hw_port_name = "";
    hw = new QSerialPort;
    ui->lineEdit3->setText("115200");
    ui->lineEdit1->setText(QString::number(QSerialPortInfo::availablePorts().length()));
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            ui->lineEdit2->setText(QString::number(serialPortInfo.vendorIdentifier()));
            ui->lineEdit5->setText(QString::number(serialPortInfo.productIdentifier()));
            hw_port_name = serialPortInfo.portName();
            hw_is_available = true;
        }
    }

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
        ui->comboBox1->addItem(serialPortInfo.portName());
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()
{
    QString bauds = ui->lineEdit3->text();
    int vbauds = bauds.toInt();
    if(vbauds == 115200){
        hw ->setBaudRate(QSerialPort::Baud115200);
        ui->lineEdit4->setText(bauds);
    }
    else{
    }

    if(hw_is_available){
        hw ->setPortName(hw_port_name);
        hw->open(QIODevice::ReadWrite);
        hw->setDataBits(QSerialPort::Data8);
        hw->setParity(QSerialPort::NoParity);
        hw->setStopBits(QSerialPort::OneStop);
        hw->setFlowControl(QSerialPort::NoFlowControl);
    }
    else{
        QMessageBox::information(this,"Error","No esta disponible el puerto serial");
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    hw->close();
    QMessageBox::information(this,"Fin de Comunicación","Comunicación Serial Terminada!");
    close();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    TiempoMuestreo = value*100;
    cronometro->start(TiempoMuestreo);
    ui->lcdNumber_2->display(TiempoMuestreo);
}

void MainWindow::on_dial_valueChanged()
{
    pot = ui->dial->value();
    //hw->write(pot);
}
