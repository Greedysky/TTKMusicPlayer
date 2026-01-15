#include "musiclrccontainerforinterior.h"
#include "musiclrcmanagerforinterior.h"
#include "musiclrcfloatwidget.h"
#include "musiclrcfloatplaywidget.h"
#include "musiclrclocallinkwidget.h"
#include "musictoastlabel.h"
#include "musiclrcanalysis.h"
#include "musicurlutils.h"
#include "musicvoicepopwidget.h"
#include "musicinteriorlrcuiobject.h"
#include "musicrightareawidget.h"
#include "musiclrccommentswidget.h"
#include "musiclrctranslatedwidget.h"
#include "musiclrcposterwidget.h"
#include "musiclayoutanimationwidget.h"
#include "musicbottomareawidget.h"
#include "musictopareawidget.h"
#include "musicbackgroundmanager.h"
#include "musicwidgetheaders.h"
#include "ttkclickedlabel.h"

#include <QClipboard>
#include <QApplication>

static constexpr int LRC_CHANGED_OFFSET_LIMIT = 20;
static constexpr int LRC_TIME_LABEL_POSITION = 20;

MusicLrcContainerForInterior::MusicLrcContainerForInterior(QWidget *parent)
    : MusicLrcContainer(parent),
      m_lrcChangeOffset(0),
      m_lrcChangeDelta(-1),
      m_mouseLeftPressed(false),
      m_lrcChangeState(false),
      m_mouseMoved(false),
      m_lrcDisplayAll(false),
      m_showArtistBackground(true),
      m_animationFreshTime(0),
      m_lrcSizeProperty(-1),
      m_timePositionOffset(0),
      m_floatPlayWidget(nullptr),
      m_commentsWidget(nullptr),
      m_translatedWidget(nullptr)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vBoxLayout);

    m_containerType = LRC_INTERIOR_TPYE;
    m_layoutWidget = new MusicVLayoutAnimationWidget(this);
    m_layoutWidget->connectTo(this);
    m_lrcFloatWidget = new MusicLrcFloatWidget(this);

    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
       MusicLrcManager *w = new MusicLrcManagerForInterior(this);
       w->setText({});
       m_layoutWidget->addWidget(w);
       m_lrcManagers.append(w);
    }
    vBoxLayout->addWidget(m_layoutWidget);

    initFunctionLabel();
    createNoLrcCurrentInfo();

    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->setText(tr("No song is playing now"));
}

MusicLrcContainerForInterior::~MusicLrcContainerForInterior()
{
    clearAllMusicLRCManager();
    delete m_functionLabel;
    delete m_lrcFloatWidget;
    delete m_floatPlayWidget;
    delete m_noLrcCurrentInfo;
    delete m_commentsWidget;
    delete m_translatedWidget;
    delete m_layoutWidget;
}

void MusicLrcContainerForInterior::startDrawLrc()
{
    m_lrcManagers[m_lrcAnalysis->lineMiddle()]->startDrawLrc();
}

void MusicLrcContainerForInterior::stopDrawLrc()
{
    m_lrcManagers[m_lrcAnalysis->lineMiddle()]->stopDrawLrc();
    m_layoutWidget->stop();
}

void MusicLrcContainerForInterior::applyParameter()
{
    MusicLrcContainer::applyParameter();
    const int size = G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
    if(m_lrcSizeProperty == -1)
    {
        m_lrcSizeProperty = size;
    }

    if(m_lrcChangeDelta == -1)
    {
        m_lrcChangeDelta = size;
    }

    setLrcSize(size);
    // update state
    m_mufflerButton->updateVoiceRemove(false);
}

void MusicLrcContainerForInterior::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->isValid() && !m_mouseLeftPressed)
    {
        m_animationFreshTime = time;
        m_layoutWidget->start();
    }
}

void MusicLrcContainerForInterior::updateCurrentLrc(MusicLrcAnalysis::State state)
{
    m_layoutWidget->stop();

    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        m_lrcManagers[i]->setText({});
    }

    if(state == MusicLrcAnalysis::State::Failed)
    {
        m_lrcManagers[m_lrcAnalysis->lineMiddle()]->setText(tr("No lrc data file found"));
        showNoLrcCurrentInfo();
    }
    else
    {
        m_lrcManagers[m_lrcAnalysis->lineMiddle()]->setText(tr("No song is playing now"));
        m_noLrcCurrentInfo->hide();
    }
}

QString MusicLrcContainerForInterior::text() const
{
    return m_lrcManagers[m_lrcAnalysis->lineMiddle()]->text();
}

qint64 MusicLrcContainerForInterior::findTimePosition(qint64 time)
{
    return m_lrcAnalysis->findTimePosition(time);
}

void MusicLrcContainerForInterior::setLrcSize(int size)
{
    int index = mapLrcSizeProperty(size);
    if(index == -1)
    {
        return;
    }

    setLrcSizeProperty(index);
    index = (mapLrcSizeProperty(m_lrcSizeProperty) - index) / 2;
    m_lrcSizeProperty = size;
    m_lrcAnalysis->setCurrentIndex(m_lrcAnalysis->currentIndex() - index - 1);

    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        m_lrcManagers[i]->setLrcFontSize(size);
        m_lrcManagers[i]->setY(35 + size);
        m_lrcManagers[i]->setText(m_lrcAnalysis->text(i));
    }
    G_SETTING_PTR->setValue(MusicSettingManager::LrcSize, size);

    resizeWindow();
    setItemStyleSheet();
    startDrawLrc();
}

int MusicLrcContainerForInterior::lrcSize() const noexcept
{
    return G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
}

void MusicLrcContainerForInterior::resizeWindow()
{
    int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    int height = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height();

    if(m_lrcDisplayAll)
    {
        width += LEFT_SIDE_WIDTH_MIN;
    }

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        width += (m_lrcDisplayAll ? 0 : LEFT_SIDE_WIDTH_MIN);
        height += LEFT_SIDE_WIDTH_MIN / 2;
    }

    resizeWidth(width - WINDOW_WIDTH_MIN, height - WINDOW_HEIGHT_MIN);
}

void MusicLrcContainerForInterior::createFloatPlayWidget()
{
    delete m_floatPlayWidget;
    m_floatPlayWidget = nullptr;

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        m_floatPlayWidget = new MusicLrcFloatPlayWidget(this);
        m_floatPlayWidget->resizeGeometry(width(), height());
    }
}

void MusicLrcContainerForInterior::lrcWidgetShowFullScreen()
{
    QHBoxLayout *layout = TTKObjectCast(QHBoxLayout*, m_functionLabel->layout());
    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        layout->removeItem(layout->itemAt(layout->count() - 1));
    }
    else
    {
        layout->addStretch(1);
    }
}

void MusicLrcContainerForInterior::lrcSizeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0: setLrcSize(14); break;
        case 1: setLrcSize(18); break;
        case 2: setLrcSize(26); break;
        case 3: setLrcSize(36); break;
        case 4: setLrcSize(72); break;
        default: break;
    }
}

void MusicLrcContainerForInterior::lrcTimePositionChanged(QAction *action)
{
    int timeValue = 0;
    switch(action->data().toInt())
    {
        case 0: timeValue = -0.5 * TTK_DN_S2MS; break;
        case 1: timeValue = -1.0 * TTK_DN_S2MS; break;
        case 2: timeValue = -2.0 * TTK_DN_S2MS; break;
        case 3: timeValue = -5.0 * TTK_DN_S2MS; break;
        case 4: timeValue = +0.5 * TTK_DN_S2MS; break;
        case 5: timeValue = +1.0 * TTK_DN_S2MS; break;
        case 6: timeValue = +2.0 * TTK_DN_S2MS; break;
        case 7: timeValue = +5.0 * TTK_DN_S2MS; break;
        default: break;
    }

    m_timePositionOffset += timeValue;
    setTimePosition(timeValue);
}

void MusicLrcContainerForInterior::revertTimePosition()
{
    if(m_timePositionOffset == 0)
    {
        return;
    }

    setTimePosition(-m_timePositionOffset);
}

void MusicLrcContainerForInterior::saveLrcTimeChanged()
{
    m_lrcAnalysis->saveData();
}

void MusicLrcContainerForInterior::artistBackgroundChanged()
{
    m_showArtistBackground = !m_showArtistBackground;
    Q_EMIT backgroundChanged();
}

void MusicLrcContainerForInterior::lrcOpenFileDir() const
{
    TTK::Url::openUrl(m_lrcAnalysis->currentFilePath());
}

void MusicLrcContainerForInterior::lrcCopyClipboard() const
{
    QClipboard *clipBoard = QApplication::clipboard();
    clipBoard->setText(m_lrcAnalysis->dataString());
}

void MusicLrcContainerForInterior::showLocalLinkWidget()
{
    MusicLrcLocalLinkWidget widget(this);
    widget.setCurrentSongName(m_currentSongName);
    widget.exec();
}

void MusicLrcContainerForInterior::showSongCommentsWidget()
{
    if(m_currentSongName.isEmpty())
    {
        return;
    }

    delete m_commentsWidget;
    m_commentsWidget = new MusicLrcCommentsWidget(this);
    m_commentsWidget->initialize(true);
    m_commentsWidget->setGeometry(0, height() / 5, width(), height() * 4 / 5);
    m_commentsWidget->show();

    MusicToastLabel::popup(tr("Loading now ... "));
    m_commentsWidget->setCurrentSongName(m_currentSongName);
}

void MusicLrcContainerForInterior::showLrcPosterWidget()
{
    MusicLrcPosterWidget widget(this);
    widget.setCurrentLrcs(m_lrcAnalysis->dataList(), m_currentSongName);
    widget.exec();
}

void MusicLrcContainerForInterior::showSongMovieWidget()
{
    if(m_currentSongName.isEmpty())
    {
        return;
    }

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
    }
    MusicRightAreaWidget::instance()->showVideoSearchedFound(m_currentSongName, {});
}

void MusicLrcContainerForInterior::updateAnimationLrc()
{
    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        m_lrcManagers[i]->setText(m_lrcAnalysis->text(i));
    }

    m_lrcAnalysis->setCurrentIndex(m_lrcAnalysis->currentIndex() + 1);
    m_lrcManagers[m_lrcAnalysis->lineMiddle()]->startDrawLrcMask(m_animationFreshTime);
    setItemStyleSheet();
}

void MusicLrcContainerForInterior::translatedLrcData()
{
    const QString &path = m_lrcAnalysis->currentFilePath();
    if(path.isEmpty())
    {
        return;
    }

    delete m_translatedWidget;
    m_translatedWidget = new MusicLrcTranslatedWidget(this);
    m_translatedWidget->setLrcAnalysisModel(m_lrcAnalysis);
    m_translatedWidget->setGeometry(0, height() / 5.0, width(), height() * 4 / 5.0);
    m_translatedWidget->show();

    MusicToastLabel::popup(tr("Loading now ... "));
    m_translatedWidget->setCurrentSongName(m_currentSongName);
}

void MusicLrcContainerForInterior::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    const bool hasLrcContainer = !m_lrcAnalysis->isEmpty();
    menu.addAction(tr("Lrc Search"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("Lrc Update"), this, SIGNAL(currentLrcUpdated()));
    menu.addAction(tr("Lrc Make"), this, SLOT(showLrcMakedWidget()));
    menu.addAction(tr("Lrc Poster"), this, SLOT(showLrcPosterWidget()))->setEnabled(hasLrcContainer);
    menu.addSeparator();
    menu.addAction(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen() ? tr("Show Normal Mode") : tr("Show Fullscreen Mode"),
                   MusicBottomAreaWidget::instance(), SLOT(lrcWidgetShowFullScreen()));
    menu.addSeparator();

    QMenu *colorMenu = menu.addMenu(tr("Color"));
    QMenu *lrcSizeMenu = menu.addMenu(tr("Lrc Size"));
    QMenu *lrcTimeFastMenu = menu.addMenu(tr("Time After"));
    QMenu *lrcTimeSlowMenu = menu.addMenu(tr("Time Before"));
    lrcTimeFastMenu->setEnabled(hasLrcContainer);
    lrcTimeSlowMenu->setEnabled(hasLrcContainer);

    menu.addAction(tr("Revert Mode"), this, SLOT(revertTimePosition()))->setEnabled(hasLrcContainer);
    menu.addAction(tr("Save Mode"), this, SLOT(saveLrcTimeChanged()))->setEnabled(hasLrcContainer);
    menu.addSeparator();

    QActionGroup *actions = new QActionGroup(this);
    actions->addAction(lrcSizeMenu->addAction(tr("Smaller")))->setData(0);
    actions->addAction(lrcSizeMenu->addAction(tr("Small")))->setData(1);
    actions->addAction(lrcSizeMenu->addAction(tr("Middle")))->setData(2);
    actions->addAction(lrcSizeMenu->addAction(tr("Big")))->setData(3);
    actions->addAction(lrcSizeMenu->addAction(tr("Bigger")))->setData(4);

    int index = -1, size = G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
    switch(size)
    {
        case 14: index = 0; break;
        case 18: index = 1; break;
        case 26: index = 2; break;
        case 36: index = 3; break;
        case 72: index = 4; break;
        default: break;
    }

    if(index > -1 && index < actions->actions().count())
    {
        actions->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
    connect(actions, SIGNAL(triggered(QAction*)), SLOT(lrcSizeChanged(QAction*)));

    lrcSizeMenu->addSeparator();
    lrcSizeMenu->addAction(tr("Custom"), this, SLOT(currentLrcCustom()));
    createColorMenu(colorMenu);

    QActionGroup *lrcTimeFastActions = new QActionGroup(this);
    lrcTimeFastActions->addAction(lrcTimeFastMenu->addAction(tr("After 0.5s")))->setData(0);
    lrcTimeFastActions->addAction(lrcTimeFastMenu->addAction(tr("After 1.0s")))->setData(1);
    lrcTimeFastActions->addAction(lrcTimeFastMenu->addAction(tr("After 2.0s")))->setData(2);
    lrcTimeFastActions->addAction(lrcTimeFastMenu->addAction(tr("After 5.0s")))->setData(3);
    connect(lrcTimeFastActions, SIGNAL(triggered(QAction*)), SLOT(lrcTimePositionChanged(QAction*)));
    TTK::Widget::adjustMenuPosition(lrcTimeFastMenu);
    //
    QActionGroup *lrcTimeSlowActions = new QActionGroup(this);
    lrcTimeSlowActions->addAction(lrcTimeSlowMenu->addAction(tr("Before 0.5s")))->setData(4);
    lrcTimeSlowActions->addAction(lrcTimeSlowMenu->addAction(tr("Before 1.0s")))->setData(5);
    lrcTimeSlowActions->addAction(lrcTimeSlowMenu->addAction(tr("Before 2.0s")))->setData(6);
    lrcTimeSlowActions->addAction(lrcTimeSlowMenu->addAction(tr("Before 5.0s")))->setData(7);
    connect(lrcTimeSlowActions, SIGNAL(triggered(QAction*)), SLOT(lrcTimePositionChanged(QAction*)));
    TTK::Widget::adjustMenuPosition(lrcTimeSlowMenu);
    //
    QAction *artAction = menu.addAction(tr("Art Turn Off"), this, SLOT(artistBackgroundChanged()));
    m_showArtistBackground ? artAction->setText(tr("Art Turn Off")) : artAction->setText(tr("Art Turn On"));
    QAction *showLrc = menu.addAction(tr("Lrc Turn Off"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? showLrc->setText(tr("Lrc Turn Off")) : showLrc->setText(tr("Lrc Turn On"));
    menu.addAction(tr("Set As Background"), MusicTopAreaWidget::instance(), SLOT(setArtistBackground()))->setEnabled(!G_BACKGROUND_PTR->isEmpty());
    menu.addSeparator();

    const QString &filePath = m_lrcAnalysis->currentFilePath();
    const bool fileCheck = !filePath.isEmpty() && QFile::exists(filePath);

    QMenu *lrcLinkMenu = menu.addMenu(tr("Lrc Link"));
    lrcLinkMenu->addAction(tr("Local Lrc Link"), this, SLOT(showLocalLinkWidget()));
    QAction *lrcLinkAc = lrcLinkMenu->addAction(tr("Link Off"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("Link Off")) : lrcLinkAc->setText(tr("Link On"));
    TTK::Widget::adjustMenuPosition(lrcLinkMenu);

    menu.addAction(tr("Copy To Clip"), this, SLOT(lrcCopyClipboard()))->setEnabled(fileCheck);
    menu.addAction(tr("Open Lrc File"), this, SLOT(lrcOpenFileDir()))->setEnabled(fileCheck);

    menu.addSeparator();
    menu.addAction(tr("Settings"), this, SLOT(currentLrcCustom()));

    TTK::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicLrcContainerForInterior::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_functionLabel && m_functionLabel->geometry().contains(event->pos()))
        {
            return; //block mouse event in function label area
        }

        m_mouseMoved = false;
        m_mouseLeftPressed = true;
        m_pressedPos = QtGlobalPosition(event);
        m_lrcChangeState = false;
        m_lrcChangeOffset = 0;

        setCursor(Qt::CrossCursor);
        update();
    }
}

void MusicLrcContainerForInterior::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseLeftPressed && m_lrcAnalysis->isValid())
    {
        m_layoutWidget->stop();

        m_mouseMoved = true;
        const int offset = QtGlobalPositionY(event) - m_pressedPos.y();
        m_pressedPos = QtGlobalPosition(event);

        m_lrcChangeOffset += offset;
        if(m_lrcChangeState && offset > 0)
        {
            m_lrcChangeState = false;
            m_lrcChangeOffset = 0;
        }
        else if(!m_lrcChangeState && offset < 0)
        {
            m_lrcChangeState = true;
            m_lrcChangeOffset = 0;
        }

        if(m_lrcChangeOffset !=0 && m_lrcChangeOffset % LRC_CHANGED_OFFSET_LIMIT == 0)
        {
            int index = m_lrcAnalysis->currentIndex();
                index += m_lrcChangeOffset / LRC_CHANGED_OFFSET_LIMIT;
            m_lrcChangeOffset = 0;

            if(index < 0)
            {
                index = 0;
            }
            else if(index >= m_lrcAnalysis->count())
            {
                index = m_lrcAnalysis->count() - m_lrcAnalysis->lineMiddle() + 2;
            }

            int value = G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
                value = (mapLrcSizeProperty(m_lrcChangeDelta) - mapLrcSizeProperty(value)) / 2;

            m_lrcAnalysis->setCurrentIndex(index);
            for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
            {
                m_lrcManagers[i]->setText(m_lrcAnalysis->text(i - value - 1));
            }
            setItemStyleSheet();
        }

        update();
    }
}

void MusicLrcContainerForInterior::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::ArrowCursor);
        m_mouseLeftPressed = false;
        m_pressedPos = QtGlobalPosition(event);
        update();

        if(m_lrcAnalysis->isValid() && m_mouseMoved)
        {
            const qint64 time = m_lrcAnalysis->findTime(m_lrcAnalysis->currentIndex());
            if(time != -1)
            {
                Q_EMIT updateCurrentTime(time);
            }
        }
    }
}

void MusicLrcContainerForInterior::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_functionLabel && m_functionLabel->geometry().contains(event->pos()))
    {
        return; //block mouse event in function label area
    }
    MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
}

void MusicLrcContainerForInterior::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(!m_mouseLeftPressed)
    {
        return;
    }

    QLinearGradient linearGradient(0, 0, width(), 0);
    linearGradient.setColorAt(0.0, QColor(255, 254, 161));
    linearGradient.setColorAt(0.3, Qt::transparent);
    linearGradient.setColorAt(0.7, Qt::transparent);
    linearGradient.setColorAt(1.0, QColor(255, 254, 161));

    const int line = (height() - m_functionLabel->height()) / 2;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QPen(linearGradient, 1));
    painter.drawLine(LRC_TIME_LABEL_POSITION, line, width() - LRC_TIME_LABEL_POSITION, line);

    qint64 v = m_lrcAnalysis->currentIndex() - 1;
    if(v < 0)
    {
        v = 0;
    }
    else if(v >= m_lrcAnalysis->count())
    {
        v = m_lrcAnalysis->count() - m_lrcAnalysis->lineMiddle() + 2;
    }

    v = m_lrcAnalysis->findTime(v);
    painter.drawText(LRC_TIME_LABEL_POSITION, line - LRC_TIME_LABEL_POSITION / 2, TTKTime::formatDuration(v));
}

void MusicLrcContainerForInterior::resizeEvent(QResizeEvent *event)
{
    MusicLrcContainer::resizeEvent(event);
    resizeWindow();
}

void MusicLrcContainerForInterior::initFunctionLabel()
{
    m_functionLabel = new QWidget(this);
    m_functionLabel->setFixedHeight(40);
    m_functionLabel->lower();

    QHBoxLayout *functionLayout = new QHBoxLayout(m_functionLabel);
    functionLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *transButton = new QPushButton(m_functionLabel);
    QPushButton *movieButton = new QPushButton(m_functionLabel);
    QPushButton *messageButton = new QPushButton(m_functionLabel);
    QPushButton *photoButton = new QPushButton(m_functionLabel);
    m_mufflerButton = new MusicVoicePopWidget(m_functionLabel);

#ifdef Q_OS_UNIX
    transButton->setFocusPolicy(Qt::NoFocus);
    movieButton->setFocusPolicy(Qt::NoFocus);
    messageButton->setFocusPolicy(Qt::NoFocus);
    photoButton->setFocusPolicy(Qt::NoFocus);
    m_mufflerButton->setFocusPolicy(Qt::NoFocus);
#endif

    transButton->setFixedSize(30, 30);
    movieButton->setFixedSize(30, 30);
    messageButton->setFixedSize(30, 30);
    photoButton->setFixedSize(30, 30);

    transButton->setStyleSheet(TTK::UI::InteriorTranslation);
    movieButton->setStyleSheet(TTK::UI::InteriorMovie);
    messageButton->setStyleSheet(TTK::UI::InteriorMessage);
    photoButton->setStyleSheet(TTK::UI::InteriorPhoto);

    transButton->setCursor(Qt::PointingHandCursor);
    movieButton->setCursor(Qt::PointingHandCursor);
    messageButton->setCursor(Qt::PointingHandCursor);
    photoButton->setCursor(Qt::PointingHandCursor);

    transButton->setToolTip(tr("Translation"));
    movieButton->setToolTip(tr("Movie"));
    messageButton->setToolTip(tr("Message"));
    photoButton->setToolTip(tr("Photo"));

    connect(transButton, SIGNAL(clicked()), SLOT(translatedLrcData()));
    connect(movieButton, SIGNAL(clicked()), SLOT(showSongMovieWidget()));
    connect(messageButton, SIGNAL(clicked()), SLOT(showSongCommentsWidget()));
    connect(photoButton, SIGNAL(clicked()), m_lrcFloatWidget, SLOT(showArtistPhotoWidget()));

    functionLayout->addStretch(1);
    functionLayout->addWidget(transButton);
    functionLayout->addWidget(m_mufflerButton);
    functionLayout->addWidget(movieButton);
    functionLayout->addWidget(messageButton);
    functionLayout->addWidget(photoButton);
    functionLayout->addStretch(1);
    m_functionLabel->setLayout(functionLayout);

    layout()->addWidget(m_functionLabel);
}

void MusicLrcContainerForInterior::createColorMenu(QMenu *menu)
{
    QActionGroup *actions = new QActionGroup(this);
    actions->addAction(menu->addAction(tr("Yellow")))->setData(0);
    actions->addAction(menu->addAction(tr("Blue")))->setData(1);
    actions->addAction(menu->addAction(tr("Gray")))->setData(2);
    actions->addAction(menu->addAction(tr("Pink")))->setData(3);
    actions->addAction(menu->addAction(tr("Green")))->setData(4);
    actions->addAction(menu->addAction(tr("Red")))->setData(5);
    actions->addAction(menu->addAction(tr("Purple")))->setData(6);
    actions->addAction(menu->addAction(tr("Orange")))->setData(7);
    actions->addAction(menu->addAction(tr("Indigo")))->setData(8);
    connect(actions, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu->addSeparator();
    menu->addAction(tr("Custom"), this, SLOT(currentLrcCustom()));

    const int index = G_SETTING_PTR->value("LrcColor").toInt();
    if(index > -1 && index < actions->actions().count())
    {
        actions->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
}

void MusicLrcContainerForInterior::setTimePosition(qint64 pos)
{
    m_lrcAnalysis->setTimePosition(pos);
    const qint64 beforeTime = findTimePosition(m_currentTime);
    updateCurrentLrc(beforeTime);

    if(m_timePositionOffset + pos == 0)
    {
        m_timePositionOffset = 0;
    }

    QString message;
    if(m_timePositionOffset > 0)
    {
        message = tr("After %1s").arg(m_timePositionOffset * 1.0 / TTK_DN_S2MS);
    }
    else if(m_timePositionOffset < 0)
    {
        message = tr("Before %1s").arg(-m_timePositionOffset * 1.0 / TTK_DN_S2MS);
    }
    else
    {
        message = tr("Restore");
    }

    MusicToastLabel::popup(message);
}

void MusicLrcContainerForInterior::createNoLrcCurrentInfo()
{
    m_noLrcCurrentInfo = new TTKClickedLabel(this);
    TTK::Widget::setFontSize(m_noLrcCurrentInfo, 15);
    TTK::Widget::setFontStyle(m_noLrcCurrentInfo, TTK::FontStyleMode::Underline);
    m_noLrcCurrentInfo->setStyleSheet(TTK::UI::ColorStyle12);
    m_noLrcCurrentInfo->setText(tr("Lrc Make"));

    connect(m_noLrcCurrentInfo, SIGNAL(clicked()), SLOT(showLrcMakedWidget()));
    m_noLrcCurrentInfo->hide();
}

void MusicLrcContainerForInterior::showNoLrcCurrentInfo()
{
    const int w = TTK::Widget::fontTextWidth(m_noLrcCurrentInfo->font(), m_noLrcCurrentInfo->text());
    const int h = TTK::Widget::fontTextHeight(m_noLrcCurrentInfo->font());
    const int offset = height() / m_lrcAnalysis->lineMax() * (m_lrcAnalysis->lineMiddle() + 1) - 20;

    m_noLrcCurrentInfo->setGeometry((width() - w) / 2, offset, w, h);
    m_noLrcCurrentInfo->show();
}

void MusicLrcContainerForInterior::initCurrentLrc(const QString &str)
{
    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        m_lrcManagers[i]->setText({});
    }
    m_lrcManagers[m_lrcAnalysis->lineMiddle()]->setText(str);
}

void MusicLrcContainerForInterior::setItemStyleSheet()
{
    const int length = m_lrcAnalysis->lineMax();
    if(length == 11)
    {
        for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
        {
            if(i == 0 || i == 10)
            {
                setItemStyleSheet(i, 5, 90);
            }
            else if(i == 1 || i == 9)
            {
                setItemStyleSheet(i, 4, 80);
            }
            else if(i == 2 || i == 8)
            {
                setItemStyleSheet(i, 3, 60);
            }
            else if(i == 3 || i == 7)
            {
                setItemStyleSheet(i, 2, 40);
            }
            else if(i == 4 || i == 6)
            {
                setItemStyleSheet(i, 1, 20);
            }
            else
            {
                setItemStyleSheet(i, 0, 0);
            }
        }
    }
    else if(length == 9)
    {
        for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
        {
            if(i == 0 || i == 8)
            {
                setItemStyleSheet(i, 8, 90);
            }
            else if(i == 1 || i == 7)
            {
                setItemStyleSheet(i, 6, 75);
            }
            else if(i == 2 || i == 6)
            {
                setItemStyleSheet(i, 4, 50);
            }
            else if(i == 3 || i == 5)
            {
                setItemStyleSheet(i, 2, 25);
            }
            else
            {
                setItemStyleSheet(i, 0, 0);
            }
        }
    }
    else if(length == 7)
    {
        for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
        {
            if(i == 0 || i == 6)
            {
                setItemStyleSheet(i, 12, 90);
            }
            else if(i == 1 || i == 5)
            {
                setItemStyleSheet(i, 8, 60);
            }
            else if(i == 2 || i == 4)
            {
                setItemStyleSheet(i, 4, 30);
            }
            else
            {
                setItemStyleSheet(i, 0, 0);
            }
        }
    }
    else if(length == 5)
    {
        for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
        {
            if(i == 0 || i == 4)
            {
                setItemStyleSheet(i, 16, 90);
            }
            else if(i == 1 || i == 3)
            {
                setItemStyleSheet(i, 8, 45);
            }
            else
            {
                setItemStyleSheet(i, 0, 0);
            }
        }
    }
    else if(length == 3)
    {
        for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
        {
            if(i == 0 || i == 2)
            {
                setItemStyleSheet(i, 20, 90);
            }
            else
            {
                setItemStyleSheet(i, 0, 0);
            }
        }
    }
}

void MusicLrcContainerForInterior::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInterior *w = TTKObjectCast(MusicLrcManagerForInterior*, m_lrcManagers[index]);
    w->setFontSize(size);

    int value = G_SETTING_PTR->value("LrcColorTransparent").toInt() - transparent;
    value = (value < TTK_RN_MIN) ? TTK_RN_MIN : value;
    value = (value > TTK_RN_MAX) ? TTK_RN_MAX : value;
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(G_SETTING_PTR->value("LrcColor").toInt() != -1)
    {
        setLinearGradientColor(TTKStaticCast(MusicLrcColor::Color, G_SETTING_PTR->value("LrcColor").toInt()));
    }
    else
    {
        setLinearGradientColor({TTK::readColorConfig(G_SETTING_PTR->value("LrcFrontgroundColor").toString()),
                                TTK::readColorConfig(G_SETTING_PTR->value("LrcBackgroundColor").toString())});
    }
}

int MusicLrcContainerForInterior::mapLrcSizeProperty(int size)
{
    if(size <= 14)
    {
        return 0;
    }
    else if(14 < size && size <= 18)
    {
        return 2;
    }
    else if(18 < size && size <= 26)
    {
        return 4;
    }
    else if(26 < size && size <= 36)
    {
        return 6;
    }
    else if(36 < size && size <= 72)
    {
        return 8;
    }
    else
    {
        return -1;
    }
}

void MusicLrcContainerForInterior::setLrcSizeProperty(int property)
{
    const int length = MUSIC_LRC_INTERIOR_MAX_LINE - property;
    m_lrcAnalysis->setLineMax(length);

    for(MusicLrcManager *manager : qAsConst(m_lrcManagers))
    {
        manager->show();
        manager->reset();
        m_layoutWidget->removeWidget(manager);
    }

    for(int i = 0; i < length; ++i)
    {
        m_layoutWidget->addWidget(m_lrcManagers[i]);
    }

    for(int i = length; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
        m_lrcManagers[i]->hide();
    }
}

void MusicLrcContainerForInterior::resizeWidth(int w, int h)
{
    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        TTKObjectCast(MusicLrcManagerForInterior*, m_lrcManagers[i])->setLrcPerWidth(w);
    }

    m_lrcFloatWidget->resizeGeometry(w, h);
    if(m_floatPlayWidget)
    {
        m_floatPlayWidget->resizeGeometry(width(), height());
    }

    if(m_lrcAnalysis->isEmpty())
    {
        initCurrentLrc(tr("No lrc data file found"));
        showNoLrcCurrentInfo();
    }
    else if(m_currentTime != 0 && m_lrcAnalysis->currentIndex() == 0)
    {
        initCurrentLrc(tr("No song is playing now"));
    }

    if(m_commentsWidget)
    {
        m_commentsWidget->setGeometry(0, height() / 5, width(), height() * 4 / 5);
    }

    if(m_translatedWidget)
    {
        m_translatedWidget->setGeometry(0, height() / 5, width(), height() * 4 / 5);
    }
}
