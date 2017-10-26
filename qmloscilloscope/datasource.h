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

typedef struct {
    double distance;
    QVector<double> _sqerr;
    QVector<double> _corr;
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
    void showFromBuffer(int b_index, int block);
    void readData(int buffer_part, QByteArray *buffer, QHostAddress sender);
    void update(QAbstractSeries *series, QAbstractSeries *scatter);
    void update();
    void updateAllWaveforms(QAbstractSeries *series, int set);
    void updateAllWaveforms();
    void updateDistances(QAbstractSeries *series, int i, int set);
    void updateDistances();
    void updateSurface3D(QtDataVisualization::QAbstract3DSeries *series);

    void initCorrelationParameters(float  sigTau, float Fdskr);

    void processSignal(QVector <double> &in, QVector <double> &out);
    void processSignal(Measurement *m);
    void processSignal();

    void savePoint(double distance, int nf, int saveAsZeroSignal);
    void startRecording(QString fbasename);


    void openFile(QString openfname);

    int getSubtractZeroSignal();
    void setSubtractZeroSignal(int s);

    int getUseFilter();
    void setUseFilter(int uf);

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

    Q_INVOKABLE void saveAsScanData0();

    Q_INVOKABLE void calcDistances();

    Q_INVOKABLE double getCurrentDistance();
    Q_INVOKABLE double getReceiverLevel(int receiver);

    Q_INVOKABLE void setSaveDistance0(bool val);

    Q_INVOKABLE int getBufferSize();

    Q_INVOKABLE void resetScanIndex();
    Q_INVOKABLE void setScanIndex();
    Q_INVOKABLE void setAllWaveformsSeries(QAbstractSeries *series, int set);
    Q_INVOKABLE void setSeries(QAbstractSeries *series, int i);
    Q_INVOKABLE void setDistanceSeries(QAbstractSeries *series, int i);

    Q_INVOKABLE double getScanValue(QVector<float> *data, QVector<float> *data0, int i, bool use_abs);

    Q_INVOKABLE void textChanged(QString text);

    Q_INVOKABLE QString getReceiverName(int index);

    Q_INVOKABLE QString getIP(int index);
    Q_INVOKABLE QString getEmitter(int index);
    Q_INVOKABLE QString getRow(int index);

    Q_INVOKABLE QString getIP();
    Q_INVOKABLE QString getEmitter();
    Q_INVOKABLE QString getRow();

    Q_INVOKABLE void setUnitIndex(int index);
    Q_INVOKABLE void copyToHistory();
    Q_INVOKABLE void saveMMAndHistory();

    Q_INVOKABLE void setHasData(bool hd);
    Q_INVOKABLE void loadHistoryFromFile(QString fname);
    //Q_INVOKABLE void newScan();
    Q_INVOKABLE void SetDistance(float distance);
    Q_INVOKABLE void updateCorrelationChart(QtDataVisualization::QAbstract3DSeries *series);

    Q_INVOKABLE void collapseMMAndHistory(int block_size);

    Q_INVOKABLE void copyHistoryToClipboard();
private:
    QMimeData *mimeData {nullptr};

    bool write_history {false};
    void setCurrentSet_0();

    bool testScanIndex();

    //
    double fc{1e+9};
    double deltaf;
    unsigned short ford {8};
    double Td {1e-9};

    void findMaxLess(QVector<float> *data, QVector<int>  *stat_data, QVector<Peak> *peak_data, int margin_left, int margin_right);
    float findMaxLess(QVector<float> *data, float v, int *index);
    QVector<QAbstractSeries*> series {nullptr};
    QVector<QAbstractSeries*> distanceSeries {nullptr};
    QVector<QAbstractSeries *> allWaveformsSeries;

    QVector<float> receiver_levels;


    int _step {4};
    QVector<QVector<QPointF>> _points;

    int currentUnitIndex {0};
    QString ipNum {"1"}, emitterNum {"0"}, rowNum {"0"};

    QVector<QVector<QVector<float>>> history_scan_data;
    int history_index {0};
    void increaseHistoryIndex();
    bool has_data {false};

    int history_depth {330};

    //QVector<QMap<int, QVector<unsigned short>>> full_scan_data;

    int max_scan_index {buffer_size * 8};

    QVector<int> scan_index;
    QVector<QVector<int>> max_index_stat;
    QVector<QVector<float>> scan_data;
    QVector<QVector<float>> scan_data_0;
    QVector<QVector<float>> processed_data;

    QVector<float> distance_from_mm;
    QVector<float> distance_weight_from_mm;

    QVector<QVector<float>> *current_set {nullptr};
    QVector<QVector<float>> *current_set_0 {nullptr};
    QVector<QVector<int>> *current_max_index_stat {nullptr};


    int max_change {2000};
    QVector<float> last_good_value;

    QVector<QVector<Peak>> peak_data;
    QVector<QVector<Peak>> *current_peak_data{nullptr};

    QVector<QVector<float>> distance_data;
    QVector<QVector<float>> distance_data_0;
    // 0 - distance from raw data
    // 1 - distance saved as 0
    bool use_distance_0 {false};

    double zero_distance {0.1};

    double receiver_multiplier {0.04};

    int distance_index {0};
    int saved_level_index {1};
    int raw_level_index {2};

    QVector<double> sum_of_values;

    QVector<double> zero_receiver_levels;

    bool use_scan_data_0 {false};
    QVector<bool> channel_data_received;

    //QVector<QVector<double>> X, Y;
    int inputsPerReceiver {8};
    int channelsInPacket {2};
    int getUnitIndex(int buffer_part, QByteArray *buffer, QHostAddress sender);
    int maxchannels {32};

    MeasurementModel *measurementModel {nullptr};
    int zeroIndex {0};
    void clearMeasurementModel();

    double _LP0_Td {1.0/(100.0*1E9)}, _LP0_fc {1000*1E6}, _LP0_ford {8};
    double _HP0_Td {1.0/(100.0*1E9)}, _HP0_fc {300*1E6}, _HP0_ford {2};
    double _LP1_Td {1.0/(100.0*1E9)}, _LP1_fc {1000*1E6}, _LP1_ford {2};

    int _N{363}; //TODO: remove hardcode!

    QQuickItem  *object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QVector<QVector<QPointF> > m_data;

    QVector<double> m_distances;

    int m_index{0};

    QReadWriteLock mtx;
    QReadWriteLock dst_lock;
    QReadWriteLock surface_data_lock;
    //QReadWriteLock surface_data_lock;

    QFile *datafile {nullptr}, *markupfile {nullptr}, *pointfile {nullptr}, *zerofile {nullptr};

    QVector<QVector<double>> X, Y;
    //QVector<tk::spline> s;

    QVector<std::queue<int>> start_pos_acc;
    QVector<std::queue<int>> object_pos_acc;

    QString fname {""}, zerofilename;

    int nchannels {64};

    int nframes {-1};
    QVector<int> fcount;
    float distance {-1.0};
    int is_measured {0};

    float sigTau;
    float Fdskr;
    //float Td;
    float kt_dt;
    float Fc;
    float* signal {nullptr};
    QVector<double> buffer_in;
    QVector <double> buffer_out;
    QVector<double *> raw_acc {nullptr};
    QVector<double *> processed_acc {nullptr};
    int buffer_size {_N};
    QVector<int> shY;
    QVector<Measurement *> _data;

    QVector<double *> zero_signal;
    int saveAsZeroSignal {0};
    int subtractZeroSignal {0};
    int useFilter {1};

    static bool IsPowerOfTwo(ulong x);
    void calcCorrelationFunc(QVector <double> &in, QVector <double>&out, float *corrfunct, int n__corr, int numsmpl);
    static void MakeHPButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford);
    static void MakeLPButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford);
    static void Make_BP_ButterworthFilter(vectorc& H, double fc, double deltaf, unsigned short ford, double Td);
    void cfft(vectorc& a);
    void icfft(vectorc& a);

    static int _FindMaxValueInRangeOFArray(QVector <double> smp, int numsmp, int lowlim, int highlim);

    void accumulateChannel(int b_index);

    void clearMeasurementData();


    void compareToData(int channel = -1);

    double corr(double *X, double *Y, int N);


};

#endif // DATASOURCE_H
