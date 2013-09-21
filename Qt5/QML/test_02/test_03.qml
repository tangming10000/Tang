import QtQuick 2.0

Rectangle {
    color:"lightgray"
    width: 400
    height: 200

    property int animatedValue: 0
    SequentialAnimation on animatedValue {
        loops: Animation.Infinite
        PropertyAnimation{ to: 500;duration: 1000}
        PropertyAnimation{ to: 0;duration: 1000}
    }
    Text{
        anchors.centerIn:  parent
        text: animatedValue
    }



//    Rectangle {
//        color:"blue"
//        width : parent.width / 2
//        height: parent.height
//    }

//    Rectangle  {
//        color:"yellow"
//        width :parent.width / 2
//        height: parent.height
//        x:parent.width /2
//    }
}
