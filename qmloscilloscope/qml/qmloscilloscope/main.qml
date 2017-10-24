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

import QtQuick 2.2
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

//import QtQuick.Controls.Styles 1.4

import Qt.labs.settings 1.0


Item {
    id: main
    width: 1024
    height: 768
    property alias checkBox_subtractZeroSignal: checkBox_subtractZeroSignal

    signal textChanged(string msg);
    //property alias column: column

    //color: "#ffffff"
    z:-1

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
                y: 54
                width: 124
                height: 32
                model: myIPsListModel
                textRole: "text"
                onActivated: _intercom.setMyIP(currentText);
            }


            Button {
                id: button
                x: 8
                y: 8
                width: 53
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
                x: 107
                y: 92
                width: 85
                height: 30
                model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"]
                onActivated: _intercom.setSpeed(currentText);
            }
            Text {
                id: text7
                x: 8
                y: 126
                text: qsTr("Scan time:")
                font.pixelSize: 12
            }

            Text {
                id: text_dataReceiveTimeElapsed
                x: 71
                y: 126
                width: 53
                height: 27
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }

            Text {
                id: text9
                x: 130
                y: 128
                text: qsTr("ms.")
                font.pixelSize: 12
            }

            Text {
                id: text10
                x: 8
                y: 159
                text: qsTr("Timeout:")
                font.pixelSize: 12
            }




            SpinBox {
                id: spinBox_dataReceiveTimeout
                x: 67
                y: 159
                width: 109
                height: 40
                value: appSettings.dataReceiveTimeout
                from: 10
                to: 1000
                /*editable: true
                valueFromText: function(text, locale) { return Number.fromLocaleString(locale, text); }*/
                onValueChanged: _intercom.setTimeout(value);
            }

            Text {
                id: text8
                x: 8
                y: 203
                text: qsTr("Packets received:")
                font.pixelSize: 12
            }



            Text {
                id: text11
                x: 8
                y: 219
                text: qsTr("ip 1:")
                font.pixelSize: 12
            }

            Text {
                id: text_packetsReceived1
                x: 8
                y: 239
                width: 26
                height: 25
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }







            Text {
                id: text12
                x: 39
                y: 219
                width: 25
                height: 0
                text: qsTr("ip 2:")
                font.pixelSize: 12
            }

            Text {
                id: text_packetsReceived2
                x: 39
                y: 239
                width: 26
                height: 27
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24

            }






            Text {
                id: text13
                x: 72
                y: 219
                text: qsTr("ip 3:")
                font.pixelSize: 12
            }

            Text {
                id: text_packetsReceived3
                x: 71
                y: 239
                width: 26
                height: 25
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }






            Text {
                id: text14
                x: 103
                y: 219
                text: qsTr("ip 4:")
                font.pixelSize: 12
            }

            Text {
                id: text_packetsReceived4
                x: 103
                y: 239
                width: 26
                height: 30
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }






            Button {
                id: button2
                x: 8
                y: 438
                width: 77
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
                x: 91
                y: 336
                width: 65
                height: 40
                checked: dataSource.getSubtractZeroSignal();
                text: qsTr("use")
                autoExclusive: false
                spacing: 5
                font.weight: Font.Normal
                onCheckedChanged: {
                    dataSource.setSubtractZeroSignal(checked);
                    updateSingleWaveform();
                }
            }



            CheckBox {
                id: checkBoxUseFile
                x: 91
                y: 438
                width: 33
                height: 40
                checked: _interfaceHelper.getIntValue("checkBoxUseFile");
                onCheckedChanged: {
                    _interfaceHelper.setValue("checkBoxUseFile", checked * 1);
                }
            }



            TextField {
                id: textField3
                x: 130
                y: 438
                width: 62
                height: 40
                text: qsTr("0")
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
                    main.textChanged("zeroIndex=" + text);
                }
            }



            ListView {
                id: listView
                x: 8
                y: 484
                width: 185
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



            Button {
                id: button3
                x: 67
                y: 8
                width: 55
                height: 40
                text: qsTr("Scan")
                checked: false
                checkable: true
                font.pointSize: 8
                onClicked: {
                    if (checked)    {
                        _intercom.setContinueScan(true);
                        //_intercom.sendScan();
                    }
                    else {
                        _intercom.setContinueScan(false);
                    }
                }
            }



            Button {
                id: button4
                x: 138
                y: 54
                width: 54
                height: 32
                text: qsTr("Test")
                checkable: false
                checked: false
                font.pointSize: 8
                onClicked: _intercom.sendTest();
            }



            Button {
                id: button5
                x: 8
                y: 336
                width: 82
                height: 40
                text: qsTr("Save signal 0")
                checkable: false
                checked: false
                font.pointSize: 8
                onClicked: dataSource.saveAsScanData0();
            }



            Button {
                id: button6
                x: 138
                y: 8
                width: 55
                height: 40
                text: qsTr("Plot")
                checked: false
                checkable: false
                font.pointSize: 8
                onClicked: {
                   //dataSource.updateAllWaveforms(series_0_0, 0);
                    //dataSource.updateAllWaveforms(series_0_1, 1);
                    timeline_3d_unit.addScan();
                    dataSource.calcDistances();
                    updateSingleWaveform();
                    dataSource.updateSurface3D(surfaceSeries);
                }
            }



            Button {
                id: button7
                x: 148
                y: 336
                width: 45
                height: 40
                text: qsTr("Save")
                checked: false
                checkable: false
                font.pointSize: 8
                onClicked: {
                    main.textChanged("Save history");
                }
            }


            Text {
                id: text_betweenScansTimeElapsed
                x: 92
                y: 275
                width: 53
                height: 27
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }

            Text {
                id: text15
                x: 8
                y: 272
                text: qsTr("between scans:")
                font.pixelSize: 12
            }

            Text {
                id: text16
                x: 8
                y: 308
                text: qsTr("readData:")
                font.pixelSize: 12
            }

            Text {
                id: text_readDataTimeElapsed
                x: 92
                y: 308
                width: 53
                height: 27
                color: "#ffffff"
                text: qsTr("0")
                font.pixelSize: 24
            }

            Button {
                id: button_saveDistance0
                x: 8
                y: 392
                width: 184
                height: 40
                text: qsTr("Save distance 0")
                checkable: false
                checked: false
                font.pointSize: 8
                onClicked: {
                    dataSource.setSaveDistance0(true);

                }
            }

        }



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

                    //SL
                    Column {
                        anchors.fill: parent
                        TabBar {
                            id: tabBar
                            width: parent.width
                            currentIndex: 0
                            TabButton {
                                text: qsTr("Single waveform")
                            }
                            TabButton {
                                text: qsTr("All waveforms")
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
                        Row {
                            id: row_selectReceiver
                            ComboBox {
                                id: comboBox_selectIP
                                model: ["1", "2", "3", "4"]
                                currentIndex: model.indexOf(dataSource.getIP())
                                onActivated: {
                                    dataSource.selectIP(currentText);
                                    updateSingleWaveform();
                                }
                            }
                            ComboBox {
                                id: comboBox_selectEmitter
                                model: ["0", "1", "2", "3", "4", "5", "6", "7"]
                                currentIndex: model.indexOf(dataSource.getEmitter())
                                onActivated: {
                                    dataSource.selectEmitter(currentText);
                                    updateSingleWaveform();
                                }
                            }
                            ComboBox {
                                id: comboBox_selectRow
                                model: ["0", "1"]
                                currentIndex: model.indexOf(dataSource.getRow())
                                onActivated: {
                                    dataSource.selectRow(currentText);
                                    updateSingleWaveform();
                                }
                            }
                            Button {
                                id: button_selectOnCircle
                                text: qsTr("Select...")
                                onClicked: {
                                    rect_circleSelector.visible = true;
                                }
                            }

                            Text {
                                id: single_waveform_distance
                                text: qsTr("---")
                                font.pixelSize: 12
                                color: "white"

                            }
                            Timer {
                                id: refreshTimer
                                interval: 1 / 60 * 1000 // 60 Hz
                                running: true
                                repeat: true
                                onTriggered: {
                                    setSingleWaveformDistanceText(dataSource.getCurrentDistance());
                                }
                            }
                        }
                        StackLayout {
                            width: parent.width
                            height: parent.height - tabBar.height
                            currentIndex: tabBar.currentIndex
                            Item {
                                id: single_waveform_tab
                                width: parent.width
                                height: parent.height
                                Column {
                                    anchors.fill: parent

                                    ScopeView {
                                        id: scopeView


                                        width: parent.width
                                        height: parent.height - row_selectReceiver.height
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
                            }
                            Item {
                                id: all_waveforms
                                width: parent.width
                                height: parent.height
                                Row {
                                    width: parent.width
                                    height: parent.height
                                    PolarChartView {
                                        height: parent.height
                                        width: parent.width / 2
                                        antialiasing: true
                                        ValueAxis {
                                            id: axisAngular_0_0
                                            min: 0
                                            max: 32 * 20
                                            tickCount: 33
                                            labelsVisible: false;
                                        }

                                        ValueAxis {
                                            id: axisRadial_0_0
                                            min: 0
                                            max: 2500
                                            labelsVisible: false;
                                        }
                                        SplineSeries {
                                            name: "Set 0"
                                            id: series_0_0
                                            axisAngular: axisAngular_0_0
                                            axisRadial: axisRadial_0_0
                                            pointsVisible: false
                                            pointLabelsVisible: false
                                            width: 2
                                            color: "red"
                                            onPointsReplaced: {

                                            }
                                        }

                                        SplineSeries {
                                            name: "Set 1"
                                            id: series_0_1
                                            axisAngular: axisAngular_0_0
                                            axisRadial: axisRadial_0_0
                                            pointsVisible: false
                                            pointLabelsVisible: false
                                            width: 2
                                            color: "blue"
                                        }
                                    }
                                    PolarChartView {
                                        height: parent.height
                                        width: parent.width / 2
                                        antialiasing: true
                                        ValueAxis {
                                            id: axisAngular_1_0
                                            min: 0
                                            max: 32
                                            tickCount: 33
                                            labelsVisible: true;
                                        }

                                        ValueAxis {
                                            id: axisRadial_1_0
                                            min: -500
                                            max: 2500
                                            labelsVisible: true;
                                            tickCount: 7
                                        }
                                        ScatterSeries {
                                            name: "1st"
                                            id: series_1_0
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false


                                            color: "red"
                                            opacity: 0.5
                                        }
                                        ScatterSeries {
                                            name: "2nd"
                                            id: series_1_1
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false

                                            color: "red"
                                            opacity: 0.25
                                        }
                                        ScatterSeries {
                                            name: "3rd"
                                            id: series_1_2
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false

                                            color: "red"
                                            opacity: 0.15
                                        }

                                        ScatterSeries {
                                            name: "1st"
                                            id: series_1_3
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false

                                            color: "blue"
                                            opacity: 0.5
                                        }
                                        ScatterSeries {
                                            name: "2nd"
                                            id: series_1_4
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false

                                            color: "blue"
                                            opacity: 0.25
                                        }
                                        ScatterSeries {
                                            name: "3rd"
                                            id: series_1_5
                                            axisAngular: axisAngular_1_0
                                            axisRadial: axisRadial_1_0
                                            pointsVisible: true
                                            pointLabelsVisible: false

                                            color: "blue"
                                            opacity: 0.15
                                        }
                                        Component.onCompleted: {
                                            dataSource.setDistanceSeries(series_1_0, 0);
                                            dataSource.setDistanceSeries(series_1_1, 1);
                                            dataSource.setDistanceSeries(series_1_2, 2);

                                            dataSource.setDistanceSeries(series_1_3, 3);
                                            dataSource.setDistanceSeries(series_1_4, 4);
                                            dataSource.setDistanceSeries(series_1_5, 5);
                                        }
                                    }
                                }

                            }
                            Item {
                                id: distances_diagram
                                width: parent.width
                                height: parent.height
                                Row {
                                    width: parent.width
                                    height: parent.height

                                }
                            }

                            Item {
                                id: timeline_3d
                                width: parent.width
                                height: parent.height

                                Scene3D {
                                    anchors.fill: parent
                                    focus: true

                                    Timeline3D {
                                        id: timeline_3d_unit
                                    }
                                }

                            }
                            Item {
                                id: timeline_3d_surface
                                anchors.fill: parent
                                ColorGradient {
                                    id: surfaceGradient
                                    ColorGradientStop { position: 0.0; color: "darkslategray" }
                                    ColorGradientStop { id: middleGradient; position: 0.25; color: "peru" }
                                    ColorGradientStop { position: 0.75; color: "red" }
                                }
                                Surface3D {
                                    id: timeline_3d_surface_object
                                    anchors.fill: parent

                                    theme: Theme3D {
                                        type: Theme3D.ThemeStoneMoss
                                        font.family: "STCaiyun"
                                        font.pointSize: 35
                                        colorStyle: Theme3D.ColorStyleRangeGradient
                                        baseGradients: [surfaceGradient]
                                    }
                                    axisX:  {
                                        axisX.min = 0.0
                                        axisX.max= 100.0
                                        axisX.title = "Scan"
                                        axisX.titleVisible = true
                                    }

                                    axisZ: {
                                        axisZ.min= 0.0
                                        axisZ.max= 2500.0
                                        axisZ.title = "Distance"
                                        axisZ.titleVisible = true
                                    }
                                    axisY: {
                                        axisY.min = -500
                                        axisY.max = 500
                                        axisY.title = "Signal level"
                                        axisY.titleVisible = true
                                    }

                                    horizontalAspectRatio: 0.5

                                    shadowQuality: AbstractGraph3D.ShadowQualityMedium
                                    selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndRow
                                    //scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft



                                    Surface3DSeries {
                                        id: surfaceSeries
                                        flatShadingEnabled: false
                                        drawMode: Surface3DSeries.DrawSurface
                                    }

                                    Component.onCompleted: {
                                        dataSource.updateSurface3D(surfaceSeries);
                                        //dataSource.setAllWaveformsSeries(series_0_0, 0);
                                        //dataSource.setAllWaveformsSeries(series_0_1, 1);

                                    }
                                }
                            }
                        }
                        //SL
                    }


                }

            }

            Item {
                id: item2
                width: parent.width
                height: 200

                Slider {
                    id: slider1
                    x: 0
                    y: 0
                    width: parent.width - 500
                    height: 36
                    stepSize: 1
                    to: 3100
                    from: 0
                    value: appSettings.slider1_value
                    onValueChanged: {
                        _intercom.sendShift(value, 1);
                        textField_slider1.text = Math.floor(value);
                    }
                }

                TextField {
                    id: textField_slider1
                    x: slider1.x + slider1.width+5
                    y: slider1.y
                    width: 82
                    height: 36
                    text: slider1.value
                    selectByMouse: true
                    onAccepted: {
                        slider1.value = text
                    }

                    MouseArea {
                        cursorShape: Qt.IBeamCursor
                        anchors.leftMargin: 0
                        anchors.rightMargin: 0
                        anchors.topMargin: 0
                        acceptedButtons: Qt.NoButton
                        anchors.fill: parent
                        anchors.bottomMargin: 0
                    }
                }

                Slider {
                    id: slider2
                    x: 0
                    y: slider1.y + slider1.height
                    width: slider1.width
                    height: 36
                    from: 0
                    to: 3100
                    stepSize: 1
                    value: appSettings.slider2_value
                    onValueChanged: {
                        _intercom.sendShift(value, 2);
                        textField_slider2.text = Math.floor(value);
                    }
                }

                TextField {
                    id: textField_slider2
                    x: slider2.x + slider2.width+5
                    y: slider2.y
                    width: 82
                    height: 36
                    text: slider2.value
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.bottomMargin: 0
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        anchors.fill: parent
                        anchors.leftMargin: 0
                        acceptedButtons: Qt.NoButton
                    }
                    selectByMouse: true
                }

                Slider {
                    id: slider3
                    x: 0
                    y: slider2.y + slider2.height
                    width: slider1.width
                    height: 36
                    from: 0
                    to: 3100
                    value: appSettings.slider3_value
                    stepSize: 1
                    onValueChanged: {
                        _intercom.sendShift(value, 3);
                        textField_slider3.text = Math.floor(value);
                    }
                }

                TextField {
                    id: textField_slider3
                    x: slider3.x + slider3.width+5
                    y: slider3.y
                    width: 82
                    height: 36
                    text: slider3.value
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.bottomMargin: 0
                        anchors.rightMargin: 0
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        anchors.leftMargin: 0
                        acceptedButtons: Qt.NoButton
                    }
                    selectByMouse: true
                }

                Slider {
                    id: slider4
                    x: 0
                    y: slider3.y + slider3.height
                    width: slider1.width
                    height: 36
                    from: 0
                    to: 3100
                    stepSize: 1
                    value: appSettings.slider4_value
                    onValueChanged: {
                        _intercom.sendShift(value, 4);
                        textField_slider4.text = Math.floor(value);
                    }
                }

                TextField {
                    id: textField_slider4
                    x: slider4.x + slider4.width+5
                    y: slider4.y
                    width: 82
                    height: 36
                    text: slider4.value
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.bottomMargin: 0
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        anchors.fill: parent
                        anchors.leftMargin: 0
                        acceptedButtons: Qt.NoButton
                    }
                    selectByMouse: true
                }
                Button {
                    id: button_sendShifts
                    x: textField_slider4.x
                    y: textField_slider4.y + textField_slider4.height+5
                    width: 55
                    height: 40
                    text: qsTr("Resend")
                    checked: false
                    checkable: false
                    font.pointSize: 8
                    onClicked: _intercom.sendShift(slider1.value, slider2.value, slider3.value, slider4.value);
                }

                TextField {
                    id: textField_slider1_level
                    x: textField_slider1.x + textField_slider1.width + 10
                    y: textField_slider1.y
                    width: 82
                    height: 36
                    selectByMouse: true
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.fill: parent
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                    }
                }
                TextField {
                    id: textField_slider2_level
                    x: textField_slider2.x + textField_slider2.width + 10
                    y: textField_slider2.y
                    width: 82
                    height: 36
                    selectByMouse: true
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.fill: parent
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                    }
                }
                TextField {
                    id: textField_slider3_level
                    x: textField_slider3.x + textField_slider3.width + 10
                    y: textField_slider3.y
                    width: 82
                    height: 36
                    selectByMouse: true
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.fill: parent
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                    }
                }
                TextField {
                    id: textField_slider4_level
                    x: textField_slider4.x + textField_slider4.width + 10
                    y: textField_slider4.y
                    width: 82
                    height: 36
                    selectByMouse: true
                    MouseArea {
                        anchors.topMargin: 0
                        anchors.fill: parent
                        anchors.rightMargin: 0
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                    }
                }

                Button {
                    id: button_scanRange
                    x: textField_slider4_level.x
                    y: button_sendShifts.y
                    width: 55
                    height: 40
                    text: qsTr("Scan range")
                    checked: false
                    checkable: false
                    font.pointSize: 8
                    onClicked: {
                        _intercom.scanRange();
                    }
                }
/*
                double fc{1e+9};
                double deltaf;
                unsigned short ford {8};
                double Td {1e-9};*/
                TextField {
                    id: textField_fc
                    x: textField_slider4_level.x + textField_slider4_level.width + 10
                    y: textField_slider1_level.y
                    width: 83
                    height: 40
                    text: appSettings.fc
                    selectByMouse: true

                    onEditingFinished: {
                        main.textChanged("fc="+textField_fc.text);
                    }
                }
                TextField {
                    id: textField_deltaf
                    x: textField_fc.x
                    y:  textField_slider2_level.y
                    width: 83
                    height: 40
                    text: appSettings.deltaf
                    selectByMouse: true

                    onEditingFinished: {
                        main.textChanged("deltaf="+textField_deltaf.text);
                    }
                }
                TextField {
                    id: textField_ford
                    x: textField_fc.x
                    y:  textField_slider3_level.y
                    width: 83
                    height: 40
                    text: appSettings.ford
                    selectByMouse: true
                    onEditingFinished: {
                        main.textChanged("ford="+textField_ford.text);
                    }
                }
                TextField {
                    id: textField_Td
                    x: textField_fc.x
                    y:  textField_slider4_level.y
                    width: 83
                    height: 40
                    text: appSettings.td
                    selectByMouse: true

                    onEditingFinished: {
                        main.textChanged("Td="+textField_Td.text);
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

            dataSource.loadHistoryFromFile(fileDialog.fileUrl.toString().replace("file:///", ""))
            //Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            //Qt.quit()
        }
        //Component.onCompleted: visible = true
    }

    //    Timer {
    //        id: startPlotTimer
    //        interval: scanTimer.interval / 2.0
    //        triggeredOnStart: false
    //        running: false
    //        repeat: false
    //        onTriggered: {
    //            plotTimer.running = true;
    //        }
    //    }

    //    Timer {
    //        id: scanTimer
    //        interval: 500
    //        running: false
    //        repeat: true
    //        onTriggered: {
    //            _intercom.sendScan();
    //            startPlotTimer.running = true;
    //        }
    //    }



    function setSingleWaveformDistanceText (distance) {
        single_waveform_distance.text = "Distance = " + distance;
    }
    function updateAllWaveforms (){
        //dataSource.updateAllWaveforms(series_0_0, 0);
        //dataSource.updateAllWaveforms(series_0_1, 1);
    }
    function updateDistances (){
        /*dataSource.updateDistances(series_1_0, 0);
        dataSource.updateDistances(series_1_1, 1);
        dataSource.updateDistances(series_1_2, 2);*/
        var fix = 8;
        textField_slider1_level.text = dataSource.getReceiverLevel(0).toFixed(0);
        textField_slider2_level.text = dataSource.getReceiverLevel(1).toFixed(0);
        textField_slider3_level.text = dataSource.getReceiverLevel(2).toFixed(0);
        textField_slider4_level.text = dataSource.getReceiverLevel(3).toFixed(0);
    }

    function updateSingleWaveform() {
        dataSource.update(scopeView.lineSeries1);
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

    function setPacketsReceived(v1, v2, v3, v4) {
        text_packetsReceived1.text = v1===0?"":v1;
        text_packetsReceived2.text = v2===0?"":v2;
        text_packetsReceived3.text = v3===0?"":v3;
        text_packetsReceived4.text = v4===0?"":v4;
    }
    /*function setReceiverLevels(l1, l2, l3, l4) {
        textField_slider1_level.text = l1;
        textField_slider2_level.text = l2;
        textField_slider3_level.text = l3;
        textField_slider4_level.text = l4;
    }*/
    function setDataReceiveTimeElapsed(elapsed) {
        text_dataReceiveTimeElapsed.text = elapsed;
    }
    function setBetweenScansTimeElapsed(elapsed) {
        text_betweenScansTimeElapsed.text = elapsed;
    }
    function setReadDataTimeElapsed(elapsed) {
        text_readDataTimeElapsed.text = elapsed;
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

    function setFilterValues () {
        main.textChanged("fc="+textField_fc.text);
        main.textChanged("deltaf="+textField_deltaf.text);
        main.textChanged("ford="+textField_ford.text);
        main.textChanged("Td="+textField_Td.text);
    }

    //![2]
    Component.onCompleted: {
        //timeline_3d_object.connectDataSource(dataSource);
        _intercom.setTimeout(spinBox_dataReceiveTimeout.value);
        setSingleWaveformDistanceText(0);
        setPacketsReceived(0, 0, 0, 0);




    }
    Settings {
        id: appSettings
        property alias dataReceiveTimeout: spinBox_dataReceiveTimeout.value
        property alias slider1_value: slider1.value
        property alias slider2_value: slider2.value
        property alias slider3_value: slider3.value
        property alias slider4_value: slider4.value

        property alias fc: textField_fc.text
        property alias deltaf: textField_deltaf.text
        property alias ford: textField_ford.text
        property alias td: textField_Td.text
    }
    Rectangle {
        id: rect_circleSelector
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2
        width: 768
        height: 768
        visible: false
        z: 100


        STReceiverWidget {
            unitIndex: 0
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 1
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 2
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 3
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 4
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 5
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 6
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 7
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 8
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 9
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 10
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 11
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 12
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 13
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 14
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 15
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 16
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 17
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 18
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 19
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 20
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 21
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 22
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 23
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 24
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 25
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 26
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 27
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 28
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 29
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 30
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 31
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 32
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 33
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 34
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 35
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 36
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 37
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 38
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 39
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 40
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 41
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 42
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 43
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 44
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 45
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 46
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 47
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 48
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 49
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 50
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 51
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 52
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 53
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 54
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 55
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 56
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 57
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 58
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 59
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 60
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 61
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 62
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
        STReceiverWidget {
            unitIndex: 63
            onClicked: {
                buttonOnCircleClicked(unitIndex)
            }
        }
    }
    function buttonOnCircleClicked(index) {
        rect_circleSelector.visible = false;
        dataSource.setUnitIndex(index);

        comboBox_selectIP.currentIndex = comboBox_selectIP.model.indexOf(dataSource.getIP());
        //dataSource.selectIP(ip);
        comboBox_selectEmitter.currentIndex = comboBox_selectEmitter.model.indexOf(dataSource.getEmitter());
        //dataSource.selectEmitter(emitter);
        comboBox_selectRow.currentIndex = comboBox_selectRow.model.indexOf(dataSource.getRow());
        //dataSource.selectRow(row);*/

        updateSingleWaveform();
    }
    function getBtnX(x0, R, N) {
        return x0 + R * Math.sin(Math.PI / 32 + N * Math.PI / 16);
    }
    function getBtnY(y0, R, N) {
        return y0 - R * Math.cos(Math.PI / 32 + N * Math.PI / 16);
    }

    function setSliders(v1, v2, v3, v4) {
        slider1.value = v1;
        slider2.value = v2;
        slider3.value = v3;
        slider4.value = v4;
    }
}

