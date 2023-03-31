cmake_minimum_required(VERSION 2.8.11)

project(GenerateSkin)

set(HEADER_FILES
  toolsskin.h
)

set(SOURCE_FILES
  toolsskin.cpp
  main.cpp
)
  
set(UI_FILES
  toolsskin.ui
)
  
if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_wrap_ui(UIC_FILES ${UI_FILES})
  
  add_executable(${PROJECT_NAME} ${UIC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets TTKCore)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_wrap_ui(UIC_FILES ${UI_FILES})
  
  add_executable(${PROJECT_NAME} ${UIC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} TTKCore)
endif()
