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
import QtCharts 2.1

//![1]
ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark
    property bool openGL: true
    property bool openGLSupported: true
    property alias yMax: axisY.max
    property alias yMin: axisY.min
    property alias y2Max: axisY2.max
    property alias y2Min: axisY2.min
    //property alias lineSeries1: lineSeries1
    property alias chartView: chartView
    property string color0: "#ff0000"
    property string color1: "green"
    property string color2: "#4488ff"

    property string color0_1: "#ff4444"
    property string color1_1: "green"
    property string color2_1: "#88aaff"
    onOpenGLChanged: {
        if (openGLSupported) {
            series("signal 1").useOpenGL = openGL;
            series("signal s1").useOpenGL = openGL;
            series("signal 2").useOpenGL = openGL;
            series("signal s2").useOpenGL = openGL;
            series("signal selected 1").useOpenGL = openGL;
            series("signal selected 2").useOpenGL = openGL;
        }
    }
    Component.onCompleted: {
        if (!series(0).useOpenGL) {
            openGLSupported = false
            openGL = false
        }
        console.log("series.length =" + series.length);
        dataSource.setSeries(max_i0, 0);
        dataSource.setSeries(max_i1, 1);
        dataSource.setSeries(max_i2, 2);
        dataSource.setSeries(slice_i0, 3);
        dataSource.setSeries(slice_i1, 4);
        dataSource.setSeries(slice_i2, 5);
        dataSource.setSeries(processed_i0, 6);
        dataSource.setSeries(processed_i1, 7);
        dataSource.setSeries(processed_i2, 8);

    }

    ValueAxis {
        id: axisY
        min: (appSettings.useAbsoluteValues?0:(-appSettings.maxSignalLevel))
        max: appSettings.maxSignalLevel
    }
    ValueAxis {
        id: axisY2
        min:(appSettings.useAbsoluteValues?0:(-appSettings.maxProcessedLevel))
        max:appSettings.maxProcessedLevel
    }


    ValueAxis {
        id: axisX
        min: appSettings.d0
        max: appSettings.dN
    }
    ScatterSeries {
           id:max_i0
           name: "Max " + appSettings.i0
           axisX: axisX
                   axisY: axisY
                   color: color0
       }
    ScatterSeries {
           id: max_i1
           name: "Max " + appSettings.i1
           axisX: axisX
                   axisY: axisY
                   color: color1
       }
    ScatterSeries {
           id: max_i2
           name: "Max " + appSettings.i2
           axisX: axisX
                   axisY: axisY
                   color: color2
       }
    LineSeries {
        id: slice_i0
        name: "Slice " + appSettings.i0
        axisX: axisX
        axisY: axisY
        useOpenGL: chartView.openGL
        color: color0
    }
    LineSeries {
        id: slice_i1
        name: "Slice " + appSettings.i1
        axisX: axisX
        axisY: axisY
        useOpenGL: chartView.openGL
        color: color1
    }
    LineSeries {
        id: slice_i2
        name: "Slice " + appSettings.i2
        axisX: axisX
        axisYRight: axisY
        useOpenGL: chartView.openGL
        color: color2
    }
    LineSeries {
        visible: show_i0_1.checked
        id: processed_i0
        name: "Processed " + appSettings.i0
        axisX: axisX
        axisYRight: axisY2
        useOpenGL: chartView.openGL
        color: color0_1
    }
    LineSeries {
        id: processed_i1
        visible: show_i1_1.checked
        name: "Processed " + appSettings.i1
        axisX: axisX
        axisYRight: axisY2
        useOpenGL: chartView.openGL
        color: color1_1
    }
    LineSeries {
        id: processed_i2
        visible: show_i2_1.checked
        name: "Processed " + appSettings.i2
        axisX: axisX
        axisYRight: axisY2
        useOpenGL: chartView.openGL
        color: color2_1
    }
//![1]

    //![2]
    Timer {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: false
        repeat: true
        onTriggered: {
            dataSource.update(chartView.series(0));
            /*dataSource.update(chartView.series(1));
            dataSource.update(chartView.series(2));
            dataSource.update(chartView.series(3));
            dataSource.update(chartView.series(4));
            dataSource.update(chartView.series(5));*/
        }
    }
    //![2]

    //![3]
    function changeSeriesType(type) {
        chartView.removeAllSeries();

        // Create two new series of the correct type. Axis x is the same for both of the series,
        // but the series have their own y-axes to make it possible to control the y-offset
        // of the "signal sources".
        if (type == "line") {
            var series1 = chartView.createSeries(ChartView.SeriesTypeLine, "signal 1", axisX, axisY);
            series1.useOpenGL = chartView.openGL
            var seriesS1 = chartView.createSeries(ChartView.SeriesTypeLine, "signal s1", axisX, axisY2);
            seriesS1.useOpenGL = chartView.openGL

            var series2 = chartView.createSeries(ChartView.SeriesTypeLine, "signal 2", axisX, axisY);
            series2.useOpenGL = chartView.openGL
            var seriesS2 = chartView.createSeries(ChartView.SeriesTypeLine, "signal s2", axisX, axisY2);
            seriesS2.useOpenGL = chartView.openGL

            var seriesSelected1 = chartView.createSeries(ChartView.SeriesTypeLine, "signal selected 1", axisX, axisY);
            seriesSelected1.useOpenGL = chartView.openGL
            var seriesSelected2 = chartView.createSeries(ChartView.SeriesTypeLine, "signal selected 2", axisX, axisY);
            seriesSelected1.useOpenGL = chartView.openGL

        } else {
            var series1 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal 1", axisX, axisY);
            series1.markerSize = 2;
            series1.borderColor = "transparent";
            series1.useOpenGL = chartView.openGL

            var seriesS1 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal s1", axisX, axisY2);
            seriesS1.markerSize = 2;
            seriesS1.borderColor = "transparent";
            seriesS1.useOpenGL = chartView.openGL

            var series2 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal 2", axisX, axisY2);
            series2.markerSize = 2;
            series2.borderColor = "transparent";
            series2.useOpenGL = chartView.openGL

            var seriesS2 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal s2", axisX, axisY2);
            seriesS2.markerSize = 2;
            seriesS2.borderColor = "transparent";
            seriesS2.useOpenGL = chartView.openGL

            var seriesSelected1 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal selected 1", axisX, axisY);
            seriesSelected1.markerSize = 2;
            seriesSelected1.borderColor = "transparent";
            seriesSelected1.useOpenGL = chartView.openGL

            var seriesSelected2 = chartView.createSeries(ChartView.SeriesTypeScatter, "signal selected 1", axisX, axisY);
            seriesSelected2.markerSize = 2;
            seriesSelected2.borderColor = "transparent";
            seriesSelected2.useOpenGL = chartView.openGL
        }
    }

    function createAxis(min, max) {
        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
                                  + min + "; max: " + max + " }", chartView);
    }
    //![3]

    function setAnimations(enabled) {
        if (enabled)
            chartView.animationOptions = ChartView.SeriesAnimations;
        else
            chartView.animationOptions = ChartView.NoAnimation;
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
}
