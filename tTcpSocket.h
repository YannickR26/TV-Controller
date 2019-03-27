#ifndef TTCPSOCKET_H
#define TTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

class tTcpSocket : public QObject
{
    Q_OBJECT

public:
    tTcpSocket(QObject *parent = 0);

    bool connectToHost(const QHostAddress &addr, quint16 port);
    void listenSocket(const QHostAddress &addr, quint16 port);
    void writeData(QByteArray data);
    bool readData(QByteArray *data_);
    void disconnect();
    QString getError() { return this->socket->errorString(); }

signals:
    void dataReceived(QByteArray data);

public slots:
    void connected();
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpSocket 							*socket;
    QTcpServer                          *server;
    QHash<QTcpSocket*, QByteArray*> 	buffers; 	//We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> 		sizes; 		//We need to store the size to verify if a block has received completely


};

#endif // TTCPSOCKET_H
