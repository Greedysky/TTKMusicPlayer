#include "musicdesktopwallpaperwidget.h"
#include "ui_musicdesktopwallpaperwidget.h"
#include "musicdesktopwallpaperthread.h"
#include "musicuiobject.h"
#include "musicdatadownloadthread.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"
#include "musicregeditmanager.h"
#include "musicnumberdefine.h"
#include "musicutils.h"

#include <QFileDialog>
#include <QStyledItemDelegate>

MusicDesktopWallpaperWidget::MusicDesktopWallpaperWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicDesktopWallpaperWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    initWidgetStyle();
    initParameters();

    connect(ui->netRadioButton, SIGNAL(clicked()), SLOT(netRadioButtonPressed()));
    connect(ui->localRadioButton, SIGNAL(clicked()), SLOT(localRadioButtonPressed()));
    connect(ui->playRadioButton, SIGNAL(clicked()), SLOT(playRadioButtonPressed()));

    connect(ui->viewButton, SIGNAL(clicked()), SLOT(viewButtonPressed()));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonPressed()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stopButtonPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(cancelButtonPressed()));

    ui->localRadioButton->setChecked(true);
    localRadioButtonPressed();

    m_wallThread = new MusicDesktopWallpaperThread(this);
}

MusicDesktopWallpaperWidget::~MusicDesktopWallpaperWidget()
{
    QFile file(QString("%1%2").arg(TEMPORARY_DIR).arg(JPG_FILE));
    if(file.exists())
    {
        file.remove();
    }
    m_wallThread->stopAndQuitThread();
    delete m_wallThread;
    delete ui;
}

QString MusicDesktopWallpaperWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDesktopWallpaperWidget::initWidgetStyle() const
{
    ui->urlLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->netRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->localRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->playRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->pictureEffect->setItemDelegate(new QStyledItemDelegate(ui->pictureEffect));
    ui->pictureEffect->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->pictureEffect->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->pictureFunc->setItemDelegate(new QStyledItemDelegate(ui->pictureFunc));
    ui->pictureFunc->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->pictureFunc->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->pictureType->setItemDelegate(new QStyledItemDelegate(ui->pictureType));
    ui->pictureType->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->pictureType->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeH->setItemDelegate(new QStyledItemDelegate(ui->timeH));
    ui->timeH->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->timeH->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeM->setItemDelegate(new QStyledItemDelegate(ui->timeM));
    ui->timeM->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->timeM->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->timeS->setItemDelegate(new QStyledItemDelegate(ui->timeS));
    ui->timeS->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
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
    for(int i=0; i<MT_D; ++i)
    {
        h << tr("%1H").arg(i);
    }
    for(int i=0; i<MT_H; ++i)
    {
        m << tr("%1M").arg(i);
        s << tr("%1S").arg(i);
    }
    ui->timeH->addItems(h);
    ui->timeM->addItems(m);
    ui->timeS->addItems(s);
}

void MusicDesktopWallpaperWidget::viewButtonPressed()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        ui->urlLineEdit->setText(path);
    }

    QStringList filters;
    filters << "*.bmp" << "*.jpg" <<"*.jpeg" << "*.png";
    foreach(QFileInfo file, MusicUtils::UCore::findFile(path, filters))
    {
        m_path << file.absoluteFilePath();
    }
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
            m_path << QString("%1%2").arg(TEMPORARY_DIR).arg(JPG_FILE);
            MusicDataDownloadThread *background = new MusicDataDownloadThread(ui->urlLineEdit->text().trimmed(),
                                                      m_path[0], MusicDownLoadThreadAbstract::Download_BigBG, this);
            connect(background, SIGNAL(downLoadDataChanged(QString)), SLOT(parameterFinished()));
            background->startToDownload();
            break;
        }
        case 1:
            parameterFinished(); break;
        case 2:
            m_path << "Play";
            parameterFinished(); break;
        default: break;
    }
}

void MusicDesktopWallpaperWidget::parameterFinished()
{
    MusicObject::MStriantMap para;
    para.insert("Mode", m_currentMode);
    para.insert("Path", m_path);
    para.insert("Effect", ui->pictureEffect->currentIndex());
    para.insert("Type", ui->pictureType->currentIndex());
    para.insert("Func", ui->pictureFunc->currentIndex());
    para.insert("Time", ui->timeH->currentIndex()*MT_H2S +
                        ui->timeM->currentIndex()*MT_M2S +
                        ui->timeS->currentIndex() );
    para.insert("Close", ui->recoveryWallpaper->isChecked());
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
#if defined Q_OS_WIN
    MusicRegeditManager().setDesktopWallAutoStart(autoStart);
#else
    Q_UNUSED(autoStart)
#endif
}

void MusicDesktopWallpaperWidget::show()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
