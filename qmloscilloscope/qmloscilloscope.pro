QT += charts qml quick network

HEADERS += \
    datasource.h \
    shared_mutex.h \
    tssocket.h \
    address_provider.h \
    intercom.h

SOURCES += \
    main.cpp \
    datasource.cpp \
    shared_mutex.cpp \
    tssocket.cpp \
    address_provider.cpp \
    intercom.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/*

