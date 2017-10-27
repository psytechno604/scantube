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

    property string backgroundColor: "#444444"
    property string outputTextColor: "yellow"
    property string staticTextColor: "red"
    //property string inputTextColor: "yellow"

    signal textChanged(string msg);

    z:-1

    Rectangle {
        anchors.fill: parent
        z: - 100
        color: backgroundColor
    }

    Row {
        id: row
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        z: -1

        Column {
            id : leftcolumn
            width : 200
            height: parent.height
            spacing: 5
            Row {
                spacing: 1
                Button {
                    id: buttonStart
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
                Button {
                    id: button_scanRange
                    width: 55
                    height: 40
                    text: qsTr("Scan range")
                    checked: false
                    checkable: false
                    font.pointSize: 8
                    onClicked: {
                        if (checkboxWriteHistory.checked)
                            _intercom.scanRange(textFieldFullscanCountdownStart.text);
                        else
                            _intercom.scanRange(1);
                    }
                }
                CheckBox {
                    id: checkboxWriteHistory

                    width: 33
                    height: 40
                    checked: appSettings.writeHistory;
                    onCheckedChanged: {
                        main.textChanged("write history=" + checked);
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Set write history or not");
                }
                TextField {
                    id: textFieldFullscanCountdownStart

                    width: 40
                    height: 40
                    text: qsTr("10")
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
                }
            }
            Row {
                spacing: 1
                ComboBox {
                    id: comboBoxAccumulation

                    width: 85
                    height: 30
                    model: ["1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024"]
                    onActivated: _intercom.setAccumulation(currentText);
                }
                ComboBox {
                    id: comboBoxSpeed

                    width: 85
                    height: 30
                    model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"]
                    onActivated: _intercom.setSpeed(currentText);
                }
            }
            Row {
                spacing: 1
                Button {
                    id: buttonLoadFile
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

                Button {
                    id: buttonSave

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
            }
            Row {
                spacing: 1
                CheckBox {
                    id: checkBox_subtractZeroSignal

                    checked: dataSource.getSubtractZeroSignal();
                    autoExclusive: false
                    spacing: 5
                    font.weight: Font.Normal
                    onCheckedChanged: {
                        dataSource.setSubtractZeroSignal(checked);
                        updateSingleWaveform();
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Subtract element " + textFieldZeroIndex.text)
                }
                TextField {
                    id: textFieldZeroIndex
                    width: 40
                    height: 40
                    text: appSettings.zeroIndex
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
                Button {
                    id: buttonPlot

                    width: 55
                    height: 40
                    text: qsTr("Plot")
                    checked: false
                    checkable: false
                    font.pointSize: 8
                    onClicked: {
                        //dataSource.updateAllWaveforms(series_0_0, 0);
                        //dataSource.updateAllWaveforms(series_0_1, 1);
                        /*timeline_3d_unit.addScan();
                        dataSource.calcDistances();
                        updateSingleWaveform();*/
                        dataSource.updateSurface3D(surfaceSeries);
                        dataSource.updateCorrelationChart(surfaceSeriesCorr);
                    }
                }



            }
            Row {
                spacing: 2
                Column {
                    Text {
                        id: text7_Scan_time
                        text: qsTr("Scan time:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_dataReceiveTimeElapsed
                        width: 53
                        height: 27
                        text: qsTr("0")
                        font.pixelSize: 24
                        color: outputTextColor
                    }

                }

                Column {
                    Text {
                        id: text15_between_scans_
                        text: qsTr("between scans:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_betweenScansTimeElapsed
                        width: 53
                        height: 27
                        text: qsTr("0")
                        font.pixelSize: 24
                        color: outputTextColor
                    }

                }

                Column {

                    Text {
                        id: text16_readData_

                        text: qsTr("readData:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }

                    Text {
                        id: text_readDataTimeElapsed
                        width: 53
                        height: 27
                        color: outputTextColor
                        text: qsTr("0")
                        font.pixelSize: 24
                    }

                }
            }
            Row {
                spacing: 2
                Column {
                    Text {
                        id: text11_ip_1_
                        text: qsTr("ip 1:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_packetsReceived1
                        width: 26
                        height: 25
                        text: qsTr("0")
                        font.pixelSize: 24
                        color: outputTextColor
                    }

                }
                Column {

                    Text {
                        id: text12_ip_2_
                        text: qsTr("ip 2:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_packetsReceived2
                        width: 26
                        height: 27
                        color: outputTextColor
                        text: qsTr("0")
                        font.pixelSize: 24

                    }

                }
                Column {
                    Text {
                        id: text13_ip_3_

                        text: qsTr("ip 3:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_packetsReceived3
                        width: 26
                        height: 25
                        color: outputTextColor
                        text: qsTr("0")
                        font.pixelSize: 24
                    }

                }
                Column {
                    Text {
                        id: text14_ip_4_

                        text: qsTr("ip 4:")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_packetsReceived4

                        width: 26
                        height: 30
                        color: outputTextColor
                        text: qsTr("0")
                        font.pixelSize: 24
                    }
                }
            }
            Row {
                Button {
                    id: button_copyToClipboard
                    width: 82
                    height: 40
                    text: qsTr("Copy")
                    checkable: false
                    checked: false
                    font.pointSize: 8
                    onClicked: {
                        dataSource.copyHistoryToClipboard();
                    }
                }

            }
            Row {
                height: parent.height - y
                //width: parent.width
                ListView {
                    id: listViewMeasurements
                    height: parent.height
                    width: leftcolumn.width
                    keyNavigationWraps: false
                    highlightFollowsCurrentItem: false
                    boundsBehavior: Flickable.StopAtBounds
                    highlightRangeMode: ListView.NoHighlightRange
                    snapMode: ListView.NoSnap
                    contentHeight: 0
                    rotation: 0
                    flickableDirection: Flickable.VerticalFlick
                    contentWidth: 0
                    spacing: 1
                    orientation: ListView.Vertical
                    model: measurementModel
                    clip: true
                    delegate: Item {
                        //x: 5
                        //width: 192
                        height: 20
                        Row {
                            id: row1

                            Text {
                                property variant data: model
                                text: model.text
                                font.bold: false
                                anchors.verticalCenter: parent.verticalCenter
                                color: outputTextColor
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
                        //dataSource.showByIndex(listViewMeasurements.currentIndex);
                    }

                }
            }
            /*
            ComboBox {
                id: comboBoxMyIP
                visible: false
                width: 124
                height: 32
                model: myIPsListModel
                textRole: "text"
                onActivated: _intercom.setMyIP(currentText);
            }
            Button {
                visible: false
                id: buttonTest
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
            Text {
                visible: false
                id: text10_Timeout_
                x: 8
                y: 159
                text: qsTr("Timeout:")
                font.pixelSize: 12
                color: staticTextColor
            }
            SpinBox {
                visible: false
                id: spinBox_dataReceiveTimeout
                x: 67
                y: 159
                width: 109
                height: 40
                value: appSettings.dataReceiveTimeout
                from: 10
                to: 1000

                onValueChanged: _intercom.setTimeout(value);
            }
            */
        }

        Column {
            id: rightcolumn
            x: 0
            width: parent.width-200
            height: parent.height
            z: -1
            Item {
                id: contentItem
                y: 0
                width: parent.width
                height: parent.height-200
                Column {
                    width: parent.width
                    height: parent.height
                    //SL
                    Column {
                        width: parent.width
                        height: parent.height
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
                                color: outputTextColor

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
                            height: parent.height - tabBar.height - row_selectReceiver.height
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
                                    ColorGradientStop { position: 0.0; color: "#0033ff" }
                                    ColorGradientStop { id: middleGradient; position: 0.25; color: "#ffff11" }
                                    ColorGradientStop { position: 0.75; color: "red" }
                                }
                                Surface3D {
                                    id: timeline_3d_surface_object
                                    anchors.fill: parent

                                    theme: Theme3D {
                                        type: Theme3D.ThemeQt
                                        font.family: "STCaiyun"
                                        font.pointSize: 35
                                        colorStyle: Theme3D.ColorStyleRangeGradient
                                        baseGradients: [surfaceGradient]
                                    }
                                    axisX:  ValueAxis3D {
                                        min : 0.0
                                        max : 330.0
                                        title : "Scan"
                                        titleVisible : true
                                    }

                                    axisZ: ValueAxis3D {
                                        min :  70.0
                                        max : 150.0
                                        title  : "Distance"
                                        titleVisible : true
                                    }
                                    axisY: ValueAxis3D {
                                        min : -500
                                        max : 500
                                        title : "Signal level"
                                        titleVisible : true
                                    }

                                    horizontalAspectRatio: 1

                                    shadowQuality: AbstractGraph3D.ShadowQualityMedium
                                    selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndRow
                                    //scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft



                                    Surface3DSeries {
                                        id: surfaceSeries
                                        flatShadingEnabled: false
                                        drawMode: Surface3DSeries.DrawSurface
                                    }
                                    Surface3DSeries {
                                        visible: false
                                        id: surfaceSeriesCorr
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
                id: controlsItem
                width: parent.width
                height: 200

                Slider {
                    //visible: false;
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
                    visible: false;
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
                    //visible: false;
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


                /*
                double fc{1e+9};
                double deltaf;
                unsigned short ford {8};
                double Td {1e-9};*/
                TextField {
                    //visible: false;
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
                TextField {
                    id: textField_Command
                    x: textField_Td.x + textField_Td.width + 10
                    y:  textField_slider4_level.y
                    width: 100
                    height: 40
                    text: appSettings.commandtext
                    selectByMouse: true

                    onEditingFinished: {

                    }
                }

                Button {
                    id: button_compareToData
                    x: textField_Command.x + textField_Command.width + 10
                    y: textField_Command.y
                    width: 55
                    height: 40
                    text: qsTr("Command")
                    checked: false
                    checkable: false
                    font.pointSize: 8
                    onClicked: {
                        main.textChanged("Command=" + textField_Command.text);
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

    //![2]
    Component.onCompleted: {
        _intercom.setTimeout(appSettings.dataReceiveTimeout);
        setSingleWaveformDistanceText(0);
        setPacketsReceived(0, 0, 0, 0);
        dataSource.setWriteHistory(appSettings.writeHistory);
    }
    Settings {
        id: appSettings
        property int dataReceiveTimeout: 400
        property alias slider1_value: slider1.value
        property alias slider2_value: slider2.value
        property alias slider3_value: slider3.value
        property alias slider4_value: slider4.value

        property alias fc: textField_fc.text
        property alias deltaf: textField_deltaf.text
        property alias ford: textField_ford.text
        property alias td: textField_Td.text

        property alias commandtext: textField_Command.text

        property alias zeroIndex: textFieldZeroIndex.text

        property alias writeHistory: checkboxWriteHistory.checked
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
    function setSingleWaveformDistanceText (distance) {
        single_waveform_distance.text = "Distance = " + distance;
    }
    function updateAllWaveforms (){

    }
    function updateDistances (){

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
    function buttonOnCircleClicked(index) {
        rect_circleSelector.visible = false;

        comboBox_selectIP.currentIndex = comboBox_selectIP.model.indexOf(dataSource.getIP());
        //dataSource.selectIP(ip);
        comboBox_selectEmitter.currentIndex = comboBox_selectEmitter.model.indexOf(dataSource.getEmitter());
        //dataSource.selectEmitter(emitter);
        comboBox_selectRow.currentIndex = comboBox_selectRow.model.indexOf(dataSource.getRow());
        //dataSource.selectRow(row);*/

        updateSingleWaveform();
        dataSource.updateSurface3D(surfaceSeries);
        dataSource.updateCorrelationChart(surfaceSeriesCorr);
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

