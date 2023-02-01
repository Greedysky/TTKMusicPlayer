#include "musiclrcartphotouploadwidget.h"
#include "ui_musiclrcartphotouploadwidget.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"

#define PIXEL_SIZE 16

MusicPhotoModifiedLabel::MusicPhotoModifiedLabel(QWidget *parent)
    : QWidget(parent),
      m_ratio(0.0f),
      m_picMoved(false),
      m_width(0),
      m_height(0),
      m_originWidth(0)
{

}

void MusicPhotoModifiedLabel::setImagePath(const QString &path)
{
    m_path = path;
    m_showPix.load(path);
    m_width = m_showPix.width();
    m_height = m_showPix.height();
    m_originWidth = m_width;
    m_ratio = m_width * 1.0 / m_height;
}

void MusicPhotoModifiedLabel::saveImagePath(const QString &path) const
{
    QPixmap px(m_width, m_height);
    QPainter painter(&px);
    painter.drawPixmap(0, 0, m_width, m_height, m_showPix);
    px.save(path);
}

void MusicPhotoModifiedLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    const QPixmap pix(":/lrc/lb_transparent");

    for(int i = 0; i < ceil(width() / PIXEL_SIZE); ++i)
    {
        for(int j = 0; j <= ceil(height() / PIXEL_SIZE); ++j)
        {
            painter.drawPixmap(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, pix);
        }
    }

    m_imagePos.setX(m_imagePos.x() + m_deltaPos.x());
    m_imagePos.setY(m_imagePos.y() + m_deltaPos.y());

    const QRect imageRect(m_imagePos.x(), m_imagePos.y(), m_width, m_height);
    painter.drawPixmap(imageRect, m_showPix);
}

void MusicPhotoModifiedLabel::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    m_deltaPos = QPoint();

    const float delta = QtWheelEventDelta(event) / 100.0;
    float dv = m_width * 1.0 / m_originWidth;
    if(dv >= 5 && delta >= 0)
    {
        return;
    }

    m_width = delta >= 0 ?  m_width * delta : m_width * (1 / -delta);
    m_height = delta >= 0 ?  m_height * delta : m_height * (1 / -delta);

    if(m_width < m_height && m_width < 10)
    {
        m_width = 10;
        m_height = m_width / m_ratio;
    }
    else if(m_width > m_height && m_height < 10)
    {
        m_height = 10;
        m_width = m_height * m_ratio;
    }

    dv = m_width * 1.0 / m_originWidth;
    Q_EMIT deltaValueChanged(dv);

    m_showPix.scaled(m_width, m_height, Qt::KeepAspectRatio);
    update();
}

void MusicPhotoModifiedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton)
    {
        m_picMoved = true;
        m_pressedPos = event->pos();
    }
}

void MusicPhotoModifiedLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(m_picMoved)
    {
        m_deltaPos = event->pos() - m_pressedPos;
        m_pressedPos = event->pos();
        update();
    }
}

void MusicPhotoModifiedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_picMoved = false;
}


MusicLrcArtPhotoUploadWidget::MusicLrcArtPhotoUploadWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcArtPhotoUploadWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->artSearchEdit->setStyleSheet(MusicUIObject::LineEditStyle01);
    m_ui->uploadButton->setStyleSheet(MusicUIObject::PushButtonStyle04);
    m_ui->closeButton->setStyleSheet(MusicUIObject::PushButtonStyle04);
    m_ui->selectButton->setStyleSheet(MusicUIObject::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->uploadButton->setFocusPolicy(Qt::NoFocus);
    m_ui->closeButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->uploadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->selectButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->stateLabel->setStyleSheet(MusicUIObject::BackgroundStyle12 + MusicUIObject::ColorStyle07);

    m_ui->uploadButton->hide();
    m_ui->closeButton->hide();
    m_ui->stateLabel->hide();

    connect(m_ui->selectButton, SIGNAL(clicked()), SLOT(selectButtonClicked()));
    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->uploadButton, SIGNAL(clicked()), SLOT(uploadButtonClicked()));
    connect(m_ui->imageLabel, SIGNAL(deltaValueChanged(float)), SLOT(deltaValueChanged(float)));
}

MusicLrcArtPhotoUploadWidget::~MusicLrcArtPhotoUploadWidget()
{
    delete m_ui;
}

void MusicLrcArtPhotoUploadWidget::deltaValueChanged(float v)
{
    m_ui->deltaValueLabel->setText(QString::number(TTKStatic_cast(int, v * 100)) + "%");
}

void MusicLrcArtPhotoUploadWidget::selectButtonClicked()
{
    const QString &path = MusicUtils::File::getOpenFileName(this);
    if(path.isEmpty())
    {
        return;
    }

    const QPixmap pix(path);
    if(pix.width() < WINDOW_WIDTH_MIN || pix.height() < WINDOW_HEIGHT_MIN)
    {
        m_ui->stateLabel->show();
        m_ui->uploadButton->hide();
        m_ui->closeButton->hide();
    }
    else
    {
        m_ui->stateLabel->hide();
        m_ui->uploadButton->show();
        m_ui->closeButton->show();
        m_ui->introduceLabel->hide();
        m_ui->selectButton->hide();
        m_ui->imageLabel->setImagePath(path);
    }
}

void MusicLrcArtPhotoUploadWidget::uploadButtonClicked()
{
    const QDir dir(BACKGROUND_DIR_FULL);
    int count = 0;
    const QString &name = m_ui->artSearchEdit->text().trimmed();
    if(name.isEmpty())
    {
        MusicToastLabel::popup(tr("The art is empty!"));
        return;
    }

    for(const QFileInfo &fin : dir.entryInfoList())
    {
        if(fin.fileName().contains(name))
        {
            ++count;
        }
    }

    const QString &fileName = QString("%1%2%3").arg(BACKGROUND_DIR_FULL, name).arg(count);
    m_ui->imageLabel->saveImagePath(fileName + JPG_FILE);
    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE);
    close();
}
