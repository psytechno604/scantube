QT += charts qml quickcontrols2 network 3dcore 3drender 3dinput 3dquick 3dextras datavisualization

HEADERS += \
    datasource.h \
    shared_mutex.h \
    intercom.h \
    ipslistmodel.h \
    ap.h \
    apvt.h \
    fft.h \
    spline.h \
    interfacehelper.h \
    measurementmodel.h \
    scantube.h \
    measurement.h \
    timeline3d.h \
    addressprovider.h \
    peak.h \
    measurementdetails.h \
    constants.h

SOURCES += \
    main.cpp \
    datasource.cpp \
    shared_mutex.cpp \
    intercom.cpp \
    ipslistmodel.cpp \
    ap.cpp \
    fft.cpp \
    interfacehelper.cpp \
    measurementmodel.cpp \
    scantube.cpp \
    measurement.cpp \
    timeline3d.cpp \
    addressprovider.cpp \
    peak.cpp \
    measurementdetails.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/* \
    qml/qmloscilloscope/STReceiverWidget.qml

