#include "delugeclient.h"
#include <QDebug>
#include <QProcess>
#include <QSslSocket>
#include <QSslConfiguration>
#include <qsailfishsocket.h>
#include <QDataStream>
#include <zlib.h>


DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent)
{
    _pActiveTorrents = new QVariantList();
    _pSocket = new QSslSocket(this);
    _p_rencode_python = new QProcess(this->parent());
    connect(_pSocket, SIGNAL(encrypted()), this, SLOT(doLogin()));
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));
    connect(_pSocket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(data_written(qint64)));
    connect(_pSocket, SIGNAL(peerVerifyError(const QSslError&)), this, SLOT(ssl_error(const QSslError&)));
    connect(_pSocket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(ssl_errors(const QList<QSslError>&)));
    connect(_pSocket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(ssl_change_mode(QSslSocket::SslMode)));
    _pSocket->connectToHostEncrypted("gtsq.lan", 58846, "Deluge Daemon", QIODevice::ReadWrite);
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

void DelugeClient::ssl_error(const QSslError &error)
{
    qDebug() << "SSL ERROR: " << error;
}

void DelugeClient::ssl_errors(const QList<QSslError> &errors)
{
    qDebug() << "SSL errors: " << errors;
}
void DelugeClient::ssl_change_mode(QSslSocket::SslMode mode)
{
    qDebug() << "SSL MODE CHANGE: " << mode;
}

void DelugeClient::doLogin()
{
    QStringList args;
    QString program = "/usr/bin/python";
    args << "-c" << "import imp; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print rencode.dumps([[1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}]])";
    _p_rencode_python->start(program, args);
    _p_rencode_python->waitForFinished(-1);
    qDebug() << _p_rencode_python->readAllStandardError();
    QByteArray output = _p_rencode_python->readAllStandardOutput();
    qDebug() << "Application output " << output;
    QByteArray data = qCompress(output);
    qDebug() << "wrote: " << _pSocket->write(data);
    _pSocket->flush();
}

QVariantList DelugeClient::getTorrents()
{
    QVariantList torrents;
    torrents.append("FOO");

    QString program = "/usr/bin/python";
    QStringList args;
    // [1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}]
    args << "-c" << "import imp; rencode = imp.load_source(\"rencode\", \"/opt/sdk/share/deluge/rencode.py\"); print rencode.dumps([[1337, \"daemon.login\", [\"locusf\", \"jepjep\"],{}], [1327, \"core.get_session_state\", [],{}]])";
    _p_rencode_python->start(program, args);
    _p_rencode_python->waitForFinished(-1);
    qDebug() << _p_rencode_python->readAllStandardError();
    QByteArray output = _p_rencode_python->readAllStandardOutput();
    qDebug() << "Application output " << output;
    QByteArray data = qCompress(output);
    qDebug() << "wrote: " << _pSocket->write(data);
    _pSocket->flush();
    return torrents;
}
