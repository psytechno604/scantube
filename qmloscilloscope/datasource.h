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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>
#include <QFile>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

#include <QByteArray>
#include <QDataStream>
#include "shared_mutex.h"
#include <QFile>
#include <QQuickItem>

#include <stdio.h>
#include <math.h>
#include <limits.h>

//#include <vector>
#include <algorithm>
#include <complex>
#include <queue>
#include <map>

/*
QVector
typedef std::vector<double> vectord;
typedef std::complex<double> complexd;
typedef std::vector<complexd> vectorc;*/
typedef QVector<double> vectord;
typedef std::complex<double> complexd;
typedef QVector<complexd> vectorc;

#include "fft.h"
#include "spline.h"

typedef struct {
    double distance;
    QVector<double *> buffer;
} measurement;

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QQuickView *appViewer, QObject *parent = 0);
    ~DataSource();
Q_SIGNALS:
signals:
    void changeText(QString text);
public slots:
    void showFromBuffer(int b_index);
    void generateData(QByteArray *buffer, int row);
    void update(QAbstractSeries *series);

    void init_correlate_parameters(float  sigTau, float Fdskr);

    void process_signal(double *in, double *out, int *StartPosIndex, int *ObjectPosIndex, int numadc);

    void save_point(double distance, int nf, int saveAsZeroSignal);
    void start_recording(QString fbasename);

    void open_file(QString openfname);

    int get_channel_shift(int c);
    void set_channel_shift(int c, int sh);

    int getSubtractZeroSignal();
    void setSubtractZeroSignal(int s);

    int getUseFilter();
    void setUseFilter(int uf);

    double getValue(QString name);
    void setValue(QString name, double value);

    void showByIndex(int index);
private:
    double _LP0_Td {1.0/(100.0*1E9)}, _LP0_fc {1000*1E6}, _LP0_ford {8};
    double _HP0_Td {1.0/(100.0*1E9)}, _HP0_fc {300*1E6}, _HP0_ford {2};
    double _LP1_Td {1.0/(100.0*1E9)}, _LP1_fc {1000*1E6}, _LP1_ford {2};

    int _N{727};
    QQuickItem  *object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QVector<QVector<QPointF> > m_data;
    int m_index{0};
    shared_mutex mtx;
    QFile *datafile {nullptr}, *markupfile {nullptr}, *pointfile {nullptr}, *zerofile {nullptr};

    QVector<QVector<double>> X, Y;
    QVector<tk::spline> s;

    QVector<std::queue<int>> start_pos_acc;
    QVector<std::queue<int>> object_pos_acc;

    QString fname {""}, zerofilename;

    int nchannels {2};

    int nframes {-1};
    QVector<int> fcount;
    float distance {-1.0};
    int is_measured {0};

    float sigTau;
    float Fdskr;
    float Td;
    float kt_dt;
    float Fc;
    float* signal {nullptr};
    double *buffer_in {nullptr};
    double *buffer_out {nullptr};
    QVector<double *> raw_acc {nullptr};
    QVector<double *> processed_acc {nullptr};
    int buffer_size {_N};
    QVector<int> shY;
    QVector<measurement> _data;

    QVector<double *> zero_signal;
    int saveAsZeroSignal {0};
    int subtractZeroSignal {1};
    int useFilter {1};

    bool IsPowerOfTwo(ulong x);
    void calc_correlate_func(double *in, double *out, float *corrfunct, int n__corr, int numsmpl);
    void Make_HP_ButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford);
    void Make_LP_ButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford);
    void cfft(vectorc& a);
    void icfft(vectorc& a);

    int _FindMaxValueInRangeOFArray(double* smp, int numsmp, int lowlim, int highlim);

    void accumulateChannel(int b_index);

    void clearMeasurementData();
};

#endif // DATASOURCE_H
