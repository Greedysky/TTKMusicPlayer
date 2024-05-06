#include "musicsongdailywidget.h"
#include "musiccibarequest.h"
#include "musicsongrecommendrequest.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "musiccoremplayer.h"

MusicSongDailyWidget::MusicSongDailyWidget(QWidget *parent)
    : QFrame(parent)
{
    setObjectName(className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle10));

    m_container = new QLabel(this);
    m_container->setScaledContents(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_container);
    setLayout(layout);

    m_note = new QLabel(this);
    m_content = new QLabel(this);
    m_button = new QToolButton(this);
    m_button->setCursor(QCursor(Qt::PointingHandCursor));
    m_button->setStyleSheet(TTK::UI::BtnPlay);
    m_button->hide();

    m_player = new MusicCoreMPlayer(this);
    m_imageRequest = new MusicCiBaRequest(this);
    m_songRequest = new MusicSongRecommendRequest(this);

    connect(m_button, SIGNAL(clicked(bool)), SLOT(playSongClicked()));
    connect(m_imageRequest, SIGNAL(downLoadRawDataChanged(QByteArray)), this, SLOT(downLoadImageFinished(QByteArray)));
    connect(m_songRequest, SIGNAL(downLoadDataChanged(QString)), this, SLOT(downLoadSongFinished()));

    m_imageRequest->startToRequest();
    m_songRequest->startToSearch({});
}

MusicSongDailyWidget::~MusicSongDailyWidget()
{
    delete m_note;
    delete m_content;
    delete m_container;
    delete m_imageRequest;
}

void MusicSongDailyWidget::resizeWidget()
{
    QFont font = m_note->font();
    const int height = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height() - WINDOW_HEIGHT_MIN;
    int fontSize = 20 + height * 0.1;
    font.setPixelSize(fontSize > 70 ? 70 : fontSize);
    const int noteHeight = TTK::Widget::fontTextHeight(font);

    m_note->setFont(font);
    m_note->setGeometry(50, 50, TTK::Widget::fontTextWidth(font, m_note->text()), noteHeight);

    font = m_content->font();
    fontSize = 19 + height * 0.05;
    font.setPixelSize(fontSize > 42 ? 42 : fontSize);
    const int contentHeight = TTK::Widget::fontTextHeight(font);

    m_content->setFont(font);
    m_content->setGeometry(50, 50 + noteHeight, TTK::Widget::fontTextWidth(font, m_content->text()), contentHeight);

    m_button->setGeometry((width() - 44) / 2, (this->height() - 44) / 2, 44, 44);
}

void MusicSongDailyWidget::playSongClicked()
{
    if(m_songRequest->isEmpty())
    {
        return;
    }

    m_player->setMedia(MusicCoreMPlayer::Module::Music, m_songRequest->items().back().m_songProps.first().m_url);
    m_button->setStyleSheet(TTK::UI::BtnPause);
    m_button->hide();
}

void MusicSongDailyWidget::downLoadSongFinished()
{
    if(m_songRequest->isEmpty())
    {
        return;
    }

    m_button->show();
}

void MusicSongDailyWidget::downLoadImageFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    QImage image;
    image.loadFromData(bytes);
    m_container->setPixmap(QPixmap::fromImage(image));

    if(image.isNull())
    {
        return;
    }

    QRgb r = 0, g = 0, b = 0;
    for(int w = 0; w < image.width(); ++w)
    {
        for(int h = 0; h < image.height(); ++h)
        {
            const QRgb rgb = image.pixel(w, h);
            r += qRed(rgb);
            g += qGreen(rgb);
            b += qBlue(rgb);
        }
    }

    const int size = image.width() * image.height();
    r /= size;
    g /= size;
    b /= size;

    m_note->setText(m_imageRequest->note());
    m_content->setText(m_imageRequest->content());

    m_note->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(0xFF - r).arg(0xFF - g).arg(0xFF - b));
    m_content->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(0xFF - r).arg(0xFF - g).arg(0xFF - b));

    resizeWidget();
}

void MusicSongDailyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWidget();
}
