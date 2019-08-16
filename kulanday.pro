######################################################################
# Automatically generated by qmake (3.1) Thu Nov 30 02:14:53 2017
######################################################################

TEMPLATE = app
TARGET = kulanday

#linux-g++ | linux-g++-64 | linux-g++-32 {
#    # on linux we have to use gstreamer for audio recording directly
#    # because QAudioRecorder is broken here for over a year now
#    INCLUDEPATH += "/usr/include/gstreamer-1.0/"
#    INCLUDEPATH += "/usr/include/glib-2.0/"
#    INCLUDEPATH += "/usr/lib/glib-2.0/include/"
#    
#    LIBS += -lgstreamer-1.0
#    LIBS += -lgobject-2.0
#    #LIBS += -lglib-2.0
#
#    HEADERS += src/gst_audio_recorder.h
#    SOURCES += src/gst_audio_recorder.cpp
#}
win32 {
    RC_ICONS += ./assets/logo.ico
    
    CONFIG += static

    INCLUDEPATH += "assets/windows/"
    INCLUDEPATH += "assets/windows/"
    
    #LIBS += -lqminimal
    #LIBS += -lqwindows
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

CONFIG += qt debug #release
#CONFIG += qt release
QT += widgets\
    sql\
    multimedia\
    svg

# Input
HEADERS += \
    src/db_adapter.h \
    src/deck_item_markers_widget.h \
    src/deck_item_simple_widget.h \
    src/filter_language_combo.h \
    src/help_about_widget.h \
    src/help_markers_widget.h \
    src/q_click_label.h\
    src/settings_widget.h \
    src/text_edit_markers.h \
    src/text_edit_resizing.h \
    src/unicode_fonts.h \
    src/config.h \
    src/audio_recorder.h \
    src/db_adapter_meta.h \
    src/menubar.h \
    src/search_widget.h \
    src/main_window.h \
    src/audio_list_table.h \
    src/create_new_deck_dialog.h \
    src/inv_dirty_dozen_widget.h \
    src/dirtydozen_widget.h \
    src/decks_widget.h \
    src/deck_widget.h \
    src/move_item_dialog.h \
    src/compress_folder.h \
    src/export_decks_dialog.h \
    src/audio_converter.h

SOURCES += \
    src/deck_item_markers_widget.cpp \
    src/deck_item_simple_widget.cpp \
    src/filter_language_combo.cpp \
    src/help_about_widget.cpp \
    src/help_markers_widget.cpp \
    src/kulanday.cpp\
    src/db_adapter.cpp \
    src/q_click_label.cpp\
    src/settings_widget.cpp \
    src/text_edit_markers.cpp \
    src/text_edit_resizing.cpp \
    src/unicode_fonts.cpp \
    src/config.cpp \
    src/audio_recorder.cpp \
    src/db_adapter_meta.cpp \
    src/menubar.cpp \
    src/search_widget.cpp \
    src/main_window.cpp \
    src/audio_list_table.cpp \
    src/create_new_deck_dialog.cpp \
    src/inv_dirty_dozen_widget.cpp \
    src/dirtydozen_widget.cpp \
    src/decks_widget.cpp \
    src/deck_widget.cpp \
    src/move_item_dialog.cpp \
    src/compress_folder.cpp \
    src/export_decks_dialog.cpp \
    src/audio_converter.cpp

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
