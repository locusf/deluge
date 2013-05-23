#include "delugeclient.h"
#include <QDebug>
#include <QProcess>
#include <QSslSocket>
#include <zlib.h>


DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent)
{
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
    args << "-c" << "import imp; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print rencode.dumps([[1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}]])";
    rencode_python.start(program, args);
    rencode_python.waitForFinished(-1);
    QString message;
    message = rencode_python.readAllStandardOutput();
    QByteArray data = qCompress(message.toLocal8Bit());
    _pSocket = new QSslSocket(this);
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));
    connect(_pSocket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(data_written(qint64)));
    _pSocket->ignoreSslErrors();
    _pSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    _pSocket->connectToHostEncrypted("10.3.0.124", 58846);
    if (!_pSocket->waitForEncrypted(1000)) {
        qDebug() << _pSocket->errorString();
    }
    _pSocket->write(data);
    args = QStringList();
    args << "-c" << "import imp; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print rencode.dumps([[1327, \"get_session_state\", [],{}]])";
    rencode_python.start(program, args);
    rencode_python.waitForFinished(-1);
    message = rencode_python.readAllStandardOutput();
    data = qCompress(message.toLocal8Bit());
    _pSocket->write(data);
    //QByteArray tdata = qUncompress(_pSocket->readAll());
    //qDebug() << "Data: " << tdata.data() << " data len: " << strlen(tdata.data());
    return torrents;
}
