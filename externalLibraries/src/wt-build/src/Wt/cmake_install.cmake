# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/src/Wt

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/irina/projects/lughos/externalLibraries")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPen"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVmlImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFileResource"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMediaPlayer"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPopupMenuItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSocketNotifier"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractItemModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBreak"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTextArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDatePicker"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBatchEditProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WResource"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTreeNode"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSortFilterProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractListModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WValidationStatus"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WIntValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTimer"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WModelIndex"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFlags"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTreeTableNode"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WIdentityProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSpinBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTime"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVideo"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCombinedLocalizedStrings"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPoint"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStatelessSlot"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WWidgetItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSignalMapper"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFlashObject"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStandardItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLogger"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTreeView"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAggregateProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCompositeWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCssTheme"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCalendar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WJavaScript"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPointF"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDllDefs.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WContainerWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLocale"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTableCell"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBoxLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRasterImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDoubleSpinBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTransform"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMemoryResource"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractGLImplementation"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStringUtil"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLayoutItemImpl"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WIconPair"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WHTML5Video"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPolygonArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBrush"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WServerGLWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSignal"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractToggleButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSubMenuItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCanvasPaintDevice"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVectorImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSuggestionPopup"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WItemDelegate"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTextEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractItemDelegate"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WIOService"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDateValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTimerWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDateTime"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLengthValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMatrix4x4"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGoogleMap"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WInteractWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDefaultLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDoubleValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGroupBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRegExp"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBorderLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WInPlaceEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPaintedWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractTableModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGradient"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFitLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPanel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSound"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WString"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTabWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStreamResource"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WToolBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDate"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVirtualImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPopupMenu"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WServer"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMeasurePaintDevice"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WClientGLWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBootstrapTheme"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStackedWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTreeTable"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WIcon"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAnimation"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPushButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSplitButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDateEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRectArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCheckBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WOverlayLoadingIndicator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WProgressBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WEnvironment"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTableColumn"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMenuItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSslInfo"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLength"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTableRow"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTree"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStringListModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WJavaScriptSlot"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAudio"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WApplication"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WColor"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPainterPath"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractItemView"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTemplateFormView"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMenu"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSelectionBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStandardItemModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WEvent"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRectF"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WWebWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WException"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLink"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFileUpload"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLabel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLineEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WShadow"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGenericMatrix"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WComboBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WHBoxLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRegExpValidator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WHTML5Audio"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTemplate"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLoadingIndicator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBorder"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMessageBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WScrollArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WBoostAny"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WButtonGroup"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSvgImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFont"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVBoxLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WStringStream"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTable"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPdfImage"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTableView"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCircleArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFormWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLineF"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGlobal"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractSpinBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WItemSelectionModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPainter"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSslCertificate"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAnchor"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLayoutItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WSlider"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WTheme"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractArea"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCssDecorationStyle"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFontMetrics"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLocalDateTime"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDialog"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WObject"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WViewWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMessageResources"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WMessageResourceBundle"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAccordionLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGLWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WNavigationBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WGridLayout"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WJavaScriptPreamble"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRadioButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WDefaultLoadingIndicator"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WFormModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPopupWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVector3"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WRandom"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WCssStyleSheet"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WLocalizedStrings"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WPaintDevice"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WText"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WAbstractMedia"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WScrollBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WReadOnlyProxyModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/WVector4"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt" TYPE FILE FILES "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Utils")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Auth/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Chart/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Dbo/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Json/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Http/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Mail/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Payment/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Render/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Test/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

