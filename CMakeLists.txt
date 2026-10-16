cmake_minimum_required(VERSION 3.16)

project(Soundpad VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Widgets
    Multimedia
)

# --------------------------------------------------
# Source files
# --------------------------------------------------

set(PROJECT_SOURCES

    # Entry point
    src/main.cpp

    # MainWindow
    include/ui/mainwindow.h
    src/ui/mainwindow.cpp
    ui/mainwindow.ui

    # Implementation of MainWindow
    src/ui/defaultSetting.cpp
    src/ui/volumeControl.cpp
    src/ui/addMusic.cpp
    src/ui/soundControl.cpp
    src/ui/timeAssociated.cpp

    src/ui/connections/createModels.cpp
    src/ui/connections/loadSettings.cpp
    src/ui/connections/setupConnections.cpp
    src/ui/connections/setupContextMenus.cpp
    src/ui/connections/setupSoundManager.cpp
    src/ui/connections/setupTableView.cpp
    src/ui/connections/setupTreeView.cpp
    src/ui/connections/loadStyles.cpp
    src/ui/refreshUI.cpp
    src/ui/contextmenuLogic.cpp
    src/ui/dragDropUI.cpp

    include/ui/categoryIcon.h
    src/ui/categoryIcon.cpp

    # Add Category Dialog
    include/ui/categoryAdd.h
    src/ui/categoryAdd.cpp
    ui/categoryAdd.ui

    # Category Editing Dialog
    include/ui/categoryEdit.h
    src/ui/categoryEdit.cpp
    ui/categoryEdit.ui

    # Settings Dialog
    include/ui/settingsdialog.h
    src/ui/settingsdialog.cpp
    ui/settingsdialog.ui

    # Hotkey Dialog
    include/ui/addhotkey.h
    src/ui/addhotkey.cpp
    ui/addhotkey.ui

    # Custom Views
    include/ui/widgets/soundsTableView.h
    src/ui/widgets/soundsTableView.cpp

    include/ui/widgets/categoriesView.h
    src/ui/widgets/categoriesView.cpp

    include/ui/widgets/seekSlider.h
    src/ui/widgets/seekSlider.cpp

    # Models
    include/models/TableModel.h
    src/models/TableModel.cpp

    include/models/CategoryModel.h
    src/models/CategoryModel.cpp

    # Main logic
    include/core/repository.h
    src/core/repository.cpp

    include/core/contextManager.h
    src/core/contextManager.cpp

    include/core/hotkeymanager.h
    src/core/hotkeymanager.cpp

    include/core/SoundManager.h
    src/core/SoundManager.cpp

    # Services
    include/services/FileManager.h
    src/services/FileManager.cpp

    # Generals type and consts
    include/core/Sound.h
    include/core/SoundCategory.h
    include/core/consts.h

    # Qt-resources
    resources/resources.qrc

    # Windows-resources
    app.rc
)

# --------------------------------------------------
# Creating an executable file
# --------------------------------------------------

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(Soundpad
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/ui/cleanupSounds.cpp
    )

else()

    if(ANDROID)

        add_library(Soundpad SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(Soundpad
            ${PROJECT_SOURCES}
        )

    endif()

endif()

# --------------------------------------------------
# Paths for Locating Header Files
# --------------------------------------------------

target_include_directories(Soundpad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/widgets
)

# --------------------------------------------------
# Linking Qt Libraries
# --------------------------------------------------

target_link_libraries(Soundpad PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)


# --------------------------------------------------
# App Settings
# --------------------------------------------------

if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Soundpad
    )

endif()

set_target_properties(Soundpad PROPERTIES
    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
        ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --------------------------------------------------
# Downloading
# --------------------------------------------------

include(GNUInstallDirs)

install(TARGETS Soundpad
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --------------------------------------------------
# Finalization of Qt 6
# --------------------------------------------------

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Soundpad)
endif()
cmake_minimum_required(VERSION 3.16)

project(Soundpad VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Widgets
    Multimedia
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Multimedia
)

# --------------------------------------------------
# Source files
# --------------------------------------------------

set(PROJECT_SOURCES

    # Entry point
    src/main.cpp

    # MainWindow
    include/ui/mainwindow.h
    src/ui/mainwindow.cpp
    ui/mainwindow.ui

    # Implementation of MainWindow
    src/ui/defaultSetting.cpp
    src/ui/volumeControl.cpp
    src/ui/addMusic.cpp
    src/ui/soundControl.cpp
    src/ui/timeAssociated.cpp

    src/ui/connections/createModels.cpp
    src/ui/connections/loadSettings.cpp
    src/ui/connections/setupConnections.cpp
    src/ui/connections/setupContextMenus.cpp
    src/ui/connections/setupPlayer.cpp
    src/ui/connections/setupTableView.cpp
    src/ui/connections/setupTreeView.cpp
    src/ui/connections/loadStyles.cpp
    src/ui/findDevices.cpp
    src/ui/refreshUI.cpp
    src/ui/contextmenuLogic.cpp
    src/ui/dragDropUI.cpp

    # Add Category Dialog
    include/ui/categoryAdd.h
    src/ui/categoryAdd.cpp
    ui/categoryAdd.ui

    # Category Editing Dialog
    include/ui/categoryEdit.h
    src/ui/categoryEdit.cpp
    ui/categoryEdit.ui

    # Settings Dialog
    include/ui/settingsdialog.h
    src/ui/settingsdialog.cpp
    ui/settingsdialog.ui

    # Hotkey Dialog
    include/ui/addhotkey.h
    src/ui/addhotkey.cpp
    ui/addhotkey.ui

    # Custom Views
    include/ui/widgets/soundsTableView.h
    src/ui/widgets/soundsTableView.cpp

    include/ui/widgets/categoriesView.h
    src/ui/widgets/categoriesView.cpp

    # Models
    include/models/TableModel.h
    src/models/TableModel.cpp

    include/models/CategoryModel.h
    src/models/CategoryModel.cpp

    # Main logic
    include/core/repository.h
    src/core/repository.cpp

    include/core/contextManager.h
    src/core/contextManager.cpp

    include/core/hotkeymanager.h
    src/core/hotkeymanager.cpp

    # Services
    include/services/FileManager.h
    src/services/FileManager.cpp

    # Generals type and consts
    include/core/Sound.h
    include/core/consts.h

    # Qt-resources
    resources/resources.qrc

    # Windows-resources
    app.rc
)

# --------------------------------------------------
# Creating an executable file
# --------------------------------------------------

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(Soundpad
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/ui/cleanupSounds.cpp
    )

else()

    if(ANDROID)

        add_library(Soundpad SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(Soundpad
            ${PROJECT_SOURCES}
        )

    endif()

endif()

# --------------------------------------------------
# Paths for Locating Header Files
# --------------------------------------------------

target_include_directories(Soundpad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/widgets
)

# --------------------------------------------------
# Linking Qt Libraries
# --------------------------------------------------

target_link_libraries(Soundpad PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

# --------------------------------------------------
# App Settings
# --------------------------------------------------

if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Soundpad
    )

endif()

set_target_properties(Soundpad PROPERTIES
    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
        ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --------------------------------------------------
# Downloading
# --------------------------------------------------

include(GNUInstallDirs)

install(TARGETS Soundpad
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --------------------------------------------------
# Finalization of Qt 6
# --------------------------------------------------

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Soundpad)
endif()
