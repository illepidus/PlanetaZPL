#ifndef ZPLPRINTER_H
#define ZPLPRINTER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class ZPLPrinter : public QObject
{
    Q_OBJECT
public:
    explicit ZPLPrinter(QObject *parent = nullptr);
signals:
    void notification(QString text, int level = 0);
    void readyToPrint(bool);
    void success();
private slots:
    int processSocketError(QAbstractSocket::SocketError);
    int processSocketBytesWritten(qint64);
    int processSocketStateChange(QAbstractSocket::SocketState);
    int processConnectionTimerTimeout();
public slots:
    int print(QString text, QString address);
private:
    qint64 bytes_written;
    QTcpSocket *socket;
    QTimer *connection_timer;
    QByteArray zpl;
};

#endif // ZPLPRINTER_H
