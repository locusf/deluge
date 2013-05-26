#ifndef DELUGECLIENT_H
#define DELUGECLIENT_H

#include <QObject>
#include <QVariantList>
#include <QSslSocket>
#include <qsailfishsocket.h>

class DelugeClient : public QObject
{
    Q_OBJECT
public:
    explicit DelugeClient(QObject *parent = 0);
    ~DelugeClient();
    Q_INVOKABLE QVariantList getTorrents();

private:
    QSailfishSocket * _pSocket;
    QVariantList * _pActiveTorrents;
signals:
    
public slots:
    Q_SLOT void readTcpData();
    Q_SLOT void data_written(qint64 written);
};

#endif // DELUGECLIENT_H
