cmake_minimum_required(VERSION 2.8.11)

project(TTKUi)

SET(QRC_FILES
  ${MUSIC_UI_DIR}/MusicPlayer.qrc
)

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_add_resources(RCC_FILES ${QRC_FILES})
  
  if(WIN32)
    list(APPEND RCC_FILES TTKUi.rc)
  endif()
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} Qt5::Core)
else()
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  if(WIN32)
    list(APPEND RCC_FILES TTKUi.rc)
  endif()
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY})
endif()
