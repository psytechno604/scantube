/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Dialogs 1.0

//![1]
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3

import QtCharts 2.2
import QtDataVisualization 1.2

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick.Scene3D 2.0

import com.example.Timeline3D 1.0

import QtQuick.Controls.Styles 1.4



Item {
    id: main
    width: 1024
    height: 768
    property alias checkBox_subtractZeroSignal: checkBox_subtractZeroSignal
    property alias column: column
    //property alias controlPanel: controlPanel
    property alias text1Text: text1.text
    property alias text2Text: text2.text
    property alias text3Text: text3.text
    property alias text4Text: text4.text
    property alias text5Text: text5.text
    property alias text6Text: text6.text
    property alias pbValue: progressBar.value
    Row {
        id: row
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        z: -1

        Item {
            id : leftcolumn
            width : 200
            height: parent.height

            ComboBox {
                id: comboBox
                x: 8
                y: 8
                width: 184
                height: 32
                model: myIPsListModel
                textRole: "text"
                onActivated: _intercom.setMyIP(currentText);
            }


            Button {
                id: button
                x: 8
                y: 46
                width: 85
                height: 40
                text: qsTr("Start")
                padding: 0
                rotation: 0
                enabled: true
                bottomPadding: 0
                topPadding: 0
                rightPadding: 0
                leftPadding: 0
                font.pointSize: 8
                checkable: false
                onClicked: _intercom.on();
            }
            Button {
                id: button1
                x: 114
                y: 46
                width: 78
                height: 40
                text: qsTr("Stop")
                font.pointSize: 8
                checkable: false
                onClicked: _intercom.off();
            }
            ComboBox {
                id: comboBox1
                x: 8
                y: 92
                width: 85
                height: 30
                model: ["1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024"]
                onActivated: _intercom.setAccumulation(currentText);
            }
            ComboBox {
                id: comboBox2
                x: 114
                y: 92
                width: 78
                height: 30
                model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"]
                onActivated: _intercom.setSpeed(currentText);
            }

            TextField {
                id: textField1
                x: 8
                y: 181
                width: 85
                height: 40
                text: dataSource.getChannelShift(0)
                selectByMouse: true
                MouseArea {
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.IBeamCursor
                    anchors.fill: parent
                }
                onTextChanged:{
                    dataSource.setChannelShift(0, text*1);
                }
            }
            TextField {
                id: textField2
                x: 114
                y: 181
                width: 78
                height: 40
                text: dataSource.getChannelShift(1)
                selectByMouse: true
                MouseArea {
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.IBeamCursor
                    anchors.fill: parent
                }
                onTextChanged:{
                    dataSource.setChannelShift(1, text*1);
                }
            }

            Button {
                id: button2
                x: 8
                y: 135
                width: 85
                height: 40
                text: qsTr("Load file...")
                font.pointSize: 8
                checkable: false
                rotation: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                enabled: true
                topPadding: 0
                padding: 0
                onClicked: fileDialog.open();
            }

            CheckBox {
                id: checkBox_subtractZeroSignal
                x: 6
                y: 227
                width: 144
                height: 40
                checked: dataSource.getSubtractZeroSignal();
                text: qsTr("subtract Zero signal")
                spacing: 5
                font.weight: Font.Normal
                onCheckedChanged: {
                    dataSource.setSubtractZeroSignal(checked);
                }
                /*style: CheckBoxStyle {
                    indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 3
                            border.color: control.activeFocus ? "darkblue" : "gray"
                            border.width: 1
                            Rectangle {
                                visible: control.checked
                                color: "#555"
                                border.color: "#333"
                                radius: 1
                                anchors.margins: 4
                                anchors.fill: parent
                            }
                    }
                }*/

            }

            CheckBox {
                id: checkBox_useFilter
                x: 6
                y: 267
                width: 144
                height: 40
                text: qsTr("use filter")
                checked: dataSource.getUseFilter()
                onCheckedChanged: {
                    dataSource.setUseFilter(checked);
                }
            }

            CheckBox {
                id: checkBoxUseFile
                x: 86
                y: 135
                width: 46
                height: 40
                checked: _interfaceHelper.getIntValue("checkBoxUseFile");
                onCheckedChanged: {
                    _interfaceHelper.setValue("checkBoxUseFile", checked * 1);
                }
            }

            TextField {
                id: textField3
                x: 130
                y: 135
                width: 62
                height: 40
                text: qsTr("")
                selectByMouse: true
                MouseArea {
                    anchors.leftMargin: 0
                    anchors.bottomMargin: 0
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.IBeamCursor
                    anchors.rightMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                }
                onTextChanged: {
                    dataSource.showByIndex(text*1.0);
                }
            }

            Slider {
                id: slider1
                x: 0
                y: 323
                value: 0.5
            }

            ListView {
                id: listView
                x: 0
                y: 363
                width: 200
                height: parent.height-y
                keyNavigationWraps: false
                highlightFollowsCurrentItem: false
                boundsBehavior: Flickable.StopAtBounds
                highlightRangeMode: ListView.NoHighlightRange
                snapMode: ListView.NoSnap
                contentHeight: 0
                rotation: 0
                flickableDirection: Flickable.VerticalFlick
                contentWidth: 0
                spacing: 0
                orientation: ListView.Vertical
                model: measurementModel
                clip: true
                delegate: Item {
                    x: 5
                    width: 192
                    height: 20
                    Row {
                        id: row1
                        /*Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }*/

                        Text {
                            property variant data: model
                            text: model.text
                            font.bold: true
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        spacing: 5
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index;
                            dataSource.showByIndex(listView.currentIndex);
                        }
                    }

                }
                focus: true
                onCurrentItemChanged: {
                    dataSource.showByIndex(listView.currentIndex);
                }

            }

        }

        /*
        ControlPanel {
            id: controlPanel
            width: 200
            height: parent.height
            spacing: 10.2
            //anchors.bottomMargin: 0
            //anchors.top: parent.top
            //anchors.topMargin: 20
            //anchors.bottom: parent.bottom
            //anchors.left: parent.left
            //anchors.leftMargin: 8
            //![1]

            onSignalSourceChanged: {
                if (source == "sin")
                    dataSource.generateData(0, signalCount, sampleCount);
                else
                    dataSource.generateData(1, signalCount, sampleCount);
                scopeView.axisX().max = sampleCount;
            }
            onSeriesTypeChanged: scopeView.changeSeriesType(type);
            onRefreshRateChanged: scopeView.changeRefreshRate(rate);
            onAntialiasingEnabled: scopeView.antialiasing = enabled;
            onOpenGlChanged: {
                scopeView.openGL = enabled;
            }
            onSendOn: _intercom.on();
            onSendOff: _intercom.off();
            onSelectMyIP: _intercom.setMyIP(ip);
            onSelectAccumulation: _intercom.setAccumulation(acc);
            onSelectSpeed: _intercom.setSpeed(spd);
        }*/

        Column {
            id: column
            x: 0
            width: parent.width-200
            height: parent.height
            z: -1

            Item {
                id: item1
                y: 0
                width: parent.width
                height: parent.height-200


            Column {
                width: parent.width
                height: parent.height
                TabBar {
                    id: tabBar
                    width: parent.width
                    TabButton {
                            text: qsTr("Signal waveform")
                        }
                    TabButton {
                            text: qsTr("Distances diagram")
                        }
                    TabButton {
                            text: qsTr("Timeline 3D")
                        }
                    TabButton {
                            text: qsTr("Timeline 3D Surface")
                        }
                }
                StackLayout {
                    width: parent.width
                    height: parent.height - tabBar.height
                    currentIndex: tabBar.currentIndex
                    Item {
                        id: signal_waveform_tab
                        width: parent.width
                        height: parent.height
                        ScopeView {
                            id: scopeView


                            anchors.fill: signal_waveform_tab

                            onOpenGLSupportedChanged: {
                                if (!openGLSupported) {
                                    controlPanel.openGLButton.enabled = false
                                    controlPanel.openGLButton.currentSelection = 0
                                }
                            }
                            MouseArea {
                                z: -1
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onPressed: {
                                    console.log("onPressed")
                                    if (mouse.button == Qt.LeftButton)
                                    {
                                        console.log("Left", width, height, parent.yMax, parent.yMin)
                                        _interfaceHelper.ScopeView_LB = 1;
                                    }
                                    else if (mouse.button == Qt.RightButton)
                                    {
                                        console.log("Right")
                                        _interfaceHelper.ScopeView_RB = 1;
                                    }
                                    _interfaceHelper.ScopeView_x0 = mouseX;
                                    _interfaceHelper.ScopeView_ymax0 = parent.yMax;
                                    _interfaceHelper.ScopeView_ymin0 = parent.yMin;
                                    _interfaceHelper.ScopeView_y2max0 = parent.y2Max;
                                    _interfaceHelper.ScopeView_y2min0 = parent.y2Min;
                                    _interfaceHelper.ScopeView_y0 = mouseY;

                                    _interfaceHelper.ScopeView_axisNum = (mouseX < width/2)?0:1;
                                }
                                onPositionChanged: {
                                    if (_interfaceHelper.ScopeView_RB){

                                        var k = (mouseY - _interfaceHelper.ScopeView_y0) / height;
                                        //if (_interfaceHelper.ScopeView_axisNum == 0)    {
                                        parent.yMax = _interfaceHelper.ScopeView_ymax0 * (1 + k);
                                        parent.yMin = _interfaceHelper.ScopeView_ymin0 * (1 + k);
                                        //}
                                        //if (_interfaceHelper.ScopeView_axisNum == 1)    {
                                        parent.y2Max = _interfaceHelper.ScopeView_y2max0 * (1 + k);
                                        parent.y2Min = _interfaceHelper.ScopeView_y2min0 * (1 + k);
                                        //}
                                        console.log (mouseX, mouseY);
                                    }

                                }
                                onReleased: {
                                    console.log("onReleased")
                                    if (mouse.button == Qt.LeftButton)
                                    {
                                        console.log("Left")
                                        _interfaceHelper.ScopeView_LB = 0;
                                    }
                                    else if (mouse.button == Qt.RightButton)
                                    {
                                        console.log("Right")
                                        _interfaceHelper.ScopeView_RB = 0;
                                    }
                                }
                            }
                        }
                    }
                    Item {
                        id: distances_diagram
                        width: parent.width
                        height: parent.height
                        PolarChartView {
                            title: "Tube edge"
                            anchors.fill: parent

                            antialiasing: true
                            ValueAxis {
                                    id: axisAngular
                                    min: 0
                                    max: 32
                                    tickCount: 33
                                    labelsVisible: false;
                                }

                                ValueAxis {
                                    id: axisRadial
                                    min: 0
                                    max: 6
                                    labelsVisible: false;
                                }

                                SplineSeries {
                                    id: series1
                                    axisAngular: axisAngular
                                    axisRadial: axisRadial
                                    pointsVisible: true
                                    pointLabelsVisible: true
                                }
                                Component.onCompleted: {
                                    for (var i = 0; i <= 32; i++) {
                                        if (i==0 || i==32)
                                            series1.append(i, 5.0);
                                        else
                                            series1.append(i, 5.0 + Math.random());
                                    }
                                }
                        }
                        Timer {
                            id: refreshTimer
                            interval: 1 / 60 * 1000 // 60 Hz
                            running: true
                            repeat: true
                            onTriggered: {
                                dataSource.updateDistances(series1);
                            }
                        }
                    }
                    Item {
                        id: timeline_3d
                        anchors.fill: parent
                        Scene3D {
                            anchors.fill: parent
                            focus: true
                            Timeline3D {
                                id: timeline_3d_object
                            }
                        }


                    }
                    Item {
                        id: timeline_3d_surface
                        anchors.fill: parent

                        Surface3D {
                            id: timeline_3d_surface_object
                            anchors.fill: parent
                            axisX.min: 0.0
                            axisX.max: 100.0
                            axisX.title: "Scan"
                            axisX.titleVisible: true

                            axisZ.min: 0.0
                            axisZ.max: 727.0
                            axisZ.title: "Distance"
                            axisZ.titleVisible: true

                            axisY.min: -1024
                            axisY.max: 1024
                            axisY.title: "Signal level"
                            axisY.titleVisible: true

                            horizontalAspectRatio: 2

                            shadowQuality: AbstractGraph3D.ShadowQualityMedium
                            selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndRow
                            scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft

                            theme: Theme3D {
                                type: Theme3D.ThemeStoneMoss
                                font.family: "STCaiyun"
                                font.pointSize: 35
                                colorStyle: Theme3D.ColorStyleRangeGradient
                               // baseGradients: [surfaceGradient]
                            }

                            Surface3DSeries {
                                id: surfaceSeries
                                flatShadingEnabled: false
                                drawMode: Surface3DSeries.DrawSurface
                            }

                            Component.onCompleted: {
                               dataSource.updateSurface3D(surfaceSeries);
                            }
                        }
                    }
                }
            }
            }

            Item {
                id: item2
                width: parent.width
                height: 200

                Text {
                    id: text1
                    x: 507
                    y: 76
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 88
                    font.pixelSize: 25
                }

                Text {
                    id: text2
                    x: 508
                    y: 118
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    z: 1
                    font.pixelSize: 25
                    anchors.bottomMargin: 46
                    anchors.bottom: parent.bottom
                }

                Text {
                    id: text3
                    x: 640
                    y: 76
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    font.pixelSize: 25
                    anchors.bottomMargin: 88
                    anchors.bottom: parent.bottom
                }

                Text {
                    id: text4
                    x: 640
                    y: 118
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    font.pixelSize: 25
                    anchors.bottomMargin: 46
                    anchors.bottom: parent.bottom
                }

                Text {
                    id: text5
                    x: 507
                    y: 34
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    font.pixelSize: 25
                    anchors.bottomMargin: 130
                    anchors.bottom: parent.bottom
                }

                Text {
                    id: text6
                    x: 640
                    y: 34
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    font.pixelSize: 25
                    anchors.bottomMargin: 130
                    anchors.bottom: parent.bottom
                }

                Button {
                    id: buttonFix
                    width: 80
                    height: 36
                    text: qsTr("Fix")
                    anchors.left: parent.left
                    anchors.leftMargin: 129
                    anchors.top: parent.top
                    anchors.topMargin: 76
                    onClicked: sendFix();
                }

                TextInput {
                    id: textInput
                    x: 23
                    y: 76
                    width: 100
                    height: 36
                    text: qsTr("18.0")
                    cursorVisible: true
                    echoMode: TextInput.Normal
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 21
                    selectByMouse: true
                    MouseArea {
                        anchors.rightMargin: 0
                        anchors.bottomMargin: -1
                        anchors.leftMargin: 0
                        anchors.topMargin: 1
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                }

                CheckBox {
                    id: checkBox1
                    x: 14
                    y: 30
                    width: 46
                    height: 40
                    text: qsTr("")
                    onCheckedChanged: {
                        if (checked) {
                            dataSource.start_recording(textField.text);
                        }
                    }
                }

                ProgressBar {
                    id: progressBar
                    x: 22
                    y: 13
                    width: 200
                    height: 11
                    to: 1
                    value: 0
                }

                TextField {
                    id: textField
                    x: 60
                    y: 30
                    width: 124
                    height: 40
                    text: qsTr("scantube")
                    selectByMouse: true
                    MouseArea {
                        anchors.rightMargin: -33
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        anchors.topMargin: 0
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                }

                CheckBox {
                    id: checkBox_saveAsZeroSignal
                    x: 14
                    y: 108
                    text: qsTr("Save as Zero signal")
                }

                Slider {
                    id: slider
                    x: 0
                    y: 160
                    width: parent.width
                    height: 36
                    stepSize: 1
                    to: 100
                    from: -100
                    value: 0
                    onValueChanged: {
                        _interfaceHelper.setValue("ReceiverShift", value);
                        textField4.text = Math.floor(value);
                    }
                }

                TextField {
                    id: textField4
                    x: 350
                    y: 118
                    width: 124
                    height: 40
                    text: slider.value
                    selectByMouse: true
                    MouseArea {
                        cursorShape: Qt.IBeamCursor
                        anchors.leftMargin: 0
                        anchors.rightMargin: -33
                        anchors.topMargin: 0
                        acceptedButtons: Qt.NoButton
                        anchors.fill: parent
                        anchors.bottomMargin: 0
                    }
                }

            }

        }


    }
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //folder: shortcuts.home
        onAccepted: {
            //C:\Users\RazumovSA\Source\Repos\ScanTube\qmloscilloscope

            console.log("You chose: " + fileDialog.fileUrl)

            dataSource.open_file(fileDialog.fileUrl.toString().replace("file:///", ""))
            //Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            //Qt.quit()
        }
        //Component.onCompleted: visible = true
    }
    function clearListElements(){

    }

    function addListElement(name, colorCode)   {
        listView.model.append({
                                  name: name,
                                  colorCode: colorCode
                              });
    }

    function setDistance(distance)  {
        textInput.text = distance;
    }

    function changeProgressBar(v) {
        pbValue = v*1
        return
    }
    function sendFix() {
        if (checkBox1.checked)
            dataSource.save_point(textInput.text*1, 20, checkBox_saveAsZeroSignal.checked);
    }
    function uncheck_checkBox_saveAsZeroSignal()    {
        checkBox_saveAsZeroSignal.checked = 0;
    }

    function changeText1(msg) {
        text1Text = msg
        return
    }
    function changeText2(msg) {
        text2Text = msg
        return
    }
    function changeText3(msg) {
        text3Text = msg
        return
    }
    function changeText4(msg) {
        text4Text = msg
        return
    }
    function changeText5(msg) {
        text5Text = msg
        return
    }
    function changeText6(msg) {
        text6Text = msg
        return
    }
    //![2]
    Component.onCompleted: {
        timeline_3d_object.connectDataSource(dataSource);
    }
}

