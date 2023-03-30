cmake_minimum_required(VERSION 2.8.11)

project(TTKWatcher)

set(HEADER_FILES
  qdevicewatcher.h
  qdevicewatcher_p.h
)

set(SOURCE_FILES
  qdevicewatcher.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES qdevicewatcher_win32.cpp ${PROJECT_NAME}.rc)
elseif(APPLE)
  list(APPEND SOURCE_FILES qdevicewatcher_mac.cpp)
elseif(WINCE)
  list(APPEND SOURCE_FILES qdevicewatcher_wince.cpp)
else(UNIX)
  list(APPEND SOURCE_FILES qdevicewatcher_linux.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES}  ${MOC_FILES} ${HEADER_FILES})
  endif()

  list(APPEND QT5_LIBS Qt5::Core -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS -luser32)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()

  list(APPEND QT4_LIBS ${QT_QTCORE_LIBRARY} -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS -luser32)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
