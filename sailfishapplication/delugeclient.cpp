#include "delugeclient.h"

DelugeClient::DelugeClient(QObject *parent) :
    QObject(parent)
{
}

QVariantList DelugeClient::getTorrents() const
{
    QVariantList torrents;
    torrents.append("FOO");
    return torrents;
}
