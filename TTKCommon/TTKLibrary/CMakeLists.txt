cmake_minimum_required(VERSION 2.8.11)

project(TTKLibrary)

set(HEADER_FILES
  ttkabstractmovedialog.h
  ttkabstractmoveresizewidget.h
  ttkabstractmovewidget.h
  ttkabstractresizeinterface.h
  ttkabstractthread.h
  ttkabstractxml.h
  ttkany.h
  ttkclickedgroup.h
  ttkclickedlabel.h
  ttkclickedslider.h
  ttkcommandline.h
  ttkconcurrentqueue.h
  ttkcryptographichash.h
  ttkdefer.h
  ttkdesktopwrapper.h
  ttkfileassocation.h
  ttkglobalhelper.h
  ttkitemdelegate.h
  ttklibrary.h
  ttklibraryversion.h
  ttkplatformsystem.h
  ttksemaphoreloop.h
  ttksuperenum.h
  ttktabbutton.h
  ttkthemelinelabel.h
  ttktime.h
  ttktoastlabel.h
)

set(SOURCE_FILES
  ttkabstractmovedialog.cpp
  ttkabstractmoveresizewidget.cpp
  ttkabstractmovewidget.cpp
  ttkabstractthread.cpp
  ttkabstractxml.cpp
  ttkany.cpp
  ttkclickedgroup.cpp
  ttkclickedlabel.cpp
  ttkclickedslider.cpp
  ttkcommandline.cpp
  ttkcryptographichash.cpp
  ttkdesktopwrapper.cpp
  ttkfileassocation.cpp
  ttkglobalhelper.cpp
  ttkitemdelegate.cpp
  ttkplatformsystem.cpp
  ttksemaphoreloop.cpp
  ttksuperenum.cpp
  ttktabbutton.cpp
  ttkthemelinelabel.cpp
  ttktime.cpp
  ttktoastlabel.cpp
)
  
SET(QRC_FILES
  ${PROJECT_NAME}.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES}  ${MOC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT5_LIBS Qt5::Core Qt5::Xml Qt5::Gui Qt5::Widgets)
  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT4_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY})
  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
