cmake_minimum_required(VERSION 2.8.11)

project(TTKConfig)

set(HEADER_FILES
  musicconfigdefine.h
  musicconfigobject.h
)

set(SOURCE_FILES
  musicconfigobject.cpp
)

SET(QRC_FILES
  ${MUSIC_UI_DIR}/MusicApp.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} Qt5::Core)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY})
endif()
