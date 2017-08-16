QT += charts qml quick network

HEADERS += \
    datasource.h \
    shared_mutex.h \
    address_provider.h \
    intercom.h \
    ipslistmodel.h

SOURCES += \
    main.cpp \
    datasource.cpp \
    shared_mutex.cpp \
    address_provider.cpp \
    intercom.cpp \
    ipslistmodel.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/*

