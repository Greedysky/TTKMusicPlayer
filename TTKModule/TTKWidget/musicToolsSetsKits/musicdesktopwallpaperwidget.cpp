#include "musicdesktopwallpaperwidget.h"
#include "ui_musicdesktopwallpaperwidget.h"
#include "musicdesktopwallpaperthread.h"
#include "musicdatadownloadthread.h"
#include "musicbackgroundmanager.h"
#include "musictransitionanimationlabel.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicnumberdefine.h"
#include "musicwidgetheaders.h"
#include "musicfileutils.h"
#include "musicsinglemanager.h"

#include <QStyledItemDelegate>

MusicDesktopWallpaperItem::MusicDesktopWallpaperItem(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    setLayout(vBoxLayout);

    m_background = new MusicTransitionAnimationLabel(this);
    m_background->setScaledContents(true);
    vBoxLayout->addWidget(m_background);
}

MusicDesktopWallpaperItem::~MusicDesktopWallpaperItem()
{
    delete m_background;
}

void MusicDesktopWallpaperItem::updateBackground(const QPixmap &pix)
{
    m_background->setPixmap(pix);
}



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
    m_currentMode = 2;
    m_wallThread = new MusicDesktopWallpaperThread(this);
    m_wallItem = new MusicDesktopWallpaperItem;
    connect(m_wallThread, SIGNAL(updateBackground(QPixmap)), m_wallItem, SLOT(updateBackground(QPixmap)));
#ifdef Q_OS_WIN
    m_wallThread->sendMessageToDesktop();
    SetParent((HWND)m_wallItem->winId(), m_wallThread->findDesktopIconWnd());
#endif

    localRadioButtonPressed();
}

MusicDesktopWallpaperWidget::~MusicDesktopWallpaperWidget()
{
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());
    QFile file(QString("%1%2").arg(TEMPPATH).arg(JPG_FILE));
    if(file.exists())
    {
        file.remove();
    }

    delete m_wallThread;
    delete m_wallItem;
    delete m_ui;
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
    m_ui->timeH->setItemDelegate(new QStyledItemDelegate(m_ui->timeH));
    m_ui->timeH->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeH->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->timeM->setItemDelegate(new QStyledItemDelegate(m_ui->timeM));
    m_ui->timeM->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeM->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->timeS->setItemDelegate(new QStyledItemDelegate(m_ui->timeS));
    m_ui->timeS->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->timeS->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

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
#endif
}

void MusicDesktopWallpaperWidget::initParameters() const
{
    m_ui->pictureEffect->addItems(QStringList() << tr("none"));
    m_ui->pictureFunc->addItems(QStringList() << tr("order") << tr("random"));

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
    const QString &path = QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        m_ui->urlLineEdit->setText(path);
    }
}

void MusicDesktopWallpaperWidget::netRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(true);
    m_ui->viewButton->setEnabled(false);
    m_currentMode = 0;
}

void MusicDesktopWallpaperWidget::localRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(false);
    m_ui->viewButton->setEnabled(true);
    m_currentMode = 1;
}

void MusicDesktopWallpaperWidget::playRadioButtonPressed()
{
    m_ui->urlLineEdit->setEnabled(false);
    m_ui->viewButton->setEnabled(false);
    m_currentMode = 2;
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
            QStringList imgs;
            imgs << QString("%1%2").arg(TEMPPATH).arg(JPG_FILE);
            m_wallThread->setImagePath(imgs);

            MusicDataDownloadThread *download = new MusicDataDownloadThread(m_ui->urlLineEdit->text().trimmed(), imgs[0], MusicObject::DownloadBigBackground, this);
            connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(parameterFinished()));
            download->startToDownload();
            break;
        }
        case 1:
        {
            QStringList filters, imgs;
            filters << "*.bmp" << "*.jpg" <<"*.jpeg" << "*.png";
            foreach(const QFileInfo &file, MusicUtils::File::getFileListByDir(m_ui->urlLineEdit->text(), filters, true))
            {
                imgs << file.absoluteFilePath();
            }

            m_wallThread->setImagePath(imgs);
            parameterFinished();
            break;
        }
        case 2:
        {
            m_wallThread->setImagePath(M_BACKGROUND_PTR->getArtistPhotoPathList());
            parameterFinished();
            break;
        }
        default: break;
    }
}

void MusicDesktopWallpaperWidget::parameterFinished()
{
    const int time = m_ui->timeH->currentIndex()*MT_H2S + m_ui->timeM->currentIndex()*MT_M2S + m_ui->timeS->currentIndex();
    if(time <= 0)
    {
        MusicMessageBox message;
        message.setText(tr("time is now empty!"));
        message.exec();
        return;
    }

    m_wallThread->setInterval(time*MT_S2MS);
    m_wallThread->start();
    m_wallItem->showFullScreen();
}

void MusicDesktopWallpaperWidget::stopButtonPressed()
{
    m_wallThread->stop();
}

void MusicDesktopWallpaperWidget::cancelButtonPressed()
{
    close();
}

void MusicDesktopWallpaperWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
