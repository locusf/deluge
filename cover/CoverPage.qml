import QtQuick 1.1
import Sailfish.Silica 1.0

Rectangle {
    anchors.fill: parent
    color: "blue"
    
    Label {
        id: label
        anchors.centerIn: parent
        text: "Deluge"
    }
    
    CoverActionList {
        id: coverAction
        
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
        }
        
        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
        }
    }
}


