######################################################################
# Automatically generated by qmake (3.1) Thu Nov 30 02:14:53 2017
######################################################################

TEMPLATE = app
TARGET = kulanday

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += qt debug
QT += widgets\
    sql\
    multimedia

# Input
HEADERS += src/q_kulanday_main_window.h\
    src/q_decks_overview.h \
    src/q_deck_overview.h \
    src/db_adapter.h \
    src/q_dirtydozen_widget.h

SOURCES += src/kulanday.cpp\
    src/q_kulanday_main_window.cpp\
    src/q_decks_overview.cpp \
    src/q_deck_overview.cpp \
    src/db_adapter.cpp \
    src/q_dirtydozen_widget.cpp

#RESOURCES += kulanday.qrc

release:DESTDIR = release
release:OBJECTS_DIR = release/obj
release:MOC_DIR = release/moc
release:RCC_DIR = release/rcc
release:UI_DIR = release/ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/obj
debug:MOC_DIR = debug/moc
debug:RCC_DIR = debug/rcc
debug:UI_DIR = debug/ui
