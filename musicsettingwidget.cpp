#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include <QFontDatabase>
#include <QColorDialog>
#include <QApplication>
#include <QTranslator>

MusicSettingWidget::MusicSettingWidget(QWidget *parent) :
    MusicMoveDialogAbstract(parent),
    ui(new Ui::MusicSettingWidget)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius

    ////////////////////////////////////////////////
    m_leftButtonPress = false;
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    ////////////////////////////////////////////////
    ui->normalSet->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->hotKeySet->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->inlineLrc->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->desktopLrc->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->confirm->setStyleSheet(MusicObject::MusicSettingSubmitButton);
    ui->cancel->setStyleSheet(MusicObject::MusicSettingSubmitButton);
    ui->normalSet->setCursor(QCursor(Qt::PointingHandCursor));
    ui->hotKeySet->setCursor(QCursor(Qt::PointingHandCursor));
    ui->desktopLrc->setCursor(QCursor(Qt::PointingHandCursor));
    ui->inlineLrc->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirm->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancel->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->normalSet,SIGNAL(clicked()),this,SLOT(changeInfoWidget()));
    connect(ui->hotKeySet,SIGNAL(clicked()),this,SLOT(changeShotKeyWidget()));
    connect(ui->desktopLrc,SIGNAL(clicked()),this,SLOT(changeDesktopLrcWidget()));
    connect(ui->inlineLrc,SIGNAL(clicked()),this,SLOT(changeInlineLrcWidget()));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(commitTheResults()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(close()));

    ////////////////////////////////////////////////
    ui->autoPlayShowInfoWidget->setStyleSheet(MusicObject::MusicSettingCheckButton);
    ui->backPlayShowInfoWidget->setStyleSheet(MusicObject::MusicSettingCheckButton);
    ui->minimumShowInfoWidget->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->quitShowInfoWidget->setStyleSheet(MusicObject::MusicSettingRadioButton);

    ui->languageComboBox->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->languageComboBox->addItems(QStringList()<<tr("0")<<tr("1")<<tr("2"));
    ////////////////////////////////////////////////
    ui->showInlineSongWordWidget->setStyleSheet(MusicObject::MusicSettingCheckButton);
    ui->showDesktopSongWordWidget->setStyleSheet(MusicObject::MusicSettingCheckButton);

    ////////////////////////////////////////////////
    initInlineLrcWidget();
    ////////////////////////////////////////////////
    initDesktopLrcWidget();
    ////////////////////////////////////////////////
}

void MusicSettingWidget::initInlineLrcWidget()
{
    ui->fontComboBox->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontSizeComboBox->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontTypeComboBox->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontDefalutColorComboBox->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
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

    ui->transparentSlider->setStyleSheet(MusicObject::MusicVolumeStyleHorizontal);
    ui->noPlayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->noPlayedPushButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->noPlayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playedPushButton->setIcon(QIcon(":/color/purple"));
    ui->playedPushButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->playedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->noPlayedPushButton,SIGNAL(clicked()),SLOT(inlineLrcBgChanged()));
    connect(ui->playedPushButton,SIGNAL(clicked()),SLOT(inlineLrcFgChanged()));
    connect(ui->transparentSlider,SIGNAL(valueChanged(int)),SLOT(inlineLrcTransChanged(int)));

    ui->resetPushButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->resetPushButton,SIGNAL(clicked()),SLOT(resetInlineParameter()));

    resetInlineParameter();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    ui->fontComboBoxD->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontSizeComboBoxD->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontTypeComboBoxD->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontDefalutColorComboBoxD->setStyleSheet(MusicObject::MusicLrcComboboxEditStyle);
    ui->fontComboBoxD->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->fontSizeComboBoxD->addItems(QStringList()<<tr("smaller3")<<tr("smaller2")<<tr("smaller")<<tr("small3")
                                   <<tr("small2")<<tr("small")<<tr("middle")<<tr("big")<<tr("big2")<<tr("big3")
                                   <<tr("bigger")<<tr("bigger2")<<tr("bigger3"));
    ui->fontTypeComboBoxD->addItems(QStringList()<<"1"<<"2"<<"3"<<"4");
    ui->fontDefalutColorComboBoxD->addItems(QStringList()<<tr("origin")<<tr("red")<<tr("orange")
       <<tr("yellow")<<tr("green")<<tr("blue")<<tr("indigo")<<tr("purple")<<tr("white")<<tr("black"));
    connect(ui->fontComboBoxD,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->fontSizeComboBoxD,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->fontTypeComboBoxD,SIGNAL(currentIndexChanged(int)),SLOT(showDesktopLrcDemo(int)));
    connect(ui->fontDefalutColorComboBoxD,SIGNAL(currentIndexChanged(int)),SLOT(defaultDesktopLrcColorChanged(int)));

    ui->transparentSliderD->setStyleSheet(MusicObject::MusicVolumeStyleHorizontal);
    ui->noPlayedPushButtonD->setIcon(QIcon(":/color/purple"));
    ui->noPlayedPushButtonD->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->noPlayedPushButtonD->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playedPushButtonD->setIcon(QIcon(":/color/purple"));
    ui->playedPushButtonD->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->playedPushButtonD->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->noPlayedPushButtonD,SIGNAL(clicked()),SLOT(desktopBgChanged()));
    connect(ui->playedPushButtonD,SIGNAL(clicked()),SLOT(desktopFgChanged()));
    connect(ui->transparentSliderD,SIGNAL(valueChanged(int)),SLOT(desktopLrcTransChanged(int)));

    ui->resetPushButtonD->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->resetPushButtonD->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->resetPushButtonD,SIGNAL(clicked()),SLOT(resetDesktopParameter()));

    resetDesktopParameter();
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete ui;
}

void MusicSettingWidget::initControllerParameter()
{
    //Set init parameter
    ui->autoPlayShowInfoWidget->setChecked(M_SETTING.value(MusicSettingManager::AutoPlayChoiced).toBool());
    ui->backPlayShowInfoWidget->setChecked(M_SETTING.value(MusicSettingManager::LastPlayIndexChoiced)
               .toStringList().at(0).toInt() );
    if(!M_SETTING.value(MusicSettingManager::CloseEventChoiced).toBool())
      ui->minimumShowInfoWidget->setChecked(true);
    else
      ui->quitShowInfoWidget->setChecked(true);

    ui->languageComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::CurrentLanIndexChoiced).toInt());

    //Set init parameter
    ui->showInlineSongWordWidget->setChecked(M_SETTING.value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    ui->showDesktopSongWordWidget->setChecked(M_SETTING.value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    ui->showInlineSongWordWidget->setEnabled(false);

    ui->fontComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::LrcFamilyChoiced).toInt());
    ui->fontSizeComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::LrcSizeChoiced).toInt() - 13);
    ui->fontTypeComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::LrcTypeChoiced).toInt());
    if(M_SETTING.value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
        ui->fontDefalutColorComboBox->setCurrentIndex(M_SETTING.value(MusicSettingManager::LrcColorChoiced).toInt());
    else
    {
        ui->fontDefalutColorComboBox->setCurrentIndex(-1);
        QPixmap pixmap(16,16);
        pixmap.fill(m_lrcSelectedFg = M_SETTING.value(MusicSettingManager::LrcFgColorChoiced).value<QColor>());
        ui->playedPushButton->setIcon(QIcon(pixmap));
        pixmap.fill(m_lrcSelectedBg = M_SETTING.value(MusicSettingManager::LrcBgColorChoiced).value<QColor>());
        ui->noPlayedPushButton->setIcon(QIcon(pixmap));
        ui->showLabel->setLinearGradient(m_lrcSelectedFg,m_lrcSelectedBg);
        ui->showLabel->update();
    }
    ui->transparentSlider->setValue(M_SETTING.value(MusicSettingManager::LrcColorTransChoiced).toInt());

    ui->fontComboBoxD->setCurrentIndex(M_SETTING.value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    ui->fontSizeComboBoxD->setCurrentIndex(M_SETTING.value(MusicSettingManager::DLrcSizeChoiced).toInt() - 24);
    ui->fontTypeComboBoxD->setCurrentIndex(M_SETTING.value(MusicSettingManager::DLrcTypeChoiced).toInt());
    if(M_SETTING.value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
        ui->fontDefalutColorComboBoxD->setCurrentIndex(M_SETTING.value(MusicSettingManager::DLrcColorChoiced).toInt());
    else
    {
        ui->fontDefalutColorComboBoxD->setCurrentIndex(-1);
        QPixmap pixmap(16,16);
        pixmap.fill(m_DlrcSelectedFg = M_SETTING.value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>());
        ui->playedPushButtonD->setIcon(QIcon(pixmap));
        pixmap.fill(m_DlrcSelectedBg = M_SETTING.value(MusicSettingManager::DLrcBgColorchoiced).value<QColor>());
        ui->noPlayedPushButtonD->setIcon(QIcon(pixmap));
        ui->showLabelD->setLinearGradient(m_DlrcSelectedFg,m_DlrcSelectedBg);
        ui->showLabelD->update();
    }
    ui->transparentSliderD->setValue(M_SETTING.value(MusicSettingManager::DLrcColorTransChoiced).toInt());

}

void MusicSettingWidget::changeInfoWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicSettingWidget::changeShotKeyWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
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
    M_SETTING.setValue(MusicSettingManager::CurrentLanIndexChoiced, ui->languageComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::AutoPlayChoiced,ui->autoPlayShowInfoWidget->isChecked());
    QStringList list = M_SETTING.value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list.replace(0,QString::number(ui->backPlayShowInfoWidget->isChecked()));
    M_SETTING.setValue(MusicSettingManager::LastPlayIndexChoiced,list);
    M_SETTING.setValue(MusicSettingManager::CloseEventChoiced,ui->quitShowInfoWidget->isChecked() ? "true" : "false");

    M_SETTING.setValue(MusicSettingManager::ShowInlineLrcChoiced,ui->showInlineSongWordWidget->isChecked());
    M_SETTING.setValue(MusicSettingManager::ShowDesktopLrcChoiced,ui->showDesktopSongWordWidget->isChecked());

    M_SETTING.setValue(MusicSettingManager::LrcColorChoiced,ui->fontDefalutColorComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::LrcFamilyChoiced,ui->fontComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::LrcSizeChoiced,ui->fontSizeComboBox->currentIndex() + 13);
    M_SETTING.setValue(MusicSettingManager::LrcTypeChoiced,ui->fontTypeComboBox->currentIndex());
    M_SETTING.setValue(MusicSettingManager::LrcColorTransChoiced,ui->transparentSlider->value());

    M_SETTING.setValue(MusicSettingManager::DLrcColorChoiced,ui->fontDefalutColorComboBoxD->currentIndex());
    M_SETTING.setValue(MusicSettingManager::DLrcFamilyChoiced,ui->fontComboBoxD->currentIndex());
    M_SETTING.setValue(MusicSettingManager::DLrcSizeChoiced,ui->fontSizeComboBoxD->currentIndex() + 24);
    M_SETTING.setValue(MusicSettingManager::DLrcTypeChoiced,ui->fontTypeComboBoxD->currentIndex());
    M_SETTING.setValue(MusicSettingManager::DLrcColorTransChoiced,ui->transparentSliderD->value());

    M_SETTING.setValue(MusicSettingManager::DLrcFgColorChoiced, m_DlrcSelectedFg);
    M_SETTING.setValue(MusicSettingManager::DLrcBgColorchoiced, m_DlrcSelectedBg);
    M_SETTING.setValue(MusicSettingManager::LrcFgColorChoiced, m_lrcSelectedFg);
    M_SETTING.setValue(MusicSettingManager::LrcBgColorChoiced, m_lrcSelectedBg);

    emit parameterSettingChanged();

    this->close();
}

void MusicSettingWidget::inlineLrcFgChanged()
{
    lcrColorValue(Inline,"LRCFGCOLORCHOICED",ui->playedPushButton);
}

void MusicSettingWidget::desktopFgChanged()
{
    lcrColorValue(Desktop,"DLRCFGCOLORCHOICED",ui->playedPushButtonD);
}

void MusicSettingWidget::inlineLrcBgChanged()
{
    lcrColorValue(Inline,"LRCBGCOLORCHOICED",ui->noPlayedPushButton);
}

void MusicSettingWidget::desktopBgChanged()
{
    lcrColorValue(Desktop,"DLRCBGCOLORCHOICED",ui->noPlayedPushButtonD);
}

void MusicSettingWidget::lcrColorValue(Type key, QString type, QPushButton *obj)
{
    key == Inline ? ui->fontDefalutColorComboBox->setCurrentIndex(-1)
                  : ui->fontDefalutColorComboBoxD->setCurrentIndex(-1);
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
        return;
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
                  : ui->noPlayedPushButtonD->setIcon(QIcon(pixmap));
    pixmap.fill(QColor(222,54,4));
    key == Inline ? ui->playedPushButton->setIcon(QIcon(pixmap))
                  : ui->playedPushButtonD->setIcon(QIcon(pixmap));

    key == Inline ? m_lrcSelectedFg = QColor(222,54,4) : m_DlrcSelectedFg = QColor(222,54,4);
    key == Inline ? m_lrcSelectedBg = color : m_DlrcSelectedBg = color;
    key == Inline ? showInlineLrcDemo(-1) : showDesktopLrcDemo(-1);
}

void MusicSettingWidget::lrcTransparentValue(Type key, int index)
{
    MusicPreviewLabel* label;
    QColor fcolor = key == Inline ? m_lrcSelectedFg : m_DlrcSelectedFg;
    QColor bcolor = key == Inline ? m_lrcSelectedBg : m_DlrcSelectedBg;
    fcolor.setAlpha(2.55*index);
    bcolor.setAlpha(2.55*index);
    key == Inline ? label = ui->showLabel : label = ui->showLabelD;
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
    para<<ui->fontComboBoxD->currentText()
        <<QString::number(ui->fontSizeComboBoxD->currentIndex())
        <<QString::number(ui->fontTypeComboBoxD->currentIndex());

    ui->showLabelD->setLinearGradient(m_DlrcSelectedFg,m_DlrcSelectedBg);
    ui->showLabelD->setParameter(para);
    ui->showLabelD->update();
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
    ui->fontComboBoxD->setCurrentIndex(0);
    ui->fontSizeComboBoxD->setCurrentIndex(0);
    ui->fontTypeComboBoxD->setCurrentIndex(0);
    ui->fontDefalutColorComboBoxD->setCurrentIndex(0);
    ui->transparentSliderD->setValue(100);
}
