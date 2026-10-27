cmake_minimum_required(VERSION 3.25)

# Must be set before project(). Native arch by default;
# pass -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" for a universal binary.
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum macOS version")

project(bsc VERSION 4.1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.6 REQUIRED COMPONENTS Widgets Concurrent)
include(GNUInstallDirs)
qt_standard_project_setup()

set(BSC_SOURCES
    QBtAttrDialog.cpp
    QBtButtonSpace.cpp
    QBtCanOverwrite.cpp
    QBtConfig.cpp
    QBtCopyDialog.cpp
    QBtDeleteQuest.cpp
    QBtDeleter.cpp
    QBtDirModel.cpp
    QBtEventsController.cpp
    QBtFileViewer.cpp
    QBtFileSearch.cpp
    QBtFinder.cpp
    QBtHelp.cpp
    QBtInfoLabel.cpp
    QBtMainWindow.cpp
    QBtPackDialog.cpp
    QBtPanel.cpp
    QBtPointsButton.cpp
    QBtSettings.cpp
    QBtShared.cpp
    QBtSystemCall.cpp
    QBtSystemInfo.cpp
    QBtView.cpp
    QBtViewDelegate.cpp
    QBtViewModel.cpp
    QBtViewSelectionModel.cpp
    QBtWorkspace.cpp
    QBtCommand.cpp
    bsc.cpp
    3rdprty/dircompleter.cpp
    QBtPathBox.cpp
    QBtDirListWorker.cpp
    QBtCopyWorker.cpp
    QBtFileTimes.cpp
)

set(BSC_HEADERS
    BtTypes.h
    QBtAttrDialog.h
    QBtButtonSpace.h
    QBtCanOverwrite.h
    QBtConfig.h
    QBtCopyDialog.h
    QBtDeleteQuest.h
    QBtDeleter.h
    QBtDirModel.h
    QBtEvent.h
    QBtEventsController.h
    QBtFileInfo.h
    QBtFileViewer.h
    QBtFileSearch.h
    QBtFinder.h
    QBtHelp.h
    QBtInfoField.h
    QBtInfoLabel.h
    QBtMainWindow.h
    QBtPackDialog.h
    QBtPanel.h
    QBtPointsButton.h
    QBtSettings.h
    QBtShared.h
    QBtSystemCall.h
    QBtSystemInfo.h
    QBtTabBar.h
    QBtView.h
    QBtViewDelegate.h
    QBtViewItem.h
    QBtViewModel.h
    QBtViewSelectionModel.h
    QBtViewStandardItem.h
    QBtWorkspace.h
    QBtCommand.h
    3rdprty/dircompleter.h
    QBtPathBox.h
    QBTMacTools.h
    QBtDirListWorker.h
    QBtCopyWorker.h
    QBtFileTimes.h
    QBtOverwriteAnswer.h
)

qt_add_executable(bsc
    ${BSC_SOURCES}
    ${BSC_HEADERS}
    bsc.qrc
)

target_include_directories(bsc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} 3rdprty)
target_link_libraries(bsc PRIVATE Qt6::Widgets Qt6::Concurrent)

if(MSVC)
    target_compile_options(bsc PRIVATE /W4)
else()
    target_compile_options(bsc PRIVATE -Wall -Wextra)
endif()

if(APPLE)
    enable_language(OBJCXX)
    set(BSC_BUNDLE_ID com.yourcompany.bsc)

    target_sources(bsc PRIVATE QBTMacTools.mm)
    target_link_libraries(bsc PRIVATE "-framework CoreFoundation" "-framework Cocoa")
    # QBTMacTools.mm looks the running instance up by this identifier.
    target_compile_definitions(bsc PRIVATE BSC_BUNDLE_ID="${BSC_BUNDLE_ID}")

    set(BSC_ICON ${CMAKE_CURRENT_SOURCE_DIR}/bsc.icns)
    target_sources(bsc PRIVATE ${BSC_ICON})
    set_source_files_properties(${BSC_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    set_target_properties(bsc PROPERTIES
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME bsc
        MACOSX_BUNDLE_BUNDLE_NAME "Beesoft Commander"
        MACOSX_BUNDLE_GUI_IDENTIFIER ${BSC_BUNDLE_ID}
        MACOSX_BUNDLE_ICON_FILE bsc.icns
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
elseif(WIN32)
    set_target_properties(bsc PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

install(TARGETS bsc
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Build and start the app: cmake --build --preset run
add_custom_target(run
    COMMAND $<TARGET_FILE:bsc>
    DEPENDS bsc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL
    COMMENT "Starting bsc"
)
