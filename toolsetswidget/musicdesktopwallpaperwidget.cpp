#include "musicdesktopwallpaperwidget.h"
#include "ui_musicdesktopwallpaperwidget.h"
#include "musicdesktopwallpaperthread.h"
#include "musicuiobject.h"
#include "musicdatadownloadthread.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MusicDesktopWallpaperWidget::MusicDesktopWallpaperWidget(QWidget *parent) :
    MusicMoveWidgetAbstract(parent),
    ui(new Ui::MusicDesktopWallpaperWidget)
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

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
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
    m_wallThread->stopAndQuitThread();
    delete m_wallThread;
    delete ui;
}

void MusicDesktopWallpaperWidget::initWidgetStyle()
{
    ui->urlLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->netRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->localRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->playRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    ui->pictureEffect->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->pictureFunc->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->pictureType->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->timeH->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->timeM->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->timeS->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->openWithstart->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->recoveryWallpaper->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->viewButton->setCursor(QCursor(Qt::PointingHandCursor));
}

void MusicDesktopWallpaperWidget::initParameters()
{
    ui->pictureEffect->addItems(QStringList()<<tr("none"));
    ui->pictureType->addItems(QStringList()<<tr("center")<<tr("tile")<<tr("drawing"));
    ui->pictureFunc->addItems(QStringList()<<tr("order")<<tr("random"));
    QStringList h,m,s;
    for(int i=0; i<24; ++i)
        h<<tr("%1H").arg(i);
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
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    if(list.count() == 0) return;
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." | fileInfo.fileName() == ".." )
        {
            ++i;
            continue;
        }
        if(!fileInfo.isDir())
        {
            if( fileInfo.suffix() == "png" || fileInfo.suffix() == "bmp" ||
                fileInfo.suffix() == "jpg" )
            m_path<<fileInfo.absoluteFilePath();
        }
        ++i;
    }while(i < list.size());
}

void MusicDesktopWallpaperWidget::viewButtonPressed()
{
    QString path =  QFileDialog::getExistingDirectory(this, "" , "./");
    if(!path.isEmpty())
        ui->urlLineEdit->setText(path);

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
        QMessageBox::information(this,tr("QMusicPlayer"),tr("url is now empty!"));
        return;
    }
    switch(m_currentMode)
    {
        case 0:
         {
            m_path << QString("./tmp%1").arg(JPG_FILE);
            MusicDataDownloadThread *song = new MusicDataDownloadThread(ui->urlLineEdit->text().trimmed(),
                                                                        m_path[0], this);
            connect(song, SIGNAL(musicDownLoadFinished(QString)),SLOT(parameterFinished()));
            song->startToDownload();
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

void MusicDesktopWallpaperWidget::setAutoStart(bool autoStart)
{
    const QString REG_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QString applicationName = QApplication::applicationName();
    QSettings settings(REG_RUN, QSettings::NativeFormat);
    autoStart ? settings.setValue(applicationName, QApplication::applicationFilePath().replace("/", "\\"))
              : settings.remove(applicationName);
}
