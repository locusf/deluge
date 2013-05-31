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
    read_times(0)
{
    Py_Initialize();
    _pActiveTorrents = new QVariantList();
    _pSocket = new QSslSocket(this);
    _pArray = new QByteArray();
    object zlib_mod = import("zlib");
    dezlib = zlib_mod.attr("decompressobj")();
    compzlib = zlib_mod;
    imp = import("imp");
    rencode = imp.attr("load_source")("rencode", "/opt/sdk/share/deluge/rencode.py");
    _p_rencode_python = new QProcess(this->parent());
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    connect(_pSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(wrotebytes(qint64)));
    connect(this, SIGNAL(completed_packet()), this, SLOT(read_completed()));
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
    qint64 bytes_to_read = _pSocket->bytesAvailable();
    qDebug() << "Read something of length " << bytes_to_read;

    // First chunk
    if (bytes_to_read <= 4096 && read_times == 0)
    {
        _pSocket->read(bytes_to_read);
        read_times++;
    // All large chunks
    } else if (bytes_to_read == 4096 && read_times > 0) {
        _pArray->append(_pSocket->read(bytes_to_read));
        read_times++;
    // Last small chunk
    } else if (bytes_to_read < 4096 && read_times > 0) {
        _pArray->append(_pSocket->read(bytes_to_read));
        read_times = 0;
        emit completed_packet();
    }
}

void DelugeClient::wrotebytes(qint64 len){
    qDebug() << "Wrote bytes for: " << len;
}

void DelugeClient::read_completed()
{
    try {
        const char* cdata = extract<const char *>(dezlib.attr("decompress")(_pArray->constData()));
        qDebug() << cdata;
        qDebug() << rencode.attr("loads")(cdata);
    } catch(error_already_set const &) {
        PyErr_Print();
    }
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
        list_torrents.append("core.get_torrents_status");
        list status_params;
        status_params.append(dict());
        status_params.append(list());
        list_torrents.append(status_params);
        list_torrents.append(dict());
        params.append(list_torrents);
        char* cdata = extract<char *>(compzlib.attr("compress")(rencode.attr("dumps")(params)));
        QByteArray data(cdata);
        _pSocket->write(data);
    }catch(error_already_set const &) {
        PyErr_Print();
    }
    return torrents;
}
