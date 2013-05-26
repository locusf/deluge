#ifndef QSAILFISHSOCKET_H
#define QSAILFISHSOCKET_H

#include <QSslSocket>

class QSailfishSocket : public QSslSocket
{
public:
    QSailfishSocket();
    void setLocalPortS(quint16 port);
};

#endif // QSAILFISHSOCKET_H
