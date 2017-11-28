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

import QtCharts 2.1
import QtDataVisualization 1.2

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick.Scene3D 2.0

import com.example.Timeline3D 1.0

//import QtQuick.Controls.Styles 1.4

import Qt.labs.settings 1.0

import QtQuick.LocalStorage 2.0
import "settings.js" as Settings

import Qt.labs.platform 1.0


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

    property real maxFor_i0: appSettings.maxSignalLevel

    property int nchannels;

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
                        updateSingleWaveform();
                    }
                }

            }
            Row {
                CheckBox {
                    id: useAbsoluteValues
                    checked: appSettings.useAbsoluteValues
                    onCheckedChanged: {
                        dataSource.setUseAbsoluteValues(checked);
                        updateSingleWaveform();
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
                    id: button_copyControlValues
                    width: parent.width /3
                    height: 40
                    text: qsTr("Copy control values")
                    checkable: false
                    checked: false
                    font.pointSize: 8
                    onClicked: {
                        copyControlValues();
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

                CheckBox {
                    id: show_i0
                    checked: appSettings.show_i0
                    onCheckedChanged: {
                        //dataSource.update(show_i0.checked, appSettings.i0, 0, 3, 6);
                        updateSingleWaveform();
                    }
                    background: Rectangle {color: scopeView.color0}
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
                        //dataSource.update(show_i0.checked, appSettings.i0, 0, 3, 6);
                        updateSingleWaveform();
                    }
                }
                CheckBox {
                    id: show_i0_1
                    checked: appSettings.show_i0_1
                    background: Rectangle {color: scopeView.color0_1}
                }
                Button {
                    text: qsTr("Fix")
                    width: 40
                    onClicked: {
                        setControlValues(0, appSettings.i0);
                    }
                }
            }
            Row {

                CheckBox {
                    id: show_i1
                    checked: appSettings.show_i1
                    onCheckedChanged: {
                        //dataSource.update(show_i1.checked, appSettings.i1, 1, 4, 7);
                        updateSingleWaveform();
                    }
                    background: Rectangle {color: scopeView.color1}
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
                        //dataSource.update(show_i1.checked, appSettings.i1, 1, 4, 7);
                        updateSingleWaveform();
                    }
                }
                CheckBox {
                    id: show_i1_1
                    checked: appSettings.show_i1_1
                    background: Rectangle {color: scopeView.color1_1}
                }
            }
            Row {

                CheckBox {
                    id: show_i2
                    checked: appSettings.show_i2
                    onCheckedChanged: {
                        //dataSource.update(show_i2.checked, appSettings.i2, 2, 5, 8);
                        updateSingleWaveform();
                    }
                    background: Rectangle {color: scopeView.color2}
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
                        //dataSource.update(show_i2.checked, text, 2, 5, 8);
                        updateSingleWaveform();
                    }
                }
                CheckBox {
                    id: show_i2_1
                    checked: appSettings.show_i2_1
                    background: Rectangle {color: scopeView.color2_1}
                }
            }
            Row {
                TextField {
                    id: physicalParameter0
                    text: appSettings.physicalParameter0
                    onTextChanged: {
                        dataSource.setPhysicalParameter0(text);
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
                                text: qsTr("Significance")
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

                            Button {
                                id: button_selectOnCircle
                                text: qsTr("Select...")
                                onClicked: {
                                    rect_circleSelector.visible = true;
                                }
                            }

                            Text {
                                id: single_waveform_message
                                text: qsTr("---")
                                font.pixelSize: 24
                                color: outputTextColor

                            }
                            Column {
                                TextField {
                                    id: prolazPosition
                                    text: Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].prolazPosition", 0)
                                    width:60
                                    selectByMouse: true
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("Direct signal position")
                                }
                                Button {
                                    height: 10
                                    width: prolazPosition.width
                                    onClicked: {
                                        var p0max = dataSource.getMaxAt(appSettings.i0, appSettings.currentUnitIndex, 1, 0, 500);
                                        prolazPosition.text = p0max.x;
                                    }
                                }
                            }
                            Column {
                                TextField {
                                    id: reflection10Position
                                    text: Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection10Position", 0)
                                    width:60
                                    selectByMouse: true
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("10cm distance position")

                                }
                                Button {
                                    height: 10
                                    width: reflection10Position.width
                                    onClicked: {
                                        var p0max = dataSource.getMaxAt(appSettings.i0, appSettings.currentUnitIndex, 1, 0, 500);
                                        reflection10Position.text = p0max.x;
                                    }
                                }
                            }
                            Column {
                                TextField {
                                    id: reflection20Position
                                    text: Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection20Position", 0)
                                    width:60
                                    selectByMouse: true
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("20cm distance position")

                                }
                                Button {
                                    height: 10
                                    width: reflection20Position.width
                                    onClicked: {
                                        var p0max = dataSource.getMaxAt(appSettings.i0, appSettings.currentUnitIndex, 1, 0, 500);
                                        reflection20Position.text = p0max.x;
                                    }
                                }
                            }
                            Column {
                                CheckBox {
                                    id: simpleLPCutoffOn
                                    width:60
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("simple LP Cutoff ON/OFF")
                                    onCheckedChanged: {
                                        dataSource.setCutoffParameters(simpleLPCutoffOn.checked, simpleLPCutoff.text, appSettings.currentUnitIndex);
                                        updateSingleWaveform();
                                    }
                                }
                                Button {
                                    height: 10
                                    width: simpleLPCutoffOn.width
                                    onClicked: {
                                        simpleLPCutoffOn.checked = cutoff_on.checked;
                                    }
                                }
                            }
                            Column {
                                enabled: simpleLPCutoffOn.checked
                                TextField {
                                    id: simpleLPCutoff
                                    text: Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff", "")
                                    width:60
                                    selectByMouse: true
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("simple LP Cutoff")
                                    onTextChanged: {
                                        dataSource.setCutoffParameters(simpleLPCutoffOn.checked, simpleLPCutoff.text, appSettings.currentUnitIndex);
                                        updateSingleWaveform();
                                    }
                                }
                                Button {
                                    height: 10
                                    width: simpleLPCutoff.width
                                    onClicked: {
                                        simpleLPCutoff.text = cutoff_Level.text;
                                    }
                                }
                            }
                            Column {
                                CheckBox {
                                    id: simpleLPCutoff0On
                                    width:60
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("simple LP Cutoff0 ON/OFF")
                                    onCheckedChanged: {
                                        dataSource.setCutoff0Parameters(simpleLPCutoff0On.checked, simpleLPCutoff0.text, appSettings.currentUnitIndex);
                                        updateSingleWaveform();
                                    }
                                }
                                Button {
                                    height: 10
                                    width: simpleLPCutoff0On.width
                                    onClicked: {
                                        simpleLPCutoff0On.checked = cutoff0_on.checked;
                                    }
                                }
                            }
                            Column {
                                enabled: simpleLPCutoff0On.checked
                                TextField {
                                    id: simpleLPCutoff0
                                    text: Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff0", "")
                                    width:60
                                    selectByMouse: true
                                    ToolTip.visible: hovered
                                    ToolTip.text: qsTr("simple LP Cutoff0")
                                    onTextChanged: {
                                        dataSource.setCutoff0Parameters(simpleLPCutoff0On.checked, simpleLPCutoff0.text, appSettings.currentUnitIndex);
                                        updateSingleWaveform();
                                    }
                                }
                                Button {
                                    height: 10
                                    width: simpleLPCutoff0.width
                                    onClicked: {
                                        simpleLPCutoff0.text = cutoff0_Level.text;
                                    }
                                }
                            }
                            CheckBox {
                                id: useCentimeters
                                checked: appSettings.useCentimeters
                                onCheckedChanged: {
                                    updateSingleWaveform();
                                }
                            }

                            Button {
                                id: saveReceiverProperties
                                text: "Save"
                                onClicked: {
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].prolazPosition", prolazPosition.text);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].reflection10Position", reflection10Position.text);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].reflection20Position", reflection20Position.text);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff0On", simpleLPCutoff0On.checked);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff0", simpleLPCutoff0.text);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoffOn", simpleLPCutoffOn.checked);
                                    Settings.set("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff", simpleLPCutoff.text);
                                }
                                width:60
                            }
                            Button {
                                id: saveReceiverProperties0
                                text: "Save all as [0]"
                                onClicked: {
                                    msgDialog_saveReceiverProperties0.visible = true;
                                }
                                width:60
                            }
                            MessageDialog {
                                id: msgDialog_saveReceiverProperties0
                                title: "Dangerous operation"
                                buttons: MessageDialog.Ok | MessageDialog.Cancel
                                text: "Are you sure to set all direct signals positions automatically?"

                                onOkClicked: {
                                    console.log("ok");
                                    for(var i=0; i<64; i++) {
                                        var p0max = dataSource.getMaxAt(appSettings.i0, i, 1, 0, 500);
                                        Settings.set("receiverProperties["+i+"].prolazPosition", p0max.x);
                                    }
                                }
                                onCancelClicked: {
                                    console.log("cancel");
                                }
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
                                TextField {
                                    x: scopeView.width - width
                                    y: 0
                                    id: maxProcessedLevel
                                    width: 55
                                    height: 36
                                    text: appSettings.maxProcessedLevel
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
                                Column {
                                    width: parent.width
                                    height: parent.height
                                    Row {
                                        width: parent.width
                                        height: parent.height
                                        ChartView {
                                            width: parent.width
                                            height: parent.height//-40
                                            id: significanceChart
                                            ValueAxis {
                                                id: significance_axisX
                                                min: 0
                                                max: 1
                                            }
                                            ValueAxis {
                                                id: significance_axisY
                                                min: 0
                                                max: 1
                                            }
                                            LineSeries {
                                                id: significanceLineSeries;
                                                name: "Significance of measurement " + appSettings.i0 + ", current unit index =" + appSettings.currentUnitIndex
                                                axisX: significance_axisX
                                                axisY: significance_axisY
                                            }
                                        }
                                    }

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
                                width: parent.width/4
                                Row {
                                    Text {
                                        text: "avg"
                                        font.pixelSize: 17
                                    }
                                }

                                Row {
                                    id: avg_mainrow
                                    width: parent.width
                                    Text {
                                        id: avg_on_
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: avg_on
                                        checked: appSettings.avg_on
                                        onCheckedChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                    Text {
                                        id: avg_step_
                                        text: "step:"
                                    }
                                    TextField {
                                        id: avg_Step
                                        enabled: avg_on.checked
                                        text:appSettings.avg_Step
                                        width: 40
                                        onTextChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                }

                                Row {
                                    Text {
                                        text: "cutoff"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: cutoff_mainrow

                                    Text {
                                        id: cutoff_on_
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: cutoff_on
                                        checked: appSettings.cutoff_on
                                        onCheckedChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                    Text {
                                        id: cutoff_step_
                                        text: "level:"
                                    }
                                    TextField {
                                        id: cutoff_Level
                                        enabled: cutoff_on.checked
                                        text:appSettings.cutoff_Level
                                        width: 40
                                        onTextChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                }

                                Row {
                                    Text {
                                        text: "cutoff0"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: cutoff0_mainrow

                                    Text {
                                        id: cutoff0_on_
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: cutoff0_on
                                        checked: appSettings.cutoff0_on
                                        onCheckedChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                    Text {
                                        id: cutoff0_step_
                                        text: "level:"
                                    }
                                    TextField {
                                        id: cutoff0_Level
                                        enabled: cutoff0_on.checked
                                        text:appSettings.cutoff0_Level
                                        width: 40
                                        onTextChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                }
                            }

                            Column {
                                property int inputPosition: 150
                                width: parent.width/4
                                Row {
                                    Text {
                                        text: "HP"
                                        font.pixelSize: 17
                                    }
                                }

                                Row {
                                    id: hP_mainrow
                                    width: parent.width
                                    Text {
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: hP_on
                                        checked: appSettings.hP_on
                                        onCheckedChanged: {
                                            setFilterParameters();
                                        }
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: hP_order
                                        enabled: hP_on.checked
                                        currentIndex: appSettings.hP_order_index
                                        width:60
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        onActivated: {
                                            setFilterParameters();
                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: hP_Td_txt
                                        text: qsTr("Td:")

                                    }
                                    TextField {
                                        id: hP_Td
                                        text: appSettings.hP_Td
                                        enabled: hP_on.checked
                                        width: hP_mainrow.width - hP_Td_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                                Row {

                                    Text {
                                        id: hP_fc_txt
                                        text: qsTr("fc:")

                                    }
                                    TextField {
                                        id: hP_fc
                                        text: appSettings.hP_fc
                                        enabled: hP_on.checked
                                        width: hP_mainrow.width - hP_fc_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                            }
                            Column {
                                width: parent.width/4
                                Row {
                                    Text {
                                        text: "LP"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: lP_mainrow
                                    width: parent.width
                                    Text {
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: lP_on
                                        checked: appSettings.lP_on
                                        onCheckedChanged: setFilterParameters();
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: lP_order
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        enabled: lP_on.checked
                                        currentIndex: appSettings.lP_order_index
                                        width:60
                                        onActivated: {
                                            setFilterParameters();
                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: lP_Td_txt
                                        text: qsTr("Td:")

                                    }
                                    TextField {
                                        id: lP_Td
                                        text: appSettings.lP_Td
                                        enabled: lP_on.checked
                                        width: lP_mainrow.width - lP_Td_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                                Row {

                                    Text {
                                        id: lP_fc_txt
                                        text: qsTr("fc:")

                                    }
                                    TextField {
                                        id: lP_fc
                                        text: appSettings.lP_fc
                                        enabled: lP_on.checked
                                        width: lP_mainrow.width - lP_fc_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                            }
                            Column {
                                width: parent.width/4
                                Row {
                                    Text {
                                        text: "BP"
                                        font.pixelSize: 17
                                    }
                                }
                                Row {
                                    id: bP_mainrow
                                    width: parent.width
                                    Text {
                                        text: "on:"

                                    }
                                    CheckBox {
                                        id: bP_on
                                        checked: appSettings.bP_on
                                        onCheckedChanged: setFilterParameters();
                                    }
                                    Text {
                                        text: "filter order:"
                                    }
                                    ComboBox {
                                        id: bP_order
                                        model: ["1", "2", "3", "4", "5", "6", "7", "8"]
                                        enabled: bP_on.checked
                                        currentIndex: appSettings.bP_order_index
                                        width:60
                                        onActivated: {
                                            setFilterParameters();
                                        }
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_Td_txt
                                        text: qsTr("Td:")

                                    }
                                    TextField {
                                        id: bP_Td
                                        text: appSettings.bP_Td
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_Td_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_fc_txt
                                        text: qsTr("fc:")

                                    }
                                    TextField {
                                        id: bP_fc
                                        text: appSettings.bP_fc
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_fc_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                                Row {

                                    Text {
                                        id: bP_deltaf_txt
                                        text: qsTr("deltaf:")

                                    }
                                    TextField {
                                        id: bP_deltaf
                                        text: appSettings.bP_deltaf
                                        enabled: bP_on.checked
                                        width: bP_mainrow.width - bP_fc_txt.width
                                        onTextChanged: setFilterParameters();
                                    }
                                }
                            }
                        }
                    }
                    Item {

                    }
                    Item {
                        Row {
                            width: parent.width
                            height: 40
                            TextField {
                                id: significanceChartStep
                                text: appSettings.significanceChartStep
                            }

                            Button {
                                id: significance_Button
                                text: "Refresh"
                                onClicked: {
                                    dataSource.updateSignificance(appSettings.significanceChartStep,
                                                                  appSettings.i0,
                                                                  appSettings.currentUnitIndex,
                                                                  1,
                                                                  0,
                                                                  500);
                                }
                            }

                        }
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

            console.log("You chose: " + fileDialog.file)

            dataSource.loadHistoryFromFile(fileDialog.file.toString().replace("file:///", ""))
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
        setSingleWaveformMessageText("Receiver " + dataSource.getCurrentUnitIndex());
        setPacketsReceived(0, 0, 0, 0);
        dataSource.setWriteHistory(appSettings.writeHistory);

        dataSource.setSurface3DScanStep(appSettings.surface3d_scanStep);

        setFilterParameters();

        dataSource.setPhysicalParameter0(appSettings.physicalParameter0);

        console.log(Settings.get("test setting", 0));

        Settings.set("test setting", 666);

        dataSource.setCurrentUnitIndex(appSettings.currentUnitIndex);
        console.log("current unit index=" + appSettings.currentUnitIndex);

        dataSource.setSignificanceSeries(significanceLineSeries, 0);

        nchannels = dataSource.getNChannels();

        setCutoffParameters();


    }
    Connections {
        target: dataSource
        onCurrentUnitIndexChanged: {
            updateSingleWaveform();
            setSingleWaveformMessageText("Receiver " + dataSource.getCurrentUnitIndex());

            prolazPosition.text = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].prolazPosition", "");
            reflection10Position.text = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection10Position", "");
            reflection20Position.text = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection20Position", "");

            simpleLPCutoffOn.checked = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoffOn", false);
            simpleLPCutoff.text = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff", "");

            simpleLPCutoff0On.checked = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff0On", false);
            simpleLPCutoff0.text = Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].simpleLPCutoff0", "");
        }
        onFileLoaded: {
            updateSingleWaveform();
        }
    }
    Settings {
        id: appSettings

        property alias maxMainControlValue: maxMainControlValue.text

        property alias significanceChartStep: significanceChartStep.text
        property alias useCentimeters: useCentimeters.checked

        property int currentUnitIndex: 0

        property var receiverProperties: []


        property alias cutoff_on: cutoff_on.checked
        property alias cutoff_Level: cutoff_Level.text

        property alias cutoff0_on: cutoff0_on.checked
        property alias cutoff0_Level: cutoff0_Level.text

        property alias physicalParameter0: physicalParameter0.text



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

        property alias avg_on: avg_on.checked
        property alias avg_Step: avg_Step.text

        property alias maxSignalLevel: maxSignalLevel.text
        property alias maxProcessedLevel: maxProcessedLevel.text

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
        TextField {
            id: maxMainControlValue
            text: appSettings.maxMainControlValue
        }
        Rectangle {
            property real phi: 0
            id: mainMark
            height: 10
            width: 40
            color: "red"

            x: getMainMarkX(parent.width/2 - width/2, parent.width * 0.45, phi);
            y: getMainMarkY(parent.height/2 - height/2, parent.height * 0.45, phi);

            transform: Rotation {
                origin.x: mainMark.width/2
                origin.y: mainMark.height/2
                angle: mainMark.phi * 180 / Math.PI
            }
        }

        Button {
            text: qsTr("X")
            width: height
            x: parent.width - width
            onClicked: {
                parent.visible = false;
            }
        }
        Repeater {
            id: stReceiverWidgets
            model: 64
            STReceiverWidget {
                unitIndex: index
            }
        }
    }
    function getMainMarkX(x0, R, phi) {
        return x0 + R * Math.sin(phi);
    }
    function getMainMarkY(y0, R, phi) {
        return y0 - R * Math.cos(phi);
    }

    function updateWidgets()    {
        for(var i=0; i<nchannels; i++)  {
            stReceiverWidgets.itemAt(i).mainControlValue = dataSource.getChannelMainControlValue(i);
        }
        mainMark.phi = dataSource.getMainControlDirection();
    }

    function copyControlValues() {
        var str = "ID,cutoff0_on,cutoff0,cutoff_on,cutoff,prolaz,p10,p20,max level,significance 0.25,significance 0.5,significance 0.75,max orig level\n";
        for (var i=0; i<nchannels; i++) {
            var prolaz = Settings.get("receiverProperties["+i+"].prolazPosition", "");
            var p10 = Settings.get("receiverProperties["+i+"].reflection10Position", "");
            var p20 = Settings.get("receiverProperties["+i+"].reflection20Position", "");

            var cutoff_on = Settings.get("receiverProperties["+i+"].simpleLPCutoffOn", false);
            var cutoff = Settings.get("receiverProperties["+i+"].simpleLPCutoff", "");

            var cutoff0_on = Settings.get("receiverProperties["+i+"].simpleLPCutoff0On", false);
            var cutoff0 = Settings.get("receiverProperties["+i+"].simpleLPCutoff0", "");

            var s0max = dataSource.getMaxAt(appSettings.i0, i, 0, 0, 500);
            var p0max = dataSource.getMaxAt(appSettings.i0, i, 1, 0, 500);

            var s025 = dataSource.getSignificance1(p0max, 0.25, appSettings.i0, i, 0, 500);
            var s050 = dataSource.getSignificance1(p0max, 0.5, appSettings.i0, i, 0, 500);
            var s075 = dataSource.getSignificance1(p0max, 0.75, appSettings.i0, i, 0, 500);

            str = str + i + ","+cutoff0_on+","+cutoff0+","+cutoff_on+","+cutoff+","+prolaz+","+p10+","+p20+","+p0max.y+","+s025+","+s050+","+s075+","+s0max.y+"\n";

            dataSource.setControlValues(1, i, [cutoff0_on, cutoff0, cutoff_on, cutoff, prolaz, p10, p20, p0max.y, s025, s050, s075, s0max.y]);
        }

        dataSource.copyTextToClipboard(str);
    }
    function setControlValues(layer, measurementIndex) {
        for (var i=0; i<nchannels; i++) {
            var prolaz = Settings.get("receiverProperties["+i+"].prolazPosition", "");
            var p10 = Settings.get("receiverProperties["+i+"].reflection10Position", "");
            var p20 = Settings.get("receiverProperties["+i+"].reflection20Position", "");

            var cutoff_on = Settings.get("receiverProperties["+i+"].simpleLPCutoffOn", false);
            var cutoff = Settings.get("receiverProperties["+i+"].simpleLPCutoff", "");

            var cutoff0_on = Settings.get("receiverProperties["+i+"].simpleLPCutoff0On", false);
            var cutoff0 = Settings.get("receiverProperties["+i+"].simpleLPCutoff0", "");

            var s0max = dataSource.getMaxAt(measurementIndex, i, 0, 0, 500);
            var p0max = dataSource.getMaxAt(measurementIndex, i, 1, 0, 500);

            var s025 = dataSource.getSignificance1(p0max, 0.25, measurementIndex, i, 0, 500);
            var s050 = dataSource.getSignificance1(p0max, 0.5, measurementIndex, i, 0, 500);
            var s075 = dataSource.getSignificance1(p0max, 0.75, measurementIndex, i, 0, 500);

            dataSource.setControlValues(layer, i, [cutoff0_on, cutoff0, cutoff_on, cutoff, prolaz, p10, p20, p0max.y, s025, s050, s075, s0max.y]);
        }
        updateWidgets();
    }

    function setCutoffParameters()  {
        for (var i=0; i<nchannels; i++) {
            var val = Settings.get("receiverProperties["+i+"].simpleLPCutoff", "");
            dataSource.setCutoffParameters(true, val, i);
            val = Settings.get("receiverProperties["+i+"].simpleLPCutoff0", "");
            dataSource.setCutoff0Parameters(true, val, i);
        }
    }

    function updateSurface3D ()     {
        dataSource.updateSurface3D(surfaceSeries);
    }

    function setSingleWaveformMessageText (text) {
        single_waveform_message.text = text;
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



        var s0max = dataSource.getMaxAt(appSettings.i0, appSettings.currentUnitIndex, 0, 0, 500);
        var p0max = dataSource.getMaxAt(appSettings.i0, appSettings.currentUnitIndex, 1, 0, 500);
        var s1max = dataSource.getMaxAt(appSettings.i1, appSettings.currentUnitIndex, 0, 0, 500);
        var p1max = dataSource.getMaxAt(appSettings.i1, appSettings.currentUnitIndex, 1, 0, 500);
        var s2max = dataSource.getMaxAt(appSettings.i2, appSettings.currentUnitIndex, 0, 0, 500);
        var p2max = dataSource.getMaxAt(appSettings.i2, appSettings.currentUnitIndex, 1, 0, 500);

        appSettings.maxSignalLevel = s0max.y>s1max.y?s0max.y:s1max.y;
        appSettings.maxSignalLevel = s2max.y>appSettings.maxSignalLevel?s2max.y:appSettings.maxSignalLevel;

        appSettings.maxProcessedLevel = p0max.y>p1max.y?p0max.y:p1max.y;
        appSettings.maxProcessedLevel = p2max.y>appSettings.maxProcessedLevel?p2max.y:appSettings.maxProcessedLevel;

        var p10, p20;
        if (appSettings.useCentimeters) {
            p10 = 1.0*Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection10Position", 10.0);
            p20 = 1.0*Settings.get("receiverProperties["+appSettings.currentUnitIndex+"].reflection20Position", 20.0);
        }
        else {
            p10 = 10;
            p20 = 20;
        }
        dataSource.update(appSettings.show_i0, appSettings.i0, 0, 3, 6, p10, p20);
        dataSource.update(appSettings.show_i1, appSettings.i1, 1, 4, 7, p10, p20);
        dataSource.update(appSettings.show_i2, appSettings.i2, 2, 5, 8, p10, p20);
        appSettings.dN = 10.0 + 1.0*(dataSource.getMaxDistance(appSettings.i0, appSettings.currentUnitIndex) - p10)/(p20-p10)*10.0;
        appSettings.d0 = 10.0 + 1.0*(0 - p10)/(p20-p10)*10.0;
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
                                       bP_on.checked, bP_order.currentText, bP_fc.text, bP_Td.text, bP_deltaf.text,
                                       avg_on.checked, avg_Step.text);
        dataSource.setCutoffParameters(cutoff_on.checked, cutoff_Level.text);
        dataSource.setCutoff0Parameters(cutoff0_on.checked, cutoff0_Level.text);
        updateSingleWaveform();
    }
    function buttonOnCircleClicked(index) {
        rect_circleSelector.visible = false;

        //comboBox_selectIP.currentIndex = comboBox_selectIP.model.indexOf(dataSource.getIP());
        //dataSource.selectIP(ip);
        //comboBox_selectEmitter.currentIndex = comboBox_selectEmitter.model.indexOf(dataSource.getEmitter());
        //dataSource.selectEmitter(emitter);
        //comboBox_selectRow.currentIndex = comboBox_selectRow.model.indexOf(dataSource.getRow());
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

