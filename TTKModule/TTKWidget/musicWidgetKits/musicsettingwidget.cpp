#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicnetworkthread.h"
#include "musicstringutils.h"
#include "musicnetworkproxy.h"
#include "musicnetworkoperator.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musicmessagebox.h"
#include "musichotkeymanager.h"
#include "musicapplicationobject.h"
#include "musiclrccolorwidget.h"
#include "musiclrcdefines.h"
#include "musiclrcmanager.h"
#include "musicregeditmanager.h"
#include "musicotherdefine.h"
#include "ttkversion.h"
#include "musicsourceupdatewidget.h"
#include "musicsinglemanager.h"
#include "musicapplication.h"
#include "musiccolordialog.h"
//qmmp
#include "qmmpsettings.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QButtonGroup>
#include <QAudioDeviceInfo>
#include <QStyledItemDelegate>

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

void MusicFunctionTableWidget::addFunctionItems(int index, const MusicFunctionItems &items)
{
    m_listIndex = index;
    for(int i=0; i<items.count(); ++i)
    {
        const MusicFunctionItem &&fItem = std::move(items[i]);
        QTableWidgetItem *item = nullptr;
        setItem(i, 0, item = new QTableWidgetItem());

                      item = new QTableWidgetItem(QIcon(fItem.m_icon), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);

                      item = new QTableWidgetItem(fItem.m_name);
        item->setTextColor(QColor(80, 80, 80));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
}

void MusicFunctionTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(column);
    emit currentIndexChanged(row + m_listIndex);
    selectRow( currentRow() );
}

void MusicFunctionTableWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    listCellEntered(-1, -1);
}


MusicSettingWidget::MusicSettingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSettingWidget)
{
    m_ui->setupUi(this);

    //
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    //
    MusicFunctionItems items;
    items << MusicFunctionItem(":/contextMenu/btn_setting", tr("Normal"))
          << MusicFunctionItem(":/contextMenu/btn_keyboard", tr("Hotkey"))
          << MusicFunctionItem(":/contextMenu/btn_download", tr("Dwonload"))
          << MusicFunctionItem(":/tiny/btn_more_normal", tr("Other"));
    m_ui->normalFunTableWidget->setRowCount(items.count());
    m_ui->normalFunTableWidget->addFunctionItems(0, items);
    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_lrc", tr("Inline"))
          << MusicFunctionItem(":/contextMenu/btn_desktopLrc", tr("Desktop"));
    m_ui->lrcFunTableWidget->setRowCount(items.count());
    m_ui->lrcFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount(), items);
    items.clear();
    items << MusicFunctionItem(":/contextMenu/btn_equalizer", tr("Equalizer"))
          << MusicFunctionItem(":/contextMenu/btn_kmicro", tr("Audio"))
          << MusicFunctionItem(":/contextMenu/btn_network", tr("NetWork"));
    m_ui->supperFunTableWidget->setRowCount(3);
    m_ui->supperFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount() + m_ui->lrcFunTableWidget->rowCount(), items);

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
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
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    initScrollWidgetPage();
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete m_ui;
}

void MusicSettingWidget::initControllerParameter()
{
    //Set init parameter
    m_ui->autoPlayCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toBool());
    m_ui->backPlayCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList().first().toInt());
    if(!M_SETTING_PTR->value(MusicSettingManager::CloseEventChoiced).toBool())
    {
        m_ui->minimumRadioBox->setChecked(true);
    }
    else
    {
        m_ui->quitRadioBox->setChecked(true);
    }
    if(!M_SETTING_PTR->value(MusicSettingManager::WindowQuitModeChoiced).toBool())
    {
        m_ui->quitOpacityRadioBox->setChecked(true);
    }
    else
    {
        m_ui->quitWindowRadioBox->setChecked(true);
    }
    m_ui->languageComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt());

    //
    QStringList hotkeys = M_SETTING_PTR->value(MusicSettingManager::HotkeyStringChoiced).toString().split(TTK_STR_SPLITER);
    if(hotkeys.count() != M_HOTKEY_PTR->count())
    {
        hotkeys = M_HOTKEY_PTR->getDefaultKeys();
    }
    m_ui->item_S2->setText(hotkeys[0]);
    m_ui->item_S4->setText(hotkeys[1]);
    m_ui->item_S6->setText(hotkeys[2]);
    m_ui->item_S8->setText(hotkeys[3]);
    m_ui->item_S10->setText(hotkeys[4]);
    m_ui->item_S12->setText(hotkeys[5]);
    m_ui->item_S14->setText(hotkeys[6]);
    m_ui->item_S16->setText(hotkeys[7]);
    m_ui->globalHotkeyBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::HotkeyEnableChoiced).toBool());
    globalHotkeyBoxChanged(m_ui->globalHotkeyBox->isChecked());

    //
    M_SETTING_PTR->value(MusicSettingManager::OtherBackgroundLosslessChoiced).toBool() ? m_ui->otherHeighImageRadioBox->click() : m_ui->otherNormalImageRadioBox->click();
    m_ui->otherCheckUpdateBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateChoiced).toBool());
    m_ui->otherSearchCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherSearchChoiced).toBool());
    m_ui->otherUseAlbumCoverCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCoverChoiced).toBool());
    m_ui->otherUseInfoCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherUseInfoChoiced).toBool());
    m_ui->otherWriteAlbumCoverCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCoverChoiced).toBool());
    m_ui->otherWriteInfoCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherWriteInfoChoiced).toBool());
    m_ui->otherSideByCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toBool());
    m_ui->otherLrcKTVCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVModeChoiced).toBool());
    m_ui->otherVersionValue->setText(QString("V") + TTKMUSIC_VERSION_STR);

    m_ui->ripplesSpectrumOpacitySlider->setValue(M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumOpacityChoiced).toInt());
    m_ui->ripplesSpectrumEnableBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumEnableChoiced).toBool());
    m_ui->ripplesSpectrumColorButton->setColors(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumColorChoiced).toString()));
    ripplesSpectrumOpacityEnableClicked(m_ui->ripplesSpectrumEnableBox->isChecked());

    //
    m_ui->downloadDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    m_ui->downloadLrcDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString());
    m_ui->downloadSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt());
    M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt() == 1 ? m_ui->downloadCacheAutoRadioBox->click() : m_ui->downloadCacheManRadioBox->click();

    MusicUtils::Widget::setComboBoxText(m_ui->downloadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString());
    MusicUtils::Widget::setComboBoxText(m_ui->uploadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimitChoiced).toString());
    M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 1 ? m_ui->downloadFullRadioBox->click() : m_ui->downloadLimitRadioBox->click();

    //
    //Set init parameter
    m_ui->showInlineCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    m_ui->showInlineCheckBox->setEnabled(false);

    m_ui->fontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcFamilyChoiced).toInt());
    m_ui->fontSizeComboBox->setCurrentIndex(MusicLrcDefines().findInlineLrcIndex(M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt()));
    m_ui->fontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt());
    m_ui->fontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
    {
        m_ui->fontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt());
    }
    else
    {
        m_ui->playedPushButton->setColors(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColorChoiced).toString()));
        m_ui->noPlayedPushButton->setColors(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColorChoiced).toString()));
        showInlineLrcDemo();
    }
    m_ui->transparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt());

    //
    m_ui->showDesktopCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    m_ui->DSingleLineCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineTypeChoiced).toBool());
    m_ui->DfontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    m_ui->DfontSizeComboBox->setCurrentIndex(MusicLrcDefines().findDesktopLrcIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt()));
    m_ui->DfontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt());
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
    {
        m_ui->DfontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt() - LRC_COLOR_OFFSET);
    }
    else
    {
        m_ui->DplayedPushButton->setColors(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColorChoiced).toString()));
        m_ui->DnoPlayedPushButton->setColors(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColorChoiced).toString()));
        showDesktopLrcDemo();
    }
    m_ui->DtransparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt());

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
    m_ui->fadeInSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValueChoiced).toInt());
    m_ui->fadeOutSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValueChoiced).toInt());
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnableChoiced).toInt())
    {
        m_ui->fadeInAndOutCheckBox->click();
    }

    //
    m_ui->downloadServerComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt());
    m_ui->closeNetWorkCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
    if(M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt() && MusicRegeditManager().isFileAssociate())
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
        M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced, false);
    }
}

void MusicSettingWidget::clearFunctionTableSelection()
{
    m_ui->normalFunTableWidget->clearSelection();
    m_ui->lrcFunTableWidget->clearSelection();
    m_ui->supperFunTableWidget->clearSelection();
}

void MusicSettingWidget::globalHotkeyBoxChanged(bool state)
{
    m_ui->item_S2->setEnabled(state);
    m_ui->item_S4->setEnabled(state);
    m_ui->item_S6->setEnabled(state);
    m_ui->item_S8->setEnabled(state);
    m_ui->item_S10->setEnabled(state);
    m_ui->item_S12->setEnabled(state);
    m_ui->item_S14->setEnabled(state);
    m_ui->item_S16->setEnabled(state);
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
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        const QString &path = dialog.directory().absolutePath();
        if(!path.isEmpty())
        {
            index == 0 ? m_ui->downloadDirEdit->setText(path + "/") : m_ui->downloadLrcDirEdit->setText(path + "/");
        }
    }
}

void MusicSettingWidget::otherVersionUpdateChanged()
{
    MusicSourceUpdateWidget(this).exec();
}

void MusicSettingWidget::ripplesSpectrumColorChanged()
{
    MusicColorDialog getColor(this);
    if(getColor.exec())
    {
        const QColor &color = getColor.color();
        m_ui->ripplesSpectrumColorButton->setColors(QList<QColor>() << color);
    }
}

void MusicSettingWidget::ripplesSpectrumOpacityChanged(int value)
{
    m_ui->ripplesSpectrumOpacityValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::ripplesSpectrumOpacityEnableClicked(bool state)
{
    m_ui->ripplesSpectrumColorButton->setEnabled(state);
    m_ui->ripplesSpectrumOpacitySlider->setEnabled(state);
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    selectFunctionTableIndex(1, 0);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_4);
}

void MusicSettingWidget::changeInlineLrcWidget()
{
    selectFunctionTableIndex(1, 1);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_5);
}

void MusicSettingWidget::changeDownloadWidget()
{
    selectFunctionTableIndex(0, 2);
    setScrollWidgetPageIndex(SETTING_WINDOW_INDEX_2);
}

void MusicSettingWidget::inlineLrcFrontgroundChanged()
{
    lcrColorValue(Inline, "LRCFRONTGROUNDGCOLOR", m_ui->playedPushButton);
}

void MusicSettingWidget::inlineLrcBackgroundChanged()
{
    lcrColorValue(Inline, "LRCBACKGROUNDCOLOR", m_ui->noPlayedPushButton);
}

void MusicSettingWidget::defaultLrcColorChanged(int value)
{
    lrcColorByDefault(Inline, value);
}

void MusicSettingWidget::inlineLrcTransChanged(int value)
{
    lrcTransparentValue(Inline, value);
    m_ui->fontTransValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::showInlineLrcDemo()
{
    MusicPreviewLabelItem item;
    item.m_family = m_ui->fontComboBox->currentText();
    item.m_size = m_ui->fontSizeComboBox->currentText().toInt();
    item.m_type = m_ui->fontTypeComboBox->currentIndex();
    item.m_frontground = m_ui->playedPushButton->getColors();
    item.m_background = m_ui->noPlayedPushButton->getColors();
    m_ui->showLabel->setLinearGradient(item);
    m_ui->showLabel->update();
}

void MusicSettingWidget::resetInlineParameter()
{
    m_ui->fontComboBox->setCurrentIndex(0);
    m_ui->fontSizeComboBox->setCurrentIndex(0);
    m_ui->fontTypeComboBox->setCurrentIndex(0);
    m_ui->fontDefaultColorComboBox->setCurrentIndex(0);
    m_ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::desktopFrontgroundChanged()
{
    lcrColorValue(Desktop, "DLRCFRONTGROUNDGCOLOR", m_ui->DplayedPushButton);
}

void MusicSettingWidget::desktopBackgroundChanged()
{
    lcrColorValue(Desktop, "DLRCBACKGROUNDCOLOR", m_ui->DnoPlayedPushButton);
}

void MusicSettingWidget::defaultDesktopLrcColorChanged(int value)
{
    lrcColorByDefault(Desktop, value);
}

void MusicSettingWidget::desktopLrcTransChanged(int value)
{
    lrcTransparentValue(Desktop, value);
    m_ui->DfontTransValueLabel->setText(QString::number(value) + "%");
}

void MusicSettingWidget::showDesktopLrcDemo()
{
    MusicPreviewLabelItem item;
    item.m_family = m_ui->DfontComboBox->currentText();
    item.m_size = m_ui->DfontSizeComboBox->currentText().toInt();
    item.m_type = m_ui->DfontTypeComboBox->currentIndex();
    item.m_frontground = m_ui->DplayedPushButton->getColors();
    item.m_background = m_ui->DnoPlayedPushButton->getColors();
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
    setNetworkProxyByType(0);
}

void MusicSettingWidget::testProxyStateChanged(bool state)
{
    MusicMessageBox message;
    message.setText(state ? tr("Test Successed!") : tr("Test Failed!") );
    message.exec();
}

void MusicSettingWidget::testNetworkConnection()
{
    MusicNetworkOperator *netOpr = new MusicNetworkOperator(this);
    connect(netOpr, SIGNAL(getNetworkOperatorFinished(QString)), SLOT(testNetworkConnectionStateChanged(QString)));
    netOpr->startToDownload();
}

void MusicSettingWidget::checkNetworkConnection()
{
    M_SINGLE_MANAGER_WIDGET_CLASS2(MusicNetworkConnectionTestWidget, this);
}

void MusicSettingWidget::testNetworkConnectionStateChanged(const QString &name)
{
    M_LOGGER_INFO(name);
    m_ui->netConnectionTypeValue->setText(!name.isEmpty() ? name : tr("Unknown"));
    m_ui->netConnectionWayValue->setText(!name.isEmpty() ? "TCP" : tr("Unknown"));
}

void MusicSettingWidget::musicFadeInAndOutClicked(bool state)
{
    m_ui->fadeInSpinBox->setEnabled(state);
    m_ui->fadeOutSpinBox->setEnabled(state);
}

void MusicSettingWidget::commitTheResults()
{
    M_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndexChoiced, m_ui->languageComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::AutoPlayChoiced, m_ui->autoPlayCheckBox->isChecked());
    QStringList list = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list[0] = QString::number(m_ui->backPlayCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, list);
    M_SETTING_PTR->setValue(MusicSettingManager::CloseEventChoiced, m_ui->quitRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::WindowQuitModeChoiced, m_ui->quitWindowRadioBox->isChecked());
    M_NETWORK_PTR->setBlockNetWork( m_ui->closeNetWorkCheckBox->isChecked() );
    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced, m_ui->setDefaultPlayerCheckBox->isChecked());

    if(m_ui->setDefaultPlayerCheckBox->isChecked())
    {
        m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    }

    if(m_ui->globalHotkeyBox->isChecked())
    {
        M_HOTKEY_PTR->setHotKey(0, m_ui->item_S2->text());
        M_HOTKEY_PTR->setHotKey(1, m_ui->item_S4->text());
        M_HOTKEY_PTR->setHotKey(2, m_ui->item_S6->text());
        M_HOTKEY_PTR->setHotKey(3, m_ui->item_S8->text());
        M_HOTKEY_PTR->setHotKey(4, m_ui->item_S10->text());
        M_HOTKEY_PTR->setHotKey(5, m_ui->item_S12->text());
        M_HOTKEY_PTR->setHotKey(6, m_ui->item_S14->text());
        M_HOTKEY_PTR->setHotKey(7, m_ui->item_S16->text());
        M_SETTING_PTR->setValue(MusicSettingManager::HotkeyStringChoiced, M_HOTKEY_PTR->getKeys().join(TTK_STR_SPLITER));
    }
    M_HOTKEY_PTR->enabledAll(m_ui->globalHotkeyBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnableChoiced, m_ui->globalHotkeyBox->isChecked());


    M_SETTING_PTR->setValue(MusicSettingManager::OtherBackgroundLosslessChoiced, m_ui->otherHeighImageRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateChoiced, m_ui->otherCheckUpdateBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSearchChoiced, m_ui->otherSearchCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCoverChoiced, m_ui->otherUseAlbumCoverCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseInfoChoiced, m_ui->otherUseInfoCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCoverChoiced, m_ui->otherWriteAlbumCoverCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteInfoChoiced, m_ui->otherWriteInfoCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSideByChoiced, m_ui->otherSideByCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSongFormat, /*m_ui->otherSongFormatComboBox->currentIndex()*/0);
    M_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVModeChoiced, m_ui->otherLrcKTVCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumOpacityChoiced, m_ui->ripplesSpectrumOpacitySlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumEnableChoiced, m_ui->ripplesSpectrumEnableBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumColorChoiced, MusicUtils::String::writeColorConfig(m_ui->ripplesSpectrumColorButton->getColors()));


    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced, m_ui->showInlineCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced, m_ui->fontDefaultColorComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFamilyChoiced, m_ui->fontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, m_ui->fontSizeComboBox->currentText());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced, m_ui->fontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransChoiced, m_ui->transparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColorChoiced, MusicUtils::String::writeColorConfig(m_ui->playedPushButton->getColors()));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColorChoiced, MusicUtils::String::writeColorConfig(m_ui->noPlayedPushButton->getColors()));


    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, m_ui->showDesktopCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineTypeChoiced, m_ui->DSingleLineCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorChoiced, m_ui->DfontDefaultColorComboBox->currentIndex() != -1 ?
                                                                   m_ui->DfontDefaultColorComboBox->currentIndex() + LRC_COLOR_OFFSET : -1);
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFamilyChoiced, m_ui->DfontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSizeChoiced, m_ui->DfontSizeComboBox->currentText());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcTypeChoiced, m_ui->DfontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransChoiced, m_ui->DtransparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColorChoiced, MusicUtils::String::writeColorConfig(m_ui->DplayedPushButton->getColors()));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColorChoiced, MusicUtils::String::writeColorConfig(m_ui->DnoPlayedPushButton->getColors()));


    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDirChoiced, m_ui->downloadDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced, m_ui->downloadLrcDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimitChoiced, m_ui->downloadCacheAutoRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSizeChoiced, m_ui->downloadSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced, m_ui->downloadFullRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced, m_ui->downloadServerComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimitChoiced, m_ui->downloadLimitSpeedComboBox->currentText());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimitChoiced, m_ui->uploadLimitSpeedComboBox->currentText());


    QmmpSettings *qmmpSettings = QmmpSettings::instance();
    int i = m_ui->replayGainModeComboBox->currentIndex();
    qmmpSettings->setReplayGainSettings(MStatic_cast(QmmpSettings::ReplayGainMode, m_ui->replayGainModeComboBox->itemData(i).toInt()),
                                        m_ui->preampSpinBox->value(),
                                        m_ui->defaultGainSpinBox->value(),
                                        m_ui->clippingCheckBox->isChecked());
    i = m_ui->bitDepthComboBox->currentIndex();
    qmmpSettings->setAudioSettings(m_ui->softVolumeCheckBox->isChecked(),
                                   MStatic_cast(Qmmp::AudioFormat, m_ui->bitDepthComboBox->itemData(i).toInt()),
                                   m_ui->ditheringCheckBox->isChecked());
    qmmpSettings->setBufferSize(m_ui->bufferSizeSpinBox->value());
    qmmpSettings->setVolumeStep(m_ui->volumeStepSpinBox->value());


    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValueChoiced, m_ui->fadeInSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValueChoiced, m_ui->fadeOutSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnableChoiced, m_ui->fadeInAndOutCheckBox->isChecked());

    if(!applyNetworkProxy())
    {
        return;
    }

    emit parameterSettingChanged();
    close();
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
    int index = value / SCROLL_ITEM_HEIGHT;
    if(index < 4)
    {
        selectFunctionTableIndex(0, index);
    }
    else if(index < 6)
    {
        selectFunctionTableIndex(1, index - 4);
    }
    else if(index < 9)
    {
        selectFunctionTableIndex(2, index - 6);
    }
}

void MusicSettingWidget::selectFunctionTableIndex(int row, int col)
{
    clearFunctionTableSelection();
    switch(row)
    {
        case 0:
            m_ui->normalFunTableWidget->selectRow(col); break;
        case 1:
            m_ui->lrcFunTableWidget->selectRow(col); break;
        case 2:
            m_ui->supperFunTableWidget->selectRow(col); break;
        default: break;
    }
}

void MusicSettingWidget::initScrollWidgetPage()
{
    //
    initNormalSettingWidget();
    initDownloadWidget();
    initOtherSettingWidget();
    initDesktopLrcWidget();
    initInlineLrcWidget();
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
    //
    m_ui->scrollAreaWidgetArea->setFixedHeight(9 * SCROLL_ITEM_HEIGHT);
    m_ui->scrollAreaWidget->raise();
    selectFunctionTableIndex(0, 0);
    //
}

void MusicSettingWidget::initNormalSettingWidget()
{
    m_ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->minimumRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->quitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->quitOpacityRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->quitWindowRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->setDefaultPlayerCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->closeNetWorkCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    QButtonGroup *b1 = new QButtonGroup(this);
    b1->addButton(m_ui->minimumRadioBox, 0);
    b1->addButton(m_ui->quitRadioBox, 1);

    QButtonGroup *b2 = new QButtonGroup(this);
    b2->addButton(m_ui->quitOpacityRadioBox, 0);
    b2->addButton(m_ui->quitWindowRadioBox, 1);

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

    m_ui->languageComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->languageComboBox));
    m_ui->languageComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->languageComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->languageComboBox->addItems(QStringList() << tr("0") << tr("1") << tr("2"));

    m_ui->globalHotkeyBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->globalHotkeyBox->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->globalHotkeyBox, SIGNAL(clicked(bool)), SLOT(globalHotkeyBoxChanged(bool)));
}

void MusicSettingWidget::initOtherSettingWidget()
{
    m_ui->otherNormalImageRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->otherHeighImageRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->otherCheckUpdateBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherSearchCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherUseAlbumCoverCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherUseInfoCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherWriteAlbumCoverCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherWriteInfoCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherSideByCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->otherLrcKTVCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->ripplesSpectrumEnableBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->ripplesSpectrumColorButton->setText(tr("Effect"));
    m_ui->ripplesSpectrumOpacitySlider->setStyleSheet(MusicUIObject::MSliderStyle06);
    connect(m_ui->ripplesSpectrumColorButton, SIGNAL(clicked()), SLOT(ripplesSpectrumColorChanged()));
    connect(m_ui->ripplesSpectrumOpacitySlider, SIGNAL(valueChanged(int)), SLOT(ripplesSpectrumOpacityChanged(int)));
    connect(m_ui->ripplesSpectrumEnableBox, SIGNAL(clicked(bool)), SLOT(ripplesSpectrumOpacityEnableClicked(bool)));

    m_ui->otherVersionUpdateButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->otherVersionUpdateButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->otherVersionUpdateButton, SIGNAL(clicked()), SLOT(otherVersionUpdateChanged()));
#ifdef Q_OS_UNIX
    m_ui->otherNormalImageRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherHeighImageRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherCheckUpdateBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherSearchCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherUseAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherUseInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteAlbumCoverCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherWriteInfoCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherSideByCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherLrcKTVCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->ripplesSpectrumEnableBox->setFocusPolicy(Qt::NoFocus);
    m_ui->otherVersionUpdateButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->otherNormalImageRadioBox->click();
}

void MusicSettingWidget::initDownloadWidget()
{
    m_ui->downloadDirEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->downloadLrcDirEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    m_ui->downloadDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->downloadLrcDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadCacheAutoRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->downloadCacheManRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->downloadFullRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->downloadLimitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_ui->downloadDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLrcDirButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheAutoRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadCacheManRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadFullRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadLimitRadioBox->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->downloadServerComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->downloadServerComboBox));
    m_ui->downloadServerComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->downloadServerComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->downloadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->downloadLimitSpeedComboBox));
    m_ui->downloadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->downloadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->uploadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->uploadLimitSpeedComboBox));
    m_ui->uploadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->uploadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->downloadSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->downloadSpinBox->setRange(1024, 5*1024);
    m_ui->downloadDirEdit->setText(MUSIC_DIR_FULL);
    m_ui->downloadLrcDirEdit->setText(LRC_DIR_FULL);
    QStringList downloadSpeed;
    downloadSpeed << "100" << "200" << "300" << "400" << "500" << "600"
                  << "700" << "800" << "900" << "1000" << "1100" << "1200";
    m_ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    m_ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_wangyiyun"), tr("wangyiMusic"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_qq"), tr("qqMusic"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_xiami"), tr("xiamiMusic"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_baidu"), tr("baiduMusic"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kuwo"), tr("kuwoMusic"));
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kugou"), tr("kugouMusic"));

    //
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->downloadCacheAutoRadioBox, 0);
    buttonGroup->addButton(m_ui->downloadCacheManRadioBox, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(downloadGroupCached(int)));

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(m_ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(m_ui->downloadLimitRadioBox, 1);
    connect(buttonGroup2, SIGNAL(buttonClicked(int)), SLOT(downloadGroupSpeedLimit(int)));

    QButtonGroup *buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(m_ui->downloadDirButton, 0);
    buttonGroup3->addButton(m_ui->downloadLrcDirButton, 1);
    connect(buttonGroup3, SIGNAL(buttonClicked(int)), SLOT(downloadDirSelected(int)));

    m_ui->downloadCacheAutoRadioBox->click();
    m_ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    m_ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->DSingleLineCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->DfontComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->DfontComboBox));
    m_ui->DfontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->DfontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->DfontSizeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->DfontSizeComboBox));
    m_ui->DfontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->DfontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->DfontTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->DfontTypeComboBox));
    m_ui->DfontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->DfontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->DfontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->DfontDefaultColorComboBox));
    m_ui->DfontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->DfontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->DfontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    m_ui->DfontSizeComboBox->addItems(MusicLrcDefines().getDesktopLrcSize());
    m_ui->DfontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    m_ui->DfontDefaultColorComboBox->addItems(QStringList() << tr("DWhite") << tr("DBlue") << tr("DRed") << tr("DBlack") << tr("DYellow") << tr("DPurple") << tr("DGreen"));

    connect(m_ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    m_ui->DtransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle06);
    m_ui->DnoPlayedPushButton->setText(tr("No"));
    m_ui->DplayedPushButton->setText(tr("Yes"));
    connect(m_ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBackgroundChanged()));
    connect(m_ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFrontgroundChanged()));
    connect(m_ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    m_ui->DresetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));
#ifdef Q_OS_UNIX
    m_ui->showDesktopCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DSingleLineCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->DresetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetDesktopParameter();
}

void MusicSettingWidget::initInlineLrcWidget()
{
    m_ui->showInlineCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->fontComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->fontComboBox));
    m_ui->fontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->fontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->fontSizeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->fontSizeComboBox));
    m_ui->fontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->fontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->fontTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->fontTypeComboBox));
    m_ui->fontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->fontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->fontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->fontDefaultColorComboBox));
    m_ui->fontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->fontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->fontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    m_ui->fontSizeComboBox->addItems(MusicLrcDefines().getInlineLrcSize());
    m_ui->fontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    m_ui->fontDefaultColorComboBox->addItems(QStringList() << tr("IYellow") << tr("IBlue") << tr("IGray") << tr("IPink") << tr("IGreen") << tr("IRed") << tr("IPurple") << tr("IOrange") << tr("IIndigo"));

    connect(m_ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    m_ui->transparentSlider->setStyleSheet(MusicUIObject::MSliderStyle06);
    m_ui->noPlayedPushButton->setText(tr("No"));
    m_ui->playedPushButton->setText(tr("Yes"));
    connect(m_ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(inlineLrcBackgroundChanged()));
    connect(m_ui->playedPushButton, SIGNAL(clicked()), SLOT(inlineLrcFrontgroundChanged()));
    connect(m_ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(inlineLrcTransChanged(int)));

    m_ui->resetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInlineParameter()));
#ifdef Q_OS_UNIX
    m_ui->showInlineCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->resetPushButton->setFocusPolicy(Qt::NoFocus);
#endif

    resetInlineParameter();
}

void MusicSettingWidget::initSoundEffectWidget()
{
    m_ui->outputTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->outputTypeComboBox));
    m_ui->outputTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->outputTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    foreach(const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        m_ui->outputTypeComboBox->addItem(info.deviceName());
    }

    m_ui->fadeInAndOutCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->fadeInAndOutCheckBox->setEnabled(false);

    m_ui->fadeInSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->fadeInSpinBox->setRange(1, 10*1000);
    m_ui->fadeInSpinBox->setValue(600);
    m_ui->fadeInSpinBox->setEnabled(false);

    m_ui->fadeOutSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->fadeOutSpinBox->setRange(1, 10*1000);
    m_ui->fadeOutSpinBox->setValue(600);
    m_ui->fadeOutSpinBox->setEnabled(false);

    m_ui->equalizerButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->equalizerPluginsButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->equalizerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->equalizerPluginsButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->fadeInAndOutCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerButton->setFocusPolicy(Qt::NoFocus);
    m_ui->equalizerPluginsButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->equalizerButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetEqualizer()));
    connect(m_ui->equalizerPluginsButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetSoundEffect()));
    connect(m_ui->fadeInAndOutCheckBox, SIGNAL(clicked(bool)), SLOT(musicFadeInAndOutClicked(bool)));
}

void MusicSettingWidget::initAudioSettingWidget()
{
    m_ui->replayGainModeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->replayGainModeComboBox));
    m_ui->replayGainModeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->replayGainModeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->preampSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->defaultGainSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->volumeStepSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    m_ui->bufferSizeSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);

    m_ui->bitDepthComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->bitDepthComboBox));
    m_ui->bitDepthComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->bitDepthComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->clippingCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->softVolumeCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->ditheringCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->clippingCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->softVolumeCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->ditheringCheckBox->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->replayGainModeComboBox->addItem (tr("Track"), QmmpSettings::REPLAYGAIN_TRACK);
    m_ui->replayGainModeComboBox->addItem (tr("Album"), QmmpSettings::REPLAYGAIN_ALBUM);
    m_ui->replayGainModeComboBox->addItem (tr("Disabled"), QmmpSettings::REPLAYGAIN_DISABLED);
    m_ui->bitDepthComboBox->addItem("16", Qmmp::PCM_S16LE);
    m_ui->bitDepthComboBox->addItem("24", Qmmp::PCM_S24LE);
    m_ui->bitDepthComboBox->addItem("32", Qmmp::PCM_S32LE);

}

void MusicSettingWidget::initNetworkWidget()
{
    m_ui->proxyIpEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyPortEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyPwdEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyUsernameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyAreaEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    m_ui->proxyTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->proxyTypeComboBox));
    m_ui->proxyTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->proxyTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->proxyTypeTestButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->proxyTypeTestButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netConnectionTypeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->netConnectionTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netCheckTypeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->netCheckTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->proxyTypeTestButton->setFocusPolicy(Qt::NoFocus);
    m_ui->netConnectionTypeButton->setFocusPolicy(Qt::NoFocus);
    m_ui->netCheckTypeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->proxyTypeComboBox->addItems(QStringList() << tr("DefaultProxy") << tr("Socks5Proxy") << tr("NoProxy") << tr("HttpProxy") << tr("HttpCachingProxy") << tr("FtpCachingProxy"));

    connect(m_ui->proxyTypeTestButton, SIGNAL(clicked()), SLOT(testNetworkProxy()));
    connect(m_ui->netConnectionTypeButton, SIGNAL(clicked()), SLOT(testNetworkConnection()));
    connect(m_ui->netCheckTypeButton, SIGNAL(clicked()), SLOT(checkNetworkConnection()));
    connect(m_ui->proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setNetworkProxyControl(int)));
    setNetworkProxyControl(2);
    m_ui->proxyTypeComboBox->setCurrentIndex(2);
}

void MusicSettingWidget::lcrColorValue(Type key, const QString &type, QLabel *obj)
{
    key == Inline ? m_ui->fontDefaultColorComboBox->setCurrentIndex(-1) : m_ui->DfontDefaultColorComboBox->setCurrentIndex(-1);

    MusicLrcColorWidget getColor(this);
    if(type == "DLRCFRONTGROUNDGCOLOR") getColor.setColors(m_ui->DplayedPushButton->getColors());
    if(type == "DLRCBACKGROUNDCOLOR") getColor.setColors(m_ui->DnoPlayedPushButton->getColors());
    if(type == "LRCFRONTGROUNDGCOLOR") getColor.setColors(m_ui->playedPushButton->getColors());
    if(type == "LRCBACKGROUNDCOLOR") getColor.setColors(m_ui->noPlayedPushButton->getColors());

    if(getColor.exec())
    {
        const QList<QColor> &colors = getColor.getColors();
        MStatic_cast(MusicColorPreviewLabel*, obj)->setColors(colors);
    }
    key == Inline ? showInlineLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::lrcColorByDefault(Type key, int index)
{
    if(index == -1)
    {
        return;
    }

    if(key == Inline)
    {
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(MStatic_cast(MusicLrcColor::LrcColorType, index));
        m_ui->playedPushButton->setColors(cl.m_frontColor);
        m_ui->noPlayedPushButton->setColors(cl.m_backColor);
        showInlineLrcDemo();
    }
    else
    {
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(MStatic_cast(MusicLrcColor::LrcColorType, index + LRC_COLOR_OFFSET));
        m_ui->DplayedPushButton->setColors(cl.m_frontColor);
        m_ui->DnoPlayedPushButton->setColors(cl.m_backColor);
        showDesktopLrcDemo();
    }
}

void MusicSettingWidget::lrcTransparentValue(Type key, int value) const
{
    MusicPreviewLabel* label;
    if(key == Inline)
    {
        label = m_ui->showLabel;
        label->setTransparent(2.55*value);
        label->setLinearGradient(m_ui->playedPushButton->getColors(), m_ui->noPlayedPushButton->getColors());
    }
    else
    {
        label = m_ui->DshowLabel;
        label->setTransparent(2.55*value);
        label->setLinearGradient(m_ui->DplayedPushButton->getColors(), m_ui->DnoPlayedPushButton->getColors());
    }
    label->update();
}

bool MusicSettingWidget::applyNetworkProxy()
{
    if(m_ui->proxyTypeComboBox->currentIndex() != 2)
    {
        return setNetworkProxyByType(1);
    }
    return true;
}

bool MusicSettingWidget::setNetworkProxyByType(int type)
{
    MusicNetworkProxy proxy;
    connect(&proxy, SIGNAL(testProxyStateChanged(bool)), SLOT(testProxyStateChanged(bool)));
    proxy.setType(m_ui->proxyTypeComboBox->currentIndex());

    QString value = m_ui->proxyIpEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("proxy hostname is empty"));
        message.exec();
        return false;
    }
    proxy.setHostName(value);

    value = m_ui->proxyPortEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("proxy port is empty"));
        message.exec();
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
