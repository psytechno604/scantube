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
Item {
    id: main
    width: 800
    height: 600
    property alias controlPanel: controlPanel


    //![2]

    Grid {
        id: grid
        width: main.width
        height: main.height

        ControlPanel {
            id: controlPanel
            width: 129
            spacing: 10.2
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 8
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
        }

        ScopeView {
            id: scopeView
            width: 683
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: controlPanel.right
            height: main.height

            onOpenGLSupportedChanged: {
                if (!openGLSupported) {
                    controlPanel.openGLButton.enabled = false
                    controlPanel.openGLButton.currentSelection = 0
                }
            }
        }
    }
    //![2]
}
