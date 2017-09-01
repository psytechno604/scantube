QT += charts qml quick network

HEADERS += \
    datasource.h \
    shared_mutex.h \
    address_provider.h \
    intercom.h \
    ipslistmodel.h \
    ap.h \
    apvt.h \
    fft.h \
    spline.h \
    interfacehelper.h \
    measurementmodel.h \
    measurement.h \
    measurement2.h

SOURCES += \
    main.cpp \
    datasource.cpp \
    shared_mutex.cpp \
    address_provider.cpp \
    intercom.cpp \
    ipslistmodel.cpp \
    ap.cpp \
    fft.cpp \
    interfacehelper.cpp \
    measurementmodel.cpp \
    measurement.cpp \
    measurement2.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/*

