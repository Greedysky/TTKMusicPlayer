#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicnetworkthread.h"
#include "musicstringutils.h"
#include "musicnetworkproxy.h"
#include "musicnetworkoperator.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musictoastlabel.h"
#include "musichotkeymanager.h"
#include "musicapplicationmodule.h"
#include "musiclrccolorwidget.h"
#include "musiclrcdefines.h"
#include "musicplatformmanager.h"
#include "musiclrcmanager.h"
#include "musicsourceupdatewidget.h"
#include "musiccolordialog.h"
#include "musicalgorithmutils.h"
#include "musicpluginwidget.h"
#include "musicfileutils.h"
#include "musicmessagebox.h"
#include "ttkversion.h"

#include <qmmp/qmmpsettings.h>

#include <QFontDatabase>
#include <QButtonGroup>
#include <QAudioDeviceInfo>

#define SCROLL_ITEM_HEIGHT 370

MusicFunctionTableWidget::MusicFunctionTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 20);
    headerview->resizeSection(2, 85);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setRowCount(3);
    m_listIndex = 0;
}

void MusicFunctionTableWidget::addFunctionItems(int index, const MusicFunctionItemList &items)
{
    m_listIndex = index;
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicFunctionItem &fItem = items[i];
        QTableWidgetItem *item = nullptr;
        setItem(i, 0, item = new QTableWidgetItem());

                      item = new QTableWidgetItem(QIcon(fItem.m_icon), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);

                      item = new QTableWidgetItem(fItem.m_name);
#if TTK_QT_VERSION_CHECK(5,13,0)
        item->setForeground(QColor(80, 80, 80));
#else
        item->setTextColor(QColor(80, 80, 80));
#endif
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

        setRowHeight(i, 28);
    }
}

void MusicFunctionTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(column);
    Q_EMIT currentIndexChanged(row + m_listIndex);
    selectRow(row);
}

void MusicFunctionTableWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    itemCellEntered(-1, -1);
}


MusicSettingWidget::MusicSettingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSettingWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    MusicFunctionItemList items;
    items << MusicFunctionItem(":/contextMenu/btn_setting", tr("Normal"))
          << MusicFunctionItem(":/contextMenu/btn_keyboard", tr("Hotkey"))
          << MusicFunctionItem(":/contextMenu/btn_download", tr("Dwonload"))
          << MusicFunctionItem(":/contextMenu/btn_spectrum", tr("Ripple"))
          << MusicFunctionItem(":/tiny/btn_more_normal", tr("Other"));
    m_ui->normalFunTableWidget->setRowCount(items.count());
    m_ui->normalFunTableWidget->addFunctionItems(0, items);

    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_desktopLrc", tr("Desktop"))
          << MusicFunctionItem(":/contextMenu/btn_lrc", tr("Interior"));
    m_ui->lrcFunTableWidget->setRowCount(items.count());
    m_ui->lrcFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount(), items);

    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_equalizer", tr("Equalizer"))
          << MusicFunctionItem(":/contextMenu/btn_kmicro", tr("Audio"))
          << MusicFunctionItem(":/contextMenu/btn_network", tr("NetWork"));
    m_ui->supperFunTableWidget->setRowCount(items.count());
    m_ui->supperFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount() + m_ui->lrcFunTableWidget->rowCount(), items);

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(setScrollWidgetPageIndex(int)));
    connect(m_ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(setScrollWidgetPageIndex(int)));
    connect(m_ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(setScrollWidgetPageIndex(int)));
    connect(m_ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(saveParameterSettings()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    initScrollWidgetPage();
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete m_ui;
}

void MusicSettingWidget::initialize()
{
    //
    m_ui->autoPlayCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::AutoPlayMode).toBool());
    m_ui->backPlayCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList().front().toInt());
    if(!G_SETTING_PTR->value(MusicSettingManager::CloseEventMode).toBool())
    {
        m_ui->minimumRadioBox->setChecked(true);
    }
    else
    {
        m_ui->quitRadioBox->setChecked(true);
    }
    if(!G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toBool())
    {
        m_ui->quitOpacityRadioBox->setChecked(true);
    }
    else
    {
        m_ui->quitWindowRadioBox->setChecked(true);
    }
    m_ui->languageComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt());

    //
    QStringList hotkeys = G_SETTING_PTR->value(MusicSettingManager::HotkeyValue).toString().split(TTK_SPLITER);
    if(hotkeys.count() != G_HOTKEY_PTR->count())
    {
        hotkeys = G_HOTKEY_PTR->defaultKeys();
    }

    m_ui->item_S02->setText(hotkeys[0]);
    m_ui->item_S04->setText(hotkeys[1]);
    m_ui->item_S06->setText(hotkeys[2]);
    m_ui->item_S08->setText(hotkeys[3]);
    m_ui->item_S10->setText(hotkeys[4]);
    m_ui->item_S12->setText(hotkeys[5]);
    m_ui->item_S14->setText(hotkeys[6]);
    m_ui->item_S16->setText(hotkeys[7]);
    m_ui->globalHotkeyBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toBool());
    globalHotkeyBoxChanged(m_ui->globalHotkeyBox->isChecked());

    //
    m_ui->rippleVersionValue->setText(QString("V") + TTK_VERSION_STR);
    m_ui->rippleVersionUpdateValue->setText(TTK_VER_TIME_STR);
    m_ui->rippleVersionFileValue->setText(MusicUtils::Algorithm::sha1(TTK_VER_TIME_STR).toHex().toUpper());
    m_ui->rippleLowPowerModeBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool());
    m_ui->rippleSpectrumEnableBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toBool());
    m_ui->rippleSpectrumColorButton->setColors(MusicLrcColor::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString()));
    rippleSpectrumOpacityEnableClicked(m_ui->rippleSpectrumEnableBox->isChecked());
    if(m_ui->rippleLowPowerModeBox->isChecked())
    {
        rippleLowPowerEnableBoxClicked(true);
    }

    //
    m_ui->otherCheckUpdateBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toBool());
    m_ui->otherUseAlbumCoverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toBool());
    m_ui->otherUseInfoCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool());
    m_ui->otherWriteAlbumCoverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool());
    m_ui->otherWriteInfoCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toBool());
    m_ui->otherSideByCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool());
    m_ui->otherLrcKTVCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool());
    m_ui->otherScreenSaverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool());

    //
    m_ui->downloadDirEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString());
    m_ui->downloadLrcDirEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadLrcDirPath).toString());
    m_ui->downloadSpinBox->setValue(G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt());
    G_SETTING_PTR->value(MusicSettingManager::DownloadCacheEnable).toInt() == 1 ? m_ui->downloadCacheAutoRadioBox->click() : m_ui->downloadCacheManRadioBox->click();

    MusicUtils::Widget::setComboBoxText(m_ui->downloadLimitSpeedComboBox, G_SETTING_PTR->value(MusicSettingManager::DownloadDownloadLimitSize).toString());
    MusicUtils::Widget::setComboBoxText(m_ui->uploadLimitSpeedComboBox, G_SETTING_PTR->value(MusicSettingManager::DownloadUploadLimitSize).toString());
    G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt() == 1 ? m_ui->downloadFullRadioBox->click() : m_ui->downloadLimitRadioBox->click();

    //
    m_ui->showInteriorCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool());
    m_ui->showInteriorCheckBox->setEnabled(false);

    m_ui->fontComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcFamily).toInt());
    m_ui->fontSizeComboBox->setCurrentIndex(MusicLrcDefines().findInteriorLrcIndex(G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt()));
    m_ui->fontTypeComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcType).toInt());
    m_ui->fontDefaultColorComboBox->setCurrentIndex(DEFAULT_NORMAL_LEVEL);
    if(G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt() != -1)
    {
        m_ui->fontDefaultColorComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt());
    }
    else
    {
        m_ui->playedPushButton->setColors(MusicLrcColor::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColor).toString()));
        m_ui->noPlayedPushButton->setColors(MusicLrcColor::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColor).toString()));
        showInteriorLrcDemo();
    }
    m_ui->transparentSlider->setValue(G_SETTING_PTR->value(MusicSettingManager::LrcColorTransparent).toInt());

    //
    m_ui->showDesktopCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool());
    m_ui->DSingleLineCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineMode).toBool());
    m_ui->DfontComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcFamily).toInt());
    m_ui->DfontSizeComboBox->setCurrentIndex(MusicLrcDefines().findDesktopLrcIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt()));
    m_ui->DfontTypeComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcType).toInt());
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(DEFAULT_NORMAL_LEVEL);
    if(G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt() != -1)
    {
        m_ui->DfontDefaultColorComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt() - LRC_COLOR_OFFSET);
    }
    else
    {
        m_ui->DplayedPushButton->setColors(MusicLrcColor::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColor).toString()));
        m_ui->DnoPlayedPushButton->setColors(MusicLrcColor::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColor).toString()));
        showDesktopLrcDemo();
    }
    m_ui->DtransparentSlider->setValue(G_SETTING_PTR->value(MusicSettingManager::DLrcColorTransparent).toInt());

    //
    QmmpSettings *qmmpSettings = QmmpSettings::instance();
    //replay gain
    m_ui->clippingCheckBox->setChecked(qmmpSettings->replayGainPreventClipping());
    m_ui->replayGainModeComboBox->setCurrentIndex(m_ui->replayGainModeComboBox->findData(qmmpSettings->replayGainMode()));
    m_ui->preampSpinBox->setValue(qmmpSettings->replayGainPreamp());
    m_ui->defaultGainSpinBox->setValue(qmmpSettings->replayGainDefaultGain());
    //audio
    m_ui->volumeStepSpinBox->setValue(qmmpSettings->volumeStep());
    m_ui->softVolumeCheckBox->setChecked(qmmpSettings->useSoftVolume());
    m_ui->bitDepthComboBox->setCurrentIndex(m_ui->bitDepthComboBox->findData(qmmpSettings->outputFormat()));
    m_ui->ditheringCheckBox->setChecked(qmmpSettings->useDithering());
    m_ui->bufferSizeSpinBox->setValue(qmmpSettings->bufferSize());

    //
    m_ui->fadeInSpinBox->setValue(G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValue).toInt());
    m_ui->fadeOutSpinBox->setValue(G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValue).toInt());
    if(G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnable).toInt())
    {
        m_ui->fadeInAndOutCheckBox->click();
    }

    //
    m_ui->downloadServerComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt());
    m_ui->closeNetWorkCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toInt());
#ifdef Q_OS_WIN
    MusicPlatformManager platform;
    if(G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toInt() && platform.isFileAssociate())
    {
        m_ui->setDefaultPlayerCheckBox->setChecked(true);
        if(m_ui->setDefaultPlayerCheckBox->isChecked())
        {
            m_ui->setDefaultPlayerCheckBox->setEnabled(false);
        }
    }
    else
    {
        m_ui->setDefaultPlayerCheckBox->setEnabled(true);
        m_ui->setDefaultPlayerCheckBox->setChecked(false);
        G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, false);
    }
#else
    m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    m_ui->setDefaultPlayerCheckBox->setChecked(false);
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, false);
#endif
}

void MusicSettingWidget::clearFunctionTableSelection()
{
    m_ui->normalFunTableWidget->clearSelection();
    m_ui->lrcFunTableWidget->clearSelection();
    m_ui->supperFunTableWidget->clearSelection();
}

void MusicSettingWidget::globalHotkeyBoxChanged(bool state)
{
    m_ui->item_S02->setHotKeyEnabled(state);
    m_ui->item_S04->setHotKeyEnabled(state);
    m_ui->item_S06->setHotKeyEnabled(state);
    m_ui->item_S08->setHotKeyEnabled(state);
    m_ui->item_S10->setHotKeyEnabled(state);
    m_ui->item_S12->setHotKeyEnabled(state);
    m_ui->item_S14->setHotKeyEnabled(state);
    m_ui->item_S16->setHotKeyEnabled(state);
}

void MusicSettingWidget::downloadCacheClean()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to clean?"));
    if(!message.exec())
    {
        return;
    }

    MusicUtils::File::removeRecursively(APPCACHE_DIR_FULL);
    QDir dir;
    dir.mkpath(ART_DIR_FULL);
    dir.mkpath(CACHE_DIR_FULL);
    dir.mkpath(BACKGROUND_DIR_FULL);

    MusicToastLabel::popup(tr("Cache is cleaned"));
}

void MusicSettingWidget::downloadGroupCached(int index)
{
    m_ui->downloadSpinBox->setEnabled(index);
}

void MusicSettingWidget::downloadGroupSpeedLimit(int index)
{
    m_ui->downloadLimitSpeedComboBox->setEnabled(index);
    m_ui->uploadLimitSpeedComboBox->setEnabled(index);
}

void MusicSettingWidget::downloadDirSelected(int index)
{
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
    if(!path.isEmpty())
    {
        index == 0 ? m_ui->downloadDirEdit->setText(path + TTK_SEPARATOR) : m_ui->downloadLrcDirEdit->setText(path + TTK_SEPARATOR);
    }
}

void MusicSettingWidget::rippleVersionUpdateChanged()
{
    MusicSourceUpdateWidget(this).exec();
}

void MusicSettingWidget::rippleSpectrumColorChanged()
{
    MusicColorDialog dialog(this);
    if(dialog.exec())
    {
        const QColor &color = dialog.color();
        m_ui->rippleSpectrumColorButton->setColors({color});
    }
}

void MusicSettingWidget::rippleSpectrumOpacityEnableClicked(bool state)
{
    m_ui->rippleSpectrumColorButton->setEnabled(state);
}

void MusicSettingWidget::rippleLowPowerEnableBoxClicked(bool state)
{
    m_ui->rippleSpectrumEnableBox->setEnabled(!state);
    rippleSpectrumOpacityEnableClicked(m_ui->rippleSpectrumEnableBox->isEnabled() && m_ui->rippleSpectrumEnableBox->isChecked());
}

void MusicSettingWidget::otherPluginManagerChanged()
{
    MusicPluginWidget().exec();
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    selectFunctionTableIndex(1, 0);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_4);
}

void MusicSettingWidget::changeInteriorLrcWidget()
{
    selectFunctionTableIndex(1, 1);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_5);
}

void MusicSettingWidget::changeDownloadWidget()
{
    selectFunctionTableIndex(0, 2);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_2);
}

void MusicSettingWidget::interiorLrcFrontgroundChanged()
{
    lcrColorValue(LrcInterior, "LRCFRONTGROUNDGCOLOR", m_ui->playedPushButton);
}

void MusicSettingWidget::interiorLrcBackgroundChanged()
{
    lcrColorValue(LrcInterior, "LRCBACKGROUNDCOLOR", m_ui->noPlayedPushButton);
}

void MusicSettingWidget::defaultLrcColorChanged(int value)
{
    lrcColorByDefault(LrcInterior, value);
}

void MusicSettingWidget::interiorLrcTransChanged(int value)
{
    lrcTransparentValue(LrcInterior, value);
    m_ui->fontTransValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::showInteriorLrcDemo()
{
    MusicPreviewLabelItem item;
    item.m_family = m_ui->fontComboBox->currentText();
    item.m_size = m_ui->fontSizeComboBox->currentText().toInt();
    item.m_type = m_ui->fontTypeComboBox->currentIndex();
    item.m_frontground = m_ui->playedPushButton->colors();
    item.m_background = m_ui->noPlayedPushButton->colors();
    m_ui->showLabel->setLinearGradient(item);
    m_ui->showLabel->update();
}

void MusicSettingWidget::resetInteriorParameter()
{
    m_ui->fontComboBox->setCurrentIndex(0);
    m_ui->fontSizeComboBox->setCurrentIndex(0);
    m_ui->fontTypeComboBox->setCurrentIndex(0);
    m_ui->fontDefaultColorComboBox->setCurrentIndex(0);
    m_ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::desktopFrontgroundChanged()
{
    lcrColorValue(LrcDesktop, "DLRCFRONTGROUNDGCOLOR", m_ui->DplayedPushButton);
}

void MusicSettingWidget::desktopBackgroundChanged()
{
    lcrColorValue(LrcDesktop, "DLRCBACKGROUNDCOLOR", m_ui->DnoPlayedPushButton);
}

void MusicSettingWidget::defaultDesktopLrcColorChanged(int value)
{
    lrcColorByDefault(LrcDesktop, value);
}

void MusicSettingWidget::desktopLrcTransChanged(int value)
{
    lrcTransparentValue(LrcDesktop, value);
    m_ui->DfontTransValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::showDesktopLrcDemo()
{
    MusicPreviewLabelItem item;
    item.m_family = m_ui->DfontComboBox->currentText();
    item.m_size = m_ui->DfontSizeComboBox->currentText().toInt();
    item.m_type = m_ui->DfontTypeComboBox->currentIndex();
    item.m_frontground = m_ui->DplayedPushButton->colors();
    item.m_background = m_ui->DnoPlayedPushButton->colors();
    m_ui->DshowLabel->setLinearGradient(item);
    m_ui->DshowLabel->update();
}

void MusicSettingWidget::resetDesktopParameter()
{
    m_ui->DfontComboBox->setCurrentIndex(0);
    m_ui->DfontSizeComboBox->setCurrentIndex(0);
    m_ui->DfontTypeComboBox->setCurrentIndex(0);
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(0);
    m_ui->DtransparentSlider->setValue(100);
}

void MusicSettingWidget::setNetworkProxyControl(int enable)
{
    m_ui->proxyTypeTestButton->setEnabled(enable != 2);
    m_ui->proxyIpEdit->setEnabled(enable != 2);
    m_ui->proxyPortEdit->setEnabled(enable != 2);
    m_ui->proxyUsernameEdit->setEnabled(enable != 2);
    m_ui->proxyPwdEdit->setEnabled(enable != 2);
    m_ui->proxyAreaEdit->setEnabled(enable != 2);
}

void MusicSettingWidget::testNetworkProxy()
{
    setNetworkProxyByType(ProxyTest);
}

void MusicSettingWidget::testProxyStateChanged(bool state)
{
    MusicToastLabel::popup(state ? tr("Test successed!") : tr("Test failed!"));
}

void MusicSettingWidget::testNetworkConnection()
{
    MusicNetworkOperator *network = new MusicNetworkOperator(this);
    connect(network, SIGNAL(queryNetworkOperatorFinished(QString)), SLOT(testNetworkConnectionStateChanged(QString)));
    network->startToDownload();
}

void MusicSettingWidget::checkNetworkConnection()
{
    GENERATE_SINGLE_WIDGET_PARENT(MusicNetworkConnectionTestWidget, this);
}

void MusicSettingWidget::testNetworkConnectionStateChanged(const QString &name)
{
    m_ui->netConnectionTypeValue->setText(!name.isEmpty() ? name : tr("Unknown"));
    m_ui->netConnectionWayValue->setText(!name.isEmpty() ? "TCP" : tr("Unknown"));
}

void MusicSettingWidget::musicFadeInAndOutClicked(bool state)
{
    m_ui->fadeInSpinBox->setEnabled(state);
    m_ui->fadeOutSpinBox->setEnabled(state);
}

void MusicSettingWidget::saveParameterSettings()
{
    const bool languageChanged = G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt() != m_ui->languageComboBox->currentIndex();
    QStringList lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    lastPlayIndex[0] = QString::number(m_ui->backPlayCheckBox->isChecked());

    G_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndex, m_ui->languageComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::AutoPlayMode, m_ui->autoPlayCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndex, lastPlayIndex);
    G_SETTING_PTR->setValue(MusicSettingManager::CloseEventMode, m_ui->quitRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode, m_ui->quitWindowRadioBox->isChecked());
    G_NETWORK_PTR->setBlockNetWork(m_ui->closeNetWorkCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, m_ui->setDefaultPlayerCheckBox->isChecked());

    if(m_ui->setDefaultPlayerCheckBox->isChecked())
    {
        m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    }

    if(m_ui->globalHotkeyBox->isChecked())
    {
        G_HOTKEY_PTR->setHotKey(0, m_ui->item_S02->text());
        G_HOTKEY_PTR->setHotKey(1, m_ui->item_S04->text());
        G_HOTKEY_PTR->setHotKey(2, m_ui->item_S06->text());
        G_HOTKEY_PTR->setHotKey(3, m_ui->item_S08->text());
        G_HOTKEY_PTR->setHotKey(4, m_ui->item_S10->text());
        G_HOTKEY_PTR->setHotKey(5, m_ui->item_S12->text());
        G_HOTKEY_PTR->setHotKey(6, m_ui->item_S14->text());
        G_HOTKEY_PTR->setHotKey(7, m_ui->item_S16->text());
        G_SETTING_PTR->setValue(MusicSettingManager::HotkeyValue, G_HOTKEY_PTR->keys().join(TTK_SPLITER));
    }
    G_HOTKEY_PTR->enabledAll(m_ui->globalHotkeyBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable, m_ui->globalHotkeyBox->isChecked());


    G_SETTING_PTR->setValue(MusicSettingManager::RippleLowPowerMode, m_ui->rippleLowPowerModeBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable, m_ui->rippleSpectrumEnableBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor, MusicLrcColor::writeColorConfig(m_ui->rippleSpectrumColorButton->colors()));


    G_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateEnable, m_ui->otherCheckUpdateBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCover, m_ui->otherUseAlbumCoverCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseFileInfo, m_ui->otherUseInfoCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover, m_ui->otherWriteAlbumCoverCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteFileInfo, m_ui->otherWriteInfoCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByMode, m_ui->otherSideByCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode, m_ui->otherLrcKTVCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, m_ui->otherScreenSaverCheckBox->isChecked());


    G_SETTING_PTR->setValue(MusicSettingManager::ShowInteriorLrc, m_ui->showInteriorCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColor, m_ui->fontDefaultColorComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFamily, m_ui->fontComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcSize, m_ui->fontSizeComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcType, m_ui->fontTypeComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransparent, m_ui->transparentSlider->value());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColor, MusicLrcColor::writeColorConfig(m_ui->playedPushButton->colors()));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColor, MusicLrcColor::writeColorConfig(m_ui->noPlayedPushButton->colors()));


    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, m_ui->showDesktopCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineMode, m_ui->DSingleLineCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColor, m_ui->DfontDefaultColorComboBox->currentIndex() != -1 ? m_ui->DfontDefaultColorComboBox->currentIndex() + LRC_COLOR_OFFSET : -1);
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFamily, m_ui->DfontComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSize, m_ui->DfontSizeComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcType, m_ui->DfontTypeComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransparent, m_ui->DtransparentSlider->value());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColor, MusicLrcColor::writeColorConfig(m_ui->DplayedPushButton->colors()));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColor, MusicLrcColor::writeColorConfig(m_ui->DnoPlayedPushButton->colors()));


    G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath, m_ui->downloadDirEdit->text());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcDirPath, m_ui->downloadLrcDirEdit->text());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheEnable, m_ui->downloadCacheAutoRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize, m_ui->downloadSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, m_ui->downloadFullRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex, m_ui->downloadServerComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadDownloadLimitSize, m_ui->downloadLimitSpeedComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadUploadLimitSize, m_ui->uploadLimitSpeedComboBox->currentText());


    QmmpSettings *qmmpSettings = QmmpSettings::instance();
    int index = m_ui->replayGainModeComboBox->currentIndex();
    qmmpSettings->setReplayGainSettings(TTKStatic_cast(QmmpSettings::ReplayGainMode, m_ui->replayGainModeComboBox->itemData(index).toInt()),
                                        m_ui->preampSpinBox->value(),
                                        m_ui->defaultGainSpinBox->value(),
                                        m_ui->clippingCheckBox->isChecked());
    index = m_ui->bitDepthComboBox->currentIndex();
    qmmpSettings->setAudioSettings(m_ui->softVolumeCheckBox->isChecked(),
                                   TTKStatic_cast(Qmmp::AudioFormat, m_ui->bitDepthComboBox->itemData(index).toInt()),
                                   m_ui->ditheringCheckBox->isChecked());
    qmmpSettings->setBufferSize(m_ui->bufferSizeSpinBox->value());
    qmmpSettings->setVolumeStep(m_ui->volumeStepSpinBox->value());


    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValue, m_ui->fadeInSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValue, m_ui->fadeOutSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable, m_ui->fadeInAndOutCheckBox->isChecked());

    if(m_ui->proxyTypeComboBox->currentIndex() != 2 && !setNetworkProxyByType(ProxyApply))
    {
        return;
    }

    Q_EMIT parameterSettingChanged();
    close();

    if(languageChanged)
    {
        MusicMessageBox message;
        message.setText(tr("Language changed, you need to restart to take effect"));
        message.exec();
    }
}

int MusicSettingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSettingWidget::setScrollWidgetPageIndex(int index)
{
    m_ui->scrollAreaWidget->verticalScrollBar()->setValue(index * SCROLL_ITEM_HEIGHT);
}

void MusicSettingWidget::scrollWidgetValueChanged(int value)
{
    const int index = value / SCROLL_ITEM_HEIGHT;
    if(index < 5)
    {
        selectFunctionTableIndex(0, index);
    }
    else if(index < 7)
    {
        selectFunctionTableIndex(1, index - 5);
    }
    else if(index < 10)
    {
        selectFunctionTableIndex(2, index - 7);
    }
}

void MusicSettingWidget::selectFunctionTableIndex(int row, int col)
{
    clearFunctionTableSelection();
    switch(row)
    {
        case 0: m_ui->normalFunTableWidget->selectRow(col); break;
        case 1: m_ui->lrcFunTableWidget->selectRow(col); break;
        case 2: m_ui->supperFunTableWidget->selectRow(col); break;
        default: break;
    }
}

void MusicSettingWidget::initScrollWidgetPage()
{
    initNormalSettingWidget();
    initDownloadWidget();
    initSpectrumSettingWidget();
    initOtherSettingWidget();
    initDesktopLrcWidget();
    initInteriorLrcWidget();
    initSoundEffectWidget();
    initAudioSettingWidget();
    initNetworkWidget();
    //
    QVBoxLayout *scrollAreaWidgetAreaLayout = new QVBoxLayout(m_ui->scrollAreaWidgetArea);
    scrollAreaWidgetAreaLayout->setSpacing(0);
    scrollAreaWidgetAreaLayout->setContentsMargins(0, 0, 0, 0);
    m_ui->scrollAreaWidgetArea->setLayout(scrollAreaWidgetAreaLayout);
    m_ui->scrollAreaWidget->setMovedScrollBar();
    connect(m_ui->scrollAreaWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(scrollWidgetValueChanged(int)));
    //
    scrollAreaWidgetAreaLayout->addWidget(m_ui->first);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->second);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->third);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->four);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->five);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->six);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->seven);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->eight);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->nine);
    scrollAreaWidgetAreaLayout->addWidget(m_ui->ten);
    //
    m_ui->stackedWidget->hide();
    m_ui->first->show();
    m_ui->second->show();
    m_ui->third->show();
    m_ui->four->show();
    m_ui->five->show();
    m_ui->six->show();
    m_ui->seven->show();
    m_ui->eight->show();
    m_ui->nine->show();
    m_ui->ten->show();
    //
    m_ui->scrollAreaWidgetArea->setFixedHeight(scrollAreaWidgetAreaLayout->count() * SCROLL_ITEM_HEIGHT);
    m_ui->scrollAreaWidget->raise();
    selectFunctionTableIndex(0, 0);
}

void MusicSettingWidget::initNormalSettingWidget()
{
    m_ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->minimumRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->quitRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->quitOpacityRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->quitWindowRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->setDefaultPlayerCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->closeNetWorkCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    QButtonGroup *buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(m_ui->minimumRadioBox, 0);
    buttonGroup1->addButton(m_ui->quitRadioBox, 1);

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(m_ui->quitOpacityRadioBox, 0);
    buttonGroup2->addButton(m_ui->quitWindowRadioBox, 1);

#ifdef Q_OS_UNIX
    m_ui->autoPlayCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->backPlayCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->minimumRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitOpacityRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitWindowRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->setDefaultPlayerCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->closeNetWorkCheckBox->setFocusPolicy(Qt::NoFocus);

    m_ui->quitWindowRadioBox->hide();
#endif
    MusicUtils::Widget::generateComboBoxFormat(m_ui->languageComboBox);
    m_ui->languageComboBox->addItems({tr("0"), tr("1"), tr("2")});

    m_ui->globalHotkeyBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->globalHotkeyBox->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->globalHotkeyBox, SIGNAL(clicked(bool)), SLOT(globalHotkeyBoxChanged(bool)));
}

void MusicSettingWidget::initSpectrumSettingWidget()
{
    m_ui->rippleLowPowerModeBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->rippleSpectrumEnableBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    m_ui->rippleSpectrumColorButton->setText(tr("Effect"));
    connect(m_ui->rippleSpectrumColorButton, SIGNAL(clicked()), SLOT(rippleSpectrumColorChanged()));
    connect(m_ui->rippleLowPowerModeBox, SIGNAL(clicked(bool)), SLOT(rippleLowPowerEnableBoxClicked(bool)));
    connect(m_ui->rippleSpectrumEnableBox, SIGNAL(clicked(bool)), SLOT(rippleSpectrumOpacityEnableClicked(bool)));

    m_ui->rippleVersionUpdateButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->rippleVersionUpdateButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->rippleVersionUpdateButton, SIGNAL(clicked()), SLOT(rippleVersionUpdateChanged()));
#ifdef Q_OS_UNIX
    m_ui->rippleLowPowerModeBox->setFocusPolicy(Qt::NoFocus);
    m_ui->rippleSpectrumEnableBox->setFocusPolicy(Qt::NoFocus);
    m_ui->rippleVersionUpdateButton->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicSettingWidget::initOtherSettingWidget()
{
    m_ui->otherCheckUpdateBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherUseAlbumCoverCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherUseInfoCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherWriteAlbumCoverCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherWriteInfoCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherSideByCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherLrcKTVCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->otherScreenSaverCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    m_ui->otherPluginManagerButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->otherPluginManagerButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->otherPluginManagerButton, SIGNAL(clicked()), SLOT(otherPluginManagerChanged()));
#ifdef Q_OS_UNIX
    m_ui->otherCheckUpdateBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherUseAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherUseInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherSideByCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherLrcKTVCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherScreenSaverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherPluginManagerButton->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicSettingWidget::initDownloadWidget()
{
    m_ui->downloadDirEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->downloadLrcDirEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_ui->downloadDirButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->downloadLrcDirButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->downloadCacheCleanButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadCacheCleanButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadCacheAutoRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->downloadCacheManRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->downloadFullRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->downloadLimitRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_ui->downloadDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLrcDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheCleanButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheAutoRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheManRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadFullRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLimitRadioBox->setFocusPolicy(Qt::NoFocus);
#endif
    MusicUtils::Widget::generateComboBoxFormat(m_ui->downloadServerComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->downloadLimitSpeedComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->uploadLimitSpeedComboBox);

    m_ui->downloadSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->downloadSpinBox->setRange(1024, 5 * 1024);
    m_ui->downloadDirEdit->setText(MUSIC_DIR_FULL);
    m_ui->downloadLrcDirEdit->setText(LRC_DIR_FULL);
    QStringList downloadSpeed;
    downloadSpeed << "100" << "200" << "300" << "400" << "500" << "600"
                  << "700" << "800" << "900" << "1000" << "1100" << "1200";
    m_ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    m_ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_wangyiyun"), tr("WangYi Music"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_qq"), tr("QQ Music"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kuwo"), tr("KuWo Music"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kugou"), tr("KuGou Music"));

    connect(m_ui->downloadCacheCleanButton, SIGNAL(clicked()), SLOT(downloadCacheClean()));
    //
    QButtonGroup *buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(m_ui->downloadCacheAutoRadioBox, 0);
    buttonGroup1->addButton(m_ui->downloadCacheManRadioBox, 1);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup1, SIGNAL(idClicked(int)), SLOT(downloadGroupCached(int)));
#else
    connect(buttonGroup1, SIGNAL(buttonClicked(int)), SLOT(downloadGroupCached(int)));
#endif

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(m_ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(m_ui->downloadLimitRadioBox, 1);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup2, SIGNAL(idClicked(int)), SLOT(downloadGroupSpeedLimit(int)));
#else
    connect(buttonGroup2, SIGNAL(buttonClicked(int)), SLOT(downloadGroupSpeedLimit(int)));
#endif

    QButtonGroup *buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(m_ui->downloadDirButton, 0);
    buttonGroup3->addButton(m_ui->downloadLrcDirButton, 1);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup3, SIGNAL(idClicked(int)), SLOT(downloadDirSelected(int)));
#else
    connect(buttonGroup3, SIGNAL(buttonClicked(int)), SLOT(downloadDirSelected(int)));
#endif

    m_ui->downloadCacheAutoRadioBox->click();
    m_ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    m_ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->DSingleLineCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    MusicUtils::Widget::generateComboBoxFormat(m_ui->DfontComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->DfontSizeComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->DfontTypeComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->DfontDefaultColorComboBox);

    m_ui->DfontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    m_ui->DfontSizeComboBox->addItems(MusicLrcDefines().desktopLrcSize());
    m_ui->DfontTypeComboBox->addItems({"1", "2", "3", "4"});
    m_ui->DfontDefaultColorComboBox->addItems({tr("DWhite"), tr("DBlue"), tr("DRed"), tr("DBlack"), tr("DYellow"), tr("DPurple"), tr("DGreen")});

    connect(m_ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    m_ui->DtransparentSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle06);
    m_ui->DnoPlayedPushButton->setText(tr("No"));
    m_ui->DplayedPushButton->setText(tr("Yes"));
    connect(m_ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBackgroundChanged()));
    connect(m_ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFrontgroundChanged()));
    connect(m_ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    m_ui->DresetPushButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));
#ifdef Q_OS_UNIX
    m_ui->showDesktopCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DSingleLineCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DresetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetDesktopParameter();
}

void MusicSettingWidget::initInteriorLrcWidget()
{
    m_ui->showInteriorCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    MusicUtils::Widget::generateComboBoxFormat(m_ui->fontComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->fontSizeComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->fontTypeComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->fontDefaultColorComboBox);

    m_ui->fontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    m_ui->fontSizeComboBox->addItems(MusicLrcDefines().interiorLrcSize());
    m_ui->fontTypeComboBox->addItems({"1", "2", "3", "4"});
    m_ui->fontDefaultColorComboBox->addItems({tr("Yellow"), tr("Blue"), tr("Gray"), tr("Pink"), tr("Green"), tr("Red"), tr("Purple"), tr("Orange"), tr("Indigo")});

    connect(m_ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    m_ui->transparentSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle06);
    m_ui->noPlayedPushButton->setText(tr("No"));
    m_ui->playedPushButton->setText(tr("Yes"));
    connect(m_ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(interiorLrcBackgroundChanged()));
    connect(m_ui->playedPushButton, SIGNAL(clicked()), SLOT(interiorLrcFrontgroundChanged()));
    connect(m_ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(interiorLrcTransChanged(int)));

    m_ui->resetPushButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInteriorParameter()));
#ifdef Q_OS_UNIX
    m_ui->showInteriorCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->resetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetInteriorParameter();
}

void MusicSettingWidget::initSoundEffectWidget()
{
    MusicUtils::Widget::generateComboBoxFormat(m_ui->outputTypeComboBox);
    for(const QAudioDeviceInfo &info : QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        m_ui->outputTypeComboBox->addItem(info.deviceName());
    }

    m_ui->fadeInAndOutCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->fadeInAndOutCheckBox->setEnabled(false);

    m_ui->fadeInSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->fadeInSpinBox->setRange(1, 10 * 1000);
    m_ui->fadeInSpinBox->setValue(600);
    m_ui->fadeInSpinBox->setEnabled(false);

    m_ui->fadeOutSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->fadeOutSpinBox->setRange(1, 10 * 1000);
    m_ui->fadeOutSpinBox->setValue(600);
    m_ui->fadeOutSpinBox->setEnabled(false);

    m_ui->equalizerButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->equalizerPluginsButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->equalizerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->equalizerPluginsButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->fadeInAndOutCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerButton->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerPluginsButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->equalizerButton, SIGNAL(clicked()), MusicApplicationModule::instance(), SLOT(musicSetEqualizer()));
    connect(m_ui->equalizerPluginsButton, SIGNAL(clicked()), MusicApplicationModule::instance(), SLOT(musicSetSoundEffect()));
    connect(m_ui->fadeInAndOutCheckBox, SIGNAL(clicked(bool)), SLOT(musicFadeInAndOutClicked(bool)));
}

void MusicSettingWidget::initAudioSettingWidget()
{
    MusicUtils::Widget::generateComboBoxFormat(m_ui->replayGainModeComboBox);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->bitDepthComboBox);

    m_ui->preampSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->defaultGainSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->volumeStepSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);
    m_ui->bufferSizeSpinBox->setStyleSheet(MusicUIObject::MQSSSpinBoxStyle01);

    m_ui->clippingCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->softVolumeCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->ditheringCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->clippingCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->softVolumeCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->ditheringCheckBox->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->replayGainModeComboBox->addItem(tr("Track"), QmmpSettings::REPLAYGAIN_TRACK);
    m_ui->replayGainModeComboBox->addItem(tr("Album"), QmmpSettings::REPLAYGAIN_ALBUM);
    m_ui->replayGainModeComboBox->addItem(tr("Disabled"), QmmpSettings::REPLAYGAIN_DISABLED);
    m_ui->bitDepthComboBox->addItem("16", Qmmp::PCM_S16LE);
    m_ui->bitDepthComboBox->addItem("24", Qmmp::PCM_S24LE);
    m_ui->bitDepthComboBox->addItem("32", Qmmp::PCM_S32LE);
}

void MusicSettingWidget::initNetworkWidget()
{
    m_ui->proxyIpEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->proxyPortEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->proxyPwdEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->proxyUsernameEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->proxyAreaEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    MusicUtils::Widget::generateComboBoxFormat(m_ui->proxyTypeComboBox);

    m_ui->proxyTypeTestButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->proxyTypeTestButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netConnectionTypeButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->netConnectionTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netCheckTypeButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->netCheckTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->proxyTypeTestButton->setFocusPolicy(Qt::NoFocus);
    m_ui->netConnectionTypeButton->setFocusPolicy(Qt::NoFocus);
    m_ui->netCheckTypeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->proxyTypeComboBox->addItems({tr("DefaultProxy"), tr("Socks5Proxy"), tr("NoProxy"), tr("HttpProxy"), tr("HttpCachingProxy"), tr("FtpCachingProxy")});

    connect(m_ui->proxyTypeTestButton, SIGNAL(clicked()), SLOT(testNetworkProxy()));
    connect(m_ui->netConnectionTypeButton, SIGNAL(clicked()), SLOT(testNetworkConnection()));
    connect(m_ui->netCheckTypeButton, SIGNAL(clicked()), SLOT(checkNetworkConnection()));
    connect(m_ui->proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setNetworkProxyControl(int)));
    setNetworkProxyControl(2);
    m_ui->proxyTypeComboBox->setCurrentIndex(2);
}

void MusicSettingWidget::lcrColorValue(LrcType key, const QString &type, QLabel *obj)
{
    key == LrcInterior ? m_ui->fontDefaultColorComboBox->setCurrentIndex(DEFAULT_NORMAL_LEVEL) : m_ui->DfontDefaultColorComboBox->setCurrentIndex(DEFAULT_NORMAL_LEVEL);

    MusicLrcColorWidget dialog(this);
    if(type == "DLRCFRONTGROUNDGCOLOR") dialog.setColors(m_ui->DplayedPushButton->colors());
    if(type == "DLRCBACKGROUNDCOLOR") dialog.setColors(m_ui->DnoPlayedPushButton->colors());
    if(type == "LRCFRONTGROUNDGCOLOR") dialog.setColors(m_ui->playedPushButton->colors());
    if(type == "LRCBACKGROUNDCOLOR") dialog.setColors(m_ui->noPlayedPushButton->colors());

    if(dialog.exec())
    {
        const QList<QColor> &colors = dialog.colors();
        TTKStatic_cast(MusicColorPreviewLabel*, obj)->setColors(colors);
    }
    key == LrcInterior ? showInteriorLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::lrcColorByDefault(LrcType key, int index)
{
    if(index == -1)
    {
        return;
    }

    if(key == LrcInterior)
    {
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(TTKStatic_cast(MusicLrcColor::LrcColorType, index));
        m_ui->playedPushButton->setColors(cl.m_frontColor);
        m_ui->noPlayedPushButton->setColors(cl.m_backColor);
        showInteriorLrcDemo();
    }
    else
    {
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(TTKStatic_cast(MusicLrcColor::LrcColorType, index + LRC_COLOR_OFFSET));
        m_ui->DplayedPushButton->setColors(cl.m_frontColor);
        m_ui->DnoPlayedPushButton->setColors(cl.m_backColor);
        showDesktopLrcDemo();
    }
}

void MusicSettingWidget::lrcTransparentValue(LrcType key, int value) const
{
    MusicPreviewLabel* label;
    if(key == LrcInterior)
    {
        label = m_ui->showLabel;
        label->setTransparent(2.55 * value);
        label->setLinearGradient(m_ui->playedPushButton->colors(), m_ui->noPlayedPushButton->colors());
    }
    else
    {
        label = m_ui->DshowLabel;
        label->setTransparent(2.55 * value);
        label->setLinearGradient(m_ui->DplayedPushButton->colors(), m_ui->DnoPlayedPushButton->colors());
    }
    label->update();
}

bool MusicSettingWidget::setNetworkProxyByType(ProxyType type)
{
    MusicNetworkProxy proxy;
    connect(&proxy, SIGNAL(testProxyStateChanged(bool)), SLOT(testProxyStateChanged(bool)));
    proxy.setType(m_ui->proxyTypeComboBox->currentIndex());

    QString value = m_ui->proxyIpEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicToastLabel::popup(tr("Proxy hostname is empty"));
        return false;
    }
    proxy.setHostName(value);

    value = m_ui->proxyPortEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicToastLabel::popup(tr("Proxy port is empty"));
        return false;
    }

    proxy.setPort(value.toInt());
    proxy.setUser(m_ui->proxyUsernameEdit->text().trimmed());
    proxy.setPassword(m_ui->proxyPwdEdit->text().trimmed());

    if(type == 0)
    {
        proxy.testProxy();
    }
    else if(type == 1)
    {
        proxy.applyProxy();
    }
    return true;
}
