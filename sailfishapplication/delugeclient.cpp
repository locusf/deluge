#include "delugeclient.h"
#include <QDebug>
#include <QProcess>
#include <QSslSocket>
#include <QSslConfiguration>
#include <qsailfishsocket.h>
#include <QDataStream>
#include <zlib.h>

DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent),
    state(ConnectedState)
{
    _pActiveTorrents = new QVariantList();
    _pSocket = new QSslSocket(this);
    _pArray = new QByteArray();
    _p_rencode_python = new QProcess(this->parent());
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    connect(_pSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(wrotebytes(qint64)));
    _pSocket->connectToHostEncrypted("gtsq.lan", 58846, "Deluge Daemon");
}

DelugeClient::~DelugeClient()
{
    delete _pSocket;
    delete _pActiveTorrents;
}

void DelugeClient::readTcpData()
{
    qDebug() << "Read something";
    _pArray->append(_pSocket->readAll());
}

void DelugeClient::wrotebytes(qint64 len){
    qDebug() << "Wrote bytes for: " << len;
}

QVariantList DelugeClient::getTorrents()
{
    if (_pArray->length() != 0) {
        qDebug() << *_pArray;
        qDebug() << _pArray->toHex();
    }
    QVariantList torrents;
    torrents.append("FOO");
    QString program = "/usr/bin/python";
    QStringList args;
    // [1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}]
    args << "-c" << "import imp; import zlib; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print zlib.compress(rencode.dumps([[1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}], [1327, \"core.get_session_state\", [],{}]]))";
    _p_rencode_python->start(program, args);
    _p_rencode_python->waitForFinished(-1);
    qDebug() << _p_rencode_python->readAllStandardError();
    QByteArray data = _p_rencode_python->readAllStandardOutput();
    _pSocket->write(data);
    return torrents;
}
