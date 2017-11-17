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

#include <QtDataVisualization>
#include <QAbstract3DSeries>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

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
#include <QVector>
#include <QHostAddress>

#include "measurement.h"
#include "measurementmodel.h"

#include <QChartView>

#include "constants.h"

class Peak;
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

#include <QReadWriteLock>
/*
typedef struct {
    double distance;
    QVector<double> _sqerr;
    QVector<double> _corr;
    QVector<double *> buffer;
} measurement;*/


#define theValue(x, e, d) (m_useZeroSignal?(m_useAbsoluteValues?fabs(fabs(x)-fabs((*(m_measurementModel->get(m_zeroIndex)->getBuffer()))[e][d])):(x-(*(m_measurementModel->get(m_zeroIndex)->getBuffer()))[e][d])):(m_useAbsoluteValues?fabs(x):x))

/*m_useZeroSignal
m_zeroIndex
m_measurementModel->get(m_zeroIndex)*/

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QQuickView *appViewer, QObject *parent = 0);
    ~DataSource();
    Q_PROPERTY(int currentUnitIndex MEMBER m_currentUnitIndex READ getCurrentUnitIndex WRITE setCurrentUnitIndex NOTIFY currentUnitIndexChanged);
Q_SIGNALS:
signals:
    void changeText(QString text);
    void currentUnitIndexChanged();
    void fileLoaded();
public slots:
    Q_INVOKABLE int getCurrentUnitIndex();

    void readData(int buffer_part, QByteArray *buffer, QHostAddress sender);
    Q_INVOKABLE void update(bool show, int r, int i_max, int i_slice, int i_proc, int d1=10, int d2=20);
    Q_INVOKABLE int getMaxDistance(int measurementIndex, int receiverIndex);

    void updateAllWaveforms(QAbstractSeries *m_series, int set);
    void updateAllWaveforms();
    void updateDistances(QAbstractSeries *m_series, int i, int set);
    void updateDistances();
    void updateSurface3D(QtDataVisualization::QAbstract3DSeries *m_series);

    void initCorrelationParameters(float  sigTau, float Fdskr);

    void processSignal(QVector <double> &in, QVector <double> &out);
    void processSignal(Measurement *m);
    void processSignal(Measurement *m, int e);
    void processSignal();

    void savePoint(double distance, int nf, int saveAsZeroSignal);
    void startRecording(QString fbasename);


    void setUseZeroSignal(int s);

    double getValue(QString name);
    void setValue(QString name, double value);

    void showByIndex(int index);
    void updateListView();

    void setMeasurementModel(MeasurementModel *mm);

    int getMaxCorrelationShift(QVector<double> a, QVector<double> b);

    QVector<QVector<float> > *getScanData();

    Q_INVOKABLE void selectIP(QString v);
    Q_INVOKABLE void selectEmitter(QString v);
    Q_INVOKABLE void selectRow(QString v);




    Q_INVOKABLE int getBufferSize();

    Q_INVOKABLE void resetScanIndex();
    Q_INVOKABLE void setScanIndex(int scanIndex);

    Q_INVOKABLE void setAllWaveformsSeries(QAbstractSeries *m_series, int set);
    Q_INVOKABLE void setSeries(QAbstractSeries *m_series, int i);
    Q_INVOKABLE void setDistanceSeries(QAbstractSeries *m_series, int i);

    Q_INVOKABLE double getScanValue(QVector<float> *data, QVector<float> *data0, int i, bool m_useAbsoluteValues);

    Q_INVOKABLE void textChanged(QString text);

    Q_INVOKABLE QString getReceiverName(int index);

    Q_INVOKABLE QString getIP(int index);
    Q_INVOKABLE QString getEmitter(int index);
    Q_INVOKABLE QString getRow(int index);

    Q_INVOKABLE QString getIP();
    Q_INVOKABLE QString getEmitter();
    Q_INVOKABLE QString getRow();

    Q_INVOKABLE void setCurrentUnitIndex(int index);
    Q_INVOKABLE void copyToHistory();
    Q_INVOKABLE void saveMMAndHistory();

    Q_INVOKABLE void setHasData(bool hd);
    Q_INVOKABLE void loadHistoryFromFile(QString fname);
    //Q_INVOKABLE void newScan();
    Q_INVOKABLE void SetDistance(float distance);
    Q_INVOKABLE void updateCorrelationChart(QtDataVisualization::QAbstract3DSeries *m_series);

    Q_INVOKABLE void collapseMMAndHistory(int block_size);

    Q_INVOKABLE void copyHistoryToClipboard(int e_start, int e_end, int r_start, int r_end);
    Q_INVOKABLE void setWriteHistory(bool wh);

    Q_INVOKABLE void copyToSharedMemory();
    Q_INVOKABLE void setSurface3DScanStep(int step);
    Q_INVOKABLE int getNChannels();

    Q_INVOKABLE void setFilterParameters(bool hpOn, int hpFOrd, double hpFc, double hpTd,
                                         bool lpOn, int lpFOrd, double lpFc, double lpTd,
                                         bool bpOn, int bpFOrd, double bpFc, double bpTd, double bpDeltaF, bool m_avgOn, int m_avgStep);

    Q_INVOKABLE void clearData();

    Q_INVOKABLE void setUseAbsoluteValues(bool flag);

    Q_INVOKABLE void setPhysicalParameter0(QString val);
    Q_INVOKABLE QString getPhysicalParameter0();

    Q_INVOKABLE void setCutoffParameters(bool cutoffOn, double cutoffLevel);
     Q_INVOKABLE void setCutoff0Parameters(bool cutoffOn, double cutoffLevel);

    Q_INVOKABLE QPointF getMaxAt(int measurementIndex, int receiverIndex, int setIndex, int dStart, int dEnd);
   //Q_INVOKABLE int getMaxIndexAt(int measurementIndex, int receiverIndex, int setIndex, int dStart, int dEnd);
private:
    bool m_cutoff0On;
    double m_cutoff0Level;

    bool m_cutoffOn;
    double m_cutoffLevel;

    QString m_physicalParameter0;

    bool m_hpOn;
    int m_hpFOrd;
    double m_hpFc;
    double m_hpTd;

    bool m_lpOn;
    int m_lpFOrd;
    double m_lpFc;
    double m_lpTd;

    bool m_bpOn;
    int m_bpFOrd;
    double m_bpFc;
    double m_bpTd;
    double m_bpDeltaF;

    bool m_avgOn;
    int m_avgStep;


    int m_surface3DScanStep {10};
    QSharedMemory sharedMemory;

    bool m_useAbsoluteValues;
    //QChartView *chartView;

    QMimeData *m_mimeData {nullptr};

    bool m_writeHistory {false};
    void m_setCurrentSet_0();

   //bool testScanIndex();

    //
    double m_fc{1e+9};
    double m_deltaf;
    unsigned short m_ford {8};
    double m_Td {1e-9};

    void m_findMaxLess(QVector<float> *data, QVector<int>  *stat_data, QVector<Peak> *peak_data, int margin_left, int margin_right);
    float m_findMaxLess(QVector<float> *data, float v, int *index);
    QVector<QAbstractSeries *> m_series;
    QVector<QAbstractSeries *> m_distanceSeries;
    QVector<QAbstractSeries *> m_allWaveformsSeries;

    QVector<float> m_receiverLevels;


    //int m_step {4};
    //QVector<QVector<QPointF>> m_points;

    int m_currentUnitIndex {0};
    QString m_ipNum {"1"}, m_emitterNum {"0"}, m_rowNum {"0"};

    QVector<QVector<QVector<float>>> m_historyScanData;
    int m_historyIndex {0};
    void m_increaseHistoryIndex();
    bool m_hasData {false};

    int m_historyDepth {330};

    int m_bufferSize {_bufferSize};
    //QVector<QMap<int, QVector<unsigned short>>> full_scan_data;

    //int m_maxScanIndex {m_bufferSize * 8};

    int m_scanIndex {0};

    QVector<QVector<float>> m_scanData;

    QVector<QVector<float>> *m_currentSet0 {nullptr};
    QVector<QVector<int>> *m_currentMaxIndexStat {nullptr};


    int m_maxChange {2000};
    QVector<float> m_lastGoodValue;



    bool m_useZeroSignal {false};
    //bool m_useAbsoluteValues {false};


    int _getUnitIndex(int buffer_part, QByteArray *buffer, QHostAddress sender);
    //int _maxChannels {32};

    MeasurementModel *m_measurementModel {nullptr};
    int m_zeroIndex {0};
    void m_clearMeasurementModel();

    double _LP0_Td {1.0/(100.0*1E9)}, _LP0_fc {1000*1E6}, _LP0_ford {8};
    double _HP0_Td {1.0/(100.0*1E9)}, _HP0_fc {300*1E6}, _HP0_ford {2};
    double _LP1_Td {1.0/(100.0*1E9)}, _LP1_fc {1000*1E6}, _LP1_ford {2};



    QQuickItem  *_object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QVector<QVector<QPointF> > m_data;

    QVector<double> m_distances;

    int m_index{0};

    QReadWriteLock mtx;
    QReadWriteLock dst_lock;
    QReadWriteLock surface_data_lock;
    //QReadWriteLock surface_data_lock;

    QFile *m_datafile {nullptr}, *m_markupfile {nullptr}, *m_pointfile {nullptr}, *zerofile {nullptr};

    //QVector<QVector<double>> X, Y;
    //QVector<tk::spline> s;

    QVector<std::queue<int>> start_pos_acc;
    QVector<std::queue<int>> object_pos_acc;

    QString fname {""}, zerofilename;

    int m_nChannels {_nChannels};

    int nframes {-1};
    //QVector<int> fcount;
    float distance {-1.0};
    int is_measured {0};

    float sigTau;
    float Fdskr;
    //float Td;
    float kt_dt;
    float Fc;
    float* signal {nullptr};
    //QVector<double> buffer_in;
    //QVector <double> buffer_out;
    //QVector<double *> raw_acc {nullptr};
    //QVector<double *> processed_acc {nullptr};

    //QVector<Measurement *> _data;

    //QVector<double *> zero_signal;
    //int saveAsZeroSignal {0};
    //int subtractZeroSignal {0};
    //int useFilter {1};

    static bool IsPowerOfTwo(ulong x);
    void calcCorrelationFunc(QVector <double> &in, QVector <double>&out, float *corrfunct, int n__corr, int numsmpl);
    static void MakeHPButterworthFilter(vectorc& H, double m_Td, double m_fc, unsigned short m_ford);
    static void MakeLPButterworthFilter(vectorc& H, double m_Td, double m_fc, unsigned short m_ford);
    static void Make_BP_ButterworthFilter(vectorc& H, double m_fc, double m_deltaf, unsigned short m_ford, double m_Td);
    static void MakeSimpleCutoffFilter(vectorc& H, double cutoff);
    static void MakeSimpleCutoff0Filter(vectorc& H, double cutoff0);
    void cfft(vectorc& a);
    void icfft(vectorc& a);

    static int _FindMaxValueInRangeOFArray(QVector <double> smp, int numsmp, int lowlim, int highlim);

    void accumulateChannel(int b_index);

    void clearMeasurementData();


    void compareToData(int channel = -1);


};

#endif // DATASOURCE_H
