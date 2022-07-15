#include "zplprinter.h"

ZPLPrinter::ZPLPrinter(QObject *parent) : QObject(parent)
{
    bytes_written = 0;
    socket = new QTcpSocket(this);
    connection_timer = new QTimer(this);
    connection_timer->setSingleShot(true);
    connection_timer->setInterval(3000);

    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(processSocketError(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(processSocketBytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(processSocketStateChange(QAbstractSocket::SocketState)));
    QObject::connect(connection_timer, SIGNAL(timeout()), this, SLOT(processConnectionTimerTimeout()));
}

int ZPLPrinter::print(QString text, QString address)
{
    QRegExp rx;
    rx.setPattern("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):(\\d{1,5})$");
    if (rx.indexIn(address) != 0) {
        notification("Некорректный адрес принтера", 2);
        notification("Ожидаемый формат адреса: \"IP:PORT\"", 0);
        return 1;
    }
    QString ip = rx.cap(1) + "." + rx.cap(2) + "." + rx.cap(3) + "." + rx.cap(4);
    uint port = rx.cap(5).toUInt();

    rx.setPattern("^(\\^XA.*\\^XZ)$");
    if (rx.indexIn(text) != 0) {
        notification("Некорректный ZPL код", 2);
        notification("ZPL код начинается с ^XA и заканчивается ^XZ", 0);
        return 2;
    }

    zpl = text.toUtf8();

    readyToPrint(false);
    socket->connectToHost(ip, port);
    connection_timer->start();
    return 0;
}

int ZPLPrinter::processSocketError(QAbstractSocket::SocketError e)
{
    notification("Ошибка TCP #" + QString::number(e), 2);
    notification(socket->errorString(), 0);
    return 0;
}

int ZPLPrinter::processSocketBytesWritten(qint64 b)
{
    bytes_written += b;
    notification("Отправлено " + QString::number(b) + "/" + QString::number(zpl.size()) + " байт", 0);
    if (b == zpl.size()) {
        bytes_written = 0;
        socket->close();
        success();
    }
    return 0;
}

int ZPLPrinter::processSocketStateChange(QAbstractSocket::SocketState s)
{
    if (s == QAbstractSocket::ConnectedState) {
        notification("Удалось подключиться по заданному адресу", 0);
        socket->write(zpl);
        return 0;
    }

    if (s == QAbstractSocket::UnconnectedState) {
        connection_timer->stop();
        readyToPrint(true);
        return 0;
    }
    return 0;
}

int ZPLPrinter::processConnectionTimerTimeout()
{
    socket->disconnectFromHost();
    socket->abort();
    notification("Не удалось соединиться с устройством", 2);
    return 0;
}
