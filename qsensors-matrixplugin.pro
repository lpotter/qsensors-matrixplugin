TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtsensors_matrix)
QT = core sensors
CONFIG += plugin debug

OTHER_FILES = plugin.json \
    Sensors.conf
CONFIG += c++11

LIBS += -lmatrix_creator_hal

SOURCES += main.cpp \
    matrixsensorbase.cpp \
    matrixcompass.cpp \
    matrixaccelerometer.cpp \
    matrixgyroscope.cpp \
    matrixpressuresensor.cpp \
    matrixtemperaturesensor.cpp \
    matrixmagnetometer.cpp \
    matrixrotationsensor.cpp \
    matrixaltitudesensor.cpp

HEADERS += \
    matrixsensorbase.h \
    qsensors-matrixpluginprivate_p.h \
    matrixcompass.h \
    matrixaccelerometer.h \
    matrixgyroscope.h \
    matrixpressuresensor.h \
    matrixtemperaturesensor.h \
    matrixmagnetometer.h \
    matrixrotationsensor.h \
    matrixaltitudesensor.h


PLUGIN_TYPE = sensors

DESTDIR = $$PLUGIN_TYPE

target.path = $$[QT_INSTALL_PLUGINS]/$$PLUGIN_TYPE
INSTALLS += target

config.path=/etc/xdg/QtProject
config.files= Sensors.conf
INSTALLS += config
