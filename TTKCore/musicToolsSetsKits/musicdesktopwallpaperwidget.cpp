#include "musicdesktopwallpaperwidget.h"
#include "ui_musicdesktopwallpaperwidget.h"
#include "musicdesktopwallpaperthread.h"
#include "musicuiobject.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicregeditmanager.h"
#include "musicnumberdefine.h"
#include "musiccoreutils.h"

#include <QFileDialog>
#include <QStyledItemDelegate>

MusicDesktopWallpaperWidget::MusicDesktopWallpaperWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDesktopWallpaperWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    initWidgetStyle();
    initParameters();

    connect(m_ui->netRadioButton, SIGNAL(clicked()), SLOT(netRadioButtonPressed()));
    connect(m_ui->localRadioButton, SIGNAL(clicked()), SLOT(localRadioButtonPressed()));
    connect(m_ui->playRadioButton, SIGNAL(clicked()), SLOT(playRadioButtonPressed()));

    connect(m_ui->viewButton, SIGNAL(clicked()), SLOT(viewButtonPressed()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonPressed()));
    connect(m_ui->stopButton, SIGNAL(clicked()), SLOT(stopButtonPressed()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(cancelButtonPressed()));

    m_ui->localRadioButton->setChecked(true);
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
    delete m_ui;
}

QString MusicDesktopWallpaperWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDesktopWallpaperWidget::closeEvent(QCloseEvent *event)
{
    emit resetFlag(MusicObject::TT_Wallpaper);
    MusicAbstractMoveWidget::closeEvent(event);
}

void MusicDesktopWallpaperWidget::initWidgetStyle() const
{
    m_ui->urlLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->netRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->localRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->playRadioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->pictureEffect->setItemDelegate(new QStyledItemDelegate(m_ui->pictureEffect));
    m_ui->pictureEffect->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->pictureEffect->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->pictureFunc->setItemDelegate(new QStyledItemDelegate(m_ui->pictureFunc));
    m_ui->pictureFunc->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->pictureFunc->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->pictureType->setItemDelegate(new QStyledItemDelegate(m_ui->pictureType));
    m_ui->pictureType->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->pictureType->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->timeH->setItemDelegate(new QStyledItemDelegate(m_ui->timeH));
    m_ui->timeH->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeH->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->timeM->setItemDelegate(new QStyledItemDelegate(m_ui->timeM));
    m_ui->timeM->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeM->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->timeS->setItemDelegate(new QStyledItemDelegate(m_ui->timeS));
    m_ui->timeS->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeS->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->openWithstart->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->recoveryWallpaper->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->viewButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->netRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->localRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->viewButton->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->stopButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openWithstart->setFocusPolicy(Qt::NoFocus);
    m_ui->recoveryWallpaper->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicDesktopWallpaperWidget::initParameters() const
{
    m_ui->pictureEffect->addItems(QStringList()<<tr("none"));
    m_ui->pictureType->addItems(QStringList()<<tr("center")<<tr("tile")<<tr("drawing"));
    m_ui->pictureFunc->addItems(QStringList()<<tr("order")<<tr("random"));
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
    m_ui->timeH->addItems(h);
    m_ui->timeM->addItems(m);
    m_ui->timeS->addItems(s);
}

void MusicDesktopWallpaperWidget::viewButtonPressed()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        m_ui->urlLineEdit->setText(path);
    }

    QStringList filters;
    filters << "*.bmp" << "*.jpg" <<"*.jpeg" << "*.png";
    foreach(const QFileInfo &file, MusicUtils::Core::findFile(path, filters))
    {
        m_path << file.absoluteFilePath();
    }
}

void MusicDesktopWallpaperWidget::netRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(true);
    m_ui->viewButton->setEnabled(false);
    m_currentMode = 0;
    m_path.clear();
}

void MusicDesktopWallpaperWidget::localRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(false);
    m_ui->viewButton->setEnabled(true);
    m_currentMode = 1;
    m_path.clear();
}

void MusicDesktopWallpaperWidget::playRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(false);
    m_ui->viewButton->setEnabled(false);
    m_currentMode = 2;
    m_path.clear();
    m_ui->urlLineEdit->setText(".");
}

void MusicDesktopWallpaperWidget::confirmButtonPressed()
{
    if(m_ui->urlLineEdit->text().trimmed().isEmpty())
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
            MusicDataDownloadThread *background = new MusicDataDownloadThread(m_ui->urlLineEdit->text().trimmed(),
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
    para.insert("Effect", m_ui->pictureEffect->currentIndex());
    para.insert("Type", m_ui->pictureType->currentIndex());
    para.insert("Func", m_ui->pictureFunc->currentIndex());
    para.insert("Time", m_ui->timeH->currentIndex()*MT_H2S +
                        m_ui->timeM->currentIndex()*MT_M2S +
                        m_ui->timeS->currentIndex() );
    para.insert("Close", m_ui->recoveryWallpaper->isChecked());
    m_wallThread->setParamters(para);
    m_wallThread->start();
    setAutoStart(m_ui->openWithstart->isChecked());
    hide();
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
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
