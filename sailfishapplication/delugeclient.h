#ifndef DELUGECLIENT_H
#define DELUGECLIENT_H

#include <QObject>
#include <QVariantList>
#include <QVariant>

class DelugeClient : public QObject
{
    Q_OBJECT
public:
    explicit DelugeClient(QObject *parent = 0);
    Q_INVOKABLE QVariantList getTorrents() const;
signals:
    
public slots:
    
};

#endif // DELUGECLIENT_H
