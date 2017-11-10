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

    property string backgroundColor: "#888888"
    property string outputTextColor: "#ffdd00"
    property string staticTextColor: "#000000"
    //property string inputTextColor: "yellow"

    property alias text_currentShift: text_currentShift.text

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
        spacing: 2
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
                    text: appSettings.fullscanCountdownStart
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
                width: parent.width
                spacing: 1
                /*ComboBox {
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
                }*/
                Button {
                    text : "Clear"
                    width: parent.width/2
                    onClicked: {
                        dataSource.clearData();
                    }
                }
                Button {
                    text : "Copy to shared memory"
                    width: parent.width/2
                    onClicked: {
                        dataSource.copyToSharedMemory();
                    }
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
                    id: useZeroSignal

                    checked: appSettings.useZeroSignal;
                    autoExclusive: false
                    spacing: 5
                    font.weight: Font.Normal
                    onCheckedChanged: {
                        dataSource.setUseZeroSignal(checked);
                        updateSingleWaveform();
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Use zero index " + textFieldZeroIndex.text)
                }
                Text {
                    text: qsTr("Zero index:")
                }

                TextField {
                    enabled: useZeroSignal.checked
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

            }
            Row {
                CheckBox {
                    id: useAbsoluteValues
                    checked: appSettings.useAbsoluteValues
                    onCheckedChanged: {

                    }
                }
                Text {
                    text: qsTr("Use absolute value")
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
                Column {
                    Text {
                        id: text14_ADC_

                        text: qsTr("ADC")
                        font.pixelSize: 12
                        color: staticTextColor
                    }
                    Text {
                        id: text_currentShift

                        width: 26
                        height: 30
                        color: outputTextColor
                        text: qsTr("0")
                        font.pixelSize: 24
                    }
                }
            }
            Row {
                spacing: 1
                width: parent.width
                Button {                    
                    id: button_copyToClipboard
                    width: parent.width /3

                    height: 40
                    text: qsTr("Copy")
                    checkable: false
                    checked: false
                    font.pointSize: 8
                    onClicked: {
                        var i = dataSource.getCurrentUnitIndex();
                        dataSource.copyHistoryToClipboard(i, i, 0, listViewMeasurements.model.rowCount()-1);
                    }
                }
                Button {
                    id: button_copyAllToClipboard
                    width: parent.width /3
                    height: 40
                    text: qsTr("Copy all")
                    checkable: false
                    checked: false
                    font.pointSize: 8
                    onClicked: {
                        dataSource.copyHistoryToClipboard(0, dataSource.getNChannels()-1, 0, listViewMeasurements.model.rowCount()-1);
                    }
                }

            }
            Row {
                spacing: 1
                width: parent.width
                TextField {
                    id: r_start
                    width: parent.width /3
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("First measurement to copy");
                }
                TextField {
                    id: r_end
                    width: parent.width /3
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Last measurement to copy");
                }
                Button {
                    id: button_copySpecialToClipboard
                    width: parent.width /3
                    height: 40
                    text: qsTr("Copy special")
                    checkable: false
                    checked: false
                    font.pointSize: 8
                    onClicked: {
                        dataSource.copyHistoryToClipboard(0, dataSource.getNChannels()-1, r_start.text, r_end.text);
                    }
                }
            }

            Row {
                Text {
                    text: qsTr("Slice 0")
                }
                CheckBox {
                    id: show_i0
                    checked: appSettings.show_i0
                    onCheckedChanged: {
                        dataSource.update(show_i0.checked, appSettings.i0, 0, 3, 6);
                    }
                    background: Rectangle {color: scopeView.color0}
                }
                Text {
                    text: qsTr("index=")
                }
                TextField {
                    id: textField_i0

                    width: 40
                    text: appSettings.i0
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
                        dataSource.update(show_i0.checked, appSettings.i0, 0, 3, 6);
                    }
                }
                CheckBox {
                    id: show_i0_1
                    checked: appSettings.show_i0_1
                    background: Rectangle {color: scopeView.color0_1}
                }
            }
            Row {
                Text {
                    text: qsTr("Slice 1")
                }
                CheckBox {
                    id: show_i1
                    checked: appSettings.show_i1
                    onCheckedChanged: {
                        dataSource.update(show_i1.checked, appSettings.i1, 1, 4, 7);
                    }
                    background: Rectangle {color: scopeView.color1}
                }
                Text {
                    text: qsTr("index=")
                }
                TextField {
                    id: textField_i1

                    width: 40
                    text: appSettings.i1
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
                        dataSource.update(show_i1.checked, appSettings.i1, 1, 4, 7);
                    }
                }
                CheckBox {
                    id: show_i1_1
                    checked: appSettings.show_i1_1
                    background: Rectangle {color: scopeView.color1_1}
                }
            }
            Row {
                Text {
                    text: qsTr("Slice 2")
                }
                CheckBox {
                    id: show_i2
                    checked: appSettings.show_i2
                    onCheckedChanged: {
                        dataSource.update(show_i2.checked, appSettings.i2, 2, 5, 8);
                    }
                    background: Rectangle {color: scopeView.color2}
                }
                Text {
                    text: qsTr("index=")
                }
                TextField {
                    id: textField_i2

                    width: 40
                    text: appSettings.i2
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
                        dataSource.update(show_i2.checked, text, 2, 5, 8);
                    }
                }
                CheckBox {
                    id: show_i2_1
                    checked: appSettings.show_i2_1
                    background: Rectangle {color: scopeView.color2_1}
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
                    onCountChanged: {
                        r_start.text = model.rowCount()-1;
                        r_end.text = model.rowCount()-1;
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
                        }
                        StackLayout {
                            width: parent.width
                            height: parent.height - tabBar.height - row_selectReceiver.height
                            currentIndex: tabBar.currentIndex
                            Item {

                                anchors.fill: parent

                                ScopeView {
                                    id: scopeView
                                    x: 0
                                    y: 0
                                    width: parent.width
                                    height: parent.height - row_selectReceiver.height


                                }
                                TextField {
                                    x: 0
                                    y: scopeView.height - height
                                    id: d0
                                    width: 55
                                    height: 36
                                    text: appSettings.d0
                                    selectByMouse: true
                                }
                                TextField {
                                    x: scopeView.width - width
                                    y: scopeView.height - height
                                    id: dN
                                    width: 55
                                    height: 36
                                    text: appSettings.dN
                                    selectByMouse: true
                                }
                                TextField {
                                    x: 0
                                    y: 0
                                    id: maxSignalLevel
                                    width: 55
                                    height: 36
                                    text: appSettings.maxSignalLevel
                                    selectByMouse: true
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
                                    ColorGradientStop { position: 0.0; color: "#00ff00" }
                                    ColorGradientStop { id: middleGradient; position: 0.1; color: "#ffff00" }
                                    ColorGradientStop { id: middleGradient1; position: 0.2; color: "#0000ff" }
                                    ColorGradientStop { position: 0.3; color: "#ff0000" }
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
                                        min : appSettings.surface3d_x_min
                                        max : appSettings.surface3d_x_max
                                        title : appSettings.surface3d_x_name //scan
                                        titleVisible : true
                                    }

                                    axisZ: ValueAxis3D {
                                        min :  appSettings.surface3d_z_min
                                        max : appSettings.surface3d_z_max
                                        title  : appSettings.surface3d_z_name //distance
                                        titleVisible : true
                                    }
                                    axisY: ValueAxis3D {
                                        min : appSettings.surface3d_y_min
                                        max : appSettings.surface3d_y_max
                                        title : appSettings.surface3d_y_name //signal level
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
                StackLayout {
                    width: parent.width
                    height: parent.height - tabBar.height - row_selectReceiver.height
                    currentIndex: tabBar.currentIndex
                    Item {
                        id: filterControl
                        anchors.fill: parent


                        Row {
                            anchors.fill: parent
                            Column {
                                property int inputPosition: 150
                                width: parent.width/3
                                Row {
                                    Text {
                                        text: "HP"
                                        font.pixelSize: 17
                                    }
                                }

                                Row {
                                    id: hP_mainrow

                                    Text {
                                        text: "on:"
                                        width: 40
                                    }
                                    CheckBox {
                                        id: hP_on
                                        checked: appSettings.hP_on
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: hP_order
                                        enabled: hP_on.checked
                                        currentIndex: appSettings.hP_order_index
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        onActivated: {

                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: hP_Td_txt
                                        text: qsTr("Td:")
                                        width: 40
                                    }
                                    TextField {
                                        id: hP_Td
                                        text: appSettings.hP_Td
                                        enabled: hP_on.checked
                                        width: hP_mainrow.width - hP_Td_txt.width
                                    }
                                }
                                Row {

                                    Text {
                                        id: hP_fc_txt
                                        text: qsTr("fc:")
                                        width: 40
                                    }
                                    TextField {
                                        id: hP_fc
                                        text: appSettings.hP_fc
                                        enabled: hP_on.checked
                                        width: hP_mainrow.width - hP_fc_txt.width
                                    }
                                }
                            }
                            Column {
                                width: parent.width/3
                                Row {
                                    Text {
                                        text: "LP"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: lP_mainrow
                                    Text {
                                        text: "on:"
                                        width: 40
                                    }
                                    CheckBox {
                                        id: lP_on
                                        checked: appSettings.lP_on
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: lP_order
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        enabled: lP_on.checked
                                        currentIndex: appSettings.lP_order_index
                                        onActivated: {

                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: lP_Td_txt
                                        text: qsTr("Td:")
                                        width: 40
                                    }
                                    TextField {
                                        id: lP_Td
                                        text: appSettings.lP_Td
                                        enabled: lP_on.checked
                                        width: lP_mainrow.width - lP_Td_txt.width
                                    }
                                }
                                Row {

                                    Text {
                                        id: lP_fc_txt
                                        text: qsTr("fc:")
                                        width: 40
                                    }
                                    TextField {
                                        id: lP_fc
                                        text: appSettings.lP_fc
                                        enabled: lP_on.checked
                                        width: lP_mainrow.width - lP_fc_txt.width
                                    }
                                }
                            }
                            Column {
                                width: parent.width/3
                                Row {
                                    Text {
                                        text: "BP"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: bP_mainrow
                                    Text {
                                        text: "on:"
                                        width: 40
                                    }
                                    CheckBox {
                                        id: bP_on
                                        checked: appSettings.bP_on
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: bP_order
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        enabled: bP_on.checked
                                        currentIndex: appSettings.bP_order_index
                                        onActivated: {

                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_Td_txt
                                        text: qsTr("Td:")
                                        width: 40
                                    }
                                    TextField {
                                        id: bP_Td
                                        text: appSettings.bP_Td
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_Td_txt.width
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_fc_txt
                                        text: qsTr("fc:")
                                        width: 40
                                    }
                                    TextField {
                                        id: bP_fc
                                        text: appSettings.bP_fc
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_fc_txt.width
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_deltaf_txt
                                        text: qsTr("deltaf:")
                                        width: 40
                                    }
                                    TextField {
                                        id: bP_deltaf
                                        text: appSettings.bP_deltaf
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_fc_txt.width
                                    }
                                }
                            }
                        }
                    }
                    Item {

                    }
                    Item {

                    }
                    Item {
                        Button {
                            text: qsTr("Draw")
                            onClicked: {
                                timeline_3d_unit.addScan();
                            }
                        }
                    }
                    Item {
                        anchors.fill: parent
                        Row {
                            anchors.fill: parent
                            Column {
                                width: parent.width/3
                                Row {
                                    width:parent.width
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_x_name
                                        text: appSettings.surface3d_x_name
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_y_name
                                        text: appSettings.surface3d_y_name
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_z_name
                                        text: appSettings.surface3d_z_name
                                        selectByMouse: true
                                    }
                                }
                                Row {
                                    width:parent.width
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_x_min
                                        text: appSettings.surface3d_x_min
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_y_min
                                        text: appSettings.surface3d_y_min
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_z_min
                                        text: appSettings.surface3d_z_min
                                        selectByMouse: true
                                    }
                                }
                                Row {
                                    width:parent.width
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_x_max
                                        text: appSettings.surface3d_x_max
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_y_max
                                        text: appSettings.surface3d_y_max
                                        selectByMouse: true
                                    }
                                    TextField {
                                        width: parent.width/3
                                        id: surface3d_z_max
                                        text: appSettings.surface3d_z_max
                                        selectByMouse: true
                                    }
                                }
                            }
                            Column {
                                width: parent.width/3
                                Item { //placeholder
                                    anchors.fill: parent
                                }
                            }
                            Column {
                                width: parent.width/3
                                TextField {
                                    width: parent.width/3
                                    id: surface3d_scanStep
                                    text: appSettings.surface3d_scanStep
                                    selectByMouse: true
                                    onAccepted: {
                                        dataSource.setSurface3DScanStep(appSettings.surface3d_scanStep);
                                    }
                                }
                                Button {
                                    id: buttonPlot

                                    width: parent.width/3
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
                                        //dataSource.updateCorrelationChart(surfaceSeriesCorr);
                                    }
                                }
                            }
                        }
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
        _intercom.setFullscanCountdownStart(appSettings.fullscanCountdownStart);
        setSingleWaveformDistanceText(0);
        setPacketsReceived(0, 0, 0, 0);
        dataSource.setWriteHistory(appSettings.writeHistory);

        dataSource.setSurface3DScanStep(appSettings.surface3d_scanStep);
    }
    Connections {
        target: dataSource
        onCurrentUnitIndexChanged: {
            updateSingleWaveform();
        }
        onFileLoaded: {
            updateSingleWaveform();
        }
    }
    Settings {
        id: appSettings






        property alias fullscanCountdownStart: textFieldFullscanCountdownStart.text

        property alias surface3d_scanStep: surface3d_scanStep.text

        property alias surface3d_x_name: surface3d_x_name.text
        property alias surface3d_x_min: surface3d_x_min.text
        property alias surface3d_x_max: surface3d_x_max.text

        property alias surface3d_y_name: surface3d_y_name.text
        property alias surface3d_y_min: surface3d_y_min.text
        property alias surface3d_y_max: surface3d_y_max.text

        property alias surface3d_z_name: surface3d_z_name.text
        property alias surface3d_z_min: surface3d_z_min.text
        property alias surface3d_z_max: surface3d_z_max.text


        property alias hP_on: hP_on.checked
        property alias hP_order_index: hP_order.currentIndex
        property alias hP_Td: hP_Td.text
        property alias hP_fc: hP_fc.text

        property alias lP_on: lP_on.checked
        property alias lP_order_index: lP_order.currentIndex
        property alias lP_Td: lP_Td.text
        property alias lP_fc: lP_fc.text

        property alias bP_on: bP_on.checked
        property alias bP_order_index: bP_order.currentIndex
        property alias bP_Td: bP_Td.text
        property alias bP_fc: bP_fc.text
        property alias bP_deltaf: bP_deltaf.text

        property alias maxSignalLevel: maxSignalLevel.text
        property alias useAbsoluteValues: useAbsoluteValues.checked
        property alias useZeroSignal: useZeroSignal.checked
        property int dataReceiveTimeout: 400






        property alias zeroIndex: textFieldZeroIndex.text

        property alias writeHistory: checkboxWriteHistory.checked

        property alias i0: textField_i0.text
        property alias i1: textField_i1.text
        property alias i2: textField_i2.text

        property alias show_i0: show_i0.checked
        property alias show_i1: show_i1.checked
        property alias show_i2: show_i2.checked

        property alias show_i0_1: show_i0_1.checked
        property alias show_i1_1: show_i1_1.checked
        property alias show_i2_1: show_i2_1.checked

        property alias d0: d0.text
        property alias dN: dN.text
    }
    Rectangle {
        id: rect_circleSelector
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2
        width: 768
        height: 768
        visible: false
        z: 100
        Button {
            text: qsTr("X")
            width: height
            x: parent.width - width
            onClicked: {
                parent.visible = false;
            }
        }

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
    function updateSurface3D ()     {
        dataSource.updateSurface3D(surfaceSeries);
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
        dataSource.update(appSettings.show_i0, appSettings.i0, 0, 3, 6);
        dataSource.update(appSettings.show_i1, appSettings.i1, 1, 4, 7);
        dataSource.update(appSettings.show_i2, appSettings.i2, 2, 5, 8);
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
    function setFilterParameters () {
        dataSource.setFilterParameters(hP_on.checked, hP_order.currentText, hP_fc.text, hP_Td.text,
                                       lP_on.checked, lP_order.currentText, lP_fc.text, lP_Td.text,
                                       bP_on.checked, bP_order.currentText, bP_fc.text, bP_Td.text, bP_deltaf.text);
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
        //dataSource.updateCorrelationChart(surfaceSeriesCorr);
    }
    function getBtnX(x0, R, N) {
        return x0 + R * Math.sin(Math.PI / 32 + N * Math.PI / 16);
    }
    function getBtnY(y0, R, N) {
        return y0 - R * Math.cos(Math.PI / 32 + N * Math.PI / 16);
    }

}

