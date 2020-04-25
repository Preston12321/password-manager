QT       += core gui multimedia

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
    setupdialog.cpp \
    strengthmeter.cpp \
    unlockdialog.cpp

HEADERS += \
    audiocontroller.h \
    databasecontroller.h \
    editdialog.h \
    mainwindow.h \
    passwordentry.h \
    passwordvalidator.h \
    setupdialog.h \
    strengthmeter.h \
    unlockdialog.h

FORMS += \
    editdialog.ui \
    mainwindow.ui \
    setupdialog.ui \
    unlockdialog.ui

OTHER_FILES += $$PWD/resources/

#RESOURCES += \
#    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# BEGIN COPIED CODE
# SOURCE: https://retifrav.github.io/blog/2018/06/08/qmake-copy-files/

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

copyToDestDir($$OTHER_FILES, $$OUT_PWD/resources/)

# END COPIED CODE
