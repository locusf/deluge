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
    connect(this, SIGNAL(completed_packet()), this, SLOT(read_completed()));
    connect(this, SIGNAL(loggedIn()), this, SLOT(after_login()));
    connect(this, SIGNAL(torrentsReceived(object)), this, SLOT(after_info(object)));
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
    char* data;
    data = (char*)malloc(bytes_to_read);
    qDebug() << "Read bytes: " << _pSocket->read(data, bytes_to_read);
    QByteArray arr;
    arr = QByteArray::fromRawData(const_cast<const char*>(data), bytes_to_read);
    _pArray->append(arr);
    if (bytes_to_read != 4096) {
        completed_packet();
        _pArray = 0;
        _pArray = new QByteArray();
    }
    free(data);
}

void DelugeClient::read_completed()
{
    try {
        qDebug() << "Will read " << _pArray->size() << " bytes";
        object zlib_mod = import("zlib");
        dezlib = zlib_mod.attr("decompressobj")();
        object data = dezlib.attr("decompress")(_pArray->constData());
        qDebug() << "Uncompressed data: " << extract<char *>(data);
        dezlib.attr("flush")();
        imp = import("imp");
        rencode = imp.attr("load_source")("rencode", "/opt/sdk/share/deluge/rencode.py");
        object tdata = rencode.attr("loads")(data);
        qDebug() << extract<int>(tdata[0]);
        qDebug() << extract<int>(tdata[1]);
        int respnum = extract<int>(tdata[1]);
        object result = tdata[2];
        if (respnum == 11) {
            loggedIn();
        }
        if (respnum == 12) {
            torrentsReceived(result);
        }
    } catch(error_already_set const &) {
        PyErr_Print();
    }
    qDebug() << "Packet complete";
}

QVariantList DelugeClient::getTorrents()
{
    QVariantList torrents;
    torrents.append("FOO");
    try {
        list params, inner_params;
        inner_params.append(11);
        inner_params.append("daemon.login");
        list upw;
        upw.append("locusf");
        upw.append("jepjep");
        inner_params.append(upw);
        inner_params.append(dict());
        params.append(inner_params);
        char* cdata = extract<char *>(compzlib.attr("compress")(rencode.attr("dumps")(params)));
        QByteArray data(cdata);
        qDebug() << "Wrote " << _pSocket->write(data);
    }catch(error_already_set const &) {
        PyErr_Print();
    }

    return torrents;
}

void DelugeClient::after_login() {
    try {
    list params2, get_torrents_status;
    get_torrents_status.append(12);
    get_torrents_status.append("core.get_torrents_status");
    list args;
    args.append(list());
    args.append(dict());
    get_torrents_status.append(args);
    get_torrents_status.append(dict());
    params2.append(get_torrents_status);
    char* cdata2 = extract<char *>(compzlib.attr("compress")(rencode.attr("dumps")(params2)));
    QByteArray data2(cdata2);
    qDebug() << "Wrote: " << _pSocket->write(data2);
    } catch (error_already_set const &) {
        PyErr_Print();
    }
}

void DelugeClient::after_info(object result) {
    try {
        dict res = extract<dict>(result);
        qDebug() << "Server information: " << res.get("reads");
    } catch (error_already_set const &) {
        PyErr_Print();
    }
}
