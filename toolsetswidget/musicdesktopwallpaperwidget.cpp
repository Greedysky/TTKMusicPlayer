#include "musicdesktopwallpaperwidget.h"
#include "ui_musicdesktopwallpaperwidget.h"
#include "musicdesktopwallpaperthread.h"
#include "musicuiobject.h"
#include "musicdatadownloadthread.h"
#include "musicbgthememanager.h"
#include "musicmessagebox.h"

#include <QFileDialog>
#include <QSettings>

MusicDesktopWallpaperWidget::MusicDesktopWallpaperWidget(QWidget *parent) :
    MusicAbstractMoveWidget(parent),
    ui(new Ui::MusicDesktopWallpaperWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    initWidgetStyle();
    initParameters();

    connect(ui->netRadioButton,SIGNAL(clicked()),SLOT(netRadioButtonPressed()));
    connect(ui->localRadioButton,SIGNAL(clicked()),SLOT(localRadioButtonPressed()));
    connect(ui->playRadioButton,SIGNAL(clicked()),SLOT(playRadioButtonPressed()));

    connect(ui->viewButton,SIGNAL(clicked()),SLOT(viewButtonPressed()));
    connect(ui->confirmButton,SIGNAL(clicked()),SLOT(confirmButtonPressed()));
    connect(ui->stopButton,SIGNAL(clicked()),SLOT(stopButtonPressed()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(cancelButtonPressed()));

    ui->localRadioButton->setChecked(true);
    localRadioButtonPressed();

    m_wallThread = new MusicDesktopWallpaperThread(this);
}

MusicDesktopWallpaperWidget::~MusicDesktopWallpaperWidget()
{
    QFile file(QString("%1%2").arg(TMP_DOWNLOAD).arg(JPG_FILE));
    if(file.exists())
    {
        file.remove();
    }
    m_wallThread->stopAndQuitThread();
    delete m_wallThread;
    delete ui;
}

void MusicDesktopWallpaperWidget::initWidgetStyle() const
{
    ui->urlLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->netRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->localRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->playRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->pictureEffect->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->pictureEffect->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->pictureFunc->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->pictureFunc->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->pictureType->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->pictureType->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeH->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->timeH->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeM->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->timeM->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeS->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->timeS->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->openWithstart->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->recoveryWallpaper->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->viewButton->setCursor(QCursor(Qt::PointingHandCursor));
}

void MusicDesktopWallpaperWidget::initParameters() const
{
    ui->pictureEffect->addItems(QStringList()<<tr("none"));
    ui->pictureType->addItems(QStringList()<<tr("center")<<tr("tile")<<tr("drawing"));
    ui->pictureFunc->addItems(QStringList()<<tr("order")<<tr("random"));
    QStringList h,m,s;
    for(int i=0; i<24; ++i)
    {
        h<<tr("%1H").arg(i);
    }
    for(int i=0; i<60; ++i)
    {
        m<<tr("%1M").arg(i);
        s<<tr("%1S").arg(i);
    }
    ui->timeH->addItems(h);
    ui->timeM->addItems(m);
    ui->timeS->addItems(s);
}


void MusicDesktopWallpaperWidget::findFiles(const QString &path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "png" << "bmp" << "jpg";
    foreach(QFileInfo fileInfo, list)
    {
        if( filters.contains(fileInfo.suffix()) )
        {
            m_path<<fileInfo.absoluteFilePath();
        }
    }
}

void MusicDesktopWallpaperWidget::viewButtonPressed()
{
    QString path =  QFileDialog::getExistingDirectory(this, "" , "./");
    if(!path.isEmpty())
    {
        ui->urlLineEdit->setText(path);
    }

    findFiles(path);
}

void MusicDesktopWallpaperWidget::netRadioButtonPressed()
{
    ui->urlLineEdit->setEnabled(true);
    ui->viewButton->setEnabled(false);
    m_currentMode = 0;
    m_path.clear();
}

void MusicDesktopWallpaperWidget::localRadioButtonPressed()
{
    ui->urlLineEdit->setEnabled(false);
    ui->viewButton->setEnabled(true);
    m_currentMode = 1;
    m_path.clear();
}

void MusicDesktopWallpaperWidget::playRadioButtonPressed()
{
    ui->urlLineEdit->setEnabled(false);
    ui->viewButton->setEnabled(false);
    m_currentMode = 2;
    m_path.clear();
    ui->urlLineEdit->setText(".");
}

void MusicDesktopWallpaperWidget::confirmButtonPressed()
{
    if(ui->urlLineEdit->text().trimmed().isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("url is now empty!"));
        message.exec();
        return;
    }
    switch(m_currentMode)
    {
        case 0:
         {
            m_path << QString("%1%2").arg(TMP_DOWNLOAD).arg(JPG_FILE);
            MusicDataDownloadThread *background = new MusicDataDownloadThread(ui->urlLineEdit->text().trimmed(),
                                                                              m_path[0], Download_BigBG, this);
            connect(background, SIGNAL(musicDownLoadFinished(QString)),SLOT(parameterFinished()));
            background->startToDownload();
            break;
         }
        case 1:
            parameterFinished();break;
        case 2:
            m_path << "Play";
            parameterFinished();break;
    }
}

void MusicDesktopWallpaperWidget::parameterFinished()
{
    MStriantMap para;
    para.insert("Mode", m_currentMode);
    para.insert("Path", m_path);
    para.insert("Effect",ui->pictureEffect->currentIndex());
    para.insert("Type",ui->pictureType->currentIndex());
    para.insert("Func",ui->pictureFunc->currentIndex());
    para.insert("Time",ui->timeH->currentIndex()*3600 +
                       ui->timeM->currentIndex()*60 +
                       ui->timeS->currentIndex() );
    para.insert("Close",ui->recoveryWallpaper->isChecked());
    m_wallThread->setParamters(para);
    m_wallThread->start();
    setAutoStart(ui->openWithstart->isChecked());
    close();
}

void MusicDesktopWallpaperWidget::stopButtonPressed()
{
    m_wallThread->stopAndQuitThread();
}

void MusicDesktopWallpaperWidget::cancelButtonPressed()
{
    close();
}

void MusicDesktopWallpaperWidget::setAutoStart(bool autoStart) const
{
    const QString REG_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QString applicationName = QApplication::applicationName();
    QSettings settings(REG_RUN, QSettings::NativeFormat);
    autoStart ? settings.setValue(applicationName, QApplication::applicationFilePath().replace("/", "\\"))
              : settings.remove(applicationName);
}

void MusicDesktopWallpaperWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
