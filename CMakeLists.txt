cmake_minimum_required(VERSION 3.19)
project(WindowGame LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(WindowGame
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    phase1Widget.h phase1Widget.cpp phase1Widget.ui
    phase2Widget.h phase2Widget.cpp phase2Widget.ui
    phase3Widget.h phase3Widget.cpp phase3Widget.ui
    phase4Widget.h phase4Widget.cpp phase4Widget.ui
    runawayButton.h runawayButton.cpp runawayButton.ui
    cloneWindow.h cloneWindow.cpp cloneWindow.ui
)

target_link_libraries(WindowGame
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS WindowGame
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WindowGame
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
