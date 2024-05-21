#include "musicsongdailywidget.h"
#include "musiccibarequest.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "musicimageutils.h"

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

    m_note = new QLabel(m_container);
    m_content = new QLabel(m_container);

    m_networkRequest = new MusicCiBaRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadRawDataChanged(QByteArray)), this, SLOT(downLoadImageFinished(QByteArray)));

    m_networkRequest->startToRequest();
}

MusicSongDailyWidget::~MusicSongDailyWidget()
{
    delete m_note;
    delete m_content;
    delete m_container;
    delete m_networkRequest;
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

    const QRgb rgb = TTK::Image::colorContrast(qRgb(r, g, b));
    const QString &style = QString("color:rgb(%1, %2, %3)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb));

    m_note->setStyleSheet(style);
    m_content->setStyleSheet(style);

    m_note->setText(m_networkRequest->note());
    m_content->setText(m_networkRequest->content());

    resizeWidget();
}

void MusicSongDailyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWidget();
}
