#ifndef DELUGECLIENT_H
#define DELUGECLIENT_H

#include <QObject>
#include <QVariantList>
#include <QSslSocket>

class DelugeClient : public QObject
{
    Q_OBJECT
public:
    explicit DelugeClient(QObject *parent = 0);
    Q_INVOKABLE QVariantList getTorrents();
private:
    QSslSocket * _pSocket;
signals:
    
public slots:
    Q_SLOT void readTcpData();
    Q_SLOT void data_written(qint64 written);
};

#endif // DELUGECLIENT_H
