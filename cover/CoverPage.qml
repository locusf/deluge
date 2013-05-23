import QtQuick 1.1
import Sailfish.Silica 1.0

Rectangle {
    anchors.fill: parent
    color: "steelblue"
    
    Label {
        id: label
        anchors.centerIn: parent
        text: "Deluge"
    }
    
    CoverActionList {
        id: coverAction
    }
}


