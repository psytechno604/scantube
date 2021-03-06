#ifndef INTERFACEHELPER_H
#define INTERFACEHELPER_H

#include <QObject>
#include <QJsonObject>

class InterfaceHelper : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceHelper(QObject *parent = nullptr);
    int ScopeView_x0{0}, ScopeView_y0 {0}, ScopeView_ymax0 {0}, ScopeView_ymin0 {0}, ScopeView_y2max0 {0}, ScopeView_y2min0 {0}, ScopeView_axisNum {0};
    int ScopeView_LB{0}, ScopeView_RB{0};
    int checkbox_subtractZeroSignal_checked {1};

signals:

public slots:

    int getIntValue(QString id);
    QString getQStringValue(QString id);
    void setValue(QString id, int value);
    void setValue(QString id, QString value);
private:
    int checkbox_fileInput_checked {0};
    QJsonObject jsonData;
};

#endif // INTERFACEHELPER_H
