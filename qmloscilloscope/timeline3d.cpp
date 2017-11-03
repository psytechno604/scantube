#include <Qt3DInput>

#include "timeline3d.h"

#define ANGLE 13
#define COUNT 8

Timeline3D::Timeline3D(QNode *parent)
    : QEntity(parent)
    , m_camera(new QCamera())
    , m_count(0)
    , y(5000.0f)
    , x(50000.0f)
{
    m_camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);

    // FrameGraph
    QRenderSettings *renderSettings = new QRenderSettings();
    QRenderSurfaceSelector *renderSurfaceSelector = new QRenderSurfaceSelector();
    
    QViewport *viewport = new QViewport();
    viewport->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
    
    QLayerFilter *layerFilter = new QLayerFilter();
    layerFilter->setEnabled(false);
    QCameraSelector *cameraSelector = new QCameraSelector();
    cameraSelector->setCamera(m_camera);
    
    QClearBuffers *clearBuffer = new QClearBuffers();
    clearBuffer->setBuffers(QClearBuffers::ColorDepthStencilBuffer);
    clearBuffer->setClearColor("black");
    
    clearBuffer->setParent(cameraSelector);
    cameraSelector->setParent(layerFilter);
    layerFilter->setParent(renderSurfaceSelector);
    
    renderSurfaceSelector->setParent(viewport);
    
    renderSettings->setActiveFrameGraph(viewport);
    this->addComponent(renderSettings);
       
    m_camera->setPosition(QVector3D(0.0f,5.0f,0.0f) +
                          QVector3D(-25.0f,0.0f,0.0f));
    m_camera->setViewCenter(QVector3D(0.0f,0.0f,0.0f));
    m_camera->setUpVector(QVector3D(0.0f,1.0f,0.0f));

    //phong alpha 0.3 material
    //m_planeMaterial = new QPhongAlphaMaterial();
    m_planeMaterial = new QMaterial();
    //m_planeMaterial->setAmbient(QColor(100,100,100));
    //m_planeMaterial->setDiffuse(QColor(255,255,255));
   // m_planeMaterial->setAlpha(0.3f);


    //m_planeMaterial->addParameter(new QParameter("wireframe", QVariant(true)));
    m_planeMaterial->addParameter(new QParameter("line.width", QVariant(2.0)));
    m_planeMaterial->addParameter(new QParameter("line.color", QVariant(QColor(0,0,0,1))));
    //m_planeMaterial->


    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(this);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("yellow");
    light->setIntensity(1000);
    lightEntity->addComponent(light);
    lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0, 0, 0));
    lightEntity->addComponent(lightTransform);

    m_camera->setPosition(QVector3D(y,sinf(M_PI/12)*30.0f,x+50.0f));
    m_camera->setViewCenter(QVector3D(y,0.0f,x));

    //QTimer *timer = new QTimer();
    //QObject::connect(timer, &QTimer::timeout,this,&Timeline3D::onTimerUpdate);

    //timer->start(100);
    onTimerUpdate();
}

void Timeline3D::addScan()
{
    onTimerUpdate();
    return;

    if (!dataSource)
        return;

    scan_data = dataSource->getScanData();

    if (!scan_data)
        return;
}

void Timeline3D::connectDataSource(DataSource *dataSource)
{
    this->dataSource = dataSource;
}

void Timeline3D::addPoint() {

    //return;

    float* reVertexArray;
    unsigned int* reIndexArray;

    x -= 5.0f;
    y += qrand() % 2 - 1;

    //if (!(m_count % sliceCount)) {
    if (!(m_count % 20)) {
        //every 20-th timer tick:
        m_geometryRenderer = new QGeometryRenderer();
        QGeometry* meshGeometry = new QGeometry(m_geometryRenderer);

        QByteArray vertexArray;
        //vertexArray.resize(pointCount*3*sizeof(float));
        vertexArray.resize(40*3*sizeof(float));
        reVertexArray = reinterpret_cast<float*>(vertexArray.data());

        QByteArray indexArray;
        indexArray.resize(pointCount*sizeof(int));
        reIndexArray = reinterpret_cast<unsigned int*>(indexArray.data());

        //coordinates of left vertex
        reVertexArray[0] = y-5.0f;
        reVertexArray[1] = 0.0f;
        reVertexArray[2] = x;

        //coordinates of right vertex
        reVertexArray[3] = y+5.0f;
        reVertexArray[4] = 0.0f;
        reVertexArray[5] = x;

        /**/

        /*int jV =0 , jI = 0;
        for(auto e : scan_data->keys())  {
            if (e%2)    {
                for (auto i=0; i < scan_data->value(e).count(); i++) {
                    float r = r0 + (*scan_data)[e][i];
                    float fi = (float(e) + float(i)/_N)* (2 * M_PI / sectorsCount);
                    reVertexArray[jV] = r * cos(fi);
                    reVertexArray[jV+1] = r * sin(fi);
                    reVertexArray[jV+2] = 0;
                    jV+=3;

                    reIndexArray[jI] =
                }
            }
        }*/

        vertexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer,meshGeometry);
        vertexBuffer->setUsage(Qt3DRender::QBuffer::DynamicDraw);
        vertexBuffer->setData(vertexArray);

        /*indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, meshGeometry);
        indexBuffer->setUsage(Qt3DRender::QBuffer::DynamicDraw);
        indexBuffer->setData(indexArray);*/

        // Attributes
        positionAttribute = new QAttribute(meshGeometry);
        positionAttribute->setAttributeType(QAttribute::VertexAttribute);
        positionAttribute->setBuffer(vertexBuffer);
        positionAttribute->setDataType(QAttribute::Float);
        positionAttribute->setDataSize(3);
        positionAttribute->setByteOffset(0);
        positionAttribute->setByteStride(3*sizeof(float));
        positionAttribute->setCount(2);
        positionAttribute->setName(QAttribute::defaultPositionAttributeName());

        meshGeometry->addAttribute(positionAttribute);


        /*indexAttribute = new QAttribute(meshGeometry);
        indexAttribute->setAttributeType(QAttribute::IndexAttribute);
        indexAttribute->setBuffer(indexBuffer);
        indexAttribute->setDataType(QAttribute::UnsignedInt);
        indexAttribute->setDataSize(1);*/



        m_geometryRenderer->setInstanceCount(1);
        m_geometryRenderer->setFirstVertex(0);
        m_geometryRenderer->setFirstInstance(0);
        m_geometryRenderer->setPrimitiveType(QGeometryRenderer::TriangleStrip);
        //m_geometryRenderer->setPrimitiveType(QGeometryRenderer::Triangles);
        m_geometryRenderer->setGeometry(meshGeometry);

        QEntity* entity = new QEntity(this);
        entity->addComponent(m_geometryRenderer);
        entity->addComponent(m_planeMaterial);



        return;
    }


    //update geometry
    QByteArray appendVertexArray;
    appendVertexArray.resize(2*3*sizeof(float));
    reVertexArray = reinterpret_cast<float*>(appendVertexArray.data());

    //coordinates of left vertex
    reVertexArray[0] = y-5.0f;
    reVertexArray[1] = 0.0f;
    reVertexArray[2] = x;

    //coordinates of right vertex
    reVertexArray[3] = y+5.0f;
    reVertexArray[4] = 1.0f;
    reVertexArray[5] = x;

    uint vertexCount = positionAttribute->count();
    vertexBuffer->updateData(vertexCount*3*sizeof(float),appendVertexArray);
    positionAttribute->setCount(vertexCount+2);

    qDebug() << "x=" << x << " y=" << y << " vertexCount=" << vertexCount;
}

void Timeline3D::onTimerUpdate()
{   
    addPoint();

    m_camera->setPosition(QVector3D(y,sinf(M_PI/12)*30.0f,x+50.0f));
    m_camera->setViewCenter(QVector3D(y,0.0f,x));

    //qDebug() << "x=" << x << "; y=" << y;

    lightTransform->setTranslation(QVector3D(y, 10.0f, x));

    m_count++;
}

