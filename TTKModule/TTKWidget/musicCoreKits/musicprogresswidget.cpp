#include "musicprogresswidget.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractmanager.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "ttkglobalinterface.h"

MusicProgressWidget::MusicProgressWidget(QWidget *parent)
    : QProgressDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground);

    setCancelButton(nullptr);
    setLabel(nullptr);
    setFixedSize(368, 123);
    setMinimumDuration(2);

    initialize();
}

MusicProgressWidget::~MusicProgressWidget()
{
    delete m_background;
    delete m_textLabel;
    delete m_progressBar;
}

void MusicProgressWidget::initialize()
{
    m_background = new QLabel(this);
    m_background->setGeometry(4, 4, 360, 115);

    QWidget *backgroundMask = new QWidget(this);
    backgroundMask->setGeometry(4, 29, 360, 90);
    backgroundMask->setStyleSheet(TTK::UI::BackgroundStyle11);

    QLabel *topTitleName = new QLabel(this);
    topTitleName->setText(tr("Progress Bar"));
    topTitleName->setGeometry(14, 4, 221, 25);
    topTitleName->setStyleSheet(TTK::UI::ColorStyle06 + TTK::UI::FontStyle01);

    QToolButton *topTitleCloseButton = new QToolButton(this);
    topTitleCloseButton->setGeometry(344, 6, 20, 20);
    topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    topTitleCloseButton->setEnabled(false);

    m_progressBar = new QProgressBar(this);
    setBar(m_progressBar);
    m_progressBar->setStyleSheet(TTK::UI::ProgressBar01);

    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setGeometry(24, 34, 320, 25);
}

void MusicProgressWidget::setTitle(const QString &name)
{
    m_textLabel->setText(name);
}

void MusicProgressWidget::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    m_progressBar->setGeometry(24, 69, 320, 25);
}

void MusicProgressWidget::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    TTK::setBorderShadow(this, &painter);
}

void MusicProgressWidget::show()
{
    MusicBackgroundImage image;
    if(MusicExtractManager::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl()))
    {
        m_background->setPixmap(image.m_pix.scaled(size()));
    }
    else
    {
        QPixmap pix(G_BACKGROUND_PTR->backgroundUrl());
        if(pix.isNull())
        {
            TTK_ERROR_STREAM("Load current pixmap data error, path is" << G_BACKGROUND_PTR->backgroundUrl());
            return;
        }

        m_background->setPixmap(pix.scaled(size()));
    }

    QProgressDialog::show();
}
