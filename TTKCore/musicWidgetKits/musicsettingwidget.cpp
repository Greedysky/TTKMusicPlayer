#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicnetworkthread.h"
#include "musicstringutils.h"
#include "musicnetworkproxy.h"
#include "musicnetworkoperator.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musicmessagebox.h"
#include "musicglobalhotkey.h"
#include "musicapplicationobject.h"
#include "musiclrccolorwidget.h"

#include <QFontDatabase>
#include <QColorDialog>
#include <QFileDialog>
#include <QButtonGroup>
#include <QAudioDeviceInfo>
#include <QStyledItemDelegate>

MusicFunctionTableWidget::MusicFunctionTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 20);
    headerview->resizeSection(2, 85);

    setRowCount(3);
    m_listIndex = 0;
}

MusicFunctionTableWidget::~MusicFunctionTableWidget()
{

}

QString MusicFunctionTableWidget::getClassName()
{
    return staticMetaObject.className();
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

void MusicFunctionTableWidget::addFunctionItems(int index, const QStringList &icon, const QStringList &path)
{
    m_listIndex = index;
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *item = nullptr;
        setItem(i, 0, item = new QTableWidgetItem());

                      item = new QTableWidgetItem(QIcon(icon[i]), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);

                      item = new QTableWidgetItem(path[i]);
        item->setTextColor(QColor(80, 80, 80));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
}


MusicSettingWidget::MusicSettingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSettingWidget)
{
    m_ui->setupUi(this);

    ////////////////////////////////////////////////
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    m_ui->normalFunTableWidget->setRowCount(3);
    m_ui->normalFunTableWidget->addFunctionItems(0,
        QStringList() << ":/contextMenu/btn_setting" << ":/contextMenu/btn_keyboard" << ":/contextMenu/btn_download",
        QStringList() << tr("Normal") << tr("Hotkey") << tr("Dwonload"));
    m_ui->lrcFunTableWidget->setRowCount(2);
    m_ui->lrcFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount(),
        QStringList() << ":/contextMenu/btn_lrc" << ":/contextMenu/btn_desktopLrc",
        QStringList() << tr("Inline") << tr("Desktop"));
    m_ui->supperFunTableWidget->setRowCount(2);
    m_ui->supperFunTableWidget->addFunctionItems(m_ui->normalFunTableWidget->rowCount() + m_ui->lrcFunTableWidget->rowCount(),
        QStringList() << ":/contextMenu/btn_equalizer" << ":/contextMenu/btn_network",
        QStringList() << tr("Equalizer") << tr("NetWork"));
    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), m_ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(m_ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), m_ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(m_ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), m_ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(m_ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    initNormalSettingWidget();
    initInlineLrcWidget();
    initDesktopLrcWidget();
    initDownloadWidget();
    initSoundEffectWidget();
    initNetworkWidget();
    ////////////////////////////////////////////////
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete m_ui;
}

QString MusicSettingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSettingWidget::initNormalSettingWidget()
{
    m_ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->minimumRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->quitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->setDefaultPlayerCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->closeNetWorkCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->languageComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->languageComboBox));
    m_ui->languageComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->languageComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->languageComboBox->addItems(QStringList() << tr("0") << tr("1") << tr("2"));

    m_ui->globalHotkeyBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(m_ui->globalHotkeyBox, SIGNAL(clicked(bool)), SLOT(globalHotkeyBoxChanged(bool)));
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
    m_ui->fontSizeComboBox->addItems(QStringList() << tr("smaller") << tr("small")
                                     << tr("middle") << tr("big") << tr("bigger"));
    m_ui->fontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    m_ui->fontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                                << tr("yellow") << tr("green") << tr("blue") << tr("indigo")
                                                << tr("purple") << tr("white") << tr("black"));
    connect(m_ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(m_ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    m_ui->transparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->noPlayedPushButton->setText(tr("No"));
    m_ui->playedPushButton->setText(tr("Yes"));
    connect(m_ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(inlineLrcBgChanged()));
    connect(m_ui->playedPushButton, SIGNAL(clicked()), SLOT(inlineLrcFgChanged()));
    connect(m_ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(inlineLrcTransChanged(int)));

    m_ui->resetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInlineParameter()));

    resetInlineParameter();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    m_ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
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
    m_ui->DfontSizeComboBox->addItems(QStringList() << tr("smaller3") << tr("smaller2") << tr("smaller") << tr("small3")
                                   << tr("small2") << tr("small") << tr("middle") << tr("big") << tr("big2") << tr("big3")
                                   << tr("bigger") << tr("bigger2") << tr("bigger3"));
    m_ui->DfontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    m_ui->DfontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                   << tr("yellow") << tr("green") << tr("blue") << tr("indigo") << tr("purple")
                                   << tr("white") << tr("black"));
    connect(m_ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(m_ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    m_ui->DtransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->DnoPlayedPushButton->setText(tr("No"));
    m_ui->DplayedPushButton->setText(tr("Yes"));
    connect(m_ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBgChanged()));
    connect(m_ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFgChanged()));
    connect(m_ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    m_ui->DresetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));

    resetDesktopParameter();
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
    m_ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kugou"), tr("kuhouMusic"));

    /////////////////////////////////////////////////////////////
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

void MusicSettingWidget::initSoundEffectWidget()
{
    m_ui->outputTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->downloadServerComboBox));
    m_ui->outputTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->outputTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    foreach(const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        m_ui->outputTypeComboBox->addItem(info.deviceName());
    }

    m_ui->fadeInAndOutCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

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

    connect(m_ui->equalizerButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetEqualizer()));
    connect(m_ui->equalizerPluginsButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetSoundEffect()));
    connect(m_ui->fadeInAndOutCheckBox, SIGNAL(clicked(bool)), SLOT(musicFadeInAndOutClicked(bool)));
}

void MusicSettingWidget::initNetworkWidget()
{
    m_ui->proxyIpEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyPortEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyPwdEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->proxyUsernameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    m_ui->proxyTypeComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->downloadServerComboBox));
    m_ui->proxyTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->proxyTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->proxyTypeTestButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->proxyTypeTestButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netConnectionTypeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->netConnectionTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->netCheckTypeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->netCheckTypeButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->proxyTypeComboBox->addItems(QStringList() << tr("DefaultProxy") << tr("Socks5Proxy") <<
                 tr("NoProxy") << tr("HttpProxy") << tr("HttpCachingProxy") << tr("FtpCachingProxy"));

    connect(m_ui->proxyTypeTestButton, SIGNAL(clicked()), SLOT(testNetworkProxy()));
    connect(m_ui->netConnectionTypeButton, SIGNAL(clicked()), SLOT(testNetworkConnection()));
    connect(m_ui->netCheckTypeButton, SIGNAL(clicked()), SLOT(checkNetworkConnection()));
    connect(m_ui->proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setNetworkProxyControl(int)));
    setNetworkProxyControl(2);
    m_ui->proxyTypeComboBox->setCurrentIndex(2);
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
    m_ui->languageComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt());
    ///////////////////////////////////////////////////////////////////////////
    globalHotkeyBoxChanged(m_ui->globalHotkeyBox->isChecked());
    ////////////////////////////////////////////////
    //Set init parameter
    m_ui->showInlineCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    m_ui->showDesktopCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    m_ui->showInlineCheckBox->setEnabled(false);

    m_ui->fontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcFamilyChoiced).toInt());
    m_ui->fontSizeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt() - 13);
    m_ui->fontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt());
    m_ui->fontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
    {
        m_ui->fontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt());
    }
    else
    {
        m_lrcSelectedFg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).toString());
        m_lrcSelectedBg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcBgColorChoiced).toString());
        m_ui->playedPushButton->setLinearGradient(m_lrcSelectedFg);
        m_ui->noPlayedPushButton->setLinearGradient(m_lrcSelectedBg);
        showInlineLrcDemo();
    }
    m_ui->transparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt());
    ////////////////////////////////////////////////
    m_ui->DfontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    m_ui->DfontSizeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt() - 24);
    m_ui->DfontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt());
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
    {
        m_ui->DfontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt());
    }
    else
    {
        m_DlrcSelectedFg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcFgColorChoiced).toString());
        m_DlrcSelectedBg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcBgColorChoiced).toString());
        m_ui->DplayedPushButton->setLinearGradient(m_DlrcSelectedFg);
        m_ui->DnoPlayedPushButton->setLinearGradient(m_DlrcSelectedBg);
        showDesktopLrcDemo();
    }
    m_ui->DtransparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt());
    ////////////////////////////////////////////////
    m_ui->downloadDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    m_ui->downloadLrcDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString());
    m_ui->downloadSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt());
    M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt() == 1 ?
                     m_ui->downloadCacheAutoRadioBox->click() : m_ui->downloadCacheManRadioBox->click();

    MusicUtils::Widget::setComboboxText(m_ui->downloadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString());
    MusicUtils::Widget::setComboboxText(m_ui->uploadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimitChoiced).toString());
    M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 1 ?
                     m_ui->downloadFullRadioBox->click() : m_ui->downloadLimitRadioBox->click();
    ///////////////////////////////////////////////////////////////////////////
    m_ui->fadeInSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValueChoiced).toInt());
    m_ui->fadeOutSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValueChoiced).toInt());
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnableChoiced).toInt())
    {
        m_ui->fadeInAndOutCheckBox->click();
    }
    ///////////////////////////////////////////////////////////////////////////
    m_ui->downloadServerComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt());
    m_ui->closeNetWorkCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
    m_ui->setDefaultPlayerCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt());
    if(m_ui->setDefaultPlayerCheckBox->isChecked())
    {
        m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    }
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
}

void MusicSettingWidget::changeInlineLrcWidget()
{
    m_ui->stackedWidget->setCurrentIndex(4);
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    m_ui->stackedWidget->setCurrentIndex(3);
}

void MusicSettingWidget::commitTheResults()
{
    M_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndexChoiced, m_ui->languageComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::AutoPlayChoiced, m_ui->autoPlayCheckBox->isChecked());
    QStringList list = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list[0] = QString::number(m_ui->backPlayCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, list);
    M_SETTING_PTR->setValue(MusicSettingManager::CloseEventChoiced, m_ui->quitRadioBox->isChecked());
    M_NETWORK_PTR->setBlockNetWork( m_ui->closeNetWorkCheckBox->isChecked() );
    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced, m_ui->setDefaultPlayerCheckBox->isChecked());
    if(m_ui->setDefaultPlayerCheckBox->isChecked())
    {
        m_ui->setDefaultPlayerCheckBox->setEnabled(false);
    }

    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced, m_ui->showInlineCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced, m_ui->fontDefaultColorComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFamilyChoiced, m_ui->fontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, m_ui->fontSizeComboBox->currentIndex() + 13);
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced, m_ui->fontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransChoiced, m_ui->transparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFgColorChoiced, MusicUtils::String::writeColorConfig(m_lrcSelectedFg));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcBgColorChoiced, MusicUtils::String::writeColorConfig(m_lrcSelectedBg));

    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, m_ui->showDesktopCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorChoiced, m_ui->DfontDefaultColorComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFamilyChoiced, m_ui->DfontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSizeChoiced, m_ui->DfontSizeComboBox->currentIndex() + 24);
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcTypeChoiced, m_ui->DfontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransChoiced, m_ui->DtransparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFgColorChoiced, MusicUtils::String::writeColorConfig(m_DlrcSelectedFg));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcBgColorChoiced, MusicUtils::String::writeColorConfig(m_DlrcSelectedBg));

    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDirChoiced, m_ui->downloadDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced, m_ui->downloadLrcDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimitChoiced, m_ui->downloadCacheAutoRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSizeChoiced, m_ui->downloadSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced, m_ui->downloadFullRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced, m_ui->downloadServerComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimitChoiced, m_ui->downloadLimitSpeedComboBox->currentText());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimitChoiced, m_ui->uploadLimitSpeedComboBox->currentText());

    M_HOTKEY_PTR->setHotKey(0, m_ui->item_S2->text());
    M_HOTKEY_PTR->setHotKey(1, m_ui->item_S4->text());
    M_HOTKEY_PTR->setHotKey(2, m_ui->item_S6->text());
    M_HOTKEY_PTR->setHotKey(3, m_ui->item_S8->text());
    M_HOTKEY_PTR->setHotKey(4, m_ui->item_S10->text());
    M_HOTKEY_PTR->setHotKey(5, m_ui->item_S12->text());
    M_HOTKEY_PTR->setHotKey(6, m_ui->item_S14->text());
    M_HOTKEY_PTR->enabledAll(m_ui->globalHotkeyBox->isChecked());

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

void MusicSettingWidget::inlineLrcFgChanged()
{
    lcrColorValue(Inline, "LRCFGCOLORCHOICED", m_ui->playedPushButton);
}

void MusicSettingWidget::desktopFgChanged()
{
    lcrColorValue(Desktop, "DLRCFGCOLORCHOICED", m_ui->DplayedPushButton);
}

void MusicSettingWidget::inlineLrcBgChanged()
{
    lcrColorValue(Inline, "LRCBGCOLORCHOICED", m_ui->noPlayedPushButton);
}

void MusicSettingWidget::desktopBgChanged()
{
    lcrColorValue(Desktop, "DLRCBGCOLORCHOICED", m_ui->DnoPlayedPushButton);
}

void MusicSettingWidget::lcrColorValue(Type key, const QString &type, QLabel *obj)
{
    key == Inline ? m_ui->fontDefaultColorComboBox->setCurrentIndex(-1)
                  : m_ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    MusicLrcColorWidget getColor(this);
    if(type == "DLRCFGCOLORCHOICED") getColor.setColors(m_DlrcSelectedFg);
    if(type == "DLRCBGCOLORCHOICED") getColor.setColors(m_DlrcSelectedBg);
    if(type == "LRCFGCOLORCHOICED") getColor.setColors(m_lrcSelectedFg);
    if(type == "LRCBGCOLORCHOICED") getColor.setColors(m_lrcSelectedBg);

    if(getColor.exec())
    {
        QList<QColor> colors = getColor.getColors();
        MStatic_cast(MusicColorPreviewLabel*, obj)->setLinearGradient(colors);
        if(type == "DLRCFGCOLORCHOICED") m_DlrcSelectedFg = colors;
        if(type == "DLRCBGCOLORCHOICED") m_DlrcSelectedBg = colors;
        if(type == "LRCFGCOLORCHOICED") m_lrcSelectedFg = colors;
        if(type == "LRCBGCOLORCHOICED") m_lrcSelectedBg = colors;
    }
    key == Inline ? showInlineLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::defaultDesktopLrcColorChanged(int index)
{
    lrcColorByDefault(Desktop, index);
}

void MusicSettingWidget::defaultLrcColorChanged(int index)
{
    lrcColorByDefault(Inline, index);
}

void MusicSettingWidget::lrcColorByDefault(Type key, int index)
{
    if(index == -1)
    {
        return;
    }
    QColor color(Qt::black);
    switch(index)
    {
        case 0: color = QColor(14, 179, 255);break;
        case 1: color = QColor(214, 51, 44);break;
        case 2: color = QColor(230, 130, 52);break;
        case 3: color = QColor(243, 209, 0);break;
        case 4: color = QColor(62, 164, 140);break;
        case 5: color = QColor(76, 147, 193);break;
        case 6: color = QColor(29, 237, 235);break;
        case 7: color = QColor(156, 115, 155);break;
        case 8: color = QColor(255, 255, 255);break;
        case 9: color = QColor(0, 0, 0);break;
        default: break;
    }

    if(key == Inline)
    {
        m_lrcSelectedFg.clear();
        m_lrcSelectedBg.clear();
        m_lrcSelectedFg << QColor(222, 54, 4) << QColor(255, 255, 255) << QColor(222, 54, 4);
        m_lrcSelectedBg << color << QColor(255, 255, 255) << color;

        m_ui->playedPushButton->setLinearGradient(m_lrcSelectedFg);
        m_ui->noPlayedPushButton->setLinearGradient(m_lrcSelectedBg);

        showInlineLrcDemo();
    }
    else
    {
        m_DlrcSelectedFg.clear();
        m_DlrcSelectedBg.clear();
        m_DlrcSelectedFg << QColor(222, 54, 4) << QColor(255, 255, 255) << QColor(222, 54, 4);
        m_DlrcSelectedBg << color << QColor(255, 255, 255) << color;

        m_ui->DplayedPushButton->setLinearGradient(m_DlrcSelectedFg);
        m_ui->DnoPlayedPushButton->setLinearGradient(m_DlrcSelectedBg);

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
        label->setLinearGradient(m_lrcSelectedBg, m_lrcSelectedFg);
    }
    else
    {
        label = m_ui->DshowLabel;
        label->setTransparent(2.55*value);
        label->setLinearGradient(m_DlrcSelectedBg, m_DlrcSelectedFg);
    }
    label->update();
}

void MusicSettingWidget::inlineLrcTransChanged(int index)
{
    lrcTransparentValue(Inline, index);
}

void MusicSettingWidget::desktopLrcTransChanged(int index)
{
    lrcTransparentValue(Desktop, index);
}

void MusicSettingWidget::showInlineLrcDemo()
{
    QStringList para;
    para << m_ui->fontComboBox->currentText()
         << QString::number(m_ui->fontSizeComboBox->currentIndex())
         << QString::number(m_ui->fontTypeComboBox->currentIndex());

    m_ui->showLabel->setParameter(para);
    m_ui->showLabel->setLinearGradient(m_lrcSelectedBg, m_lrcSelectedFg);
    m_ui->showLabel->update();
}

void MusicSettingWidget::showDesktopLrcDemo()
{
    QStringList para;
    para << m_ui->DfontComboBox->currentText()
         << QString::number(m_ui->DfontSizeComboBox->currentIndex())
         << QString::number(m_ui->DfontTypeComboBox->currentIndex());

    m_ui->DshowLabel->setParameter(para);
    m_ui->DshowLabel->setLinearGradient(m_DlrcSelectedBg, m_DlrcSelectedFg);
    m_ui->DshowLabel->update();
}

void MusicSettingWidget::resetInlineParameter()
{
    m_ui->fontComboBox->setCurrentIndex(0);
    m_ui->fontSizeComboBox->setCurrentIndex(0);
    m_ui->fontTypeComboBox->setCurrentIndex(0);
    m_ui->fontDefaultColorComboBox->setCurrentIndex(0);
    m_ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::resetDesktopParameter()
{
    m_ui->DfontComboBox->setCurrentIndex(0);
    m_ui->DfontSizeComboBox->setCurrentIndex(0);
    m_ui->DfontTypeComboBox->setCurrentIndex(0);
    m_ui->DfontDefaultColorComboBox->setCurrentIndex(0);
    m_ui->DtransparentSlider->setValue(100);
}

int MusicSettingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSettingWidget::clearFunctionTableSelection()
{
    m_ui->normalFunTableWidget->clearSelection();
    m_ui->lrcFunTableWidget->clearSelection();
    m_ui->supperFunTableWidget->clearSelection();
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
        QString path;
        if(!(path = dialog.directory().absolutePath()).isEmpty())
        {
            index == 0 ? m_ui->downloadDirEdit->setText(path + "/") : m_ui->downloadLrcDirEdit->setText(path + "/");
        }
    }
}

void MusicSettingWidget::setNetworkProxyControl(int enable)
{
    m_ui->proxyTypeTestButton->setEnabled(enable != 2);
    m_ui->proxyIpEdit->setEnabled(enable != 2);
    m_ui->proxyPortEdit->setEnabled(enable != 2);
    m_ui->proxyUsernameEdit->setEnabled(enable != 2);
    m_ui->proxyPwdEdit->setEnabled(enable != 2);
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

void MusicSettingWidget::testNetworkProxy()
{
    setNetworkProxyByType(0);
}

bool MusicSettingWidget::applyNetworkProxy()
{
    if(m_ui->proxyTypeComboBox->currentIndex() != 2)
    {
        return setNetworkProxyByType(1);
    }
    return true;
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
    netOpr->startToOperator();
}

void MusicSettingWidget::checkNetworkConnection()
{
    MusicNetworkConnectionTestWidget *w = new MusicNetworkConnectionTestWidget(this);
    w->show();
}

void MusicSettingWidget::testNetworkConnectionStateChanged(const QString &name)
{
    M_LOGGER_INFO(name);
    m_ui->netConnectionTypeValue->setText(!name.isEmpty() ? name : tr("Unknown"));
    m_ui->netConnectionWayValue->setText(!name.isEmpty() ? "UDP" : tr("Unknown"));
}

void MusicSettingWidget::musicFadeInAndOutClicked(bool state)
{
    m_ui->fadeInSpinBox->setEnabled(state);
    m_ui->fadeOutSpinBox->setEnabled(state);
}
