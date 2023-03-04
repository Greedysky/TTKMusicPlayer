cmake_minimum_required(VERSION 2.8.11)

project(TTKLibrary)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
  cmake_policy(SET CMP0005 OLD)
  cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
  ttkabstractmovedialog.h
  ttkabstractmoveresizewidget.h
  ttkabstractmovewidget.h
  ttkabstractresizeinterface.h
  ttkabstractthread.h
  ttkabstractxml.h
  ttkclickedgroup.h
  ttkclickedlabel.h
  ttkclickedslider.h
  ttkcommandline.h
  ttkconcurrentqueue.h
  ttkcryptographichash.h
  ttkdesktopwrapper.h
  ttkglobalhelper.h
  ttkitemdelegate.h
  ttklibrary.h
  ttklibraryversion.h
  ttksemaphoreloop.h
  ttksuperenum.h
  ttktabbutton.h
  ttktime.h
  ttktoastlabel.h
)

set(MUSIC_SOURCES
  ttkabstractmovedialog.cpp
  ttkabstractmoveresizewidget.cpp
  ttkabstractmovewidget.cpp
  ttkabstractthread.cpp
  ttkabstractxml.cpp
  ttkclickedgroup.cpp
  ttkclickedlabel.cpp
  ttkclickedslider.cpp
  ttkcommandline.cpp
  ttkcryptographichash.cpp
  ttkdesktopwrapper.cpp
  ttkglobalhelper.cpp
  ttkitemdelegate.cpp
  ttksemaphoreloop.cpp
  ttksuperenum.cpp
  ttktabbutton.cpp
  ttktime.cpp
  ttktoastlabel.cpp
)
  
SET(MUSIC_QRC
  ${PROJECT_NAME}.qrc
)

if(WIN32)
  list(APPEND MUSIC_SOURCES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT5_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${PROJECT_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  
  set(QT5_LIBS Qt5::Core Qt5::Xml Qt5::Gui Qt5::Widgets)
  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT4_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_RCC_SRCS} ${MUSIC_HEADERS})
  else()
    add_library(${PROJECT_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_RCC_SRCS} ${MUSIC_HEADERS})
  endif()
  
  set(QT4_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY})
  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
