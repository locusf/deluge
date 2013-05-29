import QtQuick 1.1
import Sailfish.Silica 1.0

Page {
    id: page
    
    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {
        id: torrentsview
        model: ListModel {id: torrentsmodel}
        header: PageHeader {
            title: "Deluge"
        }
        ViewPlaceholder {
            enabled: torrentsview.count == 0
            text: "No torrents to display."
        }
        anchors.fill: parent
        delegate: BackgroundItem {
            Label {
                id: itemlabel
                x: theme.paddingLarge
                text: name
            }
        }
        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: "Refresh"
                onClicked: {
                    var tor = delugeClient.getTorrents()
                    torrentsmodel.append({name: tor[0]})
                    console.log(tor)
                }
            }

            MenuItem {
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }
        
        // Tell SilicaFlickable the height of its content.
        contentHeight: childrenRect.height

    }
}


