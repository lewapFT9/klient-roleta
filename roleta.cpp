#include "roleta.h"
#include "ui_roleta.h"

#include <QMetaEnum>
#include <QMessageBox>

Roleta::Roleta(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Roleta)
{
    ui->setupUi(this);
    setKontroler();
}

Roleta::~Roleta()
{
    delete ui;
}

void Roleta::on_IpAddress_textChanged(const QString &arg1)
{
    QString isCorrect ="0";

    if(arg1=="..."){
        isCorrect="";

    }
    else{
        QHostAddress isValid(arg1);
        if(QAbstractSocket::IPv4Protocol==isValid.protocol()){
            isCorrect="1";
        }
    }
    ui->IpAddress->setProperty("isCorrect",isCorrect);
    style()->polish(ui->IpAddress);
}

void Roleta::on_connect_clicked()
{
    if(kontroler.currentState()==QAbstractSocket::ConnectedState)
        kontroler.disconnectFromServer();
    else{
        auto ip=ui->IpAddress->text();
        auto port=ui->port->value();
        kontroler.connectWithServer(ip,port);
    }
    auto message = "Roleta";
    kontroler.send(message);
}
void Roleta::roletaConnected()
{
    ui->messages->addItem("Roleta Connected");
    ui->connect->setText("Disconnect");
    style()->polish(ui->messages);
}

void Roleta::roletaDisconnected()
{
    ui->messages->addItem("Roleta Disconnected");
    ui->connect->setText("Connect");
    style()->polish(ui->messages);

}

void Roleta::roletaStateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    ui->messages->addItem(metaEnum.valueToKey(state));
    style()->polish(ui->messages);

}

void Roleta::roletaErrorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    ui->messages->addItem(metaEnum.valueToKey(error));
    style()->polish(ui->messages);

}

void Roleta::roletaDataReady(QByteArray data)
{
    ui->commands->addItem(QString(data));
}

void Roleta::action(QByteArray data)
{
    QString command= QString(data);
    if(command=="Lower the blind"){
        QMessageBox::information(this,"Command","Lowering the blind");
        ui->commands->addItem("Done");
    }
    else if(command=="Raise the blind"){
        QMessageBox::information(this,"Command","Rising the blind");
        ui->commands->addItem("Done");
    }
    // else if(command=="Turn on the light" && lightStatus !="On"){
    //     QMessageBox::information(this,"Command","Turning on the light");
    //     ui->commands->addItem("Done");
    //     lightStatus = "On";
    // }
    // else if(command=="Turn on the light" && lightStatus =="On"){
    //     QMessageBox::information(this,"Command","Light is already on");
    //     ui->commands->addItem("Done");
    // }
}

void Roleta::setKontroler()
{
    connect(&kontroler,&Kontroler::connected,this, &Roleta::roletaConnected);
    connect(&kontroler,&Kontroler::disconnected,this, &Roleta::roletaDisconnected);
    connect(&kontroler,&Kontroler::stateChanged,this, &Roleta::roletaStateChanged);
    connect(&kontroler,&Kontroler::errorOccurred,this, &Roleta::roletaErrorOccurred);
    connect(&kontroler,&Kontroler::dataReady,this, &Roleta::roletaDataReady);
    connect(&kontroler,&Kontroler::dataReady,this, &Roleta::action);


}
