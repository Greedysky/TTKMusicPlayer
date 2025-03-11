#include "musicbackgroundpalettewidget.h"
#include "ui_musicbackgroundpalettewidget.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractmanager.h"
#include "musiccolordialog.h"
#include "musicwidgetheaders.h"

static constexpr int COLOR_COL = 10;
static constexpr int COLOR_ROW = 6;

MusicBackgroundPalette::MusicBackgroundPalette(QWidget *parent)
    : QLabel(parent)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

MusicBackgroundPalette::~MusicBackgroundPalette()
{
    QFile::remove(TTK_COLOR_FILE);
}

void MusicBackgroundPalette::setPixmap(const QColor &color)
{
    QPixmap pix(90, 30);
    pix.fill(m_color = color);
    QLabel::setPixmap(pix);
}

void MusicBackgroundPalette::copyColorToMemory(const QColor &color)
{
    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(color);
    if(image.save(TTK_COLOR_FILE))
    {
        Q_EMIT currentColorToMemoryChanged(TTK_COLOR_FILE);
    }
}

void MusicBackgroundPalette::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    copyColorToMemory(m_color);
}

void MusicBackgroundPalette::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        Q_EMIT currentColorToFileChanged(m_color);
    }
}


MusicBackgroundPaletteWidget::MusicBackgroundPaletteWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicBackgroundPaletteWidget),
      m_confirmButtonClicked(false)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    QList<QColor> colors;
    colors << QColor(225, 152, 180);
    colors << QColor(236, 109, 113);
    colors << QColor(208, 87, 107);
    colors << QColor(233, 84, 107);
    colors << QColor(197, 61, 67);
    colors << QColor(232, 57, 41);
    colors << QColor(226, 4, 27);
    colors << QColor(201, 23, 30);
    colors << QColor(158, 61, 63);
    colors << QColor(162, 32, 65);

    colors << QColor(251, 202, 77);
    colors << QColor(250, 191, 20);
    colors << QColor(230, 180, 34);
    colors << QColor(217, 166, 46);
    colors << QColor(243, 152, 0);
    colors << QColor(236, 104, 0);
    colors << QColor(234, 85, 6);
    colors << QColor(191, 120, 58);
    colors << QColor(119, 60, 48);
    colors << QColor(100, 1, 37);

    colors << QColor(204, 126, 177);
    colors << QColor(180, 76, 151);
    colors << QColor(157, 91, 139);
    colors << QColor(122, 65, 113);
    colors << QColor(136, 72, 152);
    colors << QColor(116, 50, 92);
    colors << QColor(112, 88, 163);
    colors << QColor(77, 67, 152);
    colors << QColor(77, 90, 175);
    colors << QColor(46, 41, 48);

    colors << QColor(160, 216, 239);
    colors << QColor(132, 185, 203);
    colors << QColor(0, 163, 175);
    colors << QColor(105, 185, 198);
    colors << QColor(0, 149, 217);
    colors << QColor(39, 146, 195);
    colors << QColor(76, 108, 179);
    colors << QColor(62, 98, 173);
    colors << QColor(25, 68, 142);
    colors << QColor(42, 64, 115);

    colors << QColor(195, 216, 37);
    colors << QColor(184, 210, 0);
    colors << QColor(199, 220, 104);
    colors << QColor(147, 202, 118);
    colors << QColor(131, 155, 92);
    colors << QColor(105, 176, 118);
    colors << QColor(71, 136, 94);
    colors << QColor(2, 135, 96);
    colors << QColor(71, 89, 80);
    colors << QColor(0, 82, 67);

    colors << QColor(153, 153, 153);
    colors << QColor(165, 143, 134);
    colors << QColor(159, 111, 85);
    colors << QColor(118, 92, 71);
    colors << QColor(84, 47, 50);
    colors << QColor(102, 102, 102);
    colors << QColor(83, 78, 77);
    colors << QColor(43, 43, 43);
    colors << QColor(37, 13, 0);
    colors << QColor(22, 22, 14);
    //
    QGridLayout *layout = new QGridLayout(m_ui->mutliWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    for(int i = 0; i < COLOR_ROW; ++i)
    {
        for(int j = 0; j < COLOR_COL; ++j)
        {
            MusicBackgroundPalette *label = new MusicBackgroundPalette(this);
            QColor color = colors[i * COLOR_COL + j];
            label->setPixmap(color);
            label->setToolTip(color.name());
            layout->addWidget(label, i, j);
            m_container << label;

            connect(label, SIGNAL(currentColorToFileChanged(QColor)), SLOT(currentColorToFile(QColor)));
            connect(label, SIGNAL(currentColorToMemoryChanged(QString)), SLOT(currentColorToMemory(QString)));
        }
    }
    m_ui->mutliWidget->setLayout(layout);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->paletteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->paletteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(paletteColorClicked()));
}

MusicBackgroundPaletteWidget::~MusicBackgroundPaletteWidget()
{
    QFile::remove(TTK_COLOR_FILE);
    if(!m_confirmButtonClicked)
    {
        Q_EMIT currentColorToMemoryChanged(m_previousBackground);
    }

    qDeleteAll(m_container);
    delete m_ui;
}

void MusicBackgroundPaletteWidget::updateBackground(const QString &path)
{
    MusicBackgroundImage image;
    if(MusicExtractManager::outputSkin(&image, path))
    {
        m_ui->background->setPixmap(image.m_pix.scaled(size()));
    }
    else
    {
        m_ui->background->setPixmap(QPixmap(path).scaled(size()));
    }
}

void MusicBackgroundPaletteWidget::paletteColorClicked()
{
    if(m_currentColor.isValid())
    {
        m_confirmButtonClicked = true;

        QImage image(16, 16, QImage::Format_ARGB32);
        image.fill(m_currentColor);
        if(image.save(TTK_COLOR_FILE))
        {
            Q_EMIT currentColorToFileChanged(TTK_COLOR_FILE);
        }
    }
    close();
}

void MusicBackgroundPaletteWidget::showPaletteDialog()
{
    const QColor &color = MusicColorDialog::popup(this);
    if(color.isValid())
    {
        currentColorToFile(m_currentColor = color);
    }
}

void MusicBackgroundPaletteWidget::currentColorToFile(const QColor &color)
{
    QPixmap pix(90, 30);
    pix.fill(color);
    m_ui->colorLabel->setPixmap(pix);

    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(m_currentColor = color);
    if(image.save(TTK_COLOR_FILE))
    {
        currentColorToMemory(TTK_COLOR_FILE);
    }
}

void MusicBackgroundPaletteWidget::currentColorToMemory(const QString &path)
{
    updateBackground(path);
    Q_EMIT currentColorToMemoryChanged(path);
}

int MusicBackgroundPaletteWidget::exec()
{
    m_previousBackground = G_BACKGROUND_PTR->backgroundUrl();
    updateBackground(m_previousBackground);
    return MusicAbstractMoveDialog::exec();
}
