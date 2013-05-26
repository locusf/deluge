#ifndef DELUGECLIENT_H
#define DELUGECLIENT_H

#include <QObject>
#include <QVariantList>
#include <QSslSocket>
#include <qsailfishsocket.h>
#include <QProcess>

class DelugeClient : public QObject
{
    Q_OBJECT
public:
    explicit DelugeClient(QObject *parent = 0);
    ~DelugeClient();
    Q_INVOKABLE QVariantList getTorrents();

private:
    QSslSocket * _pSocket;
    QVariantList * _pActiveTorrents;
    QProcess * _p_rencode_python;
signals:
    
public slots:
    Q_SLOT void readTcpData();
    Q_SLOT void data_written(qint64 written);
    Q_SLOT void doLogin();
    Q_SLOT void ssl_error(const QSslError& error);
    Q_SLOT void ssl_change_mode(QSslSocket::SslMode mode);
    Q_SLOT void ssl_errors(const QList<QSslError>& errors);
};

#endif // DELUGECLIENT_H
