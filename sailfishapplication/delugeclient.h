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
    enum DelugeRequest {

    };
    DelugeRequest req;
public:
    explicit DelugeClient(QObject *parent = 0);
    ~DelugeClient();
    Q_INVOKABLE QVariantList getTorrents();

private:
    QSslSocket * _pSocket;
    QVariantList * _pActiveTorrents;
    QProcess * _p_rencode_python;
    QByteArray * _pArray;
    object dezlib, compzlib;
    object imp;
    object rencode;
    int read_times;
signals:
    void completed_packet();
    void loggedIn();
    void torrentsReceived(object torrents);
public slots:
    Q_SLOT void readTcpData();
    Q_SLOT void read_completed();
    void after_login();
    void after_info(object result);
};

#endif // DELUGECLIENT_H
