#include "musiclrccontainerforinterior.h"
#include "musiclrcmanagerforinterior.h"
#include "musiclrcfloatwidget.h"
#include "musiclrcfloatplaywidget.h"
#include "musiclrclocallinkwidget.h"
#include "musictranslationrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musiclrcanalysis.h"
#include "musicurlutils.h"
#include "musicinteriorlrcuiobject.h"
#include "musicrightareawidget.h"
#include "musiclrccommentswidget.h"
#include "musiclrctranslatedwidget.h"
#include "musiclrcposterwidget.h"
#include "musiclayoutanimationwidget.h"
#include "musicbottomareawidget.h"
#include "musicleftareawidget.h"
#include "musictopareawidget.h"
#include "musicbackgroundmanager.h"
#include "musicwidgetheaders.h"
#include "ttkclickedlabel.h"

#include <QClipboard>
#include <QApplication>
#include <QActionGroup>

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
      m_changeSpeedValue(0),
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

qint64 MusicLrcContainerForInterior::setSongTimeSpeed(qint64 time)
{
    return m_lrcAnalysis->setSongTimeSpeed(time);
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

int MusicLrcContainerForInterior::lrcSize() const
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

void MusicLrcContainerForInterior::lrcTimeSpeedChanged(QAction *action)
{
    int timeValue = 0;
    switch(action->data().toInt())
    {
        case 0: timeValue = -0.5 * TTK_DN_S2MS; break;
        case 1: timeValue = -1.0 * TTK_DN_S2MS; break;
        case 2: timeValue = -2.0 * TTK_DN_S2MS; break;
        case 3: timeValue = -5.0 * TTK_DN_S2MS; break;
        case 4: timeValue = 0.5 * TTK_DN_S2MS; break;
        case 5: timeValue = 1.0 * TTK_DN_S2MS; break;
        case 6: timeValue = 2.0 * TTK_DN_S2MS; break;
        case 7: timeValue = 5.0 * TTK_DN_S2MS; break;
        default: break;
    }

    m_changeSpeedValue += timeValue;
    revertTimeSpeed(timeValue);
}

void MusicLrcContainerForInterior::revertLrcTimeSpeed()
{
    if(m_changeSpeedValue == 0)
    {
        return;
    }
    revertTimeSpeed(-m_changeSpeedValue);
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
    TTK::Url::openUrl(QFileInfo(m_lrcAnalysis->currentFilePath()).absoluteFilePath());
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
    m_commentsWidget->setCurrentSongName(m_currentSongName);
    m_commentsWidget->show();
}

void MusicLrcContainerForInterior::showLrcPosterWidget()
{
    MusicLrcPosterWidget widget(this);
    widget.setCurrentLrcs(m_lrcAnalysis->dataList(), m_currentSongName);
    widget.exec();
}

void MusicLrcContainerForInterior::queryTranslatedLrcFinished(const QString &bytes)
{
    const QStringList &ort = m_lrcAnalysis->dataList();
    const QStringList &trt = bytes.split("\n");

    if(ort.count() > trt.count())
    {
        return;
    }

    QString text;
    for(int i = 0; i < ort.count(); ++i)
    {
        text += ort[i].trimmed() + "\r\n";
        text += trt[i].trimmed() + "\r\n";
    }

    delete m_translatedWidget;
    m_translatedWidget = new MusicLrcTranslatedWidget(this);
    m_translatedWidget->setPlainText(m_currentSongName, text);
    m_translatedWidget->setGeometry(0, height() / 5.0, width(), height() * 4 / 5.0);
    m_translatedWidget->show();
}

void MusicLrcContainerForInterior::showSongMovieClicked()
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

    MusicTranslationRequest *d = G_DOWNLOAD_QUERY_PTR->makeTranslationRequest(m_lrcAnalysis->dataString(), this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(queryTranslatedLrcFinished(QString)));
    d->setHeader("name", path);
    d->startRequest();
}

void MusicLrcContainerForInterior::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    QMenu changColorMenu(tr("Color"), &menu);
    QMenu changeLrcSize(tr("Lrc Size"), &menu);
    QMenu changeLrcTimeFast(tr("Time After"), &menu);
    QMenu changeLrcTimeSlow(tr("Time Before"), &menu);
    QMenu changeLrcLinkMenu(tr("Lrc Link"), &menu);

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
    menu.addMenu(&changColorMenu);
    menu.addMenu(&changeLrcSize);

    menu.addMenu(&changeLrcTimeFast)->setEnabled(hasLrcContainer);
    menu.addMenu(&changeLrcTimeSlow)->setEnabled(hasLrcContainer);
    menu.addAction(tr("Revert Mode"), this, SLOT(revertLrcTimeSpeed()))->setEnabled(hasLrcContainer);
    menu.addAction(tr("Save Mode"), this, SLOT(saveLrcTimeChanged()))->setEnabled(hasLrcContainer);
    menu.addSeparator();

    QActionGroup *group = new QActionGroup(this);
    group->addAction(changeLrcSize.addAction(tr("Smaller")))->setData(0);
    group->addAction(changeLrcSize.addAction(tr("Small")))->setData(1);
    group->addAction(changeLrcSize.addAction(tr("Middle")))->setData(2);
    group->addAction(changeLrcSize.addAction(tr("Big")))->setData(3);
    group->addAction(changeLrcSize.addAction(tr("Bigger")))->setData(4);

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

    if(index > -1 && index < group->actions().count())
    {
        group->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
    connect(group, SIGNAL(triggered(QAction*)), SLOT(lrcSizeChanged(QAction*)));

    changeLrcSize.addSeparator();
    changeLrcSize.addAction(tr("Custom"), this, SLOT(currentLrcCustom()));
    createColorMenu(changColorMenu);

    QActionGroup *lrcTimeFastGroup = new QActionGroup(this);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("After 0.5s")))->setData(0);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("After 1.0s")))->setData(1);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("After 2.0s")))->setData(2);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("After 5.0s")))->setData(3);
    connect(lrcTimeFastGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));
    TTK::Widget::adjustMenuPosition(&changeLrcTimeFast);
    //
    QActionGroup *lrcTimeSlowGroup = new QActionGroup(this);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("Before 0.5s")))->setData(4);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("Before 1.0s")))->setData(5);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("Before 2.0s")))->setData(6);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("Before 5.0s")))->setData(7);
    connect(lrcTimeSlowGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));
    TTK::Widget::adjustMenuPosition(&changeLrcTimeSlow);
    //
    QAction *artAction = menu.addAction(tr("Art Turn Off"), this, SLOT(artistBackgroundChanged()));
    m_showArtistBackground ? artAction->setText(tr("Art Turn Off")) : artAction->setText(tr("Art Turn On"));
    QAction *showLrc = menu.addAction(tr("Lrc Turn Off"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? showLrc->setText(tr("Lrc Turn Off")) : showLrc->setText(tr("Lrc Turn On"));
    menu.addAction(tr("Set As Background"), MusicTopAreaWidget::instance(), SLOT(setAsArtistBackground()))->setEnabled(!G_BACKGROUND_PTR->isEmpty());
    menu.addSeparator();

    const QString &filePath = m_lrcAnalysis->currentFilePath();
    const bool fileCheck = !filePath.isEmpty() && QFile::exists(filePath);

    changeLrcLinkMenu.addAction(tr("Local Lrc Link"), this, SLOT(showLocalLinkWidget()));
    QAction *lrcLinkAc = changeLrcLinkMenu.addAction(tr("Link Off"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("Link Off")) : lrcLinkAc->setText(tr("Link On"));
    TTK::Widget::adjustMenuPosition(&changeLrcLinkMenu);
    menu.addMenu(&changeLrcLinkMenu);

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
        m_mousePressedAt = QtMouseGlobalPos(event);
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
        const int offset = QtMouseGlobalY(event) - m_mousePressedAt.y();
        m_mousePressedAt = QtMouseGlobalPos(event);

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
        m_mousePressedAt = QtMouseGlobalPos(event);
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
    if(m_mouseLeftPressed)
    {
        QLinearGradient linearGradient(0, 0, width(), 0);
        linearGradient.setColorAt(0.0, QColor(255, 254, 161));
        linearGradient.setColorAt(0.3, Qt::transparent);
        linearGradient.setColorAt(0.7, Qt::transparent);
        linearGradient.setColorAt(1.0, QColor(255, 254, 161));

        const int line = (height() - m_functionLabel->height()) / 2;
        QPainter painter(this);
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
}

void MusicLrcContainerForInterior::resizeEvent(QResizeEvent *event)
{
    MusicLrcContainer::resizeEvent(event);
    resizeWindow();
}

void MusicLrcContainerForInterior::createColorMenu(QMenu &menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->addAction(menu.addAction(tr("Yellow")))->setData(0);
    group->addAction(menu.addAction(tr("Blue")))->setData(1);
    group->addAction(menu.addAction(tr("Gray")))->setData(2);
    group->addAction(menu.addAction(tr("Pink")))->setData(3);
    group->addAction(menu.addAction(tr("Green")))->setData(4);
    group->addAction(menu.addAction(tr("Red")))->setData(5);
    group->addAction(menu.addAction(tr("Purple")))->setData(6);
    group->addAction(menu.addAction(tr("Orange")))->setData(7);
    group->addAction(menu.addAction(tr("Indigo")))->setData(8);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu.addSeparator();
    menu.addAction(tr("Custom"), this, SLOT(currentLrcCustom()));

    const int index = G_SETTING_PTR->value("LrcColor").toInt();
    if(index > -1 && index < group->actions().count())
    {
        group->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
}

void MusicLrcContainerForInterior::revertTimeSpeed(qint64 pos)
{
    m_lrcAnalysis->revertTime(pos);
    const qint64 beforeTime = setSongTimeSpeed(m_currentTime);
    updateCurrentLrc(beforeTime);

    if(m_changeSpeedValue + pos == 0)
    {
        m_changeSpeedValue = 0;
    }

    QString message;
    if(m_changeSpeedValue > 0)
    {
        message = tr("After %1s").arg(m_changeSpeedValue * 1.0 / TTK_DN_S2MS);
    }
    else if(m_changeSpeedValue < 0)
    {
        message = tr("Before %1s").arg(-m_changeSpeedValue * 1.0 / TTK_DN_S2MS);
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
    TTK::Widget::setLabelFontSize(m_noLrcCurrentInfo, 15);
    TTK::Widget::setLabelFontStyle(m_noLrcCurrentInfo, TTK::FontStyleMode::Underline);
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

void MusicLrcContainerForInterior::initFunctionLabel()
{
    m_functionLabel = new QWidget(this);
    m_functionLabel->setFixedHeight(40);
    m_functionLabel->lower();

    QHBoxLayout *functionLayout = new QHBoxLayout(m_functionLabel);
    functionLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *translation = new QPushButton(this);
    QPushButton *movie = new QPushButton(this);
    QPushButton *message = new QPushButton(this);
    QPushButton *photo = new QPushButton(this);

#ifdef Q_OS_UNIX
    translation->setFocusPolicy(Qt::NoFocus);
    movie->setFocusPolicy(Qt::NoFocus);
    message->setFocusPolicy(Qt::NoFocus);
    photo->setFocusPolicy(Qt::NoFocus);
#endif

    translation->setFixedSize(30, 30);
    movie->setFixedSize(30, 30);
    message->setFixedSize(30, 30);
    photo->setFixedSize(30, 30);

    translation->setStyleSheet(TTK::UI::InteriorTranslation);
    movie->setStyleSheet(TTK::UI::InteriorMovie);
    message->setStyleSheet(TTK::UI::InteriorMessage);
    photo->setStyleSheet(TTK::UI::InteriorPhoto);

    translation->setCursor(Qt::PointingHandCursor);
    movie->setCursor(Qt::PointingHandCursor);
    message->setCursor(Qt::PointingHandCursor);
    photo->setCursor(Qt::PointingHandCursor);

    translation->setToolTip(tr("Translation"));
    movie->setToolTip(tr("Movie"));
    message->setToolTip(tr("Message"));
    photo->setToolTip(tr("Photo"));

    connect(translation, SIGNAL(clicked()), SLOT(translatedLrcData()));
    connect(movie, SIGNAL(clicked()), SLOT(showSongMovieClicked()));
    connect(message, SIGNAL(clicked()), SLOT(showSongCommentsWidget()));
    connect(photo, SIGNAL(clicked()), m_lrcFloatWidget, SLOT(showArtistPhotoWidget()));

    functionLayout->addStretch(1);
    functionLayout->addWidget(translation);
    functionLayout->addWidget(movie);
    functionLayout->addWidget(message);
    functionLayout->addWidget(photo);
    functionLayout->addStretch(1);
    m_functionLabel->setLayout(functionLayout);

    layout()->addWidget(m_functionLabel);
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
        const MusicLrcColor::Color index = TTKStaticCast(MusicLrcColor::Color, G_SETTING_PTR->value("LrcColor").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        const MusicLrcColor cl(MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcFrontgroundColor").toString()),
                               MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcBackgroundColor").toString()));
        setLinearGradientColor(cl);
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
