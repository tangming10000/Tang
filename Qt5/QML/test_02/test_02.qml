import QtQuick 2.0

Rectangle {
    width: 360
    height: 360
    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
    MouseArea {
        anchors.fill: parent
        anchors.centerIn:  parent
        onClicked: {
        //    Qt.quit();
            parent.color = "blue"
        }
    }
    focus:  true
    Keys.onPressed: {
        if( event.key == Qt.Key_Return )
        {
            color = "red"
            event.accepted = "true"
        }

        if( event.key == Qt.Key_Escape)
        {
            Qt.quit();
        }
    }
}

