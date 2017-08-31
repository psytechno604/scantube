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



QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer)
{


    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    datafile = new QFile("scantube.dat");
    markupfile = new QFile("scantube.csv");

    if (!datafile->open(QFile::WriteOnly)){
        qDebug() << "datafile cannot be opened";
    }
    if (!markupfile->open(QFile::WriteOnly)){
        qDebug() << "markupfile cannot be opened";
    }

    //emit changeText("button clicked");


    m_data.resize(nchannels * 2);
    //generateData(0, 5, 1024);

    init_correlate_parameters(80*1E-12, 100*1E9);

    shY.push_back(1145);
    shY.push_back(-160);


    buffer_in = new double[buffer_size];
    buffer_out = new double[buffer_size];

    //buffer_acc.resize(nchannels);

    X.resize(nchannels);
    Y.resize(nchannels);
    s.resize(nchannels);
    raw_acc.resize(nchannels);
    processed_acc.resize(nchannels);
    fcount.resize(nchannels);
    zero_signal.resize(nchannels);

    int T = 16;


    for(int c=0; c<nchannels; c++) {
        X[c].resize(T);
        Y[c].resize(T);
        s[c] = *(new tk::spline());
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

    for(int c=0; c<nchannels; c++) {
        s[c].set_points(X[c],Y[c]);
    }
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

void DataSource::showFromBuffer(int b_index)
{
    if (!object)
        object = m_appViewer->rootObject();

    QVector<QPointF> points;
    points.reserve(buffer_size);

    QVector<QPointF> pointsS;
    pointsS.reserve(buffer_size);

    int start_position, object_position;
    process_signal(buffer_in, buffer_out, &start_position, &object_position, buffer_size);

    for (auto i=0; i<buffer_size; i++) {
        points.append(QPointF(i, buffer_in[i]));
        pointsS.append(QPointF(i, buffer_out[i]));
    }
    //markupfile->write(QByteArray("\n"));

    if (b_index == 0) {
        QMetaObject::invokeMethod((QObject*)object, "changeText5", Q_ARG(QVariant, QString::number(start_position)));
        QMetaObject::invokeMethod((QObject*)object, "changeText1", Q_ARG(QVariant, QString::number(object_position)));
        QMetaObject::invokeMethod((QObject*)object, "changeText2", Q_ARG(QVariant, QString::number(s[b_index](object_position))));
    }
    if (b_index == 1) {
        QMetaObject::invokeMethod((QObject*)object, "changeText6", Q_ARG(QVariant, QString::number(start_position)));
        QMetaObject::invokeMethod((QObject*)object, "changeText3", Q_ARG(QVariant, QString::number(object_position)));
        QMetaObject::invokeMethod((QObject*)object, "changeText4", Q_ARG(QVariant, QString::number(s[b_index](object_position))));
    }

    m_data[b_index] = points;
    m_data[nchannels + b_index] = pointsS;

    if (is_measured) accumulateChannel(b_index);

}

void DataSource::update(QAbstractSeries *series)
{
    mtx.lock();
    if (series && m_data.size()>0) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        if (m_index > 3) {
            m_index = 0;
        }
        //qDebug() << m_index;
        if (m_index < m_data.size())    {
            //qDebug() << "update m_index=" << m_index;
            QVector<QPointF> points = m_data.at(m_index);
           // if (m_index == 0)
                xySeries->replace(points);
            m_index++;
        }

        // Use replace instead of clear + append, it's optimized for performance

    }
    mtx.unlock();
}

void DataSource::calc_correlate_func(double *in, double *out, float *corrfunct, int n__corr, int numsmpl)
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

void DataSource::init_correlate_parameters(float sigTau, float Fdskr)
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
void DataSource::process_signal(double *in, double *out, int *StartPosIndex, int *ObjectPosIndex, int numadc)
{
    int i,j,x;

      vectorc dblbubl;
      vectorc H;
      int ndaln2=numadc;

      while(!IsPowerOfTwo(ndaln2)) ndaln2++;
      ndaln2 = ndaln2*1;
      H.resize(ndaln2);
      dblbubl.resize(ndaln2);
      Fc = 1000 * 1E6;



      //__normalize_m1p1_signal(InputDecodedSig, numadc);
      /*if(0)
      {
        for(i = 0; i < numadc; i++) ProcessedSig[i] = InputDecodedSig[i];
      }
      else */
      if (useFilter)
        calc_correlate_func( in, out, signal, kt_dt, numadc );
      else {
          for (auto i=0; i<buffer_size; i++)    {
              out[i] = in[i];
          }
      }


      //---- фильтр НЧ (для убирания шумов лишних в ВЧ обл.)
      if(1)
      {
        Make_LP_ButterworthFilter(H, _LP0_Td,_LP0_fc, _LP0_ford);
        //Make_LP_ChebyshevFilter(H, 200*1E6, 0.3,2, 1.0/(100.0*1E9));

        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
        for(j=0;j<numadc;j++) dblbubl[j] = complexd( out[j],0 );

        cfft(dblbubl);
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        //reverse(dblbubl.begin(),dblbubl.end());
        //for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        //reverse(dblbubl.begin(),dblbubl.end());
        icfft(dblbubl);

        //Series2->Clear();
        for(x=0;x<ndaln2;x++)
        {
          if(x<numadc) out[x] = 1 * dblbubl[x].real();
          //double Tdr = 1.0/(100.0*1E9);
          //Series2->AddXY((((M_PI*x)/(16384*Tdr))/(M_PI*2))/1E6,abs(H[x]),"",clTeeColor);
          //Series2->AddXY(x,abs(H[x]),"",clTeeColor);
        }

      }
      //----

      //---- фильтр ВЧ (для убирания пост. составл. в сигнале)
      if(1)
      {
        Make_HP_ButterworthFilter(H, _HP0_Td,_HP0_fc, _HP0_ford);

        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
        for(j=0;j<numadc;j++) dblbubl[j] = complexd( out[j],0 );

        cfft(dblbubl);
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        //reverse(dblbubl.begin(),dblbubl.end());
        //for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        //reverse(dblbubl.begin(),dblbubl.end());
        icfft(dblbubl);

        for(x=0;x<numadc;x++)
        {
          out[x] = 1 * dblbubl[x].real();
        }
      }
      //----
      //---- включим "диод" (вычислим модуль сигнала
      if(0)
      {
        for(j=0;j<numadc;j++) out[j] = fabs(out[j]);
      }
      //----
      //---- фильтр НЧ для огибающей
      if(1)
      {
        Make_LP_ButterworthFilter(H, _LP1_Td,_LP1_fc, _LP1_ford);

        for(j=0;j<ndaln2;j++) dblbubl[j]=complexd(0,0);
        for(j=0;j<numadc;j++) dblbubl[j] = complexd( out[j],0 );

        cfft(dblbubl);
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        reverse(dblbubl.begin(),dblbubl.end());
        for(x=0;x<ndaln2;x++) dblbubl[x] *= H[x];
        reverse(dblbubl.begin(),dblbubl.end());
        icfft(dblbubl);

        for(x=0;x<numadc;x++)
        {
          out[x] = 1 * dblbubl[x].real();
        }
      }
      int prolaz_position;
      int prolaz_hibound = 345;//numadc/2
      prolaz_position = _FindMaxValueInRangeOFArray(out, numadc, 50, prolaz_hibound);
      *StartPosIndex = prolaz_position;

      //--- и наконец ищем где объект перед антенной
      int delta = numadc/32;
      int lobound_for_objpos;
      //lobound_for_objpos = prolaz_position+1+delta;
      lobound_for_objpos = 370;

      int obj_position;
      obj_position =
              _FindMaxValueInRangeOFArray(out, numadc,
                                          lobound_for_objpos,
                                          numadc-numadc/5);
      *ObjectPosIndex = obj_position;
}

void DataSource::save_point(double distance, int nframes, int saveAsZeroSignal)
{
    if (this->is_measured)
        return;

    this->distance = distance;
    for (auto c=0; c<nchannels; c++)    {
        for (auto i=0; i<buffer_size; i++) {
            raw_acc[c][i] = 0.0;
            processed_acc[c][i] = 0.0;
            if (saveAsZeroSignal)
                zero_signal[c][i] = 0.0;
        }
        fcount[c] = nframes;
    }

    this->nframes = nframes;

    this->is_measured = nchannels;
    this->saveAsZeroSignal = saveAsZeroSignal * nchannels;
    if (pointfile && pointfile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        QDataStream out(pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        out << distance;
        pointfile->close();
    }
    if (saveAsZeroSignal)   {
        zerofilename = "zerofile.dat";
        if (zerofile)
            delete zerofile;
        zerofile =new QFile(zerofilename);
        if (zerofile && zerofile->open(QIODevice::WriteOnly/* | QIODevice::Append*/)) {
            zerofile->close();
        }
    }

}

void DataSource::start_recording(QString fbasename)
{
    QString strN = QString::number(_N);

    fname = fbasename + "_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_" + strN + ".dat";//  + "_" + QString("%1").arg(QString::number(distance, 'f', 2).toDouble(), 5, 10, QChar('0'))+QString(".bin");
    fname = fname.replace(":", "_");
    if (pointfile)
        delete pointfile;
    pointfile = new QFile(fname);
    if (pointfile && pointfile->open(QIODevice::WriteOnly/* | QIODevice::Append*/)) {
        QDataStream out(pointfile);
        out.setVersion(QDataStream::Qt_5_9);
        out << nchannels;
        out << _N;
        pointfile->close();
    }
}

void DataSource::open_file(QString openfname)
{    
    QFile file(openfname);
    if (file.open(QIODevice::ReadOnly))    {
        QDataStream fileStream(&file);

        int _nchannels, _n;
        fileStream >> _nchannels;
        fileStream >> _n;
        if (_nchannels > 0 && _n > 0)   {
            clearMeasurementData();
            /*auto fSize = file.size();
            auto sizeof_int = sizeof(int);
            auto sizeof_double = sizeof(double);*/
            auto nMeasurements = (file.size() - 2 * sizeof(int)) / (_nchannels * (_n  * sizeof(double) + sizeof(int)) + sizeof(double));
            qDebug() << "nMeasurements = " << nMeasurements;

            _data.resize(nMeasurements);

            for(auto m=0; m<nMeasurements; m++){
                measurement _M;
                fileStream >> _M.distance;
                _M.buffer.resize(_nchannels);
                for(auto c=0; c<_nchannels; c++)    {
                    int channel;
                    fileStream >> channel;
                    if (channel >= _nchannels)  {
                        qDebug() << "invalid channel number";
                        file.close();
                        return;
                    }
                    _M.buffer[channel] = new double[_n];
                    for(auto i=0; i<_n; i++)    {
                        fileStream >> _M.buffer[channel][i];
                    }
                }
                _data[m] = _M;
            }
        }
        else {
            qDebug() << "Wrong packet size";
        }
        file.close();
        showByIndex(0);
    }
}

int DataSource::get_channel_shift(int c)
{
    if (c >= 0 && c < shY.size())
        return shY[c];
}

void DataSource::set_channel_shift(int c, int sh)
{
    if (c >= 0 && c < shY.size())
        shY[c] = sh;
}

int DataSource::getSubtractZeroSignal()
{
    return subtractZeroSignal;
}

void DataSource::setSubtractZeroSignal(int s)
{
    subtractZeroSignal = s;
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
    if (index>=0 && index < _data.length())   {
        for (auto c=0; c<nchannels; c++)    {
            for (auto i=0; i<buffer_size; i++)  {
                buffer_in[i] = _data[index].buffer[c][i];
            }
            showFromBuffer(c);
        }
        QMetaObject::invokeMethod((QObject*)object, "setDistance", Q_ARG(QVariant, QString::number(_data[index].distance)));
    }
}

void DataSource::generateData(QByteArray *buffer, int row)
{
   mtx.lock();



    if (!object)
        object = m_appViewer->rootObject();

    unsigned short i = (((*buffer)[0] << 8) + (*buffer)[1] );


    datafile->write(*buffer);

    auto N = (*buffer).length();

    if (N / 2 - 1 > buffer_size){
        buffer_size = N / 2 - 1;
        if (buffer_in) delete [] buffer_in;
        if (buffer_out) delete [] buffer_out;
        buffer_in = new double[buffer_size];
        buffer_out = new double[buffer_size];
    }



    //m_data.clear();

    int b_index = i%2;
    //qDebug() << "generateData b_index=" << b_index;

    unsigned short med = 0x8000;

    //buffer_in[0] = 0;
    double signal_sum = 0;
    for(auto j=0; j<buffer_size; j++) {
        unsigned char b0 = (unsigned char)(*buffer)[(j+1)*2];
        unsigned char b1 = (unsigned char)(*buffer)[(j+1)*2+1];
        //unsigned short p = ((*buffer)[i] << 8) + (*buffer)[i+1];
        double p = (b0 << 8) + b1 - med /*+ shY[b_index] - ((subtractZeroSignal)?(zero_signal[b_index][j]):(0.0))*/;
        //QString s;
        //s.sprintf("%02X", p);
        //markupfile->write(s.toUtf8());

        //markupfile->write(QString::number(p).toUtf8());
        //markupfile->write(QByteArray(";"));


        int x = j;


        buffer_in[x] = p;
        signal_sum += p;
        //pointsS.append(QPointF(x, 0));
    }   
    signal_sum = signal_sum / buffer_size;
    for (auto i=0; i<buffer_size; i++) {
        buffer_in[i] -= signal_sum + ((subtractZeroSignal)?(zero_signal[b_index][i]):(0.0));
    }
    buffer_in[0] = 0;
    //calc_correlate_func(points, pointsS, signal, kt_dt, N / 2 - 1);

    showFromBuffer(b_index);

    mtx.unlock();
}
void DataSource::Make_HP_ButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford)
{
  std::vector<complexd> Poles(ford); // полюса аналогового фильтра прототипа
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
void DataSource::Make_LP_ButterworthFilter(vectorc& H, double Td, double fc, unsigned short ford)
{
  std::vector<complexd> Poles(ford); // полюса аналогового фильтра прототипа
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
int DataSource::_FindMaxValueInRangeOFArray(double* smp, int numsmp, int lowlim, int highlim)
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

void DataSource::accumulateChannel(int b_index)
{
    if (fcount[b_index] < 0)
        return;
    for (auto i=0; i<buffer_size; i++)  {
        raw_acc[b_index][i] += buffer_in[i];
       // processed_acc[b_index][i] += buffer_out[i];
    }
    auto _tmp = QString::number((fcount[b_index] * 1.0) / (nframes * 1.0));
    QMetaObject::invokeMethod((QObject*)object, "changeProgressBar", Q_ARG(QVariant, _tmp));
    fcount[b_index]--;
    if (fcount[b_index] == 0)    {
        for (auto i=0; i<buffer_size; i++)  {
            raw_acc[b_index][i] = raw_acc[b_index][i] / nframes;
           // processed_acc[b_index][i] = processed_acc[b_index][i] / nframes;
        }

        unsigned short med = 0x8000;
        if (pointfile && pointfile->open(QIODevice::WriteOnly | QIODevice::Append)) {
            QDataStream out(pointfile);
            out.setVersion(QDataStream::Qt_5_9);
            out << b_index;
            for (auto i=0; i<buffer_size; i++){
                //out << std::htons(((unsigned short)raw_acc[b_index][i] + med));
                out << raw_acc[b_index][i];
                if (saveAsZeroSignal)   {
                    zero_signal[b_index][i] = raw_acc[b_index][i];
                }
            }


            /*for (auto i=0; i<buffer_size; i++){
                out << processed_acc[b_index][i];
            }*/
            if (saveAsZeroSignal)   {
                if (zerofile && zerofile->open(QIODevice::WriteOnly | QIODevice::Append)) {
                    QDataStream outz(zerofile);
                    outz.setVersion(QDataStream::Qt_5_9);
                    outz << b_index;

                    for (auto i=0; i<buffer_size; i++){
                       // outz << ((unsigned short)zero_signal[b_index][i]+med);
                        outz << zero_signal[b_index][i];
                    }
                    zerofile->close();
                }
                QMetaObject::invokeMethod((QObject*)object, "uncheck_checkBox_saveAsZeroSignal");
                saveAsZeroSignal --;
            }

            pointfile->close();


        }
        is_measured --;
    }
}

void DataSource::clearMeasurementData()
{
    for(auto i=0; i < _data.size(); i++){
       /* if (_data[i])
            delete _data[i];*/
        for(auto j=0;j < _data[i].buffer.size(); j++){
            if (_data[i].buffer[j])
                delete _data[i].buffer[j];
        }
    }
    _data.clear();
}
