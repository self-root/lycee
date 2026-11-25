cmake_minimum_required(VERSION 3.16)

project(Lycee VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(CTest)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Gui
    PrintSupport
    QuickWidgets
    Sql
    Widgets
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Gui
    PrintSupport
    QuickWidgets
    Sql
    Widgets
)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/xlnt/CMakeLists.txt")
    message(FATAL_ERROR
        "The xlnt submodule is missing. Run: "
        "git submodule update --init --recursive"
    )
endif()

# Build the bundled xlnt submodule as a static dependency. Keeping its optional
# programs disabled makes a normal Lycee build compile only what the app uses.
set(STATIC ON CACHE BOOL "Build xlnt as a static library" FORCE)
set(XLNT_CXX_LANG "17" CACHE STRING "C++ standard used by xlnt" FORCE)
set(TESTS OFF CACHE BOOL "Build xlnt tests" FORCE)
set(SAMPLES OFF CACHE BOOL "Build xlnt samples" FORCE)
set(BENCHMARKS OFF CACHE BOOL "Build xlnt benchmarks" FORCE)
set(PYTHON OFF CACHE BOOL "Build xlnt Python bindings" FORCE)
add_subdirectory(xlnt EXCLUDE_FROM_ALL)

# The selected xlnt fork uses uint32_t in a public header without including
# <cstdint>. Force that standard header into xlnt translation units while
# keeping the submodule itself pristine and updateable.
if(MSVC)
    target_compile_options(xlnt PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:/FIcstdint>"
    )
else()
    target_compile_options(xlnt PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:-include;cstdint>"
    )
endif()

set(LYCEE_SOURCES
    avgcalculator.cpp
    clipboardparser.cpp
    controller.cpp
    databaseaccess.cpp
    distrochartmodel.cpp
    finalavg.cpp
    finalavgtablemodel.cpp
    finalavgview.cpp
    gradelistmodel.cpp
    gradestableview.cpp
    home.cpp
    homecontroller.cpp
    klass.cpp
    klassespage.cpp
    klasslistmodel.cpp
    klasslistview.cpp
    main.cpp
    mainwindow.cpp
    newstudentform.cpp
    notespage.cpp
    pdfcreator.cpp
    rwexcel.cpp
    schoolyearlistmodel.cpp
    schoolyearsmodel.cpp
    schoolyearsview.cpp
    settingspage.cpp
    student.cpp
    studentgrade.cpp
    studentlistmodel.cpp
    studentpage.cpp
    studentpiemodel.cpp
    studentstableview.cpp
    subject.cpp
    subjectlistmodel.cpp
    subjectlisttableview.cpp
    totalisationcreateform.cpp
    trimesteravg.cpp
)

set(LYCEE_HEADERS
    avgcalculator.h
    clipboardparser.h
    controller.h
    databaseaccess.h
    distrochartmodel.h
    finalavg.h
    finalavgtablemodel.h
    finalavgview.h
    gradelistmodel.h
    gradestableview.h
    home.h
    homecontroller.h
    klass.h
    klassespage.h
    klasslistmodel.h
    klasslistview.h
    mainwindow.h
    newstudentform.h
    notespage.h
    pdfcreator.h
    rwexcel.h
    schoolyearlistmodel.h
    schoolyearsmodel.h
    schoolyearsview.h
    settingspage.h
    student.h
    studentgrade.h
    studentlistmodel.h
    studentpage.h
    studentpiemodel.h
    studentstableview.h
    subject.h
    subjectlistmodel.h
    subjectlisttableview.h
    totalisationcreateform.h
    trimesteravg.h
    utils.h
)

set(LYCEE_FORMS
    home.ui
    klassespage.ui
    mainwindow.ui
    newstudentform.ui
    notespage.ui
    settingspage.ui
    studentpage.ui
    totalisationcreateform.ui
)

add_executable(Lycee WIN32 MACOSX_BUNDLE
    ${LYCEE_SOURCES}
    ${LYCEE_HEADERS}
    ${LYCEE_FORMS}
    resources.qrc
)

target_link_libraries(Lycee PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
    xlnt
)

install(TARGETS Lycee
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

option(LYCEE_BUILD_TESTS "Build the Lycee test suite" ON)
if(BUILD_TESTING AND LYCEE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
