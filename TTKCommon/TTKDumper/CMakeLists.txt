cmake_minimum_required(VERSION 2.8.11)

project(TTKDumper)

set(HEADER_FILES
  ttkdumper.h
  miniprocess.h
)

set(SOURCE_FILES
  ttkdumper.cpp
  miniprocess.cpp
)
  
if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES}  ${MOC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT5_LIBS Qt5::Core)
  if(WIN32)
    list(APPEND QT5_LIBS -lpsapi)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT4_LIBS ${QT_QTCORE_LIBRARY})
  if(WIN32)
    list(APPEND QT4_LIBS -lpsapi)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
