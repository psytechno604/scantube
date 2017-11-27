import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {    
    property int unitIndex: 0
    property real mainControlValue: 0.0
    property color unitColor: mybtn.pressed ? getColor(mainControlValue, appSettings.maxMainControlValue, 1, 1, 1, 1, 0, 0) :
                                              (appSettings.currentUnitIndex==unitIndex?getColor(mainControlValue, appSettings.maxMainControlValue, 1, 1, 1, 1, 1, 0.5):
                                                                                        getColor(mainControlValue, appSettings.maxMainControlValue, 1, 1, 1, 0, 0, 0))

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
            buttonOnCircleClicked(unitIndex);

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
                   Text {
                       z:2
                       y: 20
                       id: txt1
                       text: Number(mainControlValue).toLocaleString(Qt.locale(), 'f', 1)
                       transform: Rotation {
                           origin.x: txt1.width/2
                           origin.y: txt1.height/2
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
                            GradientStop { position: 0.1; color: unitColor }
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
    function getColor(a, b, r_0, g_0, b_0, r_1, g_1, b_1){
        if (a >b) {
            return Qt.rgba(r_1, g_1, b_1, 1);
        }

        return Qt.rgba((b-a)/b*r_0 + a/b*r_1, (b-a)/b*g_0 + a/b*g_1, (b-a)/b*b_0 + a/b*b_1, 1);
    }
}
