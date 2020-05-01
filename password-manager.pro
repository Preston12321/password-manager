QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(QtAwesome/QtAwesome/QtAwesome.pri)
include(Qt-AES/qaesencryption.pri)

SOURCES += \
    audiocontroller.cpp \
    databasecontroller.cpp \
    editdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    passwordvalidator.cpp \
    ratingcontroller.cpp \
    setupdialog.cpp \
    unlockdialog.cpp

HEADERS += \
    audiocontroller.h \
    databasecontroller.h \
    editdialog.h \
    mainwindow.h \
    passwordentry.h \
    passwordvalidator.h \
    ratingcontroller.h \
    setupdialog.h \
    unlockdialog.h

FORMS += \
    editdialog.ui \
    mainwindow.ui \
    setupdialog.ui \
    unlockdialog.ui

win32 {
    FLAG = /s
}
unix {
    FLAG = -r
}

QMAKE_CLEAN += $$OUT_PWD/resources/ $$FLAG
QMAKE_DISTCLEAN += $$OUT_PWD/resources/ $$FLAG

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# The code below is based on code from the following post:
# https://retifrav.github.io/blog/2018/06/08/qmake-copy-files/

# Copy the given files to the given subdirectory of the output directory
defineTest(copyToOutputDir) {
    files = $$1
    dir = $$OUT_PWD/$$2

    # Use platform-specific slashes for output directory
    win32:dir ~= s,/,\\,g

    !equals(OUT_PWD, $$PWD) {
        win32 {
            QMAKE_POST_LINK += rmdir /s /q $$shell_quote($$dir) &
        }
        unix {
            QMAKE_POST_LINK += rm -r -f $$shell_quote($$dir);
        }

        for(file, files) {
            # Use platform-specific slashes for input directory
            win32:file ~= s,/,\\,g

            QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
        }

        export(QMAKE_POST_LINK)
    }
}

copyToOutputDir($$PWD/resources/, resources/)

