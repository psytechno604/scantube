#ifndef MEASUREMENTDETAILS_H
#define MEASUREMENTDETAILS_H

#include <QObject>

class MeasurementDetails
{
public:
    Q_PROPERTY(int angle READ angle WRITE setAngle NOTIFY angleChanged)
    MeasurementDetails();
};

#endif // MEASUREMENTDETAILS_H
