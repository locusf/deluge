QT += declarative network

SOURCES += $$PWD/sailfishapplication.cpp \
    sailfishapplication/delugeclient.cpp
HEADERS += $$PWD/sailfishapplication.h \
    sailfishapplication/delugeclient.h
INCLUDEPATH += $$PWD /usr/include/python2.7

TARGETPATH = /opt/sdk/bin
target.path = $$TARGETPATH

DEPLOYMENT_PATH = /opt/sdk/share/$$TARGET
qml.path = $$DEPLOYMENT_PATH
python.path = $$DEPLOYMENT_PATH
desktop.path = /opt/sdk/share/applications
LIBS += -lz -lboost_python -lpython2.7
contains(CONFIG, desktop) {
    DEFINES *= DESKTOP
    QT += opengl
}

INSTALLS += target qml desktop python

DEFINES += DEPLOYMENT_PATH=\"\\\"\"$${DEPLOYMENT_PATH}/\"\\\"\"

CONFIG += link_pkgconfig

packagesExist(qdeclarative-boostable) {
    message("Building with qdeclarative-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative-boostable
} else {
    warning("qdeclarative-boostable not available; startup times will be slower")
}




