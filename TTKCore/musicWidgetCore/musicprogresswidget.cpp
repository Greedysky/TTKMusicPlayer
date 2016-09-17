#include "musicprogresswidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include <QProgressBar>

MusicProgressWidget::MusicProgressWidget(QWidget *parent)
    : QProgressDialog(parent)
{
    setModal(true);
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setCancelButton(nullptr);
    setLabel(nullptr);
    setFixedSize(400, 155);

    initWidget();
}

MusicProgressWidget::~MusicProgressWidget()
{
    delete m_background;
    delete m_textLabel;
    delete m_progressBar;
}

QString MusicProgressWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicProgressWidget::initWidget()
{
    m_background = new QLabel(this);
    m_background->setGeometry(20, 20, 360, 115);
    QWidget *backgroundMask = new QWidget(this);
    backgroundMask->setGeometry(20, 45, 360, 90);
    backgroundMask->setStyleSheet(MusicUIObject::MBackgroundStyle13);

    QLabel *topTitleName = new QLabel(this);
    topTitleName->setText(tr("Progress Bar"));
    topTitleName->setGeometry(30, 20, 221, 25);
    topTitleName->setStyleSheet(MusicUIObject::MColorStyle01 + MusicUIObject::MFontStyle01);

    QToolButton *topTitleCloseButton = new QToolButton(this);
    topTitleCloseButton->setGeometry(360, 22, 20, 20);
    topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    topTitleCloseButton->setEnabled(false);

    m_progressBar = new QProgressBar(this);
    setBar(m_progressBar);
    m_progressBar->setStyleSheet(MusicUIObject::MProgressBar01);

    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setGeometry(40, 50, 320, 25);
}

void MusicProgressWidget::setTitle(const QString &name)
{
    m_textLabel->setText(name);
}

void MusicProgressWidget::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    m_progressBar->setGeometry(40, 85, 320, 25);
}

void MusicProgressWidget::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0, 0, WIDTH, HEIGHT, QPixmap(":/shadow/lb_left_top"));
    painter.drawPixmap(width() - WIDTH, 0, WIDTH, HEIGHT, QPixmap(":/shadow/lb_right_top"));
    painter.drawPixmap(0, height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/lb_left_bottom"));
    painter.drawPixmap(width() - WIDTH, height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/lb_right_bottom"));

    painter.drawPixmap(0, WIDTH, HEIGHT, height() - 2*WIDTH,
                       QPixmap(":/shadow/lb_left").scaled(WIDTH, height() - 2*HEIGHT));
    painter.drawPixmap(width() - WIDTH, WIDTH, HEIGHT, height() - 2*HEIGHT,
                       QPixmap(":/shadow/lb_right").scaled(WIDTH, height() - 2*HEIGHT));
    painter.drawPixmap(HEIGHT, 0, width() - 2*WIDTH, HEIGHT,
                       QPixmap(":/shadow/lb_top").scaled(width() - 2*WIDTH, HEIGHT));
    painter.drawPixmap(WIDTH, height() - HEIGHT, width() - 2*WIDTH, HEIGHT,
                       QPixmap(":/shadow/lb_bottom").scaled(width() - 2*WIDTH, HEIGHT));
}

void MusicProgressWidget::show()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    m_background->setPixmap(pix.scaled( size() ));
    QProgressDialog::show();
}
