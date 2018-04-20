######################################################################
# Automatically generated by qmake (3.1) Thu Nov 30 02:14:53 2017
######################################################################

TEMPLATE = app
TARGET = kulanday

linux-g++ | linux-g++-64 | linux-g++-32 {
    # on linux we have to use gstreamer for audio recording directly
    # because QAudioRecorder is broken here for over a year now
    INCLUDEPATH += "/usr/include/gstreamer-1.0/"
    INCLUDEPATH += "/usr/include/glib-2.0/"
    INCLUDEPATH += "/usr/lib/glib-2.0/include/"
    
    LIBS += -lgstreamer-1.0
    LIBS += -lgobject-2.0
    LIBS += -lglib-2.0
    
    HEADERS += src/audio_recorder.h
    SOURCES += src/audio_recorder.cpp
}
win32 {
    # QAudioRecorder works here properly
}

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
    multimedia\
    svg

# Input
HEADERS += \
    src/q_kulanday_main_window.h\
    src/db_adapter.h \
    src/q_dirtydozen_widget.h \
    src/q_kulanday_menubar.h \
    src/q_create_new_deck_dialog.h \
    src/q_deck_item_widget.h \
    src/q_deck_widget.h \
    src/q_decks_widget.h \
    src/q_audio_list_table.h \
    src/q_click_label.h

SOURCES += \
    src/kulanday.cpp\
    src/q_kulanday_main_window.cpp\
    src/db_adapter.cpp \
    src/q_dirtydozen_widget.cpp \
    src/q_kulanday_menubar.cpp \
    src/q_create_new_deck_dialog.cpp \
    src/q_deck_item_widget.cpp \
    src/q_deck_widget.cpp \
    src/q_decks_widget.cpp \
    src/q_audio_list_table.cpp \
    src/q_click_label.cpp

RESOURCES += \
    resources.qrc

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
