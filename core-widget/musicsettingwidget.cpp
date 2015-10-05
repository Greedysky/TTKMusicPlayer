#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicbgthememanager.h"

#include <QFontDatabase>
#include <QColorDialog>
#include <QButtonGroup>

MusicSettingWidget::MusicSettingWidget(QWidget *parent) :
    MusicAbstractMoveDialog(parent),
    ui(new Ui::MusicSettingWidget)
{
    ui->setupUi(this);

    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    ////////////////////////////////////////////////
    ui->normalSetButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->hotKeySetButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadSetButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->inlineLrcButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->desktopLrcButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);

    ui->normalSetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->hotKeySetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadSetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->desktopLrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->inlineLrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->normalSetButton, 0);
    group->addButton(ui->hotKeySetButton, 1);
    group->addButton(ui->desktopLrcButton, 2);
    group->addButton(ui->inlineLrcButton, 3);
    group->addButton(ui->downloadSetButton, 4);
    connect(group, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->minimumRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->quitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);

    ui->languageComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->languageComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->languageComboBox->addItems(QStringList()<<tr("0")<<tr("1")<<tr("2"));
    ////////////////////////////////////////////////
    ui->showInlineCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ////////////////////////////////////////////////
    initInlineLrcWidget();
    initDesktopLrcWidget();
    initDownloadWidget();
    ////////////////////////////////////////////////
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete ui;
}

void MusicSettingWidget::initInlineLrcWidget()
{
    ui->fontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->fontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->fontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 );
    ui->fontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontDefalutColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->fontDefalutColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->fontSizeComboBox->addItems(QStringList()<<tr("smaller")<<tr("small")
                                   <<tr("middle")<<tr("big")<<tr("bigger"));
    ui->fontTypeComboBox->addItems(QStringList()<<"1"<<"2"<<"3"<<"4");
    ui->fontDefalutColorComboBox->addItems(QStringList()<<tr("origin")<<tr("red")<<tr("orange")
       <<tr("yellow")<<tr("green")<<tr("blue")<<tr("indigo")<<tr("purple")<<tr("white")<<tr("black"));
    connect(ui->fontComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showInlineLrcDemo(int)));
    connect(ui->fontSizeComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showInlineLrcDemo(int)));
    connect(ui->fontTypeComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showInlineLrcDemo(int)));
    connect(ui->fontDefalutColorComboBox,SIGNAL(currentIndexChanged(int)),SLOT(defaultLrcColorChanged(int)));

    ui->transparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->noPlayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->noPlayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->noPlayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playedPushButton->setIcon(QIcon(":/color/purple"));
    ui->playedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->playedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->noPlayedPushButton,SIGNAL(clicked()),SLOT(inlineLrcBgChanged()));
    connect(ui->playedPushButton,SIGNAL(clicked()),SLOT(inlineLrcFgChanged()));
    connect(ui->transparentSlider,SIGNAL(valueChanged(int)),SLOT(inlineLrcTransChanged(int)));

    ui->resetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->resetPushButton,SIGNAL(clicked()),SLOT(resetInlineParameter()));

    resetInlineParameter();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    ui->DfontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->DfontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->DfontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->DfontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontDefalutColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->DfontDefalutColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->DfontSizeComboBox->addItems(QStringList()<<tr("smaller3")<<tr("smaller2")<<tr("smaller")<<tr("small3")
                                   <<tr("small2")<<tr("small")<<tr("middle")<<tr("big")<<tr("big2")<<tr("big3")
                                   <<tr("bigger")<<tr("bigger2")<<tr("bigger3"));
    ui->DfontTypeComboBox->addItems(QStringList()<<"1"<<"2"<<"3"<<"4");
    ui->DfontDefalutColorComboBox->addItems(QStringList()<<tr("origin")<<tr("red")<<tr("orange")
       <<tr("yellow")<<tr("green")<<tr("blue")<<tr("indigo")<<tr("purple")<<tr("white")<<tr("black"));
    connect(ui->DfontComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontSizeComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontTypeComboBox,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontDefalutColorComboBox,SIGNAL(currentIndexChanged(int)),SLOT(defaultDesktopLrcColorChanged(int)));

    ui->DtransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->DnoPlayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->DnoPlayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DnoPlayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->DplayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->DplayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DplayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->DnoPlayedPushButton,SIGNAL(clicked()),SLOT(desktopBgChanged()));
    connect(ui->DplayedPushButton,SIGNAL(clicked()),SLOT(desktopFgChanged()));
    connect(ui->DtransparentSlider,SIGNAL(valueChanged(int)),SLOT(desktopLrcTransChanged(int)));

    ui->DresetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->DresetPushButton,SIGNAL(clicked()),SLOT(resetDesktopParameter()));

    resetDesktopParameter();
}

void MusicSettingWidget::initDownloadWidget()
{
    ui->downloadDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadLrcDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadCacheAutoRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadCacheManRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadFullRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadLimitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 );
    ui->downloadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->uploadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->uploadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->downloadSpinBox->setRange(1024, 5*1024);
    ui->downloadDirEdit->setText(MUSIC_DOWNLOAD);
    ui->downloadLrcDirEdit->setText(LRC_DOWNLOAD);
    QStringList downloadSpeed;
    downloadSpeed << "100" << "200" << "300" << "400" << "500" << "600"
                  << "700" << "800" << "900" << "1000" << "1100" << "1200";
    ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->downloadCacheAutoRadioBox, 0);
    buttonGroup->addButton(ui->downloadCacheManRadioBox, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(downloadGroupCached(int)));

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(ui->downloadLimitRadioBox, 1);
    connect(buttonGroup2, SIGNAL(buttonClicked(int)), SLOT(downloadGroupSpeedLimit(int)));

    ui->downloadCacheAutoRadioBox->click();
    ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initControllerParameter()
{
    //Set init parameter
    ui->autoPlayCheckBox->setChecked(M_SETTING->value(MusicSettingManager::AutoPlayChoiced).toBool());
    ui->backPlayCheckBox->setChecked(M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced)
               .toStringList().at(0).toInt() );
    if(!M_SETTING->value(MusicSettingManager::CloseEventChoiced).toBool())
    {
        ui->minimumRadioBox->setChecked(true);
    }
    else
    {
        ui->quitRadioBox->setChecked(true);
    }

    ui->languageComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::CurrentLanIndexChoiced).toInt());

    //Set init parameter
    ui->showInlineCheckBox->setChecked(M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    ui->showDesktopCheckBox->setChecked(M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    ui->showInlineCheckBox->setEnabled(false);

    ui->fontComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcFamilyChoiced).toInt());
    ui->fontSizeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcSizeChoiced).toInt() - 13);
    ui->fontTypeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcTypeChoiced).toInt());
    if(M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
    {
        ui->fontDefalutColorComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt());
    }
    else
    {
        ui->fontDefalutColorComboBox->setCurrentIndex(-1);
        QPixmap pixmap(16,16);
        pixmap.fill(m_lrcSelectedFg = M_SETTING->value(MusicSettingManager::LrcFgColorChoiced).value<QColor>());
        ui->playedPushButton->setIcon(QIcon(pixmap));
        pixmap.fill(m_lrcSelectedBg = M_SETTING->value(MusicSettingManager::LrcBgColorChoiced).value<QColor>());
        ui->noPlayedPushButton->setIcon(QIcon(pixmap));
        ui->showLabel->setLinearGradient(m_lrcSelectedFg,m_lrcSelectedBg);
        ui->showLabel->update();
    }
    ui->transparentSlider->setValue(M_SETTING->value(MusicSettingManager::LrcColorTransChoiced).toInt());

    ui->DfontComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    ui->DfontSizeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcSizeChoiced).toInt() - 24);
    ui->DfontTypeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcTypeChoiced).toInt());
    if(M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
    {
        ui->DfontDefalutColorComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt());
    }
    else
    {
        ui->DfontDefalutColorComboBox->setCurrentIndex(-1);
        QPixmap pixmap(16,16);
        pixmap.fill(m_DlrcSelectedFg = M_SETTING->value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>());
        ui->DplayedPushButton->setIcon(QIcon(pixmap));
        pixmap.fill(m_DlrcSelectedBg = M_SETTING->value(MusicSettingManager::DLrcBgColorChoiced).value<QColor>());
        ui->DnoPlayedPushButton->setIcon(QIcon(pixmap));
        ui->DshowLabel->setLinearGradient(m_DlrcSelectedFg,m_DlrcSelectedBg);
        ui->DshowLabel->update();
    }
    ui->DtransparentSlider->setValue(M_SETTING->value(MusicSettingManager::DLrcColorTransChoiced).toInt());

}

void MusicSettingWidget::changeInlineLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MusicSettingWidget::commitTheResults()
{
    M_SETTING->setValue(MusicSettingManager::CurrentLanIndexChoiced, ui->languageComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::AutoPlayChoiced,ui->autoPlayCheckBox->isChecked());
    QStringList list = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list[0] = QString::number(ui->backPlayCheckBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::LastPlayIndexChoiced, list);
    M_SETTING->setValue(MusicSettingManager::CloseEventChoiced,ui->quitRadioBox->isChecked() ? "true" : "false");

    M_SETTING->setValue(MusicSettingManager::ShowInlineLrcChoiced,ui->showInlineCheckBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced,ui->showDesktopCheckBox->isChecked());

    M_SETTING->setValue(MusicSettingManager::LrcColorChoiced,ui->fontDefalutColorComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcFamilyChoiced,ui->fontComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcSizeChoiced,ui->fontSizeComboBox->currentIndex() + 13);
    M_SETTING->setValue(MusicSettingManager::LrcTypeChoiced,ui->fontTypeComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcColorTransChoiced,ui->transparentSlider->value());

    M_SETTING->setValue(MusicSettingManager::DLrcColorChoiced,ui->DfontDefalutColorComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcFamilyChoiced,ui->DfontComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcSizeChoiced,ui->DfontSizeComboBox->currentIndex() + 24);
    M_SETTING->setValue(MusicSettingManager::DLrcTypeChoiced,ui->DfontTypeComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcColorTransChoiced,ui->DtransparentSlider->value());

    M_SETTING->setValue(MusicSettingManager::DLrcFgColorChoiced, m_DlrcSelectedFg);
    M_SETTING->setValue(MusicSettingManager::DLrcBgColorChoiced, m_DlrcSelectedBg);
    M_SETTING->setValue(MusicSettingManager::LrcFgColorChoiced, m_lrcSelectedFg);
    M_SETTING->setValue(MusicSettingManager::LrcBgColorChoiced, m_lrcSelectedBg);

    emit parameterSettingChanged();
    close();
}

void MusicSettingWidget::inlineLrcFgChanged()
{
    lcrColorValue(Inline,"LRCFGCOLORCHOICED",ui->playedPushButton);
}

void MusicSettingWidget::desktopFgChanged()
{
    lcrColorValue(Desktop,"DLRCFGCOLORCHOICED",ui->DplayedPushButton);
}

void MusicSettingWidget::inlineLrcBgChanged()
{
    lcrColorValue(Inline,"LRCBGCOLORCHOICED",ui->noPlayedPushButton);
}

void MusicSettingWidget::desktopBgChanged()
{
    lcrColorValue(Desktop,"DLRCBGCOLORCHOICED",ui->DnoPlayedPushButton);
}

void MusicSettingWidget::lcrColorValue(Type key, QString type, QPushButton *obj)
{
    key == Inline ? ui->fontDefalutColorComboBox->setCurrentIndex(-1)
                  : ui->DfontDefalutColorComboBox->setCurrentIndex(-1);
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QPixmap pixmap(16,16);
        pixmap.fill(color);
        obj->setIcon(QIcon(pixmap));
        if(type == "DLRCFGCOLORCHOICED") m_DlrcSelectedFg = color;
        if(type == "DLRCBGCOLORCHOICED") m_DlrcSelectedBg = color;
        if(type == "LRCFGCOLORCHOICED") m_lrcSelectedFg = color;
        if(type == "LRCBGCOLORCHOICED") m_lrcSelectedBg = color;
    }
    key == Inline ? showInlineLrcDemo(-1)
                  : showDesktopLrcDemo(-1);
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
        case 0: color = QColor(14,179,255);break;
        case 1: color = QColor(214,51,44);break;
        case 2: color = QColor(230,130,52);break;
        case 3: color = QColor(243,209,0);break;
        case 4: color = QColor(62,164,140);break;
        case 5: color = QColor(76,147,193);break;
        case 6: color = QColor(29,237,235);break;
        case 7: color = QColor(156,115,155);break;
        case 8: color = QColor(255,255,255);break;
        case 9: color = QColor(0,0,0);break;
        default: break;
    }

    QPixmap pixmap(16,16);
    pixmap.fill(color);
    key == Inline ? ui->noPlayedPushButton->setIcon(QIcon(pixmap))
                  : ui->DnoPlayedPushButton->setIcon(QIcon(pixmap));
    pixmap.fill(QColor(222,54,4));
    key == Inline ? ui->playedPushButton->setIcon(QIcon(pixmap))
                  : ui->DplayedPushButton->setIcon(QIcon(pixmap));

    key == Inline ? m_lrcSelectedFg = QColor(222,54,4) : m_DlrcSelectedFg = QColor(222,54,4);
    key == Inline ? m_lrcSelectedBg = color : m_DlrcSelectedBg = color;
    key == Inline ? showInlineLrcDemo(-1) : showDesktopLrcDemo(-1);
}

void MusicSettingWidget::lrcTransparentValue(Type key, int index) const
{
    MusicPreviewLabel* label;
    QColor fcolor = key == Inline ? m_lrcSelectedFg : m_DlrcSelectedFg;
    QColor bcolor = key == Inline ? m_lrcSelectedBg : m_DlrcSelectedBg;
    fcolor.setAlpha(2.55*index);
    bcolor.setAlpha(2.55*index);
    key == Inline ? label = ui->showLabel : label = ui->DshowLabel;
    label->setTransparent(2.55*index);
    label->setLinearGradient(fcolor,bcolor);
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

void MusicSettingWidget::showInlineLrcDemo(int)
{
    QStringList para;
    para<<ui->fontComboBox->currentText()
        <<QString::number(ui->fontSizeComboBox->currentIndex())
        <<QString::number(ui->fontTypeComboBox->currentIndex());

    ui->showLabel->setLinearGradient(m_lrcSelectedFg,m_lrcSelectedBg);
    ui->showLabel->setParameter(para);
    ui->showLabel->update();
}

void MusicSettingWidget::showDesktopLrcDemo(int)
{
    QStringList para;
    para<<ui->DfontComboBox->currentText()
        <<QString::number(ui->DfontSizeComboBox->currentIndex())
        <<QString::number(ui->DfontTypeComboBox->currentIndex());

    ui->DshowLabel->setLinearGradient(m_DlrcSelectedFg,m_DlrcSelectedBg);
    ui->DshowLabel->setParameter(para);
    ui->DshowLabel->update();
}

void MusicSettingWidget::resetInlineParameter()
{
    ui->fontComboBox->setCurrentIndex(0);
    ui->fontSizeComboBox->setCurrentIndex(0);
    ui->fontTypeComboBox->setCurrentIndex(0);
    ui->fontDefalutColorComboBox->setCurrentIndex(0);
    ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::resetDesktopParameter()
{
    ui->DfontComboBox->setCurrentIndex(0);
    ui->DfontSizeComboBox->setCurrentIndex(0);
    ui->DfontTypeComboBox->setCurrentIndex(0);
    ui->DfontDefalutColorComboBox->setCurrentIndex(0);
    ui->DtransparentSlider->setValue(100);
}

int MusicSettingWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
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
