#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicnetworkthread.h"
#include "musicstringutils.h"
#include "musicnetworkproxy.h"
#include "musicnetworkoperator.h"
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
      ui(new Ui::MusicSettingWidget)
{
    ui->setupUi(this);

    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    ui->normalFunTableWidget->setRowCount(3);
    ui->normalFunTableWidget->addFunctionItems(0,
        QStringList() << ":/contextMenu/btn_setting" << ":/contextMenu/btn_keyboard" << ":/contextMenu/btn_download",
        QStringList() << tr("Normal") << tr("Hotkey") << tr("Dwonload"));
    ui->lrcFunTableWidget->setRowCount(2);
    ui->lrcFunTableWidget->addFunctionItems(ui->normalFunTableWidget->rowCount(),
        QStringList() << ":/contextMenu/btn_lrc" << ":/contextMenu/btn_desktopLrc",
        QStringList() << tr("Inline") << tr("Desktop"));
    ui->supperFunTableWidget->setRowCount(2);
    ui->supperFunTableWidget->addFunctionItems(ui->normalFunTableWidget->rowCount() + ui->lrcFunTableWidget->rowCount(),
        QStringList() << ":/contextMenu/btn_equalizer" << ":/contextMenu/btn_network",
        QStringList() << tr("Equalizer") << tr("NetWork"));
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->supperFunTableWidget, SIGNAL(currentIndexChanged(int)), SLOT(clearFunctionTableSelection()));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

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
    delete ui;
}

QString MusicSettingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSettingWidget::initNormalSettingWidget()
{
    ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->minimumRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->quitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->setDefaultPlayerCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->closeNetWorkCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ui->languageComboBox->setItemDelegate(new QStyledItemDelegate(ui->languageComboBox));
    ui->languageComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->languageComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->languageComboBox->addItems(QStringList() << tr("0") << tr("1") << tr("2"));

    ui->globalHotkeyBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(ui->globalHotkeyBox, SIGNAL(clicked(bool)), SLOT(globalHotkeyBoxChanged(bool)));
}

void MusicSettingWidget::initInlineLrcWidget()
{
    ui->showInlineCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->fontComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontComboBox));
    ui->fontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontSizeComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontSizeComboBox));
    ui->fontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontTypeComboBox));
    ui->fontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontDefaultColorComboBox));
    ui->fontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->fontSizeComboBox->addItems(QStringList() << tr("smaller") << tr("small")
                                     << tr("middle") << tr("big") << tr("bigger"));
    ui->fontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    ui->fontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                                << tr("yellow") << tr("green") << tr("blue") << tr("indigo")
                                                << tr("purple") << tr("white") << tr("black"));
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    ui->transparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->noPlayedPushButton->setText(tr("No"));
    ui->playedPushButton->setText(tr("Yes"));
    connect(ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(inlineLrcBgChanged()));
    connect(ui->playedPushButton, SIGNAL(clicked()), SLOT(inlineLrcFgChanged()));
    connect(ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(inlineLrcTransChanged(int)));

    ui->resetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInlineParameter()));

    resetInlineParameter();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->DfontComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontComboBox));
    ui->DfontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontSizeComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontSizeComboBox));
    ui->DfontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontTypeComboBox));
    ui->DfontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontDefaultColorComboBox));
    ui->DfontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->DfontSizeComboBox->addItems(QStringList() << tr("smaller3") << tr("smaller2") << tr("smaller") << tr("small3")
                                   << tr("small2") << tr("small") << tr("middle") << tr("big") << tr("big2") << tr("big3")
                                   << tr("bigger") << tr("bigger2") << tr("bigger3"));
    ui->DfontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    ui->DfontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                   << tr("yellow") << tr("green") << tr("blue") << tr("indigo") << tr("purple")
                                   << tr("white") << tr("black"));
    connect(ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo()));
    connect(ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    ui->DtransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->DnoPlayedPushButton->setText(tr("No"));
    ui->DplayedPushButton->setText(tr("Yes"));
    connect(ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBgChanged()));
    connect(ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFgChanged()));
    connect(ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    ui->DresetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));

    resetDesktopParameter();
}

void MusicSettingWidget::initDownloadWidget()
{
    ui->downloadDirEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->downloadLrcDirEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    ui->downloadDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->downloadLrcDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadCacheAutoRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadCacheManRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadFullRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadLimitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);

    ui->downloadServerComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadServerComboBox));
    ui->downloadServerComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->downloadServerComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->downloadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadLimitSpeedComboBox));
    ui->downloadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->downloadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->uploadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(ui->uploadLimitSpeedComboBox));
    ui->uploadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->uploadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->downloadSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    ui->downloadSpinBox->setRange(1024, 5*1024);
    ui->downloadDirEdit->setText(MUSIC_DIR_FULL);
    ui->downloadLrcDirEdit->setText(LRC_DIR_FULL);
    QStringList downloadSpeed;
    downloadSpeed << "100" << "200" << "300" << "400" << "500" << "600"
                  << "700" << "800" << "900" << "1000" << "1100" << "1200";
    ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    ui->downloadServerMultiple->setChecked(false);
    ui->downloadServerMultiple->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->downloadServerMultipleVip->setChecked(false);
    ui->downloadServerMultipleVip->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    QButtonGroup *serverButtonGroup = new QButtonGroup(this);
    serverButtonGroup->setExclusive(false);
    serverButtonGroup->addButton(ui->downloadServerMultiple, 0);
    serverButtonGroup->addButton(ui->downloadServerMultipleVip, 1);
    connect(serverButtonGroup, SIGNAL(buttonClicked(int)), SLOT(downloadGroupServer(int)));

    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_wangyiyun"), tr("wangyiMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_aiyinyue"), tr("dianxinMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_qq"), tr("qqMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_xiami"), tr("xiamiMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_ttdongting"), tr("ttpodMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_baidu"), tr("baiduMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kuwo"), tr("kuwoMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_kugou"), tr("kuhouMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_duomi"), tr("duomiMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_migu"), tr("miguMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_dingdong"), tr("dingdongMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_echo"), tr("echoMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/lb_yinyuetai"), tr("yinyuetaiMusic"));

    /////////////////////////////////////////////////////////////
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->downloadCacheAutoRadioBox, 0);
    buttonGroup->addButton(ui->downloadCacheManRadioBox, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(downloadGroupCached(int)));

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(ui->downloadLimitRadioBox, 1);
    connect(buttonGroup2, SIGNAL(buttonClicked(int)), SLOT(downloadGroupSpeedLimit(int)));

    QButtonGroup *buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(ui->downloadDirButton, 0);
    buttonGroup3->addButton(ui->downloadLrcDirButton, 1);
    connect(buttonGroup3, SIGNAL(buttonClicked(int)), SLOT(downloadDirSelected(int)));

    ui->downloadCacheAutoRadioBox->click();
    ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initSoundEffectWidget()
{
    ui->outputTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadServerComboBox));
    ui->outputTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->outputTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    foreach(const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        ui->outputTypeComboBox->addItem(info.deviceName());
    }

    ui->fadeInAndOutCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ui->fadeInSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    ui->fadeInSpinBox->setRange(1, 10*1000);
    ui->fadeInSpinBox->setValue(600);
    ui->fadeInSpinBox->setEnabled(false);

    ui->fadeOutSpinBox->setStyleSheet(MusicUIObject::MSpinBoxStyle01);
    ui->fadeOutSpinBox->setRange(1, 10*1000);
    ui->fadeOutSpinBox->setValue(600);
    ui->fadeOutSpinBox->setEnabled(false);

    ui->equalizerButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->equalizerPluginsButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->equalizerButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->equalizerPluginsButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->equalizerButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetEqualizer()));
    connect(ui->equalizerPluginsButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicSetSoundEffect()));
    connect(ui->fadeInAndOutCheckBox, SIGNAL(clicked(bool)), SLOT(musicFadeInAndOutClicked(bool)));
}

void MusicSettingWidget::initNetworkWidget()
{
    ui->proxyIpEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->proxyPortEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->proxyPwdEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->proxyUsernameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    ui->proxyTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadServerComboBox));
    ui->proxyTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->proxyTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->proxyTypeTestButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->proxyTypeTestButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->netConnectionTypeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->netConnectionTypeButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->proxyTypeComboBox->addItems(QStringList() << tr("DefaultProxy") << tr("Socks5Proxy") <<
                 tr("NoProxy") << tr("HttpProxy") << tr("HttpCachingProxy") << tr("FtpCachingProxy"));

    connect(ui->proxyTypeTestButton, SIGNAL(clicked()), SLOT(testNetworkProxy()));
    connect(ui->netConnectionTypeButton, SIGNAL(clicked()), SLOT(testNetworkConnection()));
    connect(ui->proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setNetworkProxyControl(int)));
    setNetworkProxyControl(2);
    ui->proxyTypeComboBox->setCurrentIndex(2);
}

void MusicSettingWidget::initControllerParameter()
{
    //Set init parameter
    ui->autoPlayCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toBool());
    ui->backPlayCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList().first().toInt());
    if(!M_SETTING_PTR->value(MusicSettingManager::CloseEventChoiced).toBool())
    {
        ui->minimumRadioBox->setChecked(true);
    }
    else
    {
        ui->quitRadioBox->setChecked(true);
    }
    ui->languageComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt());
    ///////////////////////////////////////////////////////////////////////////
    globalHotkeyBoxChanged(ui->globalHotkeyBox->isChecked());
    ////////////////////////////////////////////////
    //Set init parameter
    ui->showInlineCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    ui->showDesktopCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    ui->showInlineCheckBox->setEnabled(false);

    ui->fontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcFamilyChoiced).toInt());
    ui->fontSizeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt() - 13);
    ui->fontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt());
    ui->fontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
    {
        ui->fontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt());
    }
    else
    {
        m_lrcSelectedFg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).toString());
        m_lrcSelectedBg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::LrcBgColorChoiced).toString());
        ui->playedPushButton->setLinearGradient(m_lrcSelectedFg);
        ui->noPlayedPushButton->setLinearGradient(m_lrcSelectedBg);
        showInlineLrcDemo();
    }
    ui->transparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt());
    ////////////////////////////////////////////////
    ui->DfontComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    ui->DfontSizeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt() - 24);
    ui->DfontTypeComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt());
    ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
    {
        ui->DfontDefaultColorComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt());
    }
    else
    {
        m_DlrcSelectedFg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcFgColorChoiced).toString());
        m_DlrcSelectedBg = MusicUtils::String::readColorConfig(M_SETTING_PTR->value(MusicSettingManager::DLrcBgColorChoiced).toString());
        ui->DplayedPushButton->setLinearGradient(m_DlrcSelectedFg);
        ui->DnoPlayedPushButton->setLinearGradient(m_DlrcSelectedBg);
        showDesktopLrcDemo();
    }
    ui->DtransparentSlider->setValue(M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt());
    ////////////////////////////////////////////////
    ui->downloadDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    ui->downloadLrcDirEdit->setText(M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString());
    ui->downloadSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt());
    M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt() == 1 ?
                     ui->downloadCacheAutoRadioBox->click() : ui->downloadCacheManRadioBox->click();

    MusicUtils::Widget::setComboboxText(ui->downloadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString());
    MusicUtils::Widget::setComboboxText(ui->uploadLimitSpeedComboBox, M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimitChoiced).toString());
    M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 1 ?
                     ui->downloadFullRadioBox->click() : ui->downloadLimitRadioBox->click();
    switch(M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt())
    {
        case 1: ui->downloadServerMultiple->setChecked(true);
                ui->downloadServerMultipleVip->setChecked(false);
                break;
        case 2: ui->downloadServerMultiple->setChecked(false);
                ui->downloadServerMultipleVip->setChecked(true);
                break;
        default: ui->downloadServerMultiple->setChecked(false);
                 ui->downloadServerMultipleVip->setChecked(false);
    }
    ///////////////////////////////////////////////////////////////////////////
    ui->fadeInSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValueChoiced).toInt());
    ui->fadeOutSpinBox->setValue(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValueChoiced).toInt());
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnableChoiced).toInt())
    {
        ui->fadeInAndOutCheckBox->click();
    }
    ///////////////////////////////////////////////////////////////////////////
    ui->downloadServerComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt());
    ui->closeNetWorkCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
    ui->setDefaultPlayerCheckBox->setChecked(M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt());
    if(ui->setDefaultPlayerCheckBox->isChecked())
    {
        ui->setDefaultPlayerCheckBox->setEnabled(false);
    }
}

void MusicSettingWidget::globalHotkeyBoxChanged(bool state)
{
    ui->item_S2->setEnabled(state);
    ui->item_S4->setEnabled(state);
    ui->item_S6->setEnabled(state);
    ui->item_S8->setEnabled(state);
    ui->item_S10->setEnabled(state);
    ui->item_S12->setEnabled(state);
    ui->item_S14->setEnabled(state);
}

void MusicSettingWidget::changeInlineLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MusicSettingWidget::commitTheResults()
{
    M_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndexChoiced, ui->languageComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::AutoPlayChoiced, ui->autoPlayCheckBox->isChecked());
    QStringList list = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list[0] = QString::number(ui->backPlayCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, list);
    M_SETTING_PTR->setValue(MusicSettingManager::CloseEventChoiced, ui->quitRadioBox->isChecked());
    M_NETWORK_PTR->setBlockNetWork( ui->closeNetWorkCheckBox->isChecked() );
    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced, ui->setDefaultPlayerCheckBox->isChecked());
    if(ui->setDefaultPlayerCheckBox->isChecked())
    {
        ui->setDefaultPlayerCheckBox->setEnabled(false);
    }

    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced, ui->showInlineCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced, ui->fontDefaultColorComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFamilyChoiced, ui->fontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, ui->fontSizeComboBox->currentIndex() + 13);
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced, ui->fontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransChoiced, ui->transparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFgColorChoiced, MusicUtils::String::writeColorConfig(m_lrcSelectedFg));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcBgColorChoiced, MusicUtils::String::writeColorConfig(m_lrcSelectedBg));

    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, ui->showDesktopCheckBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorChoiced, ui->DfontDefaultColorComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFamilyChoiced, ui->DfontComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSizeChoiced, ui->DfontSizeComboBox->currentIndex() + 24);
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcTypeChoiced, ui->DfontTypeComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransChoiced, ui->DtransparentSlider->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFgColorChoiced, MusicUtils::String::writeColorConfig(m_DlrcSelectedFg));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcBgColorChoiced, MusicUtils::String::writeColorConfig(m_DlrcSelectedBg));

    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDirChoiced, ui->downloadDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced, ui->downloadLrcDirEdit->text());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimitChoiced, ui->downloadCacheAutoRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSizeChoiced, ui->downloadSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced, ui->downloadFullRadioBox->isChecked());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced, ui->downloadServerComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimitChoiced, ui->downloadLimitSpeedComboBox->currentText());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimitChoiced, ui->uploadLimitSpeedComboBox->currentText());
    int value = 0;
    if(ui->downloadServerMultiple->isChecked()) value = 1;
    else if(ui->downloadServerMultipleVip->isChecked()) value = 2;
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerMultipleChoiced, value);

    M_HOTKEY_PTR->setHotKey(0, ui->item_S2->text());
    M_HOTKEY_PTR->setHotKey(1, ui->item_S4->text());
    M_HOTKEY_PTR->setHotKey(2, ui->item_S6->text());
    M_HOTKEY_PTR->setHotKey(3, ui->item_S8->text());
    M_HOTKEY_PTR->setHotKey(4, ui->item_S10->text());
    M_HOTKEY_PTR->setHotKey(5, ui->item_S12->text());
    M_HOTKEY_PTR->setHotKey(6, ui->item_S14->text());
    M_HOTKEY_PTR->enabledAll(ui->globalHotkeyBox->isChecked());

    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValueChoiced, ui->fadeInSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValueChoiced, ui->fadeOutSpinBox->value());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnableChoiced, ui->fadeInAndOutCheckBox->isChecked());

    if(!applyNetworkProxy())
    {
        return;
    }

    emit parameterSettingChanged();
    close();
}

void MusicSettingWidget::inlineLrcFgChanged()
{
    lcrColorValue(Inline, "LRCFGCOLORCHOICED", ui->playedPushButton);
}

void MusicSettingWidget::desktopFgChanged()
{
    lcrColorValue(Desktop, "DLRCFGCOLORCHOICED", ui->DplayedPushButton);
}

void MusicSettingWidget::inlineLrcBgChanged()
{
    lcrColorValue(Inline, "LRCBGCOLORCHOICED", ui->noPlayedPushButton);
}

void MusicSettingWidget::desktopBgChanged()
{
    lcrColorValue(Desktop, "DLRCBGCOLORCHOICED", ui->DnoPlayedPushButton);
}

void MusicSettingWidget::lcrColorValue(Type key, const QString &type, QLabel *obj)
{
    key == Inline ? ui->fontDefaultColorComboBox->setCurrentIndex(-1)
                  : ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
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

        ui->playedPushButton->setLinearGradient(m_lrcSelectedFg);
        ui->noPlayedPushButton->setLinearGradient(m_lrcSelectedBg);

        showInlineLrcDemo();
    }
    else
    {
        m_DlrcSelectedFg.clear();
        m_DlrcSelectedBg.clear();
        m_DlrcSelectedFg << QColor(222, 54, 4) << QColor(255, 255, 255) << QColor(222, 54, 4);
        m_DlrcSelectedBg << color << QColor(255, 255, 255) << color;

        ui->DplayedPushButton->setLinearGradient(m_DlrcSelectedFg);
        ui->DnoPlayedPushButton->setLinearGradient(m_DlrcSelectedBg);

        showDesktopLrcDemo();
    }
}

void MusicSettingWidget::lrcTransparentValue(Type key, int value) const
{
    MusicPreviewLabel* label;
    if(key == Inline)
    {
        label = ui->showLabel;
        label->setTransparent(2.55*value);
        label->setLinearGradient(m_lrcSelectedBg, m_lrcSelectedFg);
    }
    else
    {
        label = ui->DshowLabel;
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
    para << ui->fontComboBox->currentText()
         << QString::number(ui->fontSizeComboBox->currentIndex())
         << QString::number(ui->fontTypeComboBox->currentIndex());

    ui->showLabel->setParameter(para);
    ui->showLabel->setLinearGradient(m_lrcSelectedBg, m_lrcSelectedFg);
    ui->showLabel->update();
}

void MusicSettingWidget::showDesktopLrcDemo()
{
    QStringList para;
    para << ui->DfontComboBox->currentText()
         << QString::number(ui->DfontSizeComboBox->currentIndex())
         << QString::number(ui->DfontTypeComboBox->currentIndex());

    ui->DshowLabel->setParameter(para);
    ui->DshowLabel->setLinearGradient(m_DlrcSelectedBg, m_DlrcSelectedFg);
    ui->DshowLabel->update();
}

void MusicSettingWidget::resetInlineParameter()
{
    ui->fontComboBox->setCurrentIndex(0);
    ui->fontSizeComboBox->setCurrentIndex(0);
    ui->fontTypeComboBox->setCurrentIndex(0);
    ui->fontDefaultColorComboBox->setCurrentIndex(0);
    ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::resetDesktopParameter()
{
    ui->DfontComboBox->setCurrentIndex(0);
    ui->DfontSizeComboBox->setCurrentIndex(0);
    ui->DfontTypeComboBox->setCurrentIndex(0);
    ui->DfontDefaultColorComboBox->setCurrentIndex(0);
    ui->DtransparentSlider->setValue(100);
}

int MusicSettingWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSettingWidget::clearFunctionTableSelection()
{
    ui->normalFunTableWidget->clearSelection();
    ui->lrcFunTableWidget->clearSelection();
    ui->supperFunTableWidget->clearSelection();
}

void MusicSettingWidget::downloadGroupServer(int index)
{
    if(index == 0)
    {
        ui->downloadServerMultipleVip->setChecked(false);
    }
    else
    {
        ui->downloadServerMultiple->setChecked(false);
    }
}

void MusicSettingWidget::downloadGroupCached(int index)
{
    ui->downloadSpinBox->setEnabled(index);
}

void MusicSettingWidget::downloadGroupSpeedLimit(int index)
{
    ui->downloadLimitSpeedComboBox->setEnabled(index);
    ui->uploadLimitSpeedComboBox->setEnabled(index);
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
            index == 0 ? ui->downloadDirEdit->setText(path + "/") : ui->downloadLrcDirEdit->setText(path + "/");
        }
    }
}

void MusicSettingWidget::setNetworkProxyControl(int enable)
{
    ui->proxyTypeTestButton->setEnabled(enable != 2);
    ui->proxyIpEdit->setEnabled(enable != 2);
    ui->proxyPortEdit->setEnabled(enable != 2);
    ui->proxyUsernameEdit->setEnabled(enable != 2);
    ui->proxyPwdEdit->setEnabled(enable != 2);
}

bool MusicSettingWidget::setNetworkProxyByType(int type)
{
    MusicNetworkProxy proxy;
    connect(&proxy, SIGNAL(testProxyStateChanged(bool)), SLOT(testProxyStateChanged(bool)));
    proxy.setType(ui->proxyTypeComboBox->currentIndex());
    QString value = ui->proxyIpEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("proxy hostname is empty"));
        message.exec();
        return false;
    }
    proxy.setHostName(value);
    value = ui->proxyPortEdit->text().trimmed();
    if(value.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("proxy port is empty"));
        message.exec();
        return false;
    }
    proxy.setPort(value.toInt());

    proxy.setUser(ui->proxyUsernameEdit->text().trimmed());
    proxy.setPassword(ui->proxyPwdEdit->text().trimmed());

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
    if(ui->proxyTypeComboBox->currentIndex() != 2)
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

void MusicSettingWidget::testNetworkConnectionStateChanged(const QString &name)
{
    M_LOGGER_INFO(name);
    ui->netConnectionTypeValue->setText(!name.isEmpty() ? name : tr("Unknown"));
    ui->netConnectionWayValue->setText(!name.isEmpty() ? "UDP" : tr("Unknown"));
}

void MusicSettingWidget::musicFadeInAndOutClicked(bool state)
{
    ui->fadeInSpinBox->setEnabled(state);
    ui->fadeOutSpinBox->setEnabled(state);
}
