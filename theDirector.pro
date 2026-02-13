QT += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += no_keywords

LIBS += -lgmpxx -lgmp -fopenmp -larrow -lparquet $$system(pkg-config --libs python3)
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

INCLUDEPATH += /usr/include/python3.10
DISTFILES += parquet_plt.py
script.path = $$target.path
script.files = parquet_plt.py
INSTALLS += target script
QMAKE_POST_LINK += cp -f \"$$PWD/parquet_plt.py\" \"$$OUT_PWD/\"
