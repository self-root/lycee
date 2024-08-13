QT       += core gui sql quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    avgcalculator.cpp \
    controller.cpp \
    databaseaccess.cpp \
    gradelistmodel.cpp \
    gradestableview.cpp \
    home.cpp \
    homecontroller.cpp \
    klass.cpp \
    klassespage.cpp \
    klasslistmodel.cpp \
    klasslistview.cpp \
    main.cpp \
    mainwindow.cpp \
    newstudentform.cpp \
    notespage.cpp \
    student.cpp \
    studentgrade.cpp \
    studentlistmodel.cpp \
    studentpage.cpp \
    studentstableview.cpp \
    subject.cpp \
    subjectlistmodel.cpp \
    subjectlisttableview.cpp \
    trimesteravg.cpp

HEADERS += \
    avgcalculator.h \
    controller.h \
    databaseaccess.h \
    gradelistmodel.h \
    gradestableview.h \
    home.h \
    homecontroller.h \
    klass.h \
    klassespage.h \
    klasslistmodel.h \
    klasslistview.h \
    mainwindow.h \
    newstudentform.h \
    notespage.h \
    student.h \
    studentgrade.h \
    studentlistmodel.h \
    studentpage.h \
    studentstableview.h \
    subject.h \
    subjectlistmodel.h \
    subjectlisttableview.h \
    trimesteravg.h

FORMS += \
    home.ui \
    klassespage.ui \
    mainwindow.ui \
    newstudentform.ui \
    notespage.ui \
    studentpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
