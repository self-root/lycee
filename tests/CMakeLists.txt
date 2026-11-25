find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(lycee_tests
    repository_test.cpp
    ../avgcalculator.cpp
    ../avgcalculator.h
    ../clipboardparser.cpp
    ../clipboardparser.h
    ../databaseaccess.cpp
    ../databaseaccess.h
    ../finalavg.cpp
    ../finalavg.h
    ../klass.cpp
    ../klass.h
    ../student.cpp
    ../student.h
    ../studentgrade.cpp
    ../studentgrade.h
    ../subject.cpp
    ../subject.h
    ../trimesteravg.cpp
    ../trimesteravg.h
)

target_include_directories(lycee_tests PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(lycee_tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME lycee.repository COMMAND lycee_tests)
