#include "delugeclient.h"
#include <QDebug>
#include <QProcess>
#include <QSslSocket>
#include <QSslConfiguration>
#include <qsailfishsocket.h>
#include <QDataStream>
#include <zlib.h>
#include <boost/python.hpp>
#include <Python.h>

using namespace boost::python;

DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent),
    state(ConnectedState)
{
    Py_Initialize();
    _pActiveTorrents = new QVariantList();
    _pSocket = new QSslSocket(this);
    _pArray = new QByteArray();
    zlib = import("zlib");
    imp = import("imp");
    rencode = imp.attr("load_source")("rencode", "/opt/sdk/share/deluge/rencode.py");
    _p_rencode_python = new QProcess(this->parent());
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    connect(_pSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(wrotebytes(qint64)));
    _pSocket->connectToHostEncrypted("gtsq.lan", 58846, "Deluge Daemon");
}

DelugeClient::~DelugeClient()
{
    delete _pSocket;
    delete _pActiveTorrents;
    Py_Finalize();
}

void DelugeClient::readTcpData()
{
    qDebug() << "Read something";
    _pArray->append(_pSocket->readAll());
    try {
    qDebug() << rencode.attr("loads")(_pArray->constData());
    }catch(error_already_set const &) {
        PyErr_Print();
    }
}

void DelugeClient::wrotebytes(qint64 len){
    qDebug() << "Wrote bytes for: " << len;
}

QVariantList DelugeClient::getTorrents()
{
    QVariantList torrents;
    torrents.append("FOO");
    try {
        list params, inner_params, list_torrents;
        inner_params.append(1337);
        inner_params.append("daemon.login");
        list upw;
        upw.append("locusf");
        upw.append("jepjep");
        inner_params.append(upw);
        inner_params.append(dict());
        params.append(inner_params);
        list_torrents.append(1237);
        list_torrents.append("core.get_session_state");
        list_torrents.append(list());
        list_torrents.append(dict());
        params.append(list_torrents);
        char* cdata = extract<char *>(zlib.attr("compress")(rencode.attr("dumps")(params)));
        QByteArray data(cdata);
        _pSocket->write(data);
    }catch(error_already_set const &) {
        PyErr_Print();
    }
    return torrents;
}
