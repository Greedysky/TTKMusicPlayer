cmake_minimum_required(VERSION 2.8.11)

project(MusicCore)
set(TARGET_NAME MusicCore)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_THREAD)

set(MUSIC_HEADERS
    ${MUSIC_Core_DIR}/network/musicdownloadthreadabstract.h
    ${MUSIC_Core_DIR}/network/musicdata2downloadthread.h
    ${MUSIC_Core_DIR}/network/musicdatadownloadthread.h
    ${MUSIC_Core_DIR}/network/musictextdownloadthread.h
    ${MUSIC_Core_DIR}/network/musicbgthemedownload.h
    ${MUSIC_Core_DIR}/network/musicnetworkthread.h
    ${MUSIC_Core_DIR}/network/musicdownloadqueuecache.h
    ${MUSIC_Core_DIR}/network/musicdownloadquerythreadabstract.h
    ${MUSIC_Core_DIR}/network/musicdownloadquerysinglethread.h
    ${MUSIC_Core_DIR}/network/musicdownloadquerymultiplethread.h
    ${MUSIC_Core_DIR}/musiclogger.h
    ${MUSIC_Core_DIR}/musicobject.h
    ${MUSIC_Core_DIR}/musicuiobject.h
    ${MUSIC_Core_DIR}/musicutils.h
    ${MUSIC_Core_DIR}/musicglobaldefine.h
    ${MUSIC_Core_DIR}/musicversion.h
    ${MUSIC_Core_DIR}/musictime.h
    ${MUSIC_Core_DIR}/musicxmlconfigmanager.h
    ${MUSIC_Core_DIR}/musicplayer.h
    ${MUSIC_Core_DIR}/musicplaylist.h
    ${MUSIC_Core_DIR}/musicabstractxml.h
    ${MUSIC_Core_DIR}/musicbgthememanager.h
    ${MUSIC_Core_DIR}/musicsettingmanager.h
    ${MUSIC_Core_DIR}/musicconnectionpool.h
    ${MUSIC_Core_DIR}/musicregeditmanager.h
    ${MUSIC_Core_DIR}/musicsingleton.h
    ${MUSIC_Core_DIR}/musiccoremplayer.h
    ${MUSIC_Core_DIR}/musicsong.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearch.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchedit.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchinlineedit.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchpopwidget.h
    ${MUSIC_Localsearch_DIR}/core/musiclocalsongsearchrecordobject.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanager.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanagerfordesktop.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanagerforinline.h
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainer.h
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainerfordesktop.h
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainerforinline.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcsearchwidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcsearchtablewidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcartphotoupload.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcartphotolabel.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatwidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatabstractwidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatsettingwidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatphotowidget.h
    ${MUSIC_Lrcmanager_DIR}/musiclrcmakerwidget.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidget.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforsquare.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforrectangle.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetfordiamond.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforcircle.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforsimplestyle.h
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforcomplexstyle.h
    ${MUSIC_Toolsetswidget_DIR}/musicmobiledeviceswidget.h
    ${MUSIC_Toolsetswidget_DIR}/musicaudiorecorderwidget.h
    ${MUSIC_Toolsetswidget_DIR}/musictimerwidget.h
    ${MUSIC_Toolsetswidget_DIR}/musiclocalsongsmanagerwidget.h
    ${MUSIC_Toolsetswidget_DIR}/musictransformwidget.h
    ${MUSIC_Toolsetswidget_DIR}/musicfileinformationwidget.h
    ${MUSIC_Toolsetswidget_DIR}/musicdesktopwallpaperwidget.h
    ${MUSIC_Toolsetswidget_DIR}/core/musicdesktopwallpaperthread.h
    ${MUSIC_Toolsetswidget_DIR}/core/musictimerautoobject.h
    ${MUSIC_Toolsetswidget_DIR}/core/musiclocalsongsmanagerthread.h
    ${MUSIC_Toolsetswidget_DIR}/core/musicsongtag.h
    ${MUSIC_Toolsetswidget_DIR}/musiclocalsongstablewidget.h
    ${MUSIC_Toolswidget_DIR}/musicsongssummarizied.h
    ${MUSIC_Toolswidget_DIR}/musictoolsetswidget.h
    ${MUSIC_Toolswidget_DIR}/musicmydownloadrecordwidget.h
    ${MUSIC_Toolswidget_DIR}/musicwebradiolistview.h
    ${MUSIC_Toolswidget_DIR}/musicwebradiowidget.h
    ${MUSIC_Toolswidget_DIR}/core/musicwebradiodatebase.h
    ${MUSIC_Toolswidget_DIR}/core/musicmydownloadrecordobject.h
    ${MUSIC_Toolswidget_DIR}/musicconnectmobilewidget.h
    ${MUSIC_Usermanager_DIR}/musicusermanager.h
    ${MUSIC_Usermanager_DIR}/musicuserdialog.h
    ${MUSIC_Usermanager_DIR}/musicuserlineedit.h
    ${MUSIC_Usermanager_DIR}/musicuserwindow.h
    ${MUSIC_Usermanager_DIR}/core/musicusermodel.h
    ${MUSIC_Usermanager_DIR}/core/musicuserconfigmanager.h
    ${MUSIC_Usermanager_DIR}/musicuserrecordwidget.h
    ${MUSIC_Videokits_DIR}/musicvideocontrol.h
    ${MUSIC_Videokits_DIR}/musicvideoview.h
    ${MUSIC_Videokits_DIR}/musicvideotablewidget.h
    ${MUSIC_Videokits_DIR}/musicvideoplaywidget.h
    ${MUSIC_Widget_DIR}/core/musicabstracttablewidget.h
    ${MUSIC_Widget_DIR}/core/musicabstractmovedialog.h
    ${MUSIC_Widget_DIR}/core/musicabstractmovewidget.h
    ${MUSIC_Widget_DIR}/core/musicitemdelegate.h
    ${MUSIC_Widget_DIR}/core/musicmarqueewidget.h
    ${MUSIC_Widget_DIR}/core/musicpreviewlabel.h
    ${MUSIC_Widget_DIR}/core/musicmessagebox.h
    ${MUSIC_Widget_DIR}/core/musicprogresswidget.h
    ${MUSIC_Widget_DIR}/core/musicclickedlabel.h
    ${MUSIC_Widget_DIR}/core/musicmovinglabelslider.h
    ${MUSIC_Widget_DIR}/core/musictoastlabel.h
    ${MUSIC_Widget_DIR}/musicdownloadstatuslabel.h
    ${MUSIC_Widget_DIR}/musicquerytablewidget.h
    ${MUSIC_Widget_DIR}/musicsettingwidget.h
    ${MUSIC_Widget_DIR}/musicbackgroundskindialog.h
    ${MUSIC_Widget_DIR}/musicsystemtraymenu.h
    ${MUSIC_Widget_DIR}/musicequalizerdialog.h
    ${MUSIC_Widget_DIR}/musictimersliderwidget.h
    ${MUSIC_Widget_DIR}/musicwindowextras.h
    ${MUSIC_Widget_DIR}/musicsongslistwidget.h
    ${MUSIC_Widget_DIR}/musicsongslistiteminfowidget.h
    ${MUSIC_Widget_DIR}/musicsongsearchonlinewidget.h
    ${MUSIC_Widget_DIR}/musicsongstoolitemrenamedwidget.h
    ${MUSIC_Widget_DIR}/musicsongslistplaywidget.h
    ${MUSIC_Widget_DIR}/musicqualitychoicewidget.h
    ${MUSIC_Widget_DIR}/musicbackgroundremotewidget.h
    ${MUSIC_Widget_DIR}/musicbackgroundlistwidget.h
    ${MUSIC_Widget_DIR}/musicenhancedwidget.h
    ${MUSIC_Widget_DIR}/musicdownloadwidget.h
    ${MUSIC_Widget_DIR}/musicsongsharingwidget.h
    ${MUSIC_Widget_DIR}/musicsourceupdatewidget.h
    ${MUSIC_Widget_DIR}/musicbarragewidget.h
    ${MUSIC_LIB_DIR}/musicapplication.h
    ${MUSIC_LIB_DIR}/musicleftareawidget.h
    ${MUSIC_LIB_DIR}/musictopareawidget.h
    ${MUSIC_LIB_DIR}/musicrightareawidget.h
    ${MUSIC_LIB_DIR}/musicbottomareawidget.h
    ${MUSIC_LIB_DIR}/musicapplicationobject.h
  )

set(MUSIC_SOURCES
    ${MUSIC_Core_DIR}/network/musicdownloadthreadabstract.cpp
    ${MUSIC_Core_DIR}/network/musicdata2downloadthread.cpp
    ${MUSIC_Core_DIR}/network/musicdatadownloadthread.cpp
    ${MUSIC_Core_DIR}/network/musictextdownloadthread.cpp
    ${MUSIC_Core_DIR}/network/musicbgthemedownload.cpp
    ${MUSIC_Core_DIR}/network/musicnetworkthread.cpp
    ${MUSIC_Core_DIR}/network/musicdownloadqueuecache.cpp
    ${MUSIC_Core_DIR}/network/musicdownloadquerythreadabstract.cpp
    ${MUSIC_Core_DIR}/network/musicdownloadquerysinglethread.cpp
    ${MUSIC_Core_DIR}/network/musicdownloadquerymultiplethread.cpp
    ${MUSIC_Core_DIR}/musicutils.cpp
    ${MUSIC_Core_DIR}/musictime.cpp
    ${MUSIC_Core_DIR}/musicplayer.cpp
    ${MUSIC_Core_DIR}/musicplaylist.cpp
    ${MUSIC_Core_DIR}/musicabstractxml.cpp
    ${MUSIC_Core_DIR}/musicxmlconfigmanager.cpp
    ${MUSIC_Core_DIR}/musicbgthememanager.cpp
    ${MUSIC_Core_DIR}/musicconnectionpool.cpp
    ${MUSIC_Core_DIR}/musicregeditmanager.cpp
    ${MUSIC_Core_DIR}/musiccoremplayer.cpp
    ${MUSIC_Core_DIR}/musicsong.cpp
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearch.cpp
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchedit.cpp
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchinlineedit.cpp
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchpopwidget.cpp
    ${MUSIC_Localsearch_DIR}/core/musiclocalsongsearchrecordobject.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanager.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanagerfordesktop.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcmanagerforinline.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainer.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainerfordesktop.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrccontainerforinline.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcsearchwidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcsearchtablewidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcartphotoupload.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcartphotolabel.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatwidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatabstractwidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatsettingwidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcfloatphotowidget.cpp
    ${MUSIC_Lrcmanager_DIR}/musiclrcmakerwidget.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidget.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforsquare.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforrectangle.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetfordiamond.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforcircle.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforsimplestyle.cpp
    ${MUSIC_Remotewidget_DIR}/musicremotewidgetforcomplexstyle.cpp
    ${MUSIC_Toolsetswidget_DIR}/musicmobiledeviceswidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musicaudiorecorderwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musictimerwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musiclocalsongsmanagerwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musictransformwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musicfileinformationwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/musicdesktopwallpaperwidget.cpp
    ${MUSIC_Toolsetswidget_DIR}/core/musicdesktopwallpaperthread.cpp
    ${MUSIC_Toolsetswidget_DIR}/core/musictimerautoobject.cpp
    ${MUSIC_Toolsetswidget_DIR}/core/musiclocalsongsmanagerthread.cpp
    ${MUSIC_Toolsetswidget_DIR}/core/musicsongtag.cpp
    ${MUSIC_Toolsetswidget_DIR}/musiclocalsongstablewidget.cpp
    ${MUSIC_Toolswidget_DIR}/musicsongssummarizied.cpp
    ${MUSIC_Toolswidget_DIR}/musictoolsetswidget.cpp
    ${MUSIC_Toolswidget_DIR}/musicmydownloadrecordwidget.cpp
    ${MUSIC_Toolswidget_DIR}/musicwebradiolistview.cpp
    ${MUSIC_Toolswidget_DIR}/musicwebradiowidget.cpp
    ${MUSIC_Toolswidget_DIR}/core/musicwebradiodatebase.cpp
    ${MUSIC_Toolswidget_DIR}/core/musicmydownloadrecordobject.cpp
    ${MUSIC_Toolswidget_DIR}/musicconnectmobilewidget.cpp
    ${MUSIC_Usermanager_DIR}/musicusermanager.cpp
    ${MUSIC_Usermanager_DIR}/musicuserdialog.cpp
    ${MUSIC_Usermanager_DIR}/musicuserlineedit.cpp
    ${MUSIC_Usermanager_DIR}/musicuserwindow.cpp
    ${MUSIC_Usermanager_DIR}/core/musicusermodel.cpp
    ${MUSIC_Usermanager_DIR}/core/musicuserconfigmanager.cpp
    ${MUSIC_Usermanager_DIR}/musicuserrecordwidget.cpp
    ${MUSIC_Videokits_DIR}/musicvideocontrol.cpp
    ${MUSIC_Videokits_DIR}/musicvideoview.cpp
    ${MUSIC_Videokits_DIR}/musicvideotablewidget.cpp
    ${MUSIC_Videokits_DIR}/musicvideoplaywidget.cpp
    ${MUSIC_Widget_DIR}/core/musicabstracttablewidget.cpp
    ${MUSIC_Widget_DIR}/core/musicabstractmovedialog.cpp
    ${MUSIC_Widget_DIR}/core/musicabstractmovewidget.cpp
    ${MUSIC_Widget_DIR}/core/musicitemdelegate.cpp
    ${MUSIC_Widget_DIR}/core/musicmarqueewidget.cpp
    ${MUSIC_Widget_DIR}/core/musicpreviewlabel.cpp
    ${MUSIC_Widget_DIR}/core/musicmessagebox.cpp
    ${MUSIC_Widget_DIR}/core/musicprogresswidget.cpp
    ${MUSIC_Widget_DIR}/core/musicclickedlabel.cpp
    ${MUSIC_Widget_DIR}/core/musicmovinglabelslider.cpp
    ${MUSIC_Widget_DIR}/core/musictoastlabel.cpp
    ${MUSIC_Widget_DIR}/musicdownloadstatuslabel.cpp
    ${MUSIC_Widget_DIR}/musicquerytablewidget.cpp
    ${MUSIC_Widget_DIR}/musicsettingwidget.cpp
    ${MUSIC_Widget_DIR}/musicbackgroundskindialog.cpp
    ${MUSIC_Widget_DIR}/musicsystemtraymenu.cpp
    ${MUSIC_Widget_DIR}/musicequalizerdialog.cpp
    ${MUSIC_Widget_DIR}/musictimersliderwidget.cpp
    ${MUSIC_Widget_DIR}/musicwindowextras.cpp
    ${MUSIC_Widget_DIR}/musicsongslistwidget.cpp
    ${MUSIC_Widget_DIR}/musicsongslistiteminfowidget.cpp
    ${MUSIC_Widget_DIR}/musicsongsearchonlinewidget.cpp
    ${MUSIC_Widget_DIR}/musicsongstoolitemrenamedwidget.cpp
    ${MUSIC_Widget_DIR}/musicsongslistplaywidget.cpp
    ${MUSIC_Widget_DIR}/musicqualitychoicewidget.cpp
    ${MUSIC_Widget_DIR}/musicbackgroundremotewidget.cpp
    ${MUSIC_Widget_DIR}/musicbackgroundlistwidget.cpp
    ${MUSIC_Widget_DIR}/musicenhancedwidget.cpp
    ${MUSIC_Widget_DIR}/musicdownloadwidget.cpp
    ${MUSIC_Widget_DIR}/musicsongsharingwidget.cpp
    ${MUSIC_Widget_DIR}/musicsourceupdatewidget.cpp
    ${MUSIC_Widget_DIR}/musicbarragewidget.cpp
    ${MUSIC_LIB_DIR}/musicapplication.cpp
    ${MUSIC_LIB_DIR}/musicleftareawidget.cpp
    ${MUSIC_LIB_DIR}/musictopareawidget.cpp
    ${MUSIC_LIB_DIR}/musicrightareawidget.cpp
    ${MUSIC_LIB_DIR}/musicbottomareawidget.cpp
    ${MUSIC_LIB_DIR}/musicapplicationobject.cpp
  )

SET(MUSIC_UIS
    ${MUSIC_Ui_DIR}/musicApplication/musicapplication.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcartphotoupload.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcmakerwidget.ui
    ${MUSIC_Ui_DIR}/musicLrcmanager/musiclrcsearchwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicaudiorecorderwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicdesktopwallpaperwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musicfileinformationwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musiclocalsongsmanagerwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musictimerwidget.ui
    ${MUSIC_Ui_DIR}/musicToolsetswidget/musictransformwidget.ui
    ${MUSIC_Ui_DIR}/musicToolswidget/musicwebradiowidget.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserdialog.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicusermanager.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserrecordwidget.ui
    ${MUSIC_Ui_DIR}/musicUsermanager/musicuserwindow.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicbackgroundskindialog.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicdownloadwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicequalizerdialog.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicmessagebox.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsettingwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsongsharingwidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsongslistiteminfowidget.ui
    ${MUSIC_Ui_DIR}/musicWidget/musicsourceupdatewidget.ui
)


if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  QT5_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC_DIR}/MusicPlayer.qrc ${MUSIC_QRC_DIR}/MusicPlayerShare.qrc ${MUSIC_QRC_DIR}/MusicPlayerUser.qrc ${MUSIC_QRC_DIR}/MusicPlayerVedio.qrc)

  ADD_LIBRARY(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_RCC_SRCS} ${MUSIC_UIS_H} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  if(WIN32)
    target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::WinExtras ${QMMP_LIBRARY})
  else()
    target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Network Qt5::Sql Qt5::Xml Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets ${QMMP_LIBRARY})
  endif()
else()
  QT4_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  QT4_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC_DIR}/MusicPlayer.qrc ${MUSIC_QRC_DIR}/MusicPlayerShare.qrc ${MUSIC_QRC_DIR}/MusicPlayerUser.qrc ${MUSIC_QRC_DIR}/MusicPlayerVedio.qrc)

  ADD_LIBRARY(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_RCC_SRCS} ${MUSIC_UIS_H} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  if(WIN32)
    target_link_libraries(${TARGET_NAME} ${QMMP_LIBRARY} ${QT_QTMULTIMEDIA_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})
  else(UNIX)
    target_link_libraries(${TARGET_NAME} ${QMMP_LIBRARY} -lQtMultimediaKit ${QT_QTSCRIPT_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})
  endif()
endif()
