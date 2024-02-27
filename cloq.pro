TEMPLATE = app
TARGET = cloq

QMAKE_CXXFLAGS += -std=c++17

QT += widgets svgwidgets

SOURCES += \
    main.cc \
    cloq.cc \
    cloq_item.cc

HEADERS += \
    cloq.hh \
    cloq_item.hh
