import QtQuick 2.0
import QtQuick.Controls 2.2

Item {    
    property int unitIndex: 0

    signal clicked()

    width: parent.width
    height: parent.height

    Button {
        id: mybtn
        text: dataSource.getReceiverName(unitIndex)
        width: 40
        height: 40
        x: getBtnX(parent.width/2 - width/2, getRadius(unitIndex) * parent.width/2, getSector(unitIndex))
        y: getBtnY(parent.height/2 - height/2, getRadius(unitIndex) * parent.height/2, getSector(unitIndex))
        onClicked: {            
            dataSource.setCurrentUnitIndex(unitIndex);
            parent.clicked();

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
