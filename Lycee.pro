QT       += core gui sql quickwidgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    avgcalculator.cpp \
    clipboardparser.cpp \
    controller.cpp \
    databaseaccess.cpp \
    distrochartmodel.cpp \
    finalavg.cpp \
    finalavgtablemodel.cpp \
    finalavgview.cpp \
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
    pdfcreator.cpp \
    schoolyearlistmodel.cpp \
    schoolyearsmodel.cpp \
    schoolyearsview.cpp \
    settingspage.cpp \
    student.cpp \
    studentgrade.cpp \
    studentlistmodel.cpp \
    studentpage.cpp \
    studentpiemodel.cpp \
    studentstableview.cpp \
    subject.cpp \
    subjectlistmodel.cpp \
    subjectlisttableview.cpp \
    totalisationcreateform.cpp \
    trimesteravg.cpp

HEADERS += \
    avgcalculator.h \
    clipboardparser.h \
    controller.h \
    databaseaccess.h \
    distrochartmodel.h \
    finalavg.h \
    finalavgtablemodel.h \
    finalavgview.h \
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
    pdfcreator.h \
    schoolyearlistmodel.h \
    schoolyearsmodel.h \
    schoolyearsview.h \
    settingspage.h \
    student.h \
    studentgrade.h \
    studentlistmodel.h \
    studentpage.h \
    studentpiemodel.h \
    studentstableview.h \
    subject.h \
    subjectlistmodel.h \
    subjectlisttableview.h \
    totalisationcreateform.h \
    trimesteravg.h \
    utils.h

FORMS += \
    home.ui \
    klassespage.ui \
    mainwindow.ui \
    newstudentform.ui \
    notespage.ui \
    settingspage.ui \
    studentpage.ui \
    totalisationcreateform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
