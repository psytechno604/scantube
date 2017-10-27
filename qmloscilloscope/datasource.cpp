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
#include <QDateTime>

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>

#include <QSplineSeries>
#include <QApplication>

#include "measurement.h"
#include "scantube.h"
#include "addressprovider.h"
#include "peak.h"

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

//template float pearsoncoeff<float>(QVector<float>, QVector<float>);

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer)
{
    series.resize(4);
    allWaveformsSeries.resize(2);
    distanceSeries.resize(6);

    if (!object)
        object = m_appViewer->rootObject();

    nchannels = AddressProvider::getReceiverCount() * inputsPerReceiver * channelsInPacket;
    //QQuickItem *item = qobject_cast<QQuickItem*>(object);

    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    datafile = new QFile("scantube.dat");
    markupfile = new QFile("scantube.csv");
    pointfile = new QFile("scantube.scan");

    if (!datafile->open(QFile::WriteOnly)){
        qDebug() << "datafile cannot be opened";
    }
    if (!markupfile->open(QFile::WriteOnly)){
        qDebug() << "markupfile cannot be opened";
    }

    //emit changeText("button clicked");


    m_data.resize(nchannels * 3);

    receiver_levels.resize(AddressProvider::getReceiverCount());

    history_scan_data.resize(history_depth);

    max_index_stat.resize(nchannels);
    scan_data.resize(nchannels);
    scan_data_0.resize(nchannels);
    distance_data.resize(nchannels);
    distance_data_0.resize(nchannels);
    last_good_value.resize(nchannels);
    processed_data.resize(nchannels);
    peak_data.resize(nchannels);

    distance_from_mm.resize(nchannels);
    distance_weight_from_mm.resize(nchannels);
    //for(auto i=0; i<nchannels; i++) {
        //scan_data.append(*(new QVector<unsigned short>()));
        //scan_data_0.append(*(new QVector<unsigned short>()));

        //distance_data.append(*(new QVector<double>(3)));
        //distance_data[i].fill(0.0);

//        if (!(i % _step))   {
//            _points.append(QPointF(i / _step, 0));
//        }
    //}
   sum_of_values.resize(nchannels); sum_of_values.fill(0);
   scan_index.resize(nchannels); scan_index.fill(0);
   max_scan_index = (buffer_size-1) * 7 + buffer_size;
   _points.resize(nchannels);

//   for(auto i=0; i<buffer_size; i+= _step)  {
//       _points.append(QPointF(i, 0));
//   }

    //generateData(0, 5, 1024);

    initCorrelationParameters(80*1E-12, 100*1E9);

    shY.push_back(1145);
    shY.push_back(-160);


    //buffer_in = new double[buffer_size];
    //buffer_out = new double[buffer_size];
    buffer_in.resize(buffer_size);
    buffer_out.resize(buffer_size);
    //buffer_acc.resize(nchannels);

    X.resize(nchannels);
    Y.resize(nchannels);
    //s.resize(nchannels);
    raw_acc.resize(nchannels);
    processed_acc.resize(nchannels);
    fcount.resize(nchannels);
    zero_signal.resize(nchannels);

    int T = 16;


    for(int c=0; c<nchannels; c++) {
        X[c].resize(T);
        Y[c].resize(T);
        //s[c] = *(new tk::spline());
        raw_acc[c] = new double[buffer_size];
        processed_acc[c] = new double[buffer_size];
        zero_signal[c] = new double[buffer_size];
        for (auto j=0; j<buffer_size; j++)  {
            zero_signal[c][j] = 0;
        }
    }

    int k = 0;

    X[0][k]=370;	Y[0][k]=13;		X[1][k]=370;	Y[1][k]=13; k++;
    X[0][k]=376;	Y[0][k]=14;		X[1][k]=376;	Y[1][k]=14; k++;

    X[0][k]=381;	Y[0][k]=15;		X[1][k]=380;	Y[1][k]=15; k++;

    X[0][k]=384.5;	Y[0][k]=16;		X[1][k]=384.5;	Y[1][k]=16; k++;

    X[0][k]=395;	Y[0][k]=17;		X[1][k]=393.5;	Y[1][k]=17; k++;
    X[0][k]=403.5;	Y[0][k]=18;		X[1][k]=398.5;	Y[1][k]=18; k++;
    X[0][k]=406.5;	Y[0][k]=19;		X[1][k]=407.5;	Y[1][k]=19; k++;
    X[0][k]=415;	Y[0][k]=20;		X[1][k]=418.5;	Y[1][k]=20; k++;
    X[0][k]=423.75;	Y[0][k]=21;		X[1][k]=425.5;	Y[1][k]=21; k++;
    X[0][k]=431;	Y[0][k]=22;		X[1][k]=431;	Y[1][k]=22; k++;
    X[0][k]=435;	Y[0][k]=23;		X[1][k]=432;	Y[1][k]=23; k++;
    X[0][k]=440;	Y[0][k]=24;		X[1][k]=437.5;	Y[1][k]=24; k++;
    X[0][k]=448.5;	Y[0][k]=25;		X[1][k]=445.5;	Y[1][k]=25; k++;
    X[0][k]=454;	Y[0][k]=26;		X[1][k]=455;	Y[1][k]=26; k++;
    X[0][k]=455;	Y[0][k]=26.5;		X[1][k]=459;	Y[1][k]=26.5; k++;
    X[0][k]=457;	Y[0][k]=27.2;		X[1][k]=465;	Y[1][k]=27.2; k++;

    /*for(int c=0; c<nchannels; c++) {
        s[c].set_points(X[c],Y[c]);
    }*/
}

DataSource::~DataSource()
{
    if (datafile) {
        datafile->close();
    }
    if (markupfile) {
        markupfile->close();
    }
}

void DataSource::showFromBuffer(int b_index, int block)
{
    if (!object)
        object = m_appViewer->rootObject();

    QVector<QPointF> points;
    points.reserve(buffer_size);

    //QVector<QPointF> pointsS;
    //pointsS.reserve(buffer_size);

    //int start_position, object_position;
    //process_signal(buffer_in, buffer_out, &start_position, &object_position, buffer_size);

    //compareToData(b_index);

    double myDistance = 0;
    if (measurementModel)
        myDistance = measurementModel->getDistance(b_index);

    for (auto i=0; i<buffer_size; i++) {
        points.append(QPointF(i, buffer_in[i]));
        //pointsS.append(QPointF(i, buffer_out[i]));
    }
    //markupfile->write(QByteArray("\n"));

    /*if (b_index == 0) {
        QMetaObject::invokeMethod((QObject*)object, "changeText5", Q_ARG(QVariant, QString::number(myDistance)));
        //QMetaObject::invokeMethod((QObject*)object, "changeText1", Q_ARG(QVariant, QString::number(object_position)));
        //QMetaObject::invokeMethod((QObject*)object, "changeText2", Q_ARG(QVariant, QString::number(s[b_index](object_position))));
    }
    if (b_index == 1) {
        QMetaObject::invokeMethod((QObject*)object, "changeText6", Q_ARG(QVariant, QString::number(myDistance)));
        //QMetaObject::invokeMethod((QObject*)object, "changeText3", Q_ARG(QVariant, QString::number(object_position)));
        //QMetaObject::invokeMethod((QObject*)object, "changeText4", Q_ARG(QVariant, QString::number(s[b_index](object_position))));
    }*/

    m_data[block * nchannels + b_index] = points;
    //m_data[nchannels + b_index] = pointsS;

    if (is_measured) accumulateChannel(b_index);

}

void DataSource::update(QAbstractSeries *series, QAbstractSeries *scatter)
{
    if (!current_set || (use_scan_data_0 && !current_set_0) || !current_peak_data) {
        return ;
    }
    qDebug() << "DataSource::update" << QThread::currentThreadId();
    mtx.lockForRead();

    /*if(series) {
        QVector<QPointF> points;
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        points.resize(buffer_size);
        for(auto i=0; i<buffer_size; i++) {
            points[i] =
        }
    }*/
    //auto step = 4;
    //QVector<QPointF> points;
    try {
        if (series) {
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);
            QXYSeries *xySeriesScatter = static_cast<QXYSeries *>(scatter);
            if (currentUnitIndex >= 0)    {
                //qDebug() << "update m_index=" << m_index;
                auto j=0;
                //auto avg = sum_of_values[currentUnitIndex]/scan_data[currentUnitIndex].length();
                for (auto i=0; i<(*current_set)[currentUnitIndex].length(); i+=_step) {
                    //_points.append(QPointF(i, (double(scan_data[currentUnitIndex][i]) - (use_scan_data_0?double(scan_data_0[currentUnitIndex][i]):0.0)) - avg_values[currentUnitIndex]));
                    if (j < _points[currentUnitIndex].length()) {
                        _points[currentUnitIndex][j].setX(i);
                        _points[currentUnitIndex][j].setY(getScanValue(&((*current_set)[currentUnitIndex]), (use_scan_data_0?(&((*current_set_0)[currentUnitIndex])):nullptr), i, false));
                        j++;
                    }
                }

                // if (m_index == 0)
                xySeries->replace(_points[currentUnitIndex]);

                QVector<QPointF> pointsScatter;
                for (auto k=0; k<(*current_peak_data)[currentUnitIndex].length(); k++) {
                    pointsScatter.append(QPointF((*current_peak_data)[currentUnitIndex][k].index, (*current_peak_data)[currentUnitIndex][k].value));
                }
                xySeriesScatter->replace(pointsScatter);

            }
            // Use replace instead of clear + append, it's optimized for performance

        }
    }
    catch (...)
    {

    }
    mtx.unlock();
}

void DataSource::update()
{
    current_set = &scan_data;



    update(series[0], series[1]);

    current_set = &processed_data;
    update(series[2], series[3]);
}
void DataSource::updateAllWaveforms(QAbstractSeries *series, int set)
{
    qDebug() << "DataSource::updateAllWaveforms" << QThread::currentThreadId();
    mtx.lockForRead();


    auto dss = static_cast<QSplineSeries *>(series);

    QVector<QPointF> points;

    //dss->replace()

    int k=0;

    unsigned short med = 0x8000;

    //int step = 32;

    for(auto e=0; e < scan_data.length(); e++)  {
        if ((e%2) == set)    {
            double avg = 0, maxabs = 0;
            for (auto i=0; i < scan_data.value(e).count(); i++) {
                double tmp = use_scan_data_0?(scan_data[e][i] - scan_data_0[e][i]):(scan_data[e][i]);
                avg += tmp;
                if (abs(tmp) > maxabs)
                    maxabs = abs(tmp);
            }
            /*for (auto i=0; i < scan_data.value(e).count(); i++) {
                double tmp = use_scan_data_0?(scan_data[e][i] - scan_data_0[e][i]):(scan_data[e][i]);
                avg += tmp;
                if (abs(tmp) > maxabs)
                    maxabs = abs(tmp);
            }*/
            avg = avg / scan_data[e].length();
            for (auto j=0; j < 20; j++) {
                auto i = j * scan_data[e].length() / 20;
                if (i < scan_data[e].length()) {
                    float v = 2250 - 500 * (set)+(float(use_scan_data_0?(scan_data[e][i] - scan_data_0[e][i]):(scan_data[e][i])) - avg);

                    points.append(QPointF(k++, v));
                }
            }
        }
    }
    mtx.unlock();

    dss->replace(points);


}

void DataSource::updateAllWaveforms()
{
    for(auto i=0; i<allWaveformsSeries.length(); i++) {
        if (allWaveformsSeries[i]) {
            updateAllWaveforms(allWaveformsSeries[i], i);
        }
    }
}

void DataSource::updateDistances(QAbstractSeries *series, int i, int set)
{
    if (!current_peak_data)
        return;

    dst_lock.lockForRead();


    auto dss = static_cast<QSplineSeries *>(series);
    QVector<QPointF> points;
    int j=0;
    if (set == 0 || set ==1) {

        for(auto e=set; e < current_peak_data->length(); e+=2)  {
            if ((*current_peak_data)[e].length()>0 && (*current_peak_data)[e][i].index>=0) {
                points.append(QPointF(j+0.5, (*current_peak_data)[e][i].index));
                j++;
            }
        }

    }

    //points.append(QPointF(-0.5, distance_data[set][i]));
    dss->replace(points);
    dst_lock.unlock();
}

void DataSource::updateDistances()
{
    updateDistances(distanceSeries[0], 0, 0);
    updateDistances(distanceSeries[1], 1, 0);
    updateDistances(distanceSeries[2], 2, 0);
    updateDistances(distanceSeries[3], 0, 1);
    updateDistances(distanceSeries[4], 1, 1);
    updateDistances(distanceSeries[5], 2, 1);
}

void DataSource::updateSurface3D(QtDataVisualization::QAbstract3DSeries *series)
{
    setCurrentSet_0();
    //auto sampleCountZ = 727;
    //auto sampleCountX = 101;
    //qDebug() << history_scan_data[0].length();

    if (history_scan_data[0].length() == 0 || (use_scan_data_0 && !current_set_0))   {
        return;
    }

    surface_data_lock.lockForRead();

    auto s3ds =  static_cast<QtDataVisualization::QSurface3DSeries *>(series);

    auto proxy = s3ds->dataProxy();

    //auto series = new QtDataVisualization::QSurface3DSeries(proxy);

    auto dataArray = new QtDataVisualization::QSurfaceDataArray();

    auto maxI = 0;
    for (auto h=0; h<history_scan_data.length(); h++)   {
        if (history_scan_data[h].length()>currentUnitIndex)    {
            if (history_scan_data[h][currentUnitIndex].length() > maxI) {
                maxI = history_scan_data[h][currentUnitIndex].length();
            }
        }
    }

    dataArray->reserve(maxI);

    for (int i = 0 ; i < maxI ; i++) {
        QtDataVisualization::QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(measurementModel->rowCount());
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = i/10.0; //"Distance"
        int index = 0;

        for (int j = 0; j < measurementModel->rowCount(); j++) {
            auto m = measurementModel->get(j);
            auto b = m->getBuffer();
            float x = j; // "Scan"
            float y = 0;

            if (b->length() > currentUnitIndex && (*b)[currentUnitIndex].length()>i) {
                if (use_scan_data_0)
                    y = fabs(fabs((*b)[currentUnitIndex][i]) - fabs((*current_set_0)[currentUnitIndex][i])); // "Level"
                else
                    y = fabs((*b)[currentUnitIndex][i]);
            }

            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    proxy->resetArray(dataArray);

    //proxy->

    //proxy->
    //s3ds

    surface_data_lock.unlock();
}
void DataSource::updateCorrelationChart(QtDataVisualization::QAbstract3DSeries *series)
{
    setCurrentSet_0();
    //auto sampleCountZ = 727;
    //auto sampleCountX = 101;
    //qDebug() << history_scan_data[0].length();

    if (history_scan_data[0].length() == 0 || (use_scan_data_0 && !current_set_0))   {
        return;
    }

    //compareToData(-1);

    surface_data_lock.lockForRead();

    auto s3ds =  static_cast<QtDataVisualization::QSurface3DSeries *>(series);

    auto proxy = s3ds->dataProxy();

    //auto series = new QtDataVisualization::QSurface3DSeries(proxy);

    auto dataArray = new QtDataVisualization::QSurfaceDataArray();

    auto maxI = 100;

    dataArray->reserve(maxI);

    for (int i = 0 ; i < maxI ; i++) {
        QtDataVisualization::QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(measurementModel->rowCount());
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = i; //"Distance"
        int index = 0;

        for (int j = 0; j < measurementModel->rowCount(); j++) {
            auto m = measurementModel->get(j);

            float x = j; // "Scan"
            float y = 0;
            if (m->_corr.length() > currentUnitIndex && currentUnitIndex >= 0)  {
                y = m->_corr[currentUnitIndex] * 1000;
            }


            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    proxy->resetArray(dataArray);

    //proxy->

    //proxy->
    //s3ds

    surface_data_lock.unlock();
}

void DataSource::collapseMMAndHistory(int block_size)
{
    if (block_size <= 0)
        return;

    for (auto i=0; i<measurementModel->rowCount(); i+=block_size)   {
        auto m = measurementModel->get(i);
        auto m_ = measurementModel->get(i/block_size);
        m_->distance = m->distance;
        m_->_corr.fill(0);
        auto b = m->getBuffer();
        //auto b_ = m_->getBuffer();
        for(auto e=0; e<b->length(); e++) {
            for (auto d=0; d<(*b)[e].length();d++){
                float summa = 0.0f;
                for (auto i_ = i; i_<i+block_size; i_++)    {
                    summa += measurementModel->getValue(i_, e, d);
                }
                measurementModel->setValue(i/block_size, e, d, summa / block_size);
            }
        }
    }
    measurementModel->clear(measurementModel->rowCount() / block_size);
}

void DataSource::copyHistoryToClipboard()
{
    if (mimeData)   {
        delete mimeData;
    }
    mimeData = new QMimeData();

    QByteArray itemData;
    QTextStream dataStream(&itemData, QIODevice::WriteOnly);

    if(measurementModel->rowCount() <= 0)   {
        return;
    }

    auto m = measurementModel->get(0);
    auto b = m->getBuffer();

    auto m0 = measurementModel->get(zeroIndex);
    auto b0 = m0->getBuffer();




    for(int d=0; d<(*b)[currentUnitIndex].length(); d++) {
        for(int r=0; r<measurementModel->rowCount(); r++) {
            b = measurementModel->get(r)->getBuffer();
            QString str = QString::number(fabs((*b)[currentUnitIndex][d] - (use_scan_data_0?((*b0)[currentUnitIndex][d]):0)));
            dataStream << str;
            if (r < measurementModel->rowCount()-1) {
                dataStream << ',';
            }
            else {
                dataStream << '\n';
            }
        }
    }

    mimeData->setData("csv", itemData);
    QApplication::clipboard()->setMimeData(mimeData);
}

void DataSource::setWriteHistory(bool wh)
{
    write_history = wh;
}
void DataSource::calcCorrelationFunc(QVector <double> &in, QVector <double> &out, float *corrfunct, int n__corr, int numsmpl)
{
    int i, j, offs;
    float sum;

    for(i = 0; i < numsmpl; i++) // считаем для отсчёта    s
    {
        sum=0;
        out[i] = 0;
        for(j = 0; j < n__corr; j++)
        {
            offs = j + i;
            if(offs >= numsmpl)
            {
                break;
            }
            sum += corrfunct[j] * in[offs];
        }
        sum /= (float)n__corr;
        out[i]=fabs(sum);
        //qDebug(QString::number(out[i]).toUtf8());
    }
}

void DataSource::initCorrelationParameters(float sigTau, float Fdskr)
{
    if (sigTau == 0 || Fdskr == 0)
        return;

    this->sigTau = sigTau;
    this->Fdskr = Fdskr;
    Td = 1.0/Fdskr;
    kt_dt = (sigTau*1 / Td)*2;

    signal = new float[kt_dt];
    for (int i=0;i<kt_dt;i++)
    {
        signal[i]=-sin(2.0*M_PI/kt_dt*i);
    }
    Fc = 1*1E6;
}
bool DataSource::IsPowerOfTwo(ulong x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}
void DataSource::processSignal(QVector <double> &in, QVector <double> &out)
{
    int i,j,x;

    vectorc dblbubl;
    vectorc H;
    int ndaln2=in.length();

    if (out.length()<in.length()) {
        out.resize(in.length());
    }

    while(!IsPowerOfTwo(ndaln2)) ndaln2++;
    ndaln2 = ndaln2*1;
    H.resize(ndaln2);
    dblbubl.resize(ndaln2);
    //Fc = 1000 * 1E6;

    for (auto i=0; i<in.length(); i++)    {
        out[i] = in[i];
    }



    if(1)
    {
        //MakeLPButterworthFilter(H, _LP0_Td,_LP0_fc, _LP0_ford);

        Make_BP_ButterworthFilter(H, fc, deltaf, ford, Td);

        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
        for(j=0;j<in.length();j++) dblbubl[j] = complexd( out[j],0 );

        cfft(dblbubl);
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        icfft(dblbubl);

        for(x=0;x<ndaln2;x++)
        {
            if(x<in.length()) out[x] = 1 * dblbubl[x].real();
        }
    }
    //----

    //---- фильтр ВЧ (для убирания пост. составл. в сигнале)
    if(0)
    {
        MakeHPButterworthFilter(H, _HP0_Td,_HP0_fc, _HP0_ford);

        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
        for(j=0;j<in.length();j++) dblbubl[j] = complexd( out[j],0 );

        cfft(dblbubl);
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        icfft(dblbubl);

        for(x=0;x<in.length();x++)
        {
            out[x] = 1 * dblbubl[x].real();
        }
    }
    //----
    //---- включим "диод" (вычислим модуль сигнала
    if(1)
    {
        for(j=0;j<out.length();j++) out[j] = fabs(out[j]);
    }
    //----
//    //---- фильтр НЧ для огибающей
//    if(0)
//    {
//        MakeLPButterworthFilter(H, _LP1_Td,_LP1_fc, _LP1_ford);

//        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
//        for(j=0;j<in.length();j++) dblbubl[j] = complexd( out[j],0 );

//        cfft(dblbubl);
//        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
//        reverse(dblbubl.begin(),dblbubl.end());
//        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
//        reverse(dblbubl.begin(),dblbubl.end());
//        icfft(dblbubl);

//        for(x=0;x<in.length();x++)
//        {
//            out[x] = 1 * dblbubl[x].real();
//        }
//    }
}

void DataSource::processSignal(Measurement *m)
{
    if (!m)
        return;

    auto b = m->getBuffer();
    auto pb = m->getPBuffer();
    if (pb->length()<b->length())   {
        pb->resize(b->length());
    }
    for (auto e=0; e<b->length(); e++) {
        if ((*pb)[e].length() < (*b)[e].length()) {
            (*pb)[e].resize((*b)[e].length());
        }
        QVector<double> in;
        for (auto d=0; d<(*b)[e].length(); d++) {
            in.push_back((*b)[e][d]);
        }
        QVector<double> out;
        processSignal(in, out);

        for (auto d=0; d<out.length(); d++)  {
            (*pb)[e][d] = out[d];
        }
    }
}

void DataSource::processSignal()
{
    for(int r=0; r< measurementModel->rowCount(); r++) {

    }
}

void DataSource::savePoint(double distance, int nframes, int saveAsZeroSignal)
{

    if (pointfile && pointfile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        QDataStream out(pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        out << distance;
        pointfile->close();
    }

}

void DataSource::startRecording(QString fbasename)
{
//    QString strN = QString::number(_N);

//    fname = fbasename + "_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_" + strN + ".dat";//  + "_" + QString("%1").arg(QString::number(distance, 'f', 2).toDouble(), 5, 10, QChar('0'))+QString(".bin");
//    fname = fname.replace(":", "_");
//    if (pointfile)
//        delete pointfile;
//    pointfile = new QFile(fname);
//    if (pointfile && pointfile->open(QIODevice::WriteOnly/* | QIODevice::Append*/)) {
//        QDataStream out(pointfile);
//        out.setVersion(QDataStream::Qt_5_9);
//        out << nchannels;
//        out << _N;
//        pointfile->close();
//    }
}

void DataSource::openFile(QString openfname)
{    
    QFile file(openfname);
    if (file.open(QIODevice::ReadOnly))    {
        QDataStream fileStream(&file);

        int _nchannels, _n;
        fileStream >> _nchannels;
        fileStream >> _n;
        if (_nchannels > 0 && _n > 0)   {
            //clearMeasurementData();
            clearMeasurementModel();
            /*auto fSize = file.size();
            auto sizeof_int = sizeof(int);
            auto sizeof_double = sizeof(double);*/
            auto nMeasurements = (file.size() - 2 * sizeof(int)) / (_nchannels * (_n  * sizeof(double) + sizeof(int)) + sizeof(double));
            qDebug() << "nMeasurements = " << nMeasurements;

            for(auto m=0; m<nMeasurements; m++){
                double distance;

                fileStream >> distance;
                Measurement *_M = new Measurement(distance, _nchannels, _n);
                for(auto c=0; c<_nchannels; c++)    {
                    int channel;
                    fileStream >> channel;
                    if (channel >= _nchannels)  {
                        qDebug() << "invalid channel number";
                        file.close();
                        return;
                    }
                    //_M->buffer[channel] = new double[_n];
                    for(auto i=0; i<_n; i++)    {
                        fileStream >> (*(_M->getBuffer()))[channel][i];
                    }
                }
                measurementModel->add(_M);
            }
        }
        else {
            qDebug() << "Wrong packet size";
        }
        file.close();
        //showByIndex(0);
        updateListView();
    }
}



int DataSource::getSubtractZeroSignal()
{
    return subtractZeroSignal;
}

void DataSource::setSubtractZeroSignal(int s)
{
    subtractZeroSignal = s;
    use_scan_data_0 = s;
}

int DataSource::getUseFilter()
{
    return useFilter;
}

void DataSource::setUseFilter(int uf)
{
    useFilter = uf;
}

double DataSource::getValue(QString name)
{
    if (name == "textField_LP0_Td") return _LP0_Td;
    if (name == "textField_LP0_fc") return _LP0_fc;
    if (name == "textField_LP0_ford") return _LP0_ford;

    if (name == "textField_HP0_Td") return _HP0_Td;
    if (name == "textField_HP0_fc") return _HP0_fc;
    if (name == "textField_HP0_ford") return _HP0_ford;

    if (name == "textField_LP1_Td") return _LP1_Td;
    if (name == "textField_LP1_fc") return _LP1_fc;
    if (name == "textField_LP1_ford") return _LP1_ford;
}

void DataSource::setValue(QString name, double value)
{
    if (name == "textField_LP0_Td") { _LP0_Td = value; return; }
    if (name == "textField_LP0_fc") { _LP0_fc = value; return; }
    if (name == "textField_LP0_ford") { _LP0_ford = value; return; }

    if (name == "textField_HP0_Td") { _HP0_Td = value; return; }
    if (name == "textField_HP0_fc") { _HP0_fc = value; return; }
    if (name == "textField_HP0_ford") { _HP0_ford = value; return; }

    if (name == "textField_LP1_Td") { _LP1_Td = value; return; }
    if (name == "textField_LP1_fc") { _LP1_fc = value; return; }
    if (name == "textField_LP1_ford") { _LP1_ford = value; return; }
}

void DataSource::showByIndex(int index)
{
//    if (measurementModel && index>=0 && index < measurementModel->rowCount())   {
//        auto m = measurementModel->get(index);
//        if (!m)
//            return;
//        for (auto c=0; c<nchannels; c++)    {
//            for (auto i=0; i<buffer_size; i++)  {
//                buffer_in[i] = m->buffer[c][i];
//            }
//            showFromBuffer(c, 1);
//        }
//        QMetaObject::invokeMethod((QObject*)object, "setDistance", Q_ARG(QVariant, QString::number(m->distance)));
//    }
}

void DataSource::readData(int buffer_part, QByteArray *buffer, QHostAddress sender)
{



   int ch_num = getUnitIndex(buffer_part, buffer, sender);
   if (scan_index[ch_num]>=max_scan_index) {
       return;
   }
    mtx.lockForWrite();

    int small_block = scan_index[ch_num]==0?0:1;
   if (scan_index[ch_num] >= scan_data[ch_num].length())  {
       scan_data[ch_num].resize(scan_data[ch_num].length() + buffer_size - small_block);
       scan_data_0[ch_num].resize(scan_data_0[ch_num].length() + buffer_size - small_block);
       max_index_stat[ch_num].resize(max_index_stat[ch_num].length() + buffer_size - small_block);
       processed_data[ch_num].resize(processed_data[ch_num].length() + buffer_size - small_block);
       _points[ch_num].resize(_points[ch_num].length() + (buffer_size - small_block)/_step);
   }

    if (!object)
        object = m_appViewer->rootObject();

    int b_shift = buffer_part * (buffer_size + 1) * 2;

    auto this_buffer_size = buffer_size;
    auto initial_sum_of_values = sum_of_values[ch_num];
    if (scan_index[ch_num] > 0) {
        scan_index[ch_num]--;
        this_buffer_size--;
    }
    //unsigned short block_value_shift = 0;
    for(auto j=0; j<buffer_size; j++) {
        unsigned char b0 = (unsigned char)(*buffer)[b_shift + (j+1)*2];
        unsigned char b1 = (unsigned char)(*buffer)[b_shift + (j+1)*2+1];
        unsigned short b = (b0 << 8) + b1;

        if (scan_index[ch_num] > 0 && j==0) {
//            block_value_shift = b - scan_data[ch_num][scan_index[ch_num]];
        }
        else {
            //b = b - block_value_shift;
//            if (scan_index[ch_num]>0 && abs(b - last_good_value[ch_num]) > max_change)  {
//                b = last_good_value[ch_num];
//            }
//            else {
//                last_good_value[ch_num] = b;
//            }
            scan_data[ch_num][scan_index[ch_num]] = b;
            sum_of_values[ch_num] += b;
        }
        scan_index[ch_num] ++;
    }   
    auto this_shift = unsigned short((sum_of_values[ch_num] - initial_sum_of_values) / this_buffer_size);
    for (auto i=0; i<this_buffer_size; i++) {
        scan_data[ch_num][scan_index[ch_num]-i-1] -= this_shift;
    }
    mtx.unlock();
}
void DataSource::MakeHPButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford)
{
  QVector<complexd> Poles(ford); // полюса аналогового фильтра прототипа
  complexd Z, p1, G0 = complexd(1,0), K;
  int x, nx=H.length();
  int i;
  double a;
  double wc = (2/Td)*tan(2*M_PI*fc*Td/2); // частота среза
  double w1;

  // полюса непрерывной ПФ БТВТ
  for(i=0;i<ford;i++)
  {
    a = (double)(2*i+(int)ford-1)*M_PI/2*(double)ford;
    Poles[i] = complexd(1,0) * complexd(cos(a),sin(a));
  }
  for(x=0;x<nx;x++)
  {
    w1 = (M_PI * x) / (nx * Td); /* i-ое знач частоты для дискретного фильтра */
    w1 = (2/Td)*tan(w1*Td/2); /* переходим к частоте для прототипа */
    p1 = complexd(0,w1);
    // теперь пересчитаем p для lp фильтра
    p1 = complexd(wc,0) / p1;
    K = complexd(1,0);
    // знаменатель ПФ
    for(i=0;i<ford;i++)
    {
      K *= (p1 - Poles[i]) / complexd(1,0);
    }
    // теперь получм значение ПФ дискр фильтра
    H[x] = (x==0)? 0 : ((K == complexd(0,0)) ? complexd(0,0) : G0 / K);
  }
//#undef cv
}
void DataSource::MakeLPButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford)
{
  QVector<complexd> Poles(ford); // полюса аналогового фильтра прототипа
  complexd Z, p1, G0 = complexd(1,0), K;
  int x, nx=H.length();
  int i;
  double a;
  double wc = (2/Td)*tan(2*M_PI*fc*Td/2); // частота среза
  double w1;

  // полюса непрерывной ПФ БТВТ
  for(i=0;i<ford;i++)
  {
    a = (double)(2*i+(int)ford-1)*M_PI/2*(double)ford;
    Poles[i] = complexd(1,0) * complexd(cos(a),sin(a));
  }
  for(x=0;x<nx;x++)
  {
    w1 = (M_PI * x) / (nx * Td); /* i-ое знач частоты для дискретного фильтра */
    w1 = (2/Td)*tan(w1*Td/2); /* переходим к частоте для прототипа */
    p1 = complexd(0,w1);
    // теперь пересчитаем p для lp фильтра
    p1 /= wc;
    K = complexd(1,0);
    // знаменатель ПФ
    for(i=0;i<ford;i++)
    {
      K *= (p1 - Poles[i]) / complexd(1,0);
    }
    // теперь получм значение ПФ дискр фильтра
    H[x] = (K == complexd(0,0)) ? complexd(0,0) : G0 / K;
  }
}
void DataSource::Make_BP_ButterworthFilter(vectorc& H, double fc, double deltaf, unsigned short ford, double Td)
{
  QVector<complexd> Poles(ford); // полюса аналогового фильтра прототипа
  complexd Z, p1, G0 = complexd(1,0), K;
  int x, nx=H.size();
  int i;
  double a;
  double wc = 2*M_PI*fc;//(2/Td)*tan(2*M_PI*fc*Td/2); // частота среза
  double w1;

  // полюса непрерывной ПФ БТВТ
  for(i=0;i<ford;i++)
  {
    a = (double)(2*i+(int)ford-1)*M_PI/2*(double)ford;
    Poles[i] = complexd(1,0) * complexd(cos(a),sin(a));
  }

  for(x=0;x<nx;x++)
  {
    w1 = (M_PI * x) / (nx * Td); /* i-ое знач частоты для дискретного фильтра */
    //w1 = (2/Td)*tan(w1*Td/2); /* переходим к частоте для прототипа */
    p1 = complexd(0,w1);
    // теперь пересчитаем p для режекторного фильтра
    if(w1==0) p1=0; else p1 = (p1*p1 + sqr(wc))/(2*M_PI*deltaf*p1);
    K = complexd(1,0);
    // знаменатель ПФ
    for(i=0;i<ford;i++)
    {
      K *= (p1 - Poles[i]) / complexd(1,0);
    }
    // теперь получм значение ПФ дискр фильтра
    H[x] = (x == 0) ? complexd(0,0) : G0 / K;
  }
//#undef cv
}
void DataSource::cfft(vectorc& a)
{
  int i, rs, cs;
  ap::real_1d_array x;

  cs = a.size();
  double coeff = 1/sqrt(cs);
  rs = cs * 2;
  x.setbounds(0, rs-1);

  for(i=0; i< cs; i++)
  {
    x(i*2) = a[i].real();
    x(i*2+1) = a[i].imag();
  }

  fastfouriertransform(x, cs, false);

  for(i=0; i< cs; i++)
  {
    a[i] = complexd(x(i*2), x(i*2+1));
    a[i] *= coeff;

  }
}

void DataSource::icfft(vectorc& a)
{
  int i, rs, cs;
  ap::real_1d_array x;

  cs = a.size();
  double coeff = 1/sqrt(cs);
  rs = cs * 2;
  x.setbounds(0, rs-1);

  for(i=0; i< cs; i++)
  {
    x(i*2) = a[i].real();
    x(i*2+1) = a[i].imag();
  }

  fastfouriertransform(x, cs, true);

  for(i=0; i< cs; i++)
  {
    a[i] = complexd(x(i*2), x(i*2+1));
    a[i] /= coeff;
  }
}
int DataSource::_FindMaxValueInRangeOFArray(QVector <double>smp, int numsmp, int lowlim, int highlim)
{
  int k;
  double maxv = 0;
  double y1;
  int idx = -1;

  for(k=lowlim; k < highlim; k++)
  {
    y1 = fabs(smp[k]);
    if(y1 > maxv)
    {
      maxv = y1;
      idx = k;
    }
  }
  return idx;
}

void DataSource::accumulateChannel(int ch_num)
{
    if (fcount[ch_num] < 0)
        return;
    for (auto i=0; i<buffer_size; i++)  {
        raw_acc[ch_num][i] += buffer_in[i];
       // processed_acc[b_index][i] += buffer_out[i];
    }
    auto _tmp = QString::number((fcount[ch_num] * 1.0) / (nframes * 1.0));
    QMetaObject::invokeMethod((QObject*)object, "changeProgressBar", Q_ARG(QVariant, _tmp));
    fcount[ch_num]--;
    if (fcount[ch_num] == 0)    {
        for (auto i=0; i<buffer_size; i++)  {
            raw_acc[ch_num][i] = raw_acc[ch_num][i] / nframes;
           // processed_acc[b_index][i] = processed_acc[b_index][i] / nframes;
        }

        unsigned short med = 0x8000;
        if (pointfile && pointfile->open(QIODevice::WriteOnly | QIODevice::Append)) {
            QDataStream out(pointfile);
            out.setVersion(QDataStream::Qt_5_9);
            out << ch_num;
            for (auto i=0; i<buffer_size; i++){
                //out << std::htons(((unsigned short)raw_acc[b_index][i] + med));
                out << raw_acc[ch_num][i];
                if (saveAsZeroSignal)   {
                    zero_signal[ch_num][i] = raw_acc[ch_num][i];
                }
            }


            /*for (auto i=0; i<buffer_size; i++){
                out << processed_acc[b_index][i];
            }*/
            if (saveAsZeroSignal)   {
                if (zerofile && zerofile->open(QIODevice::WriteOnly | QIODevice::Append)) {
                    QDataStream outz(zerofile);
                    outz.setVersion(QDataStream::Qt_5_9);
                    outz << ch_num;

                    for (auto i=0; i<buffer_size; i++){
                       // outz << ((unsigned short)zero_signal[b_index][i]+med);
                        outz << zero_signal[ch_num][i];
                    }
                    zerofile->close();
                }
                QMetaObject::invokeMethod((QObject*)object, "uncheck_checkBox_saveAsZeroSignal");
                saveAsZeroSignal --;
            }

            pointfile->close();
            delete pointfile;
            pointfile = nullptr;
        }
        is_measured --;
    }
}

void DataSource::updateListView()
{
    /*if (!object)
        object = m_appViewer->rootObject();
    for(auto i=0; i<_data.length(); i++)    {
        QMetaObject::invokeMethod((QObject*)object, "addListElement", Q_ARG(QVariant, QString::number(i) + ": " + QString::number(_data[i].distance) + " #1:" + QString::number(_data[i]._sqerr[0]) + " #2:" + QString::number(_data[i]._sqerr[1])), Q_ARG(QVariant, "white"));
    }*/
}

void DataSource::setMeasurementModel(MeasurementModel *mm)
{
    this->measurementModel = mm;
}

int DataSource::getMaxCorrelationShift(QVector<double> a, QVector<double> b)
{
    double max_corr = -1, tmp;
    int ret = -a.length()/2;
    /*for (auto i=-a.length()/2; i< a.length()/2; i++){
        tmp = shiftedcorr(a, b, i);
        if (tmp > max_corr) {
            max_corr = tmp;
            ret = i;
        }
    }*/
    return ret;
}

QVector<QVector<float> > *DataSource::getScanData()
{
    return &scan_data;
}

void DataSource::selectIP(QString v)
{
    ipNum = v;

    currentUnitIndex = AddressProvider::getUnitIndex(ipNum + emitterNum + rowNum);
}

void DataSource::selectEmitter(QString v)
{
    emitterNum = v;

    currentUnitIndex = AddressProvider::getUnitIndex(ipNum + emitterNum + rowNum);
}

void DataSource::selectRow(QString v)
{
    rowNum = v;

    currentUnitIndex = AddressProvider::getUnitIndex(ipNum + emitterNum + rowNum);
}

void DataSource::saveAsScanData0()
{
    mtx.lockForRead();
    for(auto e=0; e < scan_data.length(); e++)  {
        for (auto i=0; i < scan_data[e].length(); i++) {
            scan_data_0[e][i] = scan_data[e][i];
        }
    }
    mtx.unlock();
}

int DataSource::getUnitIndex(int buffer_part, QByteArray *buffer, QHostAddress sender)
{
    //TODO: work with sender, it can contain necessary data

    auto _ipNum = AddressProvider::getIndex(sender) * 100;

    unsigned short i = (((unsigned char((*buffer)[0]) << 8) + unsigned char((*buffer)[1]) )%inputsPerReceiver) * 10;

    //qDebug() << "_ipNum" << _ipNum << "first bytes=" << (((*buffer)[0] << 8) + (*buffer)[1] ) << " i=" << i;

    return AddressProvider::getUnitIndex(_ipNum + i + buffer_part);
}

void DataSource::clearMeasurementModel()
{
    this->measurementModel->clear();
}

void DataSource::compareToData(int channel)
{
    if (!measurementModel || (channel>= scan_data.length()))
        return;
    for (auto d=0; d<measurementModel->rowCount(); d++) {
        auto m = measurementModel->get(d);
        auto b = m->getBuffer();
        if (m->_corr.length() < b->length())    {
            m->_corr.resize(b->length());
        }
        if (b)  {
            if (channel >= 0)   {
                m->_corr[channel] = pearsoncoeff<float>(scan_data[channel], (*b)[channel]);
            }
            else {
                for (auto e=0; e<b->length(); e++)  {
                    m->_corr[e] = pearsoncoeff<float>(scan_data[e], (*b)[e]);
                }
            }
        }

//        m->_sqerr[channel] = 0;
//        for (int i=0; i<buffer_size; i++)   {
//            m->_sqerr[channel] += (m->buffer[channel][i]-buffer_in[i])*(m->buffer[channel][i]-buffer_in[i]);
//        }
//        m->_sqerr[channel] = sqrt(m->_sqerr[channel]);

        //m->_corr[channel] = pearsoncoeff(m->buffer[channel], buffer_in);

    }
    float x = 0.0f, y=0.0f;
    for (int e=0; e<nchannels; e++) {
        QVector<float> tmp;
        for (auto d=0; d<measurementModel->rowCount(); d++) {
            auto m = measurementModel->get(d);
            tmp.append(m->_corr[e]);
        }
        int imax;
        auto fmax = findMaxLess(&tmp, FLT_MAX, &imax);
        distance_from_mm[e] = imax;

        auto f = findMaxLess(&tmp, fmax, &imax);
        f = findMaxLess(&tmp, f, &imax);
        distance_weight_from_mm[e] = fmax - f;

        x += (fmax - f)*sin(2*M_PI*imax/32);
        y += (fmax - f)*cos(2*M_PI*imax/32);
    }
    float sector = 32*(-qAtan2(y, x)+M_PI/2)/(2*M_PI);
    if (sector < 0)
        sector = 32 + sector;
    qDebug() << "sector=" << sector;

    //measurementModel->emitDataChanged();
    //pearsoncoeff()
    //pearsoncoeff(scan_data[currentUnitIndex], history_scan_data[history_index][currentUnitIndex]);
}

double DataSource::corr(double *X, double *Y, int N)
{
    return 0;
}

void DataSource::calcDistances()
{
    qDebug() << "DataSource::calcDistances";
    // 0 - simplest method, moving average
    mtx.lockForRead();
    dst_lock.lockForWrite();



    auto window_size = 5;

    /*double avg, avgabs, maxavgabs;
    int i_maxavgabs;*/
    receiver_levels.fill(0);

    //int window_size = 100;
    //processSignal();
    //current_set = &processed_data;
    current_set = &scan_data;
    current_max_index_stat = &max_index_stat;
    current_peak_data = &peak_data;


    for(auto e=0; e < current_set->length(); e++)  {
        distance_data[e].resize(3);
        distance_data_0[e].resize(3);
        for (auto i=0; i < (*current_set)[e].length(); i++) {
            auto k_start = std::max(0, i-window_size);
            auto k = 0;
            auto max_index = k_start;
            auto max_value = fabs(double((*current_set)[e][max_index]));
            while (k < window_size && k_start + k < (*current_set)[e].length()) {
                auto value = fabs(double((*current_set)[e][k_start + k]));
                if (value > max_value) {
                    max_index = k_start+k;
                    max_value = value;
                }
                k++;
            }
            (*current_max_index_stat)[e][max_index]++;
        }
        (*current_peak_data)[e].clear();
        findMaxLess(&((*current_set)[e]), &((*current_max_index_stat)[e]), &((*current_peak_data)[e]), 100, 100);
        findMaxLess(&((*current_set)[e]), &((*current_max_index_stat)[e]), &((*current_peak_data)[e]), 100, 100);
        findMaxLess(&((*current_set)[e]), &((*current_max_index_stat)[e]), &((*current_peak_data)[e]), 100, 100);

        //qSort((*current_peak_data)[e].begin(), (*current_peak_data)[e].end(), [](const Peak& a, const Peak& b) { return a.index < b.index; });
    }

    /*
    for(auto e=0; e < scan_data.length(); e++)  {
        distance_data[e].resize(3);
        avg = 0.0;
        auto receiver = AddressProvider::getReceiver(e);

        for (auto i=0; i < scan_data[e].length(); i++) {
            auto val = double(scan_data[e][i]) - (use_scan_data_0?double(scan_data_0[e][i]):0.0);
            avg += val;            
        }

        //avg = avg / scan_data[e].length();
        for (auto i=0; i < scan_data[e].length(); i++) {
            auto val = double(scan_data[e][i]) - (use_scan_data_0?double(scan_data_0[e][i]):0.0) - avg;
            receiver_levels[receiver] += fabs(val);
            distance_data[e][raw_level_index] += fabs(val);
        }

        distance_data[e][raw_level_index] *= receiver_multiplier;

        if (save_level_0) {
            distance_data[e][saved_level_index] = distance_data[e][raw_level_index];
        }

        if (distance_data[e][raw_level_index] != 0) {
            if (distance_data[e][saved_level_index] != 0)   {
                distance_data[e][distance_index] = sqrt(1 / (distance_data[e][raw_level_index] * pow((sqrt(1/distance_data[e][saved_level_index])/zero_distance), 2)));
            }
            else {
                distance_data[e][distance_index] = sqrt(1 / distance_data[e][raw_level_index]);
            }

        }
//        maxavgabs = 0.0;
//        for (auto i=window_size-1; i < scan_data.value(e).count(); i++) {
//            avgabs = 0.0;
//            for(auto j=i-window_size+1; j<=i; j++)  {
//                avgabs = avgabs + fabs((double(scan_data[e][i]) - (use_scan_data_0?double(scan_data_0[e][i]):0.0)) - avg);
//            }
//            avgabs = avgabs / window_size;
//            if (avgabs > maxavgabs) {
//                maxavgabs = avgabs;
//                i_maxavgabs = i;
//            }
//        }
//        //distance_data[e].resize(method_index + 1);
//        distance_data[e][method_index] = i_maxavgabs;
    }
*/
    /*receiver_levels[0] =  receiver_levels[0] * receiver_multiplier;
    receiver_levels[1] =  receiver_levels[1] * receiver_multiplier;
    receiver_levels[2] =  receiver_levels[2] * receiver_multiplier;
    receiver_levels[3] =  receiver_levels[3] * receiver_multiplier;

    save_level_0 = false;*/

    dst_lock.unlock();
    mtx.unlock();
}

double DataSource::getCurrentDistance()
{
    if (currentUnitIndex < distance_data.length() && distance_data[currentUnitIndex].length()>0)
        return distance_data[currentUnitIndex][0];
}

double DataSource::getReceiverLevel(int receiver)
{
    return receiver_levels[receiver];
}

void DataSource::setSaveDistance0(bool val)
{
    use_distance_0 = val;
    if (use_distance_0)    {
        for(auto e=0; e<distance_data.length(); e++) {
            for(auto i=0; i<distance_data[e].length(); i++)
            distance_data_0[e][i] = distance_data[e][i];

        }
    }
}

int DataSource::getBufferSize()
{
    return buffer_size;
}

void DataSource::resetScanIndex()
{
    scan_index.fill(0);
    sum_of_values.fill(0);
}

void DataSource::setScanIndex()
{
     qDebug() << "testScanIndex before " << testScanIndex();
    int max_index = scan_index[0];
    for (auto i=0; i<scan_index.length(); i++) {
        if (scan_index[i] > max_index) {
            max_index = scan_index[i];
        }
    }
    for (auto i=0; i<scan_index.length(); i++) {
        if (scan_index[i] < max_index) {

            double fill_value = 0.0;
            /*if (scan_index[i] != 0) {
                fill_value = sum_of_values[i] / scan_index[i];

            }
            else {
                fill_value = 0.0;
            }*/
            //scan_index[i] = max_index;
            while (scan_index[i] < max_index)   {
                scan_data[i][scan_index[i]] = fill_value;
                scan_index[i]++;
            }
        }
    }

    qDebug() << "testScanIndex after " << testScanIndex();
}

void DataSource::setAllWaveformsSeries(QAbstractSeries *series, int set)
{
    allWaveformsSeries[set] = series;
}

void DataSource::setSeries(QAbstractSeries *series, int i)
{
    this->series[i] = series;
}

void DataSource::setDistanceSeries(QAbstractSeries *series, int i)
{
    this->distanceSeries[i] = series;
}

double DataSource::getScanValue(QVector<float> *data, QVector<float> *data0, int i, bool use_abs)
{
    if (!use_abs)
        return double((*data)[i]) - (data0?double((*data0)[i]):0.0);
    return fabs(double((*data)[i]) - (data0?double((*data0)[i]):0.0));
}

void DataSource::findMaxLess(QVector<float> *data, QVector<int> *stat_data, QVector<Peak> *peak_data, int margin_left, int margin_right)
{
    if (!data || !peak_data || margin_left < 0 || margin_right < 0 || margin_left > data->length()-1 || margin_right >data->length()-1) {
        throw new QException();
    }



    double maxv = 0.0;
    int maxi = -1;
    double val =0.0;
    bool passed = false;
    for (auto i=0; i<data->length(); i++)   {

        for (auto p=0; p<peak_data->length(); p++) {
            if ((*peak_data)[p].index >= 0 && i >= (*peak_data)[p].index - (*peak_data)[p].margin_left && i <= (*peak_data)[p].index + (*peak_data)[p].margin_right) {
                i = (*peak_data)[p].index + (*peak_data)[p].margin_right + 1;
            }
        }
        if (i<data->length()) {
            if (!passed) {
                maxv = getScanValue(data, nullptr, i, true) * (stat_data?((*stat_data)[i]?1:0):1);
                maxi = i;
                passed = true;
            }
             val = getScanValue(data, nullptr, i, true)*(stat_data?((*stat_data)[i]?1:0):1);
             if (val > maxv) {
                 maxi = i;
                 maxv = val;
             }
        }
        else if (!passed)   {
            maxi = -1;
            maxv = 0.0;
        }
    }
    peak_data->resize(peak_data->length()+1);
    (*peak_data)[peak_data->length()-1].value = maxv;
    (*peak_data)[peak_data->length()-1].index = maxi;
    (*peak_data)[peak_data->length()-1].margin_left = margin_left;
    (*peak_data)[peak_data->length()-1].margin_right = margin_right;

}

float DataSource::findMaxLess(QVector<float> *data, float v, int *index)
{
    float maxless = FLT_MIN;
    for (int i=0; i<data->length(); i++)    {
        if ((*data)[i] > maxless && (*data)[i] < v) {
            maxless = (*data)[i];
            *index = i;
        }
    }
    return maxless;
}

void DataSource::increaseHistoryIndex()
{
    if (history_index>=history_depth - 1)   {
        history_index = 0;
    }
    else {
        history_index ++;
    }
}

void DataSource::copyToHistory()
{
    if (has_data && write_history)    {
        if (history_scan_data.length() < history_index+1)   {
            history_scan_data.resize(history_depth);
        }
        history_scan_data[history_index] = scan_data;


        auto m = new Measurement();
        m->setBuffer(&(history_scan_data[history_index]));
        m->distance = distance;
        measurementModel->add(m);
        //scan_data = *(new QVector<QVector<double>>(nchannels));
        has_data = false;

        increaseHistoryIndex();
    }
}

void DataSource::saveMMAndHistory()
{
    if (pointfile && pointfile->open(QIODevice::Append/* | QIODevice::Append*/)) {
        QDataStream out(pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        for(auto i=0; i<measurementModel->rowCount(); i++)  {
            measurementModel->get(i)->writeTo(out);
        }
        pointfile->close();
    }

}

void DataSource::setHasData(bool hd)
{
    has_data = hd;
}

void DataSource::loadHistoryFromFile(QString fname)
{
    QFile file(fname);
    if (file.open(QIODevice::ReadOnly))    {
        QDataStream in(&file);

    //if (pointfile && pointfile->open(QIODevice::ReadOnly/* | QIODevice::Append*/)) {
        //QDataStream in(pointfile);
        in.setVersion(QDataStream::Qt_5_9);

        while(!in.atEnd())  {
            auto m = new Measurement();
            m->setBuffer( &(history_scan_data[history_index]));
            increaseHistoryIndex();
            m->readFrom(in);
            measurementModel->add(m);
        }

        file.close();
    }

}

void DataSource::SetDistance(float distance)
{
    this->distance = distance;
}

void DataSource::setCurrentSet_0()
{
    if (history_scan_data.length() > zeroIndex && history_scan_data[zeroIndex].length()>0) {
        current_set_0 = &(history_scan_data[zeroIndex]);
    }
}

bool DataSource::testScanIndex()
{
    int tmp = scan_index[0];
    for (auto i=0; i<scan_index.length(); i++) {
        if (scan_index[i] != tmp)   {
            //qDebug() << "scan_index[i] != tmp, scan_index[i]=" << scan_index[i] << "tmp=" << tmp;
            return false;
        }
        if (scan_data[i].length() < tmp)   {
            //qDebug() << "scan_data[i].length() < tmp, scan_data[i].length()=" << scan_data[i].length() << "tmp=" << tmp;
            return false;
        }
    }


}

void DataSource::textChanged(QString text)
{
    qDebug() << text;

    if (text.indexOf("fc=", 0, Qt::CaseSensitive) == 0){
        fc =  text.right(text.length()-3).toDouble();
    }
    if (text.indexOf("deltaf=", 0, Qt::CaseSensitive) == 0){
        deltaf =  text.right(text.length()-7).toDouble();
    }
    if (text.indexOf("ford=", 0, Qt::CaseSensitive) == 0){
        ford =  text.right(text.length()-5).toDouble();
    }
    if (text.indexOf("Td=", 0, Qt::CaseSensitive) == 0){
        Td =  text.right(text.length()-3).toDouble();
    }


    if (text.indexOf("zeroIndex=", 0, Qt::CaseSensitive) == 0){
        zeroIndex =  text.right(text.length()-10).toInt();
        setCurrentSet_0();
        qDebug() << "zeroIndex=" << zeroIndex;
    }


    if (text.indexOf("Save history", 0, Qt::CaseSensitive) == 0){
        saveMMAndHistory();
    }

    if (text.indexOf("write history=true", 0, Qt::CaseSensitive) == 0){
        write_history = true;
    }
    if (text.indexOf("write history=false", 0, Qt::CaseSensitive) == 0){
        write_history = false;
    }

    if (text.indexOf("Command=compareToData", 0, Qt::CaseSensitive) == 0){
        compareToData(-1);
    }
    if (text.indexOf("Command=collapse", 0, Qt::CaseSensitive) == 0){
        collapseMMAndHistory(10);
    }
    //calcDistances();
    //_dataSource->startRecording("test.dat");
    // update();
}

QString DataSource::getReceiverName(int index)
{
    return QString::number(AddressProvider::getPseudoIndex(index));
}

QString DataSource::getIP(int index)
{
    return getReceiverName(index).left(1);
}

QString DataSource::getEmitter(int index)
{
    return getReceiverName(index).mid(1, 1);
}

QString DataSource::getRow(int index)
{
    return getReceiverName(index).right(1);
}

QString DataSource::getRow()
{
    return getRow(currentUnitIndex);
}

QString DataSource::getIP()
{
    return getIP(currentUnitIndex);
}

QString DataSource::getEmitter()
{
    return getEmitter(currentUnitIndex);
}

void DataSource::setUnitIndex(int index)
{
    this->currentUnitIndex = index;
}
