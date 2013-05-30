#ifndef DELUGECLIENT_H
#define DELUGECLIENT_H

#include <QObject>
#include <QVariantList>
#include <QSslSocket>
#include <qsailfishsocket.h>
#include <QProcess>
#include <QMutex>
#include <boost/python.hpp>

using namespace boost::python;

class DelugeClient : public QObject
{
    Q_OBJECT
    enum DelugeState {
        ConnectedState,
        LoginState,
        DataState
    };
    DelugeState state;
public:
    explicit DelugeClient(QObject *parent = 0);
    ~DelugeClient();
    Q_INVOKABLE QVariantList getTorrents();

private:
    QSslSocket * _pSocket;
    QVariantList * _pActiveTorrents;
    QProcess * _p_rencode_python;
    QByteArray * _pArray;
    object zlib;
    object imp;
    object rencode;
signals:
    
public slots:
    Q_SLOT void readTcpData();
    Q_SLOT void wrotebytes(qint64 len);
};

#endif // DELUGECLIENT_H
