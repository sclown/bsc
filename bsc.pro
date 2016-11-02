TEMPLATE =  app
CONFIG +=  qt thread warn_on x11
QT += widgets
QMAKE_MAC_SDK = macosx10.12
#QMAKE_CXXFLAGS_DEBUG = -O0 -g
#QMAKE_CXXFLAGS_RELEASE = -O3
MOC_DIR =  ./tmp
SOURCES =  \
        QBtAttrDialog.cpp \
        QBtButtonSpace.cpp \
	QBtCanOverwrite.cpp \
	QBtColorDemo.cpp \
	QBtCompareDirsDialog.cpp \
	QBtCompareFileDialog.cpp \
	QBtConfig.cpp \
	QBtConfigApps.cpp \
	QBtConfigButtons.cpp \
	QBtConfigDialog.cpp \
	QBtConfigExtensions.cpp \
	QBtConfigPanels.cpp \
	QBtCopyDialog.cpp \
	QBtDeleteQuest.cpp \
	QBtDeleter.cpp \
	QBtDirCopyDialog.cpp \
	QBtDirModel.cpp \
	QBtDirParser.cpp \
	QBtDirSelector.cpp \
	QBtDirWorkedThread.cpp \
	QBtEditExtensions.cpp \
	QBtEventsController.cpp \
	QBtFileComparator.cpp \
	QBtFileEditor.cpp \
	QBtFileJoiner.cpp \
	QBtFileViewer.cpp \
	QBtFinder.cpp \
	QBtHelp.cpp \
	QBtInfoLabel.cpp \
	QBtMainWindow.cpp \
	QBtPackDialog.cpp \
	QBtPanel.cpp \
	QBtPointsButton.cpp \
	QBtSettings.cpp \
	QBtShared.cpp \
	QBtSyncDirsDialog.cpp \
	QBtSystemCall.cpp \
	QBtSystemInfo.cpp \
	QBtTimeStamp.cpp \
	QBtView.cpp \
	QBtViewDelegate.cpp \
	QBtViewModel.cpp \
        QBtViewSelectionModel.cpp \
	QBtWorkedThread.cpp \
	QBtWorkspace.cpp \
	bsc.cpp \
        3rdprty/dircompleter.cpp
HEADERS =  \
	BtTypes.h \
	QBtAttrDialog.h \
	QBtButtonSpace.h \
	QBtCanOverwrite.h \
	QBtColorDemo.h \
	QBtCompareDirsDialog.h \
	QBtCompareFileDialog.h \
	QBtConfig.h \
	QBtConfigApps.h \
	QBtConfigButtons.h \
	QBtConfigDialog.h \
	QBtConfigExtensions.h \
	QBtConfigPanels.h \
	QBtCopyDialog.h \
	QBtDeleteQuest.h \
	QBtDeleter.h \
	QBtDirCopyDialog.h \
	QBtDirModel.h \
	QBtDirParser.h \
	QBtDirSelector.h \
	QBtDirWorkedThread.h \
	QBtEditExtensions.h \
	QBtEvent.h \
	QBtEventsController.h \
	QBtFileComparator.h \
	QBtFileEditor.h \
	QBtFileInfo.h \
	QBtFileJoiner.h \
	QBtFileViewer.h \
	QBtFinder.h \
	QBtHelp.h \
	QBtInfoField.h \
	QBtInfoLabel.h \
	QBtMainWindow.h \
	QBtPackDialog.h \
	QBtPanel.h \
	QBtPointsButton.h \
	QBtSettings.h \
	QBtShared.h \
	QBtSyncDirsDialog.h \
	QBtSystemCall.h \
	QBtSystemInfo.h \
	QBtTabBar.h \
	QBtTimeStamp.h \
	QBtView.h \
	QBtViewDelegate.h \
	QBtViewItem.h \
        QBtViewModel.h \
        QBtViewSelectionModel.h \
        QBtViewStandardItem.h \
	QBtWorkedThread.h \
	QBtWorkspace.h \
        3rdprty/dircompleter.h
RESOURCES =  bsc.qrc
APP_SCPT_FILES.files = scripts/openTerminal.scpt \
	scripts/openFinder.scpt 
APP_SCPT_FILES.path = Contents/Resources/scripts
QMAKE_BUNDLE_DATA += APP_SCPT_FILES
