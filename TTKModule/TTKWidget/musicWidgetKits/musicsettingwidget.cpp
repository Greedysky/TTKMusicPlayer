#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicnetworkthread.h"
#include "musicnetworkproxy.h"
#include "musicnetworkoperator.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musictoastlabel.h"
#include "musichotkeymanager.h"
#include "musicapplicationmodule.h"
#include "musiclrccolorwidget.h"
#include "musiclrchelper.h"
#include "musiclrcmanager.h"
#include "musicsourceupdatewidget.h"
#include "musicfileassociationwidget.h"
#include "musiccolordialog.h"
#include "musicalgorithmutils.h"
#include "musicpluginwidget.h"
#include "musicfileutils.h"
#include "musicmessagebox.h"
#include "musicrulesanalysis.h"
#include "ttkversion.h"
#include "ttkeventfilter.h"
#include "ttkfileassociation.h"

#include <qmmp/qmmpsettings.h>

#include <QButtonGroup>
#include <QFontDatabase>
#if TTK_QT_VERSION_CHECK(6,2,0)
#  include <QAudioDevice>
#  include <QMediaDevices>
#elif !TTK_QT_VERSION_CHECK(6,0,0)
#  include <QAudioDeviceInfo>
#endif

static constexpr int SCROLL_ITEM_HEIGHT = 370;

MusicFunctionTableWidget::MusicFunctionTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_listIndex(0)
{
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 20);
    headerView->resizeSection(1, 20);
    headerView->resizeSection(2, 85);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setRowCount(3);
}

void MusicFunctionTableWidget::addCellItems(int index, const MusicFunctionItemList &items)
{
    m_listIndex = index;
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicFunctionItem &v = items[i];

        QTableWidgetItem *item = nullptr;
        setItem(i, 0, item = new QTableWidgetItem);

                      item = new QTableWidgetItem(QIcon(v.m_icon), {});
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 1, item);

                      item = new QTableWidgetItem(v.m_name);
        item->setForeground(QColor(TTK::UI::Color02));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
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


MusicSettingWidget::MusicSettingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSettingWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    MusicFunctionItemList items;
    items << MusicFunctionItem(":/contextMenu/btn_setting", tr("Normal"))
          << MusicFunctionItem(":/contextMenu/btn_keyboard", tr("Hotkey"))
          << MusicFunctionItem(":/contextMenu/btn_download", tr("Download"))
          << MusicFunctionItem(":/contextMenu/btn_spectrum", tr("Ripple"))
          << MusicFunctionItem(":/tiny/btn_more_normal", tr("Other"));
    m_ui->normalFunTableWidget->setRowCount(items.count());
    m_ui->normalFunTableWidget->addCellItems(0, items);

    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_desktop_lrc", tr("Desktop"))
          << MusicFunctionItem(":/contextMenu/btn_lrc", tr("Interior"));
    m_ui->lrcFunTableWidget->setRowCount(items.count());
    m_ui->lrcFunTableWidget->addCellItems(m_ui->normalFunTableWidget->rowCount(), items);

    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_equalizer", tr("Equalizer"))
          << MusicFunctionItem(":/contextMenu/btn_kmicro", tr("Audio"))
          << MusicFunctionItem(":/contextMenu/btn_network", tr("NetWork"));
    m_ui->supperFunTableWidget->setRowCount(items.count());
    m_ui->supperFunTableWidget->addCellItems(m_ui->normalFunTableWidget->rowCount() + m_ui->lrcFunTableWidget->rowCount(), items);

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_hotKeyEdits << m_ui->item_key1 << m_ui->item_key2 << m_ui->item_key3 << m_ui->item_key4
                  << m_ui->item_key5 << m_ui->item_key6 << m_ui->item_key7 << m_ui->item_key8
                  << m_ui->item_key9 << m_ui->item_key10 << m_ui->item_key11 << m_ui->item_key12
                  << m_ui->item_key13 << m_ui->item_key14 << m_ui->item_key15 << m_ui->item_key16;

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
    m_ui->autoStartCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::StartUpMode).toBool());
    m_ui->autoPlayCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::StartUpPlayMode).toBool());
    m_ui->lastPlayCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList().first().toInt());

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
    m_ui->languageComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LanguageIndex).toInt());

    //
    QStringList hotkeys = G_SETTING_PTR->value(MusicSettingManager::HotkeyValue).toString().split(TTK_SPLITER);
    if(hotkeys.count() != G_HOTKEY_PTR->count())
    {
        hotkeys = G_HOTKEY_PTR->defaultKeys();
    }

    for(int i = 0; i < m_hotKeyEdits.count(); ++i)
    {
        m_hotKeyEdits[i]->setText(hotkeys[i]);
    }

    m_ui->globalHotkeyBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toBool());
    globalHotkeyBoxChanged(m_ui->globalHotkeyBox->isChecked());

    //
    m_ui->rippleVersionValue->setText(QString("V") + TTK_VERSION_STR);
    m_ui->rippleVersionUpdateValue->setText(TTK_VERSION_TIME_STR);
    m_ui->rippleVersionFileValue->setText(TTK::Algorithm::sha1(TTK_VERSION_STR).toHex());
    m_ui->rippleLowPowerModeBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool());
    m_ui->rippleSpectrumEnableBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toBool());
    m_ui->rippleSpectrumColorButton->setColors(TTK::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString()));
    m_ui->rippleTransparentSlider->setValue(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumTransparent).toInt());

    rippleSpectrumEnableClicked(m_ui->rippleSpectrumEnableBox->isChecked());

    if(m_ui->rippleLowPowerModeBox->isChecked())
    {
        rippleLowPowerEnableBoxClicked(true);
    }

    //
    m_ui->otherReadAlbumCoverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toBool());
    m_ui->otherReadInfoCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherReadFileInfo).toBool());
    m_ui->otherWriteAlbumCoverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool());
    m_ui->otherWriteInfoCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toBool());
    m_ui->otherCheckUpdateBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toBool());
    m_ui->otherLogTrackCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherLogTrackEnable).toBool());
    m_ui->otherSideByCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool());
    m_ui->otherLrcKTVCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool());
    m_ui->otherPlaylistAutoSaveCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherPlaylistAutoSaveEnable).toBool());
    m_ui->otherScreenSaverCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool());
    m_ui->otherHighDpiScalingCheckBox->setCheckState(TTKStaticCast(Qt::CheckState, G_SETTING_PTR->value(MusicSettingManager::OtherHighDpiScalingEnable).toInt()));
    m_ui->otherRandomModeCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::OtherRandomShuffleMode).toBool());

    //
    m_ui->downloadDirEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString());
    m_ui->downloadLrcDirEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadLrcDirPath).toString());
    m_ui->downloadRuleEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadFileNameRule).toString());
    m_ui->downloadSpinBox->setValue(G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt());
    G_SETTING_PTR->value(MusicSettingManager::DownloadCacheEnable).toInt() == 1 ? m_ui->downloadCacheAutoRadioBox->click() : m_ui->downloadCacheManRadioBox->click();

    TTK::Widget::setComboBoxText(m_ui->downloadLimitSpeedComboBox, G_SETTING_PTR->value(MusicSettingManager::DownloadDownloadLimitSize).toString());
    TTK::Widget::setComboBoxText(m_ui->uploadLimitSpeedComboBox, G_SETTING_PTR->value(MusicSettingManager::DownloadUploadLimitSize).toString());
    G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt() == 1 ? m_ui->downloadFullRadioBox->click() : m_ui->downloadLimitRadioBox->click();

    //
    m_ui->fontComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcFamily).toInt());
    m_ui->fontSizeComboBox->setCurrentIndex(MusicLrcHelper().findInteriorLrcIndex(G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt()));
    m_ui->fontTypeComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcType).toInt());
    m_ui->fontDefaultColorComboBox->setCurrentIndex(TTK_NORMAL_LEVEL);

    if(G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt() != -1)
    {
        m_ui->fontDefaultColorComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt());
    }
    else
    {
        m_ui->playedPushButton->setColors(TTK::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColor).toString()));
        m_ui->noPlayedPushButton->setColors(TTK::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColor).toString()));
        showInteriorLrcDemo();
    }
    m_ui->transparentSlider->setValue(G_SETTING_PTR->value(MusicSettingManager::LrcColorTransparent).toInt());

    //
    m_ui->showDesktopCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool());
    m_ui->DsingleLineCheckBox->setChecked(G_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineMode).toBool());
    m_ui->DfontComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcFamily).toInt());
    m_ui->DfontSizeComboBox->setCurrentIndex(MusicLrcHelper().findDesktopLrcIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt()));
    m_ui->DfontTypeComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcType).toInt());
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(TTK_NORMAL_LEVEL);

    if(G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt() != -1)
    {
        m_ui->DfontDefaultColorComboBox->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt() - LRC_COLOR_OFFSET);
    }
    else
    {
        m_ui->DplayedPushButton->setColors(TTK::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColor).toString()));
        m_ui->DnoPlayedPushButton->setColors(TTK::readColorConfig(G_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColor).toString()));
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
    if(G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toBool())
    {
        m_ui->setDefaultPlayerCheckBox->setChecked(true);
    }
    else
    {
        m_ui->setDefaultPlayerCheckBox->setChecked(false);
        G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, false);
    }
    m_ui->defaultPlayerSettingButton->setVisible(m_ui->setDefaultPlayerCheckBox->isChecked());
#else
    m_ui->autoStartCheckBox->setEnabled(false);
    m_ui->autoStartCheckBox->setChecked(false);
    m_ui->defaultPlayerSettingButton->setVisible(false);
    G_SETTING_PTR->setValue(MusicSettingManager::StartUpMode, false);

    m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    m_ui->setDefaultPlayerCheckBox->setChecked(false);
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, false);

    m_ui->otherSideByCheckBox->setEnabled(false);
    m_ui->otherSideByCheckBox->setChecked(false);
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByMode, false);
#endif
}

void MusicSettingWidget::close()
{
    setVisible(false);
}

void MusicSettingWidget::clearFunctionTableSelection()
{
    m_ui->normalFunTableWidget->clearSelection();
    m_ui->lrcFunTableWidget->clearSelection();
    m_ui->supperFunTableWidget->clearSelection();
}

void MusicSettingWidget::fileAssociationChanged()
{
    MusicFileAssociationWidget().exec();
}

void MusicSettingWidget::globalHotkeyBoxChanged(bool state)
{
    for(MusicGlobalHotKeyEdit *edit : qAsConst(m_hotKeyEdits))
    {
        edit->setHotKeyEnabled(state);
    }
}

void MusicSettingWidget::downloadCacheClean()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to clean?"));
    if(!message.exec())
    {
        return;
    }

    TTK::File::removeRecursively(APPCACHE_DIR_FULL);

    QDir dir;
    dir.mkpath(ART_DIR_FULL);
    dir.mkpath(CACHE_DIR_FULL);
    dir.mkpath(BACKGROUND_DIR_FULL);

    MusicToastLabel::popup(tr("Cache is cleaned"));
}

void MusicSettingWidget::downloadFileNameRuleChanged(QAction *action)
{
    const QString &rule = MusicRulesAnalysis::rule(action->data().toInt());
    if(!rule.isEmpty())
    {
        m_ui->downloadRuleEdit->setText(m_ui->downloadRuleEdit->text() + rule);
    }
}

void MusicSettingWidget::serverPluginChanged()
{
    MusicServerPluginWidget().exec();
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

void MusicSettingWidget::downloadDirChanged(int index)
{
    const QString &path = TTK::File::getExistingDirectory(this);
    if(!path.isEmpty())
    {
        index == 0 ? m_ui->downloadDirEdit->setText(path) : m_ui->downloadLrcDirEdit->setText(path);
    }
}

void MusicSettingWidget::rippleVersionUpdateChanged()
{
    MusicSourceUpdateWidget(this).exec();
}

void MusicSettingWidget::rippleSpectrumColorChanged()
{
    MusicColorDialog dialog(m_ui->rippleSpectrumColorButton->color(), this);
    if(dialog.exec())
    {
        m_ui->rippleSpectrumColorButton->setColors({dialog.color()});
    }
}

void MusicSettingWidget::rippleSpectrumTransChanged(int value)
{
    m_ui->rippleTransValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::rippleSpectrumEnableClicked(bool state)
{
    m_ui->rippleSpectrumColorButton->setEnabled(state);
    m_ui->rippleTransparentSlider->setEnabled(state);
}

void MusicSettingWidget::rippleLowPowerEnableBoxClicked(bool state)
{
    m_ui->rippleSpectrumEnableBox->setEnabled(!state);
    rippleSpectrumEnableClicked(m_ui->rippleSpectrumEnableBox->isEnabled() && m_ui->rippleSpectrumEnableBox->isChecked());
}

void MusicSettingWidget::otherPluginManagerChanged()
{
    MusicQmmpPluginWidget(this).exec();
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    selectFunctionTableIndex(1, 0);
    setScrollWidgetPageIndex(SettingType::Desktop);
}

void MusicSettingWidget::changeInteriorLrcWidget()
{
    selectFunctionTableIndex(1, 1);
    setScrollWidgetPageIndex(SettingType::Interior);
}

void MusicSettingWidget::changeDownloadWidget()
{
    selectFunctionTableIndex(0, 2);
    setScrollWidgetPageIndex(SettingType::Download);
}

void MusicSettingWidget::interiorLrcFrontgroundChanged()
{
    lcrColorValue(Lrc::Interior, "LRCFRONTGROUNDGCOLOR", m_ui->playedPushButton);
}

void MusicSettingWidget::interiorLrcBackgroundChanged()
{
    lcrColorValue(Lrc::Interior, "LRCBACKGROUNDCOLOR", m_ui->noPlayedPushButton);
}

void MusicSettingWidget::defaultLrcColorChanged(int value)
{
    lrcColorByDefault(Lrc::Interior, value);
}

void MusicSettingWidget::interiorLrcTransChanged(int value)
{
    lrcTransparentValue(Lrc::Interior, value);
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
    lcrColorValue(Lrc::Desktop, "DLRCFRONTGROUNDGCOLOR", m_ui->DplayedPushButton);
}

void MusicSettingWidget::desktopBackgroundChanged()
{
    lcrColorValue(Lrc::Desktop, "DLRCBACKGROUNDCOLOR", m_ui->DnoPlayedPushButton);
}

void MusicSettingWidget::defaultDesktopLrcColorChanged(int value)
{
    lrcColorByDefault(Lrc::Desktop, value);
}

void MusicSettingWidget::desktopLrcTransChanged(int value)
{
    lrcTransparentValue(Lrc::Desktop, value);
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
    setNetworkProxyByType(Proxy::Test);
}

void MusicSettingWidget::testProxyStateChanged(bool state)
{
    MusicToastLabel::popup(state ? tr("Test successed") : tr("Test failed"));
}

void MusicSettingWidget::testNetworkConnection()
{
    MusicNetworkOperator *req = new MusicNetworkOperator(this);
    connect(req, SIGNAL(queryNetworkOperatorFinished(QString)), SLOT(testNetworkConnectionStateChanged(QString)));
    req->startToRequest();
}

void MusicSettingWidget::checkNetworkConnection()
{
    TTK_GENERATE_SINGLE_WIDGET(MusicNetworkConnectionTestWidget, this);
}

void MusicSettingWidget::testNetworkConnectionStateChanged(const QString &name)
{
    m_ui->netConnectionTypeValue->setText(!name.isEmpty() ? name : tr("Unknown"));
    m_ui->netConnectionWayValue->setText(!name.isEmpty() ? "TCP" : tr("Unknown"));
}

void MusicSettingWidget::fadeInAndOutClicked(bool state)
{
    m_ui->fadeInSpinBox->setEnabled(state);
    m_ui->fadeOutSpinBox->setEnabled(state);
}

void MusicSettingWidget::saveParameterSettings()
{
    const bool hotkeyEnabled = m_ui->globalHotkeyBox->isChecked();
    const bool languageChanged = G_SETTING_PTR->value(MusicSettingManager::LanguageIndex).toInt() != m_ui->languageComboBox->currentIndex();
    const bool highDpiScalingChanged = G_SETTING_PTR->value(MusicSettingManager::OtherHighDpiScalingEnable).toInt() != m_ui->otherHighDpiScalingCheckBox->checkState();

    QStringList lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    lastPlayIndex[0] = QString::number(m_ui->lastPlayCheckBox->isChecked());

    G_NETWORK_PTR->setBlockNetwork(m_ui->closeNetWorkCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::LanguageIndex, m_ui->languageComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::StartUpMode, m_ui->autoStartCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::StartUpPlayMode, m_ui->autoPlayCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndex, lastPlayIndex);
    G_SETTING_PTR->setValue(MusicSettingManager::CloseEventMode, m_ui->quitRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode, m_ui->quitWindowRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, m_ui->setDefaultPlayerCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable, hotkeyEnabled);

    if(hotkeyEnabled)
    {
        for(int i = 0; i < m_hotKeyEdits.count(); ++i)
        {
            G_HOTKEY_PTR->setHotKey(i, m_hotKeyEdits[i]->text());
        }

        G_HOTKEY_PTR->setEnabled(true);
        G_SETTING_PTR->setValue(MusicSettingManager::HotkeyValue, G_HOTKEY_PTR->keys().join(TTK_SPLITER));
    }
    else
    {
        G_HOTKEY_PTR->unsetShortcut();
    }

    G_SETTING_PTR->setValue(MusicSettingManager::RippleLowPowerMode, m_ui->rippleLowPowerModeBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable, m_ui->rippleSpectrumEnableBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor, TTK::writeColorConfig(m_ui->rippleSpectrumColorButton->colors()));
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumTransparent, m_ui->rippleTransparentSlider->value());

    G_SETTING_PTR->setValue(MusicSettingManager::OtherReadAlbumCover, m_ui->otherReadAlbumCoverCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherReadFileInfo, m_ui->otherReadInfoCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover, m_ui->otherWriteAlbumCoverCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteFileInfo, m_ui->otherWriteInfoCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateEnable, m_ui->otherCheckUpdateBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLogTrackEnable, m_ui->otherLogTrackCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByMode, m_ui->otherSideByCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode, m_ui->otherLrcKTVCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherPlaylistAutoSaveEnable, m_ui->otherPlaylistAutoSaveCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, m_ui->otherScreenSaverCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherHighDpiScalingEnable, m_ui->otherHighDpiScalingCheckBox->checkState());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherRandomShuffleMode, m_ui->otherRandomModeCheckBox->isChecked());

    G_SETTING_PTR->setValue(MusicSettingManager::LrcColor, m_ui->fontDefaultColorComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFamily, m_ui->fontComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcSize, m_ui->fontSizeComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcType, m_ui->fontTypeComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransparent, m_ui->transparentSlider->value());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColor, TTK::writeColorConfig(m_ui->playedPushButton->colors()));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColor, TTK::writeColorConfig(m_ui->noPlayedPushButton->colors()));

    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, m_ui->showDesktopCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineMode, m_ui->DsingleLineCheckBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColor, m_ui->DfontDefaultColorComboBox->currentIndex() != -1 ? m_ui->DfontDefaultColorComboBox->currentIndex() + LRC_COLOR_OFFSET : -1);
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFamily, m_ui->DfontComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSize, m_ui->DfontSizeComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcType, m_ui->DfontTypeComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransparent, m_ui->DtransparentSlider->value());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColor, TTK::writeColorConfig(m_ui->DplayedPushButton->colors()));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColor, TTK::writeColorConfig(m_ui->DnoPlayedPushButton->colors()));

    G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath, m_ui->downloadDirEdit->text());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcDirPath, m_ui->downloadLrcDirEdit->text());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadFileNameRule, m_ui->downloadRuleEdit->text());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheEnable, m_ui->downloadCacheAutoRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize, m_ui->downloadSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, m_ui->downloadFullRadioBox->isChecked());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex, m_ui->downloadServerComboBox->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadDownloadLimitSize, m_ui->downloadLimitSpeedComboBox->currentText());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadUploadLimitSize, m_ui->uploadLimitSpeedComboBox->currentText());

    QmmpSettings *qmmpSettings = QmmpSettings::instance();
    int index = m_ui->replayGainModeComboBox->currentIndex();
    qmmpSettings->setReplayGainSettings(TTKStaticCast(QmmpSettings::ReplayGainMode, m_ui->replayGainModeComboBox->itemData(index).toInt()),
                                        m_ui->preampSpinBox->value(),
                                        m_ui->defaultGainSpinBox->value(),
                                        m_ui->clippingCheckBox->isChecked());
    index = m_ui->bitDepthComboBox->currentIndex();
    qmmpSettings->setAudioSettings(m_ui->softVolumeCheckBox->isChecked(),
                                   TTKStaticCast(Qmmp::AudioFormat, m_ui->bitDepthComboBox->itemData(index).toInt()),
                                   m_ui->ditheringCheckBox->isChecked());
    qmmpSettings->setBufferSize(m_ui->bufferSizeSpinBox->value());
    qmmpSettings->setVolumeStep(m_ui->volumeStepSpinBox->value());

    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValue, m_ui->fadeInSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValue, m_ui->fadeOutSpinBox->value());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable, m_ui->fadeInAndOutCheckBox->isChecked());

    if(m_ui->proxyTypeComboBox->currentIndex() != 2 && !setNetworkProxyByType(Proxy::Apply))
    {
        return;
    }

    Q_EMIT parameterSettingChanged();
    close();

    // show something tips for user
    QString showTips;
    if(languageChanged)
    {
        showTips = tr("Language changed, you need to restart to take effect");
    }
    else if(highDpiScalingChanged)
    {
        showTips = tr("High DPI scaling changed, you need to restart to take effect");
    }

    if(!showTips.isEmpty())
    {
        MusicMessageBox message;
        message.setText(showTips);
        message.exec();
    }
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
    m_ui->autoPlayCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->lastPlayCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->minimumRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->quitRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->quitOpacityRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->quitWindowRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->setDefaultPlayerCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->autoStartCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->closeNetWorkCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);

    m_ui->defaultPlayerSettingButton->hide();
    m_ui->defaultPlayerSettingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->defaultPlayerSettingButton->setStyleSheet(TTK::UI::PushButtonStyle02);

    m_ui->globalHotkeyBox->setStyleSheet(TTK::UI::CheckBoxStyle01);

    QButtonGroup *buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(m_ui->minimumRadioBox, 0);
    buttonGroup1->addButton(m_ui->quitRadioBox, 1);

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(m_ui->quitOpacityRadioBox, 0);
    buttonGroup2->addButton(m_ui->quitWindowRadioBox, 1);

#ifdef Q_OS_UNIX
    m_ui->autoPlayCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->lastPlayCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->minimumRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitOpacityRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->quitWindowRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->setDefaultPlayerCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->autoStartCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->closeNetWorkCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->globalHotkeyBox->setFocusPolicy(Qt::NoFocus);

    m_ui->quitWindowRadioBox->setEnabled(false);
#endif
    TTK::Widget::generateComboBoxStyle(m_ui->languageComboBox);
    m_ui->languageComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->languageComboBox));
    m_ui->languageComboBox->addItems({tr("0"), tr("1"), tr("2")});

    connect(m_ui->globalHotkeyBox, SIGNAL(clicked(bool)), SLOT(globalHotkeyBoxChanged(bool)));
    connect(m_ui->defaultPlayerSettingButton, SIGNAL(clicked()), SLOT(fileAssociationChanged()));
    connect(m_ui->setDefaultPlayerCheckBox, SIGNAL(clicked(bool)), m_ui->defaultPlayerSettingButton, SLOT(setVisible(bool)));
}

void MusicSettingWidget::initDownloadWidget()
{
    m_ui->downloadDirEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->downloadLrcDirEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->downloadRuleEdit->setStyleSheet(TTK::UI::LineEditStyle01);

    m_ui->downloadDirButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downloadLrcDirButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downloadCacheCleanButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downloadRuleButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downloadServerPluginButton->setStyleSheet(TTK::UI::PushButtonStyle08);
    m_ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadCacheCleanButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadRuleButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadServerPluginButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadCacheAutoRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->downloadCacheManRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->downloadFullRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->downloadLimitRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_ui->downloadDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLrcDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheCleanButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadRuleButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadServerPluginButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheAutoRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheManRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadFullRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLimitRadioBox->setFocusPolicy(Qt::NoFocus);
#endif
    TTK::Widget::generateComboBoxStyle(m_ui->downloadServerComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->downloadLimitSpeedComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->uploadLimitSpeedComboBox);
    m_ui->downloadSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->downloadSpinBox));
    m_ui->downloadServerComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->downloadServerComboBox));
    m_ui->downloadLimitSpeedComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->downloadLimitSpeedComboBox));
    m_ui->uploadLimitSpeedComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->uploadLimitSpeedComboBox));

    m_ui->downloadSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->downloadSpinBox->setRange(1024, 5 * 1024);
    m_ui->downloadDirEdit->setText(MUSIC_DIR_FULL);
    m_ui->downloadLrcDirEdit->setText(LRC_DIR_FULL);

    const QStringList downloadSpeed{"100", "200", "300", "400", "500", "600", "700", "800", "900", "1000", "1100", "1200"};
    m_ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    m_ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_wangyiyun"), tr("WangYi"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kuwo"), tr("KuWo"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kugou"), tr("KuGou"));
    m_ui->downloadServerPluginButton->setIcon(QIcon(":/toolSets/btn_plugins_normal"));

    const QStringList &rules = MusicRulesAnalysis::rules();
    QMenu *menu = new QMenu(m_ui->downloadRuleButton);
    menu->setStyleSheet(TTK::UI::MenuStyle02);
    m_ui->downloadRuleButton->setMenu(menu);

    for(int i = 0; i < rules.count(); ++i)
    {
        menu->addAction(rules[i])->setData(i);
    }

    connect(m_ui->downloadCacheCleanButton, SIGNAL(clicked()), SLOT(downloadCacheClean()));
    connect(m_ui->downloadRuleButton->menu(), SIGNAL(triggered(QAction*)), SLOT(downloadFileNameRuleChanged(QAction*)));
    connect(m_ui->downloadServerPluginButton, SIGNAL(clicked()), SLOT(serverPluginChanged()));
    //
    QButtonGroup *buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(m_ui->downloadCacheAutoRadioBox, 0);
    buttonGroup1->addButton(m_ui->downloadCacheManRadioBox, 1);
    QtButtonGroupConnect(buttonGroup1, this, downloadGroupCached, TTK_SLOT);

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(m_ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(m_ui->downloadLimitRadioBox, 1);
    QtButtonGroupConnect(buttonGroup2, this, downloadGroupSpeedLimit, TTK_SLOT);

    QButtonGroup *buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(m_ui->downloadDirButton, 0);
    buttonGroup3->addButton(m_ui->downloadLrcDirButton, 1);
    QtButtonGroupConnect(buttonGroup3, this, downloadDirChanged, TTK_SLOT);

    m_ui->downloadCacheAutoRadioBox->click();
    m_ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initSpectrumSettingWidget()
{
    m_ui->rippleLowPowerModeBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->rippleSpectrumEnableBox->setStyleSheet(TTK::UI::CheckBoxStyle01);

    m_ui->rippleSpectrumColorButton->setText(tr("Effect"));
    m_ui->rippleTransparentSlider->setStyleSheet(TTK::UI::SliderStyle06);

    connect(m_ui->rippleSpectrumColorButton, SIGNAL(clicked()), SLOT(rippleSpectrumColorChanged()));
    connect(m_ui->rippleLowPowerModeBox, SIGNAL(clicked(bool)), SLOT(rippleLowPowerEnableBoxClicked(bool)));
    connect(m_ui->rippleSpectrumEnableBox, SIGNAL(clicked(bool)), SLOT(rippleSpectrumEnableClicked(bool)));
    connect(m_ui->rippleTransparentSlider, SIGNAL(valueChanged(int)), SLOT(rippleSpectrumTransChanged(int)));

    m_ui->rippleVersionUpdateButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->rippleVersionUpdateButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->rippleVersionUpdateButton, SIGNAL(clicked()), SLOT(rippleVersionUpdateChanged()));
#ifdef Q_OS_UNIX
    m_ui->rippleLowPowerModeBox->setFocusPolicy(Qt::NoFocus);
    m_ui->rippleSpectrumEnableBox->setFocusPolicy(Qt::NoFocus);
    m_ui->rippleVersionUpdateButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_ui->rippleTransparentSlider->setValue(100);
}

void MusicSettingWidget::initOtherSettingWidget()
{
    m_ui->otherReadAlbumCoverCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherReadInfoCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherWriteAlbumCoverCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherWriteInfoCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherCheckUpdateBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherLogTrackCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherSideByCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherLrcKTVCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherPlaylistAutoSaveCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherScreenSaverCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherHighDpiScalingCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->otherRandomModeCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);

    m_ui->otherPluginManagerButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->otherPluginManagerButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->otherPluginManagerButton, SIGNAL(clicked()), SLOT(otherPluginManagerChanged()));
#ifdef Q_OS_UNIX
    m_ui->otherReadAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherReadInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherCheckUpdateBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherLogTrackCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherSideByCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherLrcKTVCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherPlaylistAutoSaveCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherScreenSaverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherHighDpiScalingCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherRandomModeCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherPluginManagerButton->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    m_ui->showDesktopCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->DsingleLineCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);

    TTK::Widget::generateComboBoxStyle(m_ui->DfontComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->DfontSizeComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->DfontTypeComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->DfontDefaultColorComboBox);
    m_ui->DfontComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->DfontComboBox));
    m_ui->DfontSizeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->DfontSizeComboBox));
    m_ui->DfontTypeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->DfontTypeComboBox));
    m_ui->DfontDefaultColorComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->DfontDefaultColorComboBox));

    m_ui->DfontComboBox->addItems(QtFontFamilies());
    m_ui->DfontSizeComboBox->addItems(MusicLrcHelper().desktopLrcSize());
    m_ui->DfontTypeComboBox->addItems({"1", "2", "3", "4"});
    m_ui->DfontDefaultColorComboBox->addItems({tr("DWhite"), tr("DBlue"), tr("DRed"), tr("DBlack"), tr("DYellow"), tr("DPurple"), tr("DGreen")});

    connect(m_ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    m_ui->DtransparentSlider->setStyleSheet(TTK::UI::SliderStyle06);
    m_ui->DnoPlayedPushButton->setText(tr("No"));
    m_ui->DplayedPushButton->setText(tr("Yes"));
    connect(m_ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBackgroundChanged()));
    connect(m_ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFrontgroundChanged()));
    connect(m_ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    m_ui->DresetPushButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));
#ifdef Q_OS_UNIX
    m_ui->showDesktopCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DsingleLineCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DresetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetDesktopParameter();
}

void MusicSettingWidget::initInteriorLrcWidget()
{
    TTK::Widget::generateComboBoxStyle(m_ui->fontComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->fontSizeComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->fontTypeComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->fontDefaultColorComboBox);
    m_ui->fontComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->fontComboBox));
    m_ui->fontSizeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->fontSizeComboBox));
    m_ui->fontTypeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->fontTypeComboBox));
    m_ui->fontDefaultColorComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->fontDefaultColorComboBox));

    m_ui->fontComboBox->addItems(QtFontFamilies());
    m_ui->fontSizeComboBox->addItems(MusicLrcHelper().interiorLrcSize());
    m_ui->fontTypeComboBox->addItems({"1", "2", "3", "4"});
    m_ui->fontDefaultColorComboBox->addItems({tr("Yellow"), tr("Blue"), tr("Gray"), tr("Pink"), tr("Green"), tr("Red"), tr("Purple"), tr("Orange"), tr("Indigo")});

    connect(m_ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInteriorLrcDemo()));
    connect(m_ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    m_ui->transparentSlider->setStyleSheet(TTK::UI::SliderStyle06);
    m_ui->noPlayedPushButton->setText(tr("No"));
    m_ui->playedPushButton->setText(tr("Yes"));
    connect(m_ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(interiorLrcBackgroundChanged()));
    connect(m_ui->playedPushButton, SIGNAL(clicked()), SLOT(interiorLrcFrontgroundChanged()));
    connect(m_ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(interiorLrcTransChanged(int)));

    m_ui->resetPushButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInteriorParameter()));
#ifdef Q_OS_UNIX
    m_ui->resetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetInteriorParameter();
}

void MusicSettingWidget::initSoundEffectWidget()
{
    TTK::Widget::generateComboBoxStyle(m_ui->outputTypeComboBox);
    m_ui->outputTypeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->outputTypeComboBox));
    m_ui->fadeInSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->fadeInSpinBox));
    m_ui->fadeOutSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->fadeOutSpinBox));
#if TTK_QT_VERSION_CHECK(6,2,0)
    for(const QAudioDevice &device : QMediaDevices::audioInputs())
    {
        m_ui->outputTypeComboBox->addItem(device.description());
    }
#elif !TTK_QT_VERSION_CHECK(6,0,0)
    for(const QAudioDeviceInfo &info : QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        m_ui->outputTypeComboBox->addItem(info.deviceName());
    }
#endif

    m_ui->fadeInAndOutCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->fadeInAndOutCheckBox->setEnabled(false);

    m_ui->fadeInSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->fadeInSpinBox->setRange(1, 10 * TTK_DN_S2MS);
    m_ui->fadeInSpinBox->setValue(600);
    m_ui->fadeInSpinBox->setEnabled(false);

    m_ui->fadeOutSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->fadeOutSpinBox->setRange(1, 10 * TTK_DN_S2MS);
    m_ui->fadeOutSpinBox->setValue(600);
    m_ui->fadeOutSpinBox->setEnabled(false);

    m_ui->equalizerButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->equalizerPluginsButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->equalizerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->equalizerPluginsButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->fadeInAndOutCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerButton->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerPluginsButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->equalizerButton, SIGNAL(clicked()), MusicApplicationModule::instance(), SLOT(showEqualizerWidget()));
    connect(m_ui->equalizerPluginsButton, SIGNAL(clicked()), MusicApplicationModule::instance(), SLOT(showSoundEffectWidget()));
    connect(m_ui->fadeInAndOutCheckBox, SIGNAL(clicked(bool)), SLOT(fadeInAndOutClicked(bool)));
}

void MusicSettingWidget::initAudioSettingWidget()
{
    TTK::Widget::generateComboBoxStyle(m_ui->replayGainModeComboBox);
    TTK::Widget::generateComboBoxStyle(m_ui->bitDepthComboBox);
    m_ui->replayGainModeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->replayGainModeComboBox));
    m_ui->preampSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->preampSpinBox));
    m_ui->defaultGainSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->defaultGainSpinBox));
    m_ui->volumeStepSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->volumeStepSpinBox));
    m_ui->bufferSizeSpinBox->installEventFilter(new TTKWheelEventFilter(m_ui->bufferSizeSpinBox));
    m_ui->bitDepthComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->bitDepthComboBox));

    m_ui->preampSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->defaultGainSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->volumeStepSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);
    m_ui->bufferSizeSpinBox->setStyleSheet(TTK::UI::SpinBoxStyle01);

    m_ui->clippingCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->softVolumeCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->ditheringCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
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
    m_ui->proxyIpEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->proxyPortEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->proxyPwdEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->proxyUsernameEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->proxyAreaEdit->setStyleSheet(TTK::UI::LineEditStyle01);

    TTK::Widget::generateComboBoxStyle(m_ui->proxyTypeComboBox);
    m_ui->proxyTypeComboBox->installEventFilter(new TTKWheelEventFilter(m_ui->proxyTypeComboBox));

    m_ui->proxyTypeTestButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->proxyTypeTestButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netConnectionTypeButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->netConnectionTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netCheckTypeButton->setStyleSheet(TTK::UI::PushButtonStyle04);
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

void MusicSettingWidget::selectFunctionTableIndex(int row, int column)
{
    clearFunctionTableSelection();
    switch(row)
    {
        case 0: m_ui->normalFunTableWidget->selectRow(column); break;
        case 1: m_ui->lrcFunTableWidget->selectRow(column); break;
        case 2: m_ui->supperFunTableWidget->selectRow(column); break;
        default: break;
    }
}

void MusicSettingWidget::lcrColorValue(Lrc key, const QString &type, QLabel *obj)
{
    key == Lrc::Interior ? m_ui->fontDefaultColorComboBox->setCurrentIndex(TTK_NORMAL_LEVEL) : m_ui->DfontDefaultColorComboBox->setCurrentIndex(TTK_NORMAL_LEVEL);

    MusicLrcColorWidget dialog(this);
    if(type == "DLRCFRONTGROUNDGCOLOR") dialog.setColors(m_ui->DplayedPushButton->colors());
    else if(type == "DLRCBACKGROUNDCOLOR") dialog.setColors(m_ui->DnoPlayedPushButton->colors());
    else if(type == "LRCFRONTGROUNDGCOLOR") dialog.setColors(m_ui->playedPushButton->colors());
    else if(type == "LRCBACKGROUNDCOLOR") dialog.setColors(m_ui->noPlayedPushButton->colors());

    if(dialog.exec())
    {
        const QList<QColor> &colors = dialog.colors();
        TTKObjectCast(MusicColorPreviewLabel*, obj)->setColors(colors);
    }
    key == Lrc::Interior ? showInteriorLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::lrcColorByDefault(Lrc key, int index)
{
    if(index == -1)
    {
        return;
    }

    if(key == Lrc::Interior)
    {
        const MusicLrcColor &cl = TTK::mapIndexToColor(TTKStaticCast(MusicLrcColor::Color, index));
        m_ui->playedPushButton->setColors(cl.m_frontColor);
        m_ui->noPlayedPushButton->setColors(cl.m_backColor);
        showInteriorLrcDemo();
    }
    else
    {
        const MusicLrcColor &cl = TTK::mapIndexToColor(TTKStaticCast(MusicLrcColor::Color, index + LRC_COLOR_OFFSET));
        m_ui->DplayedPushButton->setColors(cl.m_frontColor);
        m_ui->DnoPlayedPushButton->setColors(cl.m_backColor);
        showDesktopLrcDemo();
    }
}

void MusicSettingWidget::lrcTransparentValue(Lrc key, int value) const
{
    MusicPreviewLabel* label;
    if(key == Lrc::Interior)
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

bool MusicSettingWidget::setNetworkProxyByType(Proxy type)
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

    switch(type)
    {
        case Proxy::Test: proxy.testProxy(); break;
        case Proxy::Apply: proxy.applyProxy(); break;
        default: break;
    }
    return true;
}
