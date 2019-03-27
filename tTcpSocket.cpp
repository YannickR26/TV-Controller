#include "tTcpSocket.h"
#include <QDebug>
#include <QDataStream>

static inline qint32 ArrayToInt(QByteArray source);
static inline QByteArray IntToArray(qint32 source);

tTcpSocket::tTcpSocket(QObject *parent) :
    QObject(parent)
{
    this->socket = NULL;
    this->server = NULL;
}

bool tTcpSocket::connectToHost(const QHostAddress &addr, quint16 port)
{
    this->socket = new QTcpSocket(this);

    connect(this->socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    QByteArray *buffer = new QByteArray();
    qint32 *s = new qint32(0);
    buffers.insert(socket, buffer);
    sizes.insert(socket, s);

    qDebug() << "Connecting to " << addr.toString() << ":" << port;

    // this is not blocking call
    this->socket->connectToHost(addr, port);

    return this->socket->waitForConnected(3000);
}

void tTcpSocket::listenSocket(const QHostAddress &addr, quint16 port)
{
    this->server = new QTcpServer(this);

    connect(this->server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    this->server->listen(addr, port);

    qDebug() << "Listening...";
}

void tTcpSocket::newConnection()
{
    while (this->server->hasPendingConnections())
    {
        this->socket = server->nextPendingConnection();
        connect(this->socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(this->socket, SIGNAL(disconnected()), SLOT(disconnected()));

        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(this->socket, buffer);
        sizes.insert(this->socket, s);

        qDebug() << "New Connection...";
    }
}

void tTcpSocket::writeData(QByteArray data)
{
    if(this->socket->state() == QAbstractSocket::ConnectedState)
    {
        this->socket->write(IntToArray(data.size())); //write size of data
        this->socket->write(data); //write the data itself
        this->socket->flush();
        qDebug() << "Write Data: " << data;
    }
}

void tTcpSocket::disconnect()
{
    if (this->socket != NULL)
        this->socket->close();

    if (this->server != NULL)
        this->server->close();
}

void tTcpSocket::connected()
{
    qDebug() << "connected...";
}

void tTcpSocket::disconnected()
{
    QTcpSocket *socket_ = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer_ = buffers.value(socket_);
    qint32 *s = sizes.value(socket_);
    socket_->deleteLater();
    delete buffer_;
    delete s;

    qDebug() << "disconnected...";
}

void tTcpSocket::readyRead()
{
    QTcpSocket *socket_ = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket_);
    qint32 *s = sizes.value(socket_);
    qint32 size = *s;

    qDebug() << "ready Read..." << socket_->bytesAvailable() << "  " << this->socket->bytesAvailable();

    while (socket_->bytesAvailable() > 0)
    {
        buffer->append(socket_->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                qDebug() << "emit dataReceived()...";
                emit dataReceived(data);
            }
        }
    }
}

bool tTcpSocket::readData(QByteArray *data_)
{
    QTcpSocket *socket_ = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket_);
    qint32 *s = sizes.value(socket_);
    qint32 size = *s;

    qDebug() << "Read data, byte available: " << socket_->bytesAvailable();
    if (socket_->bytesAvailable() > 0)
    {
        buffer->append(socket_->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                *data_ = data;
                return true;
            }
        }
    }
    return false;
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
