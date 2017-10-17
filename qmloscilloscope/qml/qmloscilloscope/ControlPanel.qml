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

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import IPsListModel 1.0
import QtQuick.Extras 1.4
//import QtQuick.Controls.Universal 2.0
import QtQuick.Window 2.3
import QtQuick.Dialogs.qml 1.0
import QtQuick.Templates 2.2
import QtWebEngine.Controls1Delegates 1.0
import QtQuick.Controls.Styles.Desktop 1.0
import QtQuick.Controls.Styles 1.0

Item {
    property alias openGLButton: openGLButton
    property alias antialiasButton: antialiasButton
    //spacing: 8
    Layout.fillHeight: true
    signal animationsEnabled(bool enabled)
    signal seriesTypeChanged(string type)
    signal refreshRateChanged(variant rate)
    signal signalSourceChanged(string source, int signalCount, int sampleCount)
    signal antialiasingEnabled(bool enabled)
    signal openGlChanged(bool enabled)
    signal sendOn()
    signal sendOff()
    signal selectMyIP(string ip)
    signal selectAccumulation(string acc)
    signal selectSpeed(string spd)
    height: 760
    property alias comboBox: comboBox
    property alias button: button
    property alias button1: button1
    property alias text1: text1
    property alias multiButton1: multiButton1
    property alias signalSourceButton: signalSourceButton
   property alias sampleCountButton: sampleCountButton
    property alias multiButton: multiButton
    width: 151



    Text {
        id: text1
        text: "TubeScan"
        font.pointSize: 15
        color: "white"
    }

    MultiButton {
        id: openGLButton
        text: "OpenGL: "
        items: ["false", "true"]
        currentSelection: 1
        onSelectionChanged: openGlChanged(currentSelection == 1);
    }


    MultiButton {
        id: multiButton1
        text: "Graph: "
        items: ["line", "scatter"]
        currentSelection: 0
        onSelectionChanged: seriesTypeChanged(items[currentSelection]);
    }


   MultiButton {
        id: signalSourceButton
        text: "Source: "
        items: ["sin", "linear"]
        currentSelection: 0
        onSelectionChanged: signalSourceChanged(
                                selection,
                                5,
                                sampleCountButton.items[sampleCountButton.currentSelection]);
    }


    MultiButton {
        id: sampleCountButton
        text: "Samples: "
        items: ["6", "128", "1024", "10000"]
        currentSelection: 2
        onSelectionChanged: signalSourceChanged(
                                signalSourceButton.items[signalSourceButton.currentSelection],
                                5,
                                selection);
    }


    MultiButton {
        id: multiButton
        text: "Refresh rate: "
        items: ["1", "24", "60"]
        currentSelection: 2
        onSelectionChanged: refreshRateChanged(items[currentSelection]);
    }


    MultiButton {
        id: antialiasButton
        text: "Antialias: "
        items: ["OFF", "ON"]
        enabled: true
        currentSelection: 0
        onSelectionChanged: antialiasingEnabled(currentSelection == 1);
    }


    ComboBox {
        id: comboBox
        model: myIPsListModel
        textRole: "text"
        onActivated: selectMyIP(currentText);
    }


    Button {
        id: button
        width: 40
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
        onClicked: sendOn();
    }



    Button {
        id: button1
        width: 86
        height: 40
        text: qsTr("Stop")
        font.pointSize: 8
        checkable: false
        onClicked: sendOff();
    }


    ComboBox {
        id: comboBox1
        height: 30
        model: ["1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024"]
        onActivated: selectAccumulation(currentText);
    }


    Slider {
        id: slider
        width: 151
        Layout.fillWidth: true
        stepSize: 1
        to: 194
        value: 0
    }



    Slider {
        id: slider1
        width: 151
        Layout.fillWidth: true
        stepSize: 1
        to: 95
        from: 80
        value: 80
    }




    ComboBox {
        id: comboBox2
        model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"]
        onActivated: selectSpeed(currentText);
    }

}
