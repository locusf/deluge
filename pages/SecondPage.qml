import QtQuick 1.1
import Sailfish.Silica 1.0

Page {
    id: page
    SilicaFlickable {
        id: listView
        anchors.fill: parent
        contentHeight: childrenRect.height
        Column {
            width: page.width
            spacing: theme.paddingLarge
            PageHeader {
                title: "Settings"
            }
            Row {
                SectionHeader {
                    text: "Server"
                }
                TextField {
                    width: parent.width
                    placeholderText: "server.local.com"
                    focus: true
                }
            }
        }
    }
}





