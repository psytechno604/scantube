import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Item {    
    property int unitIndex: 0

    signal clicked()

    width: parent.width
    height: parent.height

    Button {
        id: mybtn
        text: unitIndex//dataSource.getReceiverName(unitIndex)
        width: 40
        height: 40
        x: getBtnX(parent.width/2 - width/2, getRadius(unitIndex) * parent.width/2, getSector(unitIndex))
        y: getBtnY(parent.height/2 - height/2, getRadius(unitIndex) * parent.height/2, getSector(unitIndex))

        onClicked: {            
            appSettings.currentUnitIndex = unitIndex;
            dataSource.setCurrentUnitIndex(unitIndex);
            parent.clicked();
        }


               background: Rectangle {
                   implicitWidth: 100
                   implicitHeight: 25
                   border.width: parent.activeFocus ? 2 : 1
                   border.color: "#888"
                   radius: 4
                   color: parent.pressed ? "#ffffcc" : "#e3e3e3"
                   /*gradient: Gradient {
                       GradientStop { position: 0 ; color: control.pressed ? "#ffffcc" : "#eee" }
                       GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                   }*/
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
