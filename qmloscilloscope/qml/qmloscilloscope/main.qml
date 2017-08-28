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

//![1]
import QtQuick.Controls 2.2
Item {
    id: main
    width: 1024
    height: 768
    property alias column: column
    property alias controlPanel: controlPanel
    property alias text1Text: text1.text
    property alias text2Text: text2.text
    property alias text3Text: text3.text
    property alias text4Text: text4.text
    property alias text5Text: text5.text
    property alias text6Text: text6.text
    property alias pbValue: progressBar.value
    Row {
        id: row
        anchors.fill: parent
        z: -1

        ControlPanel {
            id: controlPanel
            width: 200
            height: parent.height
            spacing: 10.2
            /*anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 8*/
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

                ScopeView {
                    id: scopeView
                    width: 872
                    height: 650
                    anchors.fill: item1

                    onOpenGLSupportedChanged: {
                        if (!openGLSupported) {
                            controlPanel.openGLButton.enabled = false
                            controlPanel.openGLButton.currentSelection = 0
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
                    x: 196
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
                    x: 197
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
                    x: 529
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
                    x: 529
                    y: 118
                    width: 126
                    height: 36
                    color: "#ffffff"
                    text: qsTr("Text")
                    font.pixelSize: 25
                    anchors.bottomMargin: 46
                    anchors.bottom: parent.bottom
                }

                Slider {
                    id: slider
                    x: 52
                    y: 160
                    width: 772
                    height: 40
                    value: 0.5
                }

                CheckBox {
                    id: checkBox
                    x: 0
                    y: 160
                    width: 46
                    height: 40
                    text: qsTr("")
                }

                Text {
                    id: text5
                    x: 196
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
                    x: 529
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
                    id: button
                    width: 100
                    height: 36
                    text: qsTr("Fix")
                    anchors.left: parent.left
                    anchors.leftMargin: 23
                    anchors.top: parent.top
                    anchors.topMargin: 76
                    onClicked: sendFix();
                }

                TextInput {
                    id: textInput
                    x: 23
                    y: 34
                    width: 100
                    height: 36
                    text: qsTr("18.0")
                    cursorVisible: true
                    echoMode: TextInput.Normal
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 21
                }

            }

        }
    }

    ProgressBar {
        id: progressBar
        x: 222
        y: 581
        to: 0
        value: 0
    }
    function changeProgressBar(v) {
        pbValue = v*1
        return
    }
    function sendFix() {
        dataSource.save_point(textInput.text*1, 50);
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
}
