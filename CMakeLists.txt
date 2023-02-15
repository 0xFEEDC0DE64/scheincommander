cmake_minimum_required(VERSION 3.16)

project(lightcontrol VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS SerialPort Quick)

qt_add_executable(applightcontrol
    main.cpp
    devicetypesmodel.h devicetypesmodel.cpp
    dmxcontroller.h dmxcontroller.cpp
    dmxcontrollerthread.h dmxcontrollerthread.cpp
    lightproject.h lightproject.cpp
    devicesmodel.h devicesmodel.cpp
)

qt_add_qml_module(applightcontrol
    URI lightcontrol
    VERSION 1.0
    QML_FILES
        main.qml
        main-dev.qml
        HomePage.qml
        SettingsPage.qml
        LightControlWindow.qml
        EditableListView.qml
        DeviceTypesSettingsPage.qml
        DevicesSettingsPage.qml
)

set_target_properties(applightcontrol PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(applightcontrol
    PRIVATE
        Qt6::SerialPort
        Qt6::Quick
)

install(TARGETS applightcontrol
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
