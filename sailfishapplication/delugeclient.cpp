#include "delugeclient.h"
#include <QDebug>
#include <QProcess>
#include <QSslSocket>
#include <qsailfishsocket.h>
#include <QDataStream>
#include <zlib.h>


DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent)
{
    _pActiveTorrents = new QVariantList();
    _pSocket = new QSailfishSocket();
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));
    connect(_pSocket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(data_written(qint64)));
    _pSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    _pSocket->connectToHostEncrypted("gtsq.lan", 58846, "Deluge Daemon");
    _pSocket->setLocalPortS(10111);
    qDebug() << _pSocket->localPort();
    qDebug() << _pSocket->peerPort();
}

DelugeClient::~DelugeClient()
{
    delete _pSocket;
    delete _pActiveTorrents;
}
void DelugeClient::readTcpData()
{
    qDebug() << "Read something";
}

void DelugeClient::data_written(qint64 written)
{
    qDebug() << "Wrote " << written << " bytes";
}

QVariantList DelugeClient::getTorrents()
{
    QVariantList torrents;
    torrents.append("FOO");
    QProcess rencode_python(this->parent());
    QString program = "/usr/bin/python";
    QStringList args;
    // [1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}]
    args << "-c" << "import imp; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print rencode.dumps([[1327, \"core.get_session_state\", [],{}]])";
    rencode_python.start(program, args);
    rencode_python.waitForFinished(-1);
    qDebug() << rencode_python.readAllStandardError();
    QByteArray output = rencode_python.readAllStandardOutput();
    qDebug() << "Application output " << output;
    QByteArray data = qCompress(output);
    QDataStream stream(_pSocket);
    while(_pSocket->waitForEncrypted()) {
        qDebug() << "Encrypted!";
    }
    while(!_pSocket->waitForConnected())
    {
        qDebug() << "Not connected!";
    }
    stream << data;
    while(_pSocket->waitForBytesWritten())
    {
        qDebug() << "Wrote data!" << data.toHex();
    }
    while(_pSocket->waitForReadyRead(1000))
    {
        qDebug() << "Read something!";
        qDebug() << _pSocket->readAll();
    }
    //QByteArray tdata = qUncompress(_pSocket->readAll());
    //qDebug() << "Data: " << tdata.data() << " data len: " << strlen(tdata.data());
    return torrents;
}
