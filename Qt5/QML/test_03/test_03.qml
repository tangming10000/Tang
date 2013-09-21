import QtQuick 2.0

Rectangle {
    width: 200
    height: 300

    property var ww: width/2
    property var hh: height/3
    Column {
        Button{ width:ww; height:hh }
        Button{ width:ww; height:hh; x: ww; color:"blue" }
        Button{ width:ww; height:hh; radius: 8}
    }



}
