
#include "fft.h"

/*************************************************************************
Быстрое преобразование Фурье

Алгоритм проводит быстрое преобразование Фурье комплексной
функции, заданной nn отсчетами на действительной оси.

В зависимости от  переданных параметров, может выполняться
как прямое, так и обратное преобразование.

Входные параметры:
    nn  -   Число значений функции. Должно  быть  степенью
            двойки. Алгоритм   не  проверяет  правильность
            переданного значения.
    a   -   array [0 .. 2*nn-1] of Real
            Значения функции. I-ому значению соответствуют
            элементы   a[2*I]     (вещественная     часть)
            и a[2*I+1] (мнимая часть).
    InverseFFT
        -   направление преобразования.
            True, если обратное, False, если прямое.
            
Выходные параметры:
    a   -   результат   преобразования.   Подробнее    см.
            описание на сайте.
*************************************************************************/
void fastfouriertransform(ap::real_1d_array& a, int nn, bool inversefft)
{
    int ii;
    int jj;
    int n;
    int mmax;
    int m;
    int j;
    int istep;
    int i;
    int isign;
    double wtemp;
    double wr;
    double wpr;
    double wpi;
    double wi;
    double theta;
    double tempr;
    double tempi;

    if( inversefft )
    {
        isign = -1;
    }
    else
    {
        isign = 1;
    }
    n = 2*nn;
    j = 1;
    for(ii = 1; ii <= nn; ii++)
    {
        i = 2*ii-1;
        if( j>i )
        {
            tempr = a(j-1);
            tempi = a(j);
            a(j-1) = a(i-1);
            a(j) = a(i);
            a(i-1) = tempr;
            a(i) = tempi;
        }
        m = n/2;
        while(m>=2&&j>m)
        {
            j = j-m;
            m = m/2;
        }
        j = j+m;
    }
    mmax = 2;
    while(n>mmax)
    {
        istep = 2*mmax;
        theta = 2*ap::pi()/(isign*mmax);
        wpr = -2.0*sqr(sin(0.5*theta));
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for(ii = 1; ii <= mmax/2; ii++)
        {
            m = 2*ii-1;
            for(jj = 0; jj <= (n-m)/istep; jj++)
            {
                i = m+jj*istep;
                j = i+mmax;
                tempr = wr*a(j-1)-wi*a(j);
                tempi = wr*a(j)+wi*a(j-1);
                a(j-1) = a(i-1)-tempr;
                a(j) = a(i)-tempi;
                a(i-1) = a(i-1)+tempr;
                a(i) = a(i)+tempi;
            }
            wtemp = wr;
            wr = wr*wpr-wi*wpi+wr;
            wi = wi*wpr+wtemp*wpi+wi;
        }
        mmax = istep;
    }
    if( inversefft )
    {
        for(i = 1; i <= 2*nn; i++)
        {
            a(i-1) = a(i-1)/nn;
        }
    }
}



