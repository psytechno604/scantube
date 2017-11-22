import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {    
    property int unitIndex: 0
    property color unitColor: mybtn.pressed ? "#ff0000" : (appSettings.currentUnitIndex==unitIndex?"#ffffcc":"#888")

    signal clicked()

    width: parent.width
    height: parent.height

    Button {
        id: mybtn
        //text: "unitIndex"//dataSource.getReceiverName(unitIndex)
        width: 40
        height: 40
        x: getBtnX(parent.width/2 - width/2, getRadius(unitIndex) * parent.width/2, getSector(unitIndex))
        y: getBtnY(parent.height/2 - height/2, getRadius(unitIndex) * parent.height/2, getSector(unitIndex))

        transform: Rotation {
            origin.x: mybtn.width/2
            origin.y: mybtn.height/2
            angle: 360*(1 / 32 + getSector(unitIndex) / 16)/ (2)}

        onClicked: {            
            appSettings.currentUnitIndex = unitIndex;
            dataSource.setCurrentUnitIndex(unitIndex);
            parent.clicked();
            console.log(360*(1 / 32 + getSector(unitIndex) / 16)/ (2));
        }
               background: Rectangle {
                    id: rect0
                   implicitWidth: parent.width
                   implicitHeight: parent.height
                   /*border.width: parent.activeFocus ? 2 : 1
                   border.color:*/
                   radius: 4

                   Text {
                       z:2
                       id: txt0
                       text: unitIndex
                       transform: Rotation {
                           origin.x: txt0.width/2
                           origin.y: txt0.height/2
                           angle: -360*(1 / 32 + getSector(unitIndex) / 16)/ (2)
                       }
                   }

                   LinearGradient {
                        source: rect0
                       anchors.fill: parent
                       start: Qt.point(0, 0)
                       end: Qt.point(0, 300)
                       gradient: Gradient {
                            GradientStop { position: 0.0; color: "white" }
                            GradientStop { position: (unitIndex%3 + 1)/3; color: unitColor }
                       }
                   }
               }

    }
    function getBtnX(x0, R, N) {
        return x0 + R * Math.sin(Math.PI / 32 + N * Math.PI / 16);
    }
    function getBtnY(y0, R, N) {
        return y0 - R * Math.cos(Math.PI / 32 + N * Math.PI / 16);
    }
    function getSector(i)    {
        return Math.floor(i/2);
    }
    function getRadius(i) {
        return 0.8-0.2 * (i%2);
    }
}
