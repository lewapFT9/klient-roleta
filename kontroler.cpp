#include "kontroler.h"

Kontroler::Kontroler(QObject *parent)
    : QObject{parent}
{
    connect(&socket,&QTcpSocket::connected,this, &Kontroler::connected);
    connect(&socket,&QTcpSocket::disconnected,this, &Kontroler::disconnected);
    connect(&socket,&QTcpSocket::stateChanged,this, &Kontroler::socketStateChanged);
    connect(&socket,&QTcpSocket::errorOccurred,this, &Kontroler::errorOccurred);
    connect(&socket,&QTcpSocket::readyRead,this, &Kontroler::socketReadyRead);
}

void Kontroler::connectWithServer(QString ip, int port)
{
    if(socket.isOpen()){
        if(ip==serverIp && port==ServerPort){
            return;
        }
        socket.close();
    }
    serverIp=ip;
    ServerPort=port;
    socket.connectToHost(serverIp,ServerPort);
}

void Kontroler::disconnectFromServer()
{
    socket.close();
}

QAbstractSocket::SocketState Kontroler::currentState()
{
    return socket.state();
}

void Kontroler::send(QString message)
{
    socket.write(message.toUtf8());
}

void Kontroler::socketStateChanged(QAbstractSocket::SocketState state)
{
    if(state==QAbstractSocket::UnconnectedState){
        socket.close();
    }
    emit stateChanged(state);
}

void Kontroler::socketReadyRead()
{
    auto data=socket.readAll();
    emit dataReady(data);
}
