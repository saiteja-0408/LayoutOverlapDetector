import QtQuick 2.15
import QtQuick.Controls 2.15
ApplicationWindow{
    id:root; visible:true; width:800;height:600; title:"Layout Overlap Detector"
    Item{
        anchors.fill: parent
        Repeater{
            model: layoutModel
            Rectangle{
                 width:  rectW
                 height: rectH
                 x:      rectX
                 y:      rectY
                color: overlaps? "red":"#dddddd"
                border.color: "black"; radius:2
            }
        }
    }
    Button{
        text:"Detect Overlaps"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom; anchors.bottomMargin:10
        onClicked: controller.onDetectClicked()
    }
}
