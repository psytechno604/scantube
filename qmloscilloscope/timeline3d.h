#ifndef TIMELINE3D_H
#define TIMELINE3D_H

#include <QObject>
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

#include "datasource.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DExtras;

class Timeline3D : public QEntity
{
    Q_OBJECT
public:
    Timeline3D(QNode *parent = 0);
    Q_INVOKABLE void addScan();
    Q_INVOKABLE void connectDataSource(DataSource *dataSource);
private:
    DataSource *dataSource {nullptr};

    QCamera *m_camera;

    Qt3DCore::QTransform *lightTransform;

    int m_count;
    float x,y;

    void onTimerUpdate();

    //QPlaneMesh* m_planeMesh;
    QPhongAlphaMaterial *m_planeMaterial;

    Qt3DRender::QBuffer *vertexBuffer;
    QAttribute *positionAttribute;

    //todo temporary, remove
    QGeometryRenderer* m_geometryRenderer;

    void addPoint();
};

#endif // TIMELINE3D_H
