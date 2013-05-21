import QtQuick 1.1
import Sailfish.Silica 1.0

Page {
    id: page
    SilicaFlickable {
        id: listView
        anchors.fill: parent
        contentHeight: childrenRect.height
        Column {
            id: column
            spacing: 10
            x: theme.paddingLarge
            width: parent.width - 2*theme.paddingLarge
            PageHeader {
                title: "Settings"
            }
            Label {
                text: "Server"
            }
            TextField {
                width: parent.width
                placeholderText: "server.local.com"
                focus: true
            }
            Label {
                text: "Port"
            }
            TextField {
                width: parent.width
                placeholderText: "58846"
                inputMethodHints: Qt.ImhDialableCharactersOnly
                focus: true
            }
            Label {
                text: "User"
            }
            TextField {
                width: parent.width
                placeholderText: ""
                focus: true
            }
            Label {
                text: "Password"
            }
            TextField {
                echoMode: TextInput.Password
                width: parent.width
                placeholderText: ""
                focus: true
            }
        }
    }
}





