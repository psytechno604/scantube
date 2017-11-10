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
    m_appViewer(appViewer),
    sharedMemory("ScanTube")
{
    m_series.resize(4);
    m_allWaveformsSeries.resize(2);
    m_distanceSeries.resize(6);

    if (!_object)
        _object = m_appViewer->rootObject();

    m_nChannels = _numOfDevices * _inputsPerDevice * _channelsInPacket;
    //QQuickItem *item = qobject_cast<QQuickItem*>(object);

    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    m_datafile = new QFile("scantube.dat");
    m_markupfile = new QFile("scantube.csv");
    m_pointfile = new QFile("scantube.scan");

    if (!m_datafile->open(QFile::WriteOnly)){
        qDebug() << "datafile cannot be opened";
    }
    if (!m_markupfile->open(QFile::WriteOnly)){
        qDebug() << "markupfile cannot be opened";
    }

    //emit changeText("button clicked");

    m_scanData.resize(_nChannels);
    m_scanData.fill(QVector<float>(_scanBufferSize));

    m_data.resize(m_nChannels * 3);

    m_receiverLevels.resize(AddressProvider::getReceiverCount());

    m_historyScanData.resize(m_historyDepth);

    //m_maxIndexStat.resize(m_nChannels);
    m_scanData.resize(m_nChannels);
    //m_processedData.resize(m_nChannels);



   //_sumOfValues.resize(m_nChannels); _sumOfValues.fill(0);

   //m_maxScanIndex = (m_bufferSize) * 8;


    initCorrelationParameters(80*1E-12, 100*1E9);





    //zero_signal.resize(m_nChannels);

    int T = 16;




    int k = 0;


}

DataSource::~DataSource()
{
    if (m_datafile) {
        m_datafile->close();
    }
    if (m_markupfile) {
        m_markupfile->close();
    }
}






void DataSource::update(bool show, int r, int i_max, int i_slice, int i_proc)
{


    auto m = m_measurementModel->get(r);
    if (!m) {
        qDebug() << "DataSource::update: invalid r = " << r;
        return;
    }

    if (i_max < 0 || i_slice < 0 || i_proc < 0 || i_max >= m_series.length() || i_slice >= m_series.length() || i_proc >= m_series.length() || i_max == i_slice || i_max == i_proc || i_slice == i_proc)  {
        qDebug () << "DataSource::update: invalid parameters";
        return;
    }
    auto maxSeries = m_series[i_max];
    auto sliceSeries = m_series[i_slice];
    auto procSeries = m_series[i_proc];



    auto s = static_cast<QSplineSeries *>(sliceSeries);
    if (!s) {
        qDebug() << "DataSource::update: static_cast<QSplineSeries *>(sliceSeries) returned 0";
    }
    else {
        QVector<QPointF> points;
        auto b = m->getBuffer();
        if (show && s && b && b->length()>m_currentUnitIndex)  {
            for (int d=0; d<(*b)[m_currentUnitIndex].length(); d++)   {
                points.append(QPointF(d, (*b)[m_currentUnitIndex][d]));
            }

        }
        s->replace(points);
    }

    return; // TODO !!!

    auto p = static_cast<QSplineSeries *>(procSeries);
    if (!p) {
        qDebug() << "DataSource::update: static_cast<QSplineSeries *>(procSeries) returned 0";
    }
    else {
        QVector<QPointF> points;
        if (show) {
            processSignal(m);
            auto b = m->getPBuffer();
            if (show && p && b && b->length()>m_currentUnitIndex)  {

                for (int d=0; d<(*b)[m_currentUnitIndex].length(); d++)   {
                    points.append(QPointF(d, (*b)[m_currentUnitIndex][d]));
                }

            }
        }
        p->replace(points);
    }
}
void DataSource::updateAllWaveforms(QAbstractSeries *series, int set)
{

}

void DataSource::updateAllWaveforms()
{
    for(auto i=0; i<m_allWaveformsSeries.length(); i++) {
        if (m_allWaveformsSeries[i]) {
            updateAllWaveforms(m_allWaveformsSeries[i], i);
        }
    }
}

void DataSource::updateDistances(QAbstractSeries *series, int i, int set)
{

}

void DataSource::updateDistances()
{
    updateDistances(m_distanceSeries[0], 0, 0);
    updateDistances(m_distanceSeries[1], 1, 0);
    updateDistances(m_distanceSeries[2], 2, 0);
    updateDistances(m_distanceSeries[3], 0, 1);
    updateDistances(m_distanceSeries[4], 1, 1);
    updateDistances(m_distanceSeries[5], 2, 1);
}

void DataSource::updateSurface3D(QtDataVisualization::QAbstract3DSeries *series)
{    
    auto step = m_surface3DScanStep;

    m_setCurrentSet_0();
    //auto sampleCountZ = 727;
    //auto sampleCountX = 101;
    //qDebug() << history_scan_data[0].length();

    if (/*m_historyScanData[0].length() == 0 ||*/ (m_useZeroSignal && !m_currentSet0))   {
        return;
    }

    surface_data_lock.lockForRead();

    auto s3ds =  static_cast<QtDataVisualization::QSurface3DSeries *>(series);

    auto proxy = s3ds->dataProxy();

    //auto series = new QtDataVisualization::QSurface3DSeries(proxy);

    auto dataArray = new QtDataVisualization::QSurfaceDataArray();

    auto maxI = 0;
    for (auto h=0; h<m_measurementModel->rowCount(); h+=step)   {
        auto m = m_measurementModel->get(h);
        auto b = m->getBuffer();
        if (b && (*b).length()>m_currentUnitIndex)    {
            if ((*b)[m_currentUnitIndex].length() > maxI) {
                maxI = (*b)[m_currentUnitIndex].length();
            }
        }
    }

    dataArray->reserve(maxI);

    for (int i = 0 ; i < maxI ; i++) {
        QtDataVisualization::QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(m_measurementModel->rowCount());
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = i; //"Distance"
        int index = 0;

        for (int j = 0; j < m_measurementModel->rowCount(); j+=step) {
            auto m = m_measurementModel->get(j);
            auto b = m->getBuffer();
            float x = j/step; // "Scan"
            float y = 0;

            if (b->length() > m_currentUnitIndex && (*b)[m_currentUnitIndex].length()>i) {
                if (m_useZeroSignal)
                    y = fabs(fabs((*b)[m_currentUnitIndex][i]) - fabs((*m_currentSet0)[m_currentUnitIndex][i])); // "Level"
                else
                    y = fabs((*b)[m_currentUnitIndex][i]);
            }
            //if (y>100)  {
             //   y = -y;
            //}
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
    m_setCurrentSet_0();
    //auto sampleCountZ = 727;
    //auto sampleCountX = 101;
    //qDebug() << history_scan_data[0].length();

    if (/*m_historyScanData[0].length() == 0 ||*/ (m_useZeroSignal && !m_currentSet0))   {
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
        QtDataVisualization::QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(m_measurementModel->rowCount());
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = i; //"Distance"
        int index = 0;

        for (int j = 0; j < m_measurementModel->rowCount(); j++) {
            auto m = m_measurementModel->get(j);

            float x = j; // "Scan"
            float y = 0;
            if (m->_corr.length() > m_currentUnitIndex && m_currentUnitIndex >= 0)  {
                y = m->_corr[m_currentUnitIndex] * 1000;
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

    for (auto i=0; i<m_measurementModel->rowCount(); i+=block_size)   {
        auto m = m_measurementModel->get(i);
        auto m_ = m_measurementModel->get(i/block_size);
        m_->m_value = m->m_value;
        m_->_corr.fill(0);
        auto b = m->getBuffer();
        //auto b_ = m_->getBuffer();
        for(auto e=0; e<b->length(); e++) {
            for (auto d=0; d<(*b)[e].length();d++){
                float summa = 0.0f;
                for (auto i_ = i; i_<i+block_size; i_++)    {
                    summa += m_measurementModel->getValue(i_, e, d);
                }
                m_measurementModel->setValue(i/block_size, e, d, summa / block_size);
            }
        }
    }
    m_measurementModel->clear(m_measurementModel->rowCount() / block_size);
}

void DataSource::copyHistoryToClipboard(int e_start, int e_end, int r_start, int r_end)
{
    if (e_start < 0 || e_end >= m_nChannels || e_start > e_end ||
        r_start < 0 || r_end >= m_measurementModel->rowCount() || r_start > r_end)    {
        qDebug() << "DataSource::copyHistoryToClipboard() wrong parameters";
        return;
    }

    auto m0 = m_measurementModel->get(m_zeroIndex);
    if (m_useZeroSignal && !m0) {
        qDebug() << "m_measurementModel->get(" << m_zeroIndex << ") returned 0";
        return;
    }
    auto b0 = m0->getBuffer();

    if (m_mimeData)   {
        try {
            delete m_mimeData;
        }
        catch (...) {
            qDebug() << "unable to delete m_mimeData";
        }
    }
    m_mimeData = new QMimeData();

    QByteArray itemData;
    QTextStream dataStream(&itemData, QIODevice::WriteOnly);

    if(m_measurementModel->rowCount() <= 0)   {
        return;
    }

    auto m = m_measurementModel->get(0);
    auto b = m->getBuffer();






    for(int e=e_start; e<=e_end; e++)   {
        for(int r=r_start; r<=r_end; r++) {
            QString str = QString::number(e);
            dataStream << str << ( (r == r_end && e==e_end) ?'\n':',');
        }
        for(int r=r_start; r<=r_end; r++) {
            auto m=m_measurementModel->get(r);
            QString str = QString::number(m->getValue());
            dataStream << str << ( (r == r_end && e==e_end) ?'\n':',');
        }
        for(int d=0; d<(*b)[e].length(); d++) {
            for(int r=r_start; r<=r_end; r++) {
                b = m_measurementModel->get(r)->getBuffer();
                QString str = QString::number(fabs(fabs((*b)[e][d]) - fabs((m_useZeroSignal?((*b0)[e][d]):0))));
                dataStream << str << ( (r == r_end && e==e_end) ?'\n':',');
            }
        }
    }

    m_mimeData->setData("csv", itemData);
    QApplication::clipboard()->setMimeData(m_mimeData);
}

void DataSource::setWriteHistory(bool wh)
{
    m_writeHistory = wh;
}

void DataSource::copyToSharedMemory()
{
    if (sharedMemory.isAttached())  {
        sharedMemory.detach();
    }
    sharedMemory.setKey("ScanTube");
    sharedMemory.create(1);

    char c = 7;

    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    memcpy(to, &c, qMin(sharedMemory.size(), 1));


    sharedMemory.unlock();
}

void DataSource::setSurface3DScanStep(int step)
{
    m_surface3DScanStep = step;
}

int DataSource::getNChannels()
{
    return m_nChannels;
}

void DataSource::setFilterParameters(bool hpOn, int hpFOrd, double hpFc, double hpTd, bool lpOn, int lpFOrd, double lpFc, double lpTd, bool bpOn, int bpFOrd, double bpFc, double bpTd, double bpDeltaF)
{
    m_hpOn = hpOn;
    m_hpFOrd = hpFOrd;
    m_hpFc = hpFc;
    m_hpTd = hpTd;
    m_lpOn = lpOn;
    m_lpFOrd = lpFOrd;
    m_lpFc = lpFc;
    m_lpTd = lpTd;
    m_bpOn = bpOn;
    m_bpFOrd = bpFOrd;
    m_bpFc = bpFc;
    m_bpTd = bpTd;
    m_bpDeltaF = bpDeltaF;
}

void DataSource::clearData()
{
    m_measurementModel->clear();
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
    m_Td = 1.0/Fdskr;
    kt_dt = (sigTau*1 / m_Td)*2;

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

        Make_BP_ButterworthFilter(H, m_fc, m_deltaf, m_ford, m_Td);

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
    for(int r=0; r< m_measurementModel->rowCount(); r++) {

    }
}

void DataSource::savePoint(double distance, int nframes, int saveAsZeroSignal)
{

    if (m_pointfile && m_pointfile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        QDataStream out(m_pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        out << distance;
        m_pointfile->close();
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


void DataSource::setUseZeroSignal(int s)
{
    m_useZeroSignal = s;
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



   int ch_num = _getUnitIndex(buffer_part, buffer, sender);
   if (m_scanIndex + m_bufferSize > m_bufferSize * _ADCNumSteps) {
       return;
   }
    mtx.lockForWrite();

    //int small_block = m_scanIndex[ch_num]==0?0:1;



    if (!_object)
        _object = m_appViewer->rootObject();

    int b_shift = buffer_part * (_halfPacketSize + 1) * 2;


    float _sumOfValues = 0.0;

    //unsigned short block_value_shift = 0;

    for(auto j=0; j<m_bufferSize; j++) {
        unsigned char b0 = (unsigned char)(*buffer)[b_shift + (j+1)*2];
        unsigned char b1 = (unsigned char)(*buffer)[b_shift + (j+1)*2+1];
        unsigned short b = (b0 << 8) + b1;


        m_scanData[ch_num][m_scanIndex + j] = b;
        _sumOfValues += b;
    }   
    /*auto this_shift = unsigned short(_sumOfValues / m_bufferSize);
    for (auto i=0; i<m_bufferSize; i++) {
        m_scanData[ch_num][m_scanIndex + i] -= this_shift;
    }*/
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
    this->m_measurementModel = mm;
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
    return &m_scanData;
}

void DataSource::selectIP(QString v)
{
    m_ipNum = v;

    setCurrentUnitIndex(AddressProvider::getUnitIndex(m_ipNum + m_emitterNum + m_rowNum));
}

void DataSource::selectEmitter(QString v)
{
    m_emitterNum = v;

    setCurrentUnitIndex(AddressProvider::getUnitIndex(m_ipNum + m_emitterNum + m_rowNum));
}

void DataSource::selectRow(QString v)
{
    m_rowNum = v;

    setCurrentUnitIndex(AddressProvider::getUnitIndex(m_ipNum + m_emitterNum + m_rowNum));
}


int DataSource::_getUnitIndex(int buffer_part, QByteArray *buffer, QHostAddress sender)
{
    //TODO: work with sender, it can contain necessary data

    auto _ipNum = AddressProvider::getIndex(sender) * 100;

    unsigned short i = (((unsigned char((*buffer)[0]) << 8) + unsigned char((*buffer)[1]) )%_inputsPerDevice) * 10;

    //qDebug() << "_ipNum" << _ipNum << "first bytes=" << (((*buffer)[0] << 8) + (*buffer)[1] ) << " i=" << i;

    return AddressProvider::getUnitIndex(_ipNum + i + buffer_part);
}

void DataSource::m_clearMeasurementModel()
{
    this->m_measurementModel->clear();
}

void DataSource::compareToData(int channel)
{

}





int DataSource::getBufferSize()
{
    return m_bufferSize;
}

void DataSource::resetScanIndex()
{
    m_scanIndex = 0;
}

void DataSource::setScanIndex(int scanIndex)
{
    m_scanIndex = scanIndex;
}

void DataSource::setAllWaveformsSeries(QAbstractSeries *series, int set)
{
    m_allWaveformsSeries[set] = series;
}

void DataSource::setSeries(QAbstractSeries *series, int i)
{
    if (this->m_series.length() <= i) {
        this->m_series.resize(i+1);
    }
    this->m_series[i] = series;
}

void DataSource::setDistanceSeries(QAbstractSeries *series, int i)
{
    this->m_distanceSeries[i] = series;
}

double DataSource::getScanValue(QVector<float> *data, QVector<float> *data0, int i, bool use_abs)
{
    if (!use_abs)
        return double((*data)[i]) - (data0?double((*data0)[i]):0.0);
    return fabs(double((*data)[i]) - (data0?double((*data0)[i]):0.0));
}

void DataSource::m_findMaxLess(QVector<float> *data, QVector<int> *stat_data, QVector<Peak> *peak_data, int margin_left, int margin_right)
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

float DataSource::m_findMaxLess(QVector<float> *data, float v, int *index)
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

void DataSource::m_increaseHistoryIndex()
{
    if (m_historyIndex>=m_historyDepth - 1)   {
        m_historyIndex = 0;
    }
    else {
        m_historyIndex ++;
    }
}

void DataSource::copyToHistory()
{            
    auto m = new Measurement(distance);

    auto b = m->getBuffer();

    for (auto e=0; e<b->length() && e<m_scanData.length(); e++) {
        auto d_tgt=0, d_src=0;
        for (auto k=0; k<_ADCNumSteps; k++) {
            auto valueShift = 0.0f;

            if (k!=0 && _overlap)   {
                for (auto p=_bufferSize * k; p< _bufferSize * k + _overlap; p++)    {
                    valueShift += (m_scanData[e][p] - m_scanData[e][p - _overlap]);
                }
                valueShift /= _overlap;
                d_src += _overlap;
            }
            for (auto i=0; i<_bufferSize-(k!=0?_overlap:0); i++) {
                (*b)[e][d_tgt] = m_scanData[e][d_src] + valueShift;
                d_tgt++;
                d_src++;
            }
        }
        auto sumValues = 0.0f;
        for (auto d=0; d<_historyBufferSize; d++)   {
            sumValues += (*b)[e][d];
        }
        sumValues = sumValues / _historyBufferSize;
        for (auto d=0; d<_historyBufferSize; d++)   {
            (*b)[e][d] -= sumValues;
        }

    }



    m_measurementModel->add(m);

    m_increaseHistoryIndex();

}

void DataSource::saveMMAndHistory()
{
    if (m_pointfile && m_pointfile->open(QIODevice::Append/* | QIODevice::Append*/)) {
        QDataStream out(m_pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        for(auto i=0; i<m_measurementModel->rowCount(); i++)  {
            m_measurementModel->get(i)->writeTo(out);
        }
        m_pointfile->close();
    }

}

void DataSource::setHasData(bool hd)
{
    m_hasData = hd;
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
            //m->setBuffer( &(m_historyScanData[m_historyIndex]));
            m_increaseHistoryIndex();
            m->readFrom(in);
            m_measurementModel->add(m);
        }

        file.close();
        emit fileLoaded();
    }

}

void DataSource::SetDistance(float distance)
{
    this->distance = distance;
}

void DataSource::m_setCurrentSet_0()
{
    m_currentSet0 = nullptr;
    if (m_measurementModel->rowCount() > m_zeroIndex) {
        auto m = m_measurementModel->get(m_zeroIndex);
        if (m)  {
            m_currentSet0 = m->getBuffer();
        }
    }
}

//bool DataSource::testScanIndex()
//{
//    int tmp = scan_index[0];
//    for (auto i=0; i<scan_index.length(); i++) {
//        if (scan_index[i] != tmp)   {
//            //qDebug() << "scan_index[i] != tmp, scan_index[i]=" << scan_index[i] << "tmp=" << tmp;
//            return false;
//        }
//        if (scan_data[i].length() < tmp)   {
//            //qDebug() << "scan_data[i].length() < tmp, scan_data[i].length()=" << scan_data[i].length() << "tmp=" << tmp;
//            return false;
//        }
//    }


//}

void DataSource::textChanged(QString text)
{
    qDebug() << text;

    if (text.indexOf("fc=", 0, Qt::CaseSensitive) == 0){
        m_fc =  text.right(text.length()-3).toDouble();
    }
    if (text.indexOf("deltaf=", 0, Qt::CaseSensitive) == 0){
        m_deltaf =  text.right(text.length()-7).toDouble();
    }
    if (text.indexOf("ford=", 0, Qt::CaseSensitive) == 0){
        m_ford =  text.right(text.length()-5).toDouble();
    }
    if (text.indexOf("Td=", 0, Qt::CaseSensitive) == 0){
        m_Td =  text.right(text.length()-3).toDouble();
    }


    if (text.indexOf("zeroIndex=", 0, Qt::CaseSensitive) == 0){
        m_zeroIndex =  text.right(text.length()-10).toInt();
        m_setCurrentSet_0();
        qDebug() << "zeroIndex=" << m_zeroIndex;
    }


    if (text.indexOf("Save history", 0, Qt::CaseSensitive) == 0){
        saveMMAndHistory();
    }

    if (text.indexOf("write history=true", 0, Qt::CaseSensitive) == 0){
        m_writeHistory = true;
    }
    if (text.indexOf("write history=false", 0, Qt::CaseSensitive) == 0){
        m_writeHistory = false;
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
    return getRow(m_currentUnitIndex);
}

QString DataSource::getIP()
{
    return getIP(m_currentUnitIndex);
}

QString DataSource::getEmitter()
{
    return getEmitter(m_currentUnitIndex);
}

void DataSource::setCurrentUnitIndex(int index)
{
    this->m_currentUnitIndex = index;
    emit currentUnitIndexChanged();
}
int DataSource::getCurrentUnitIndex() {
    return m_currentUnitIndex;
}
