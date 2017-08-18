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

#include "datasource.h"
#include "shared_mutex.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_index(-1)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    //generateData(0, 5, 1024);
}

void DataSource::update(QAbstractSeries *series)
{
    mtx.lock_shared();
    if (series && m_data.count()>0) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        QVector<QPointF> points = m_data.at(0);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
    mtx.unlock_shared();
}

void DataSource::generateData(int type, int rowCount, int colCount)
{
    mtx.lock();
    // Remove previous data
    foreach (QVector<QPointF> row, m_data)
        row.clear();
    m_data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QVector<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(0);
            qreal y(0);
            switch (type) {
            case 0:
                // data with sin + random component
                y = qSin(3.14159265358979 / 50 * j) + 0.5 + (qreal) rand() / (qreal) RAND_MAX;
                x = j;
                break;
            case 1:
                // linear data
                x = j;
                y = (qreal) i / 10;
                break;
            default:
                // unknown, do nothing
                break;
            }
            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }
    mtx.unlock();
}

void DataSource::generateData(QByteArray *buffer, int row)
{
    mtx.lock();

    auto N = (*buffer).length();

    QVector<QPointF> points;
    points.reserve(N / 2 - 1);

    m_data.clear();

    for(auto i=2; i<N-1; i+=2) {
        unsigned short p = ((*buffer)[i+1] << 8) | (*buffer)[i];
        auto x = i/2;
        points.append(QPointF(x, p));
    }

    if (row < m_data.length())
        m_data[row] = points;
    else
        m_data.append(points);

    mtx.unlock();
}
