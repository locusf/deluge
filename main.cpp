
#include <QApplication>
#include <QDeclarativeView>
#include <QSslConfiguration>
#include <QSslCipher>

#include "sailfishapplication.h"
#include "delugeclient.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    QSslConfiguration::setDefaultConfiguration(config);
    QScopedPointer<QApplication> app(Sailfish::createApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(Sailfish::createView("main.qml"));
    Sailfish::showView(view.data());
    
    return app->exec();
}


