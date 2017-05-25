#include "musicsoundkmicrowidget.h"
#include "ui_musicsoundkmicrowidget.h"
#include "musicsoundkmicrosearchwidget.h"
#include "musicfunctionuiobject.h"
#include "musicvideouiobject.h"
#include "musicuiobject.h"
#include "musictime.h"

MusicSoundKMicroWidget::MusicSoundKMicroWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
    m_ui(new Ui::MusicSoundKMicroWidget)
{
    m_ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(MusicUIObject::MBackgroundStyle02);
    m_ui->topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->controlWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->timeLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);

    m_stateButtonOn = true;
    setButtonStyle(true);
    setStateButtonStyle(true);

    m_searchWidget = new MusicSoundKMicroSearchWidget;
    m_searchWidget->show();

    connect(m_ui->stateButton, SIGNAL(clicked()), SLOT(stateButtonChanged()));
}

MusicSoundKMicroWidget::~MusicSoundKMicroWidget()
{
    delete m_ui;
    delete m_searchWidget;
}

QString MusicSoundKMicroWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundKMicroWidget::setValue(qint64 position) const
{
    m_ui->timeSlider->setValue(position*MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position*MT_S2MS))
                                             .arg(MusicTime::msecTime2LabelJustified(m_ui->timeSlider->maximum())));
}

void MusicSoundKMicroWidget::durationChanged(qint64 duration) const
{
    m_ui->timeSlider->setRange(0, duration*MT_S2MS);
    m_ui->timeLabel->setText(QString("00:00/%1").arg(MusicTime::msecTime2LabelJustified(duration*MT_S2MS)));
}

void MusicSoundKMicroWidget::setButtonStyle(bool style) const
{
    m_ui->playButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnPlay :
                                            MusicUIObject::MKGVideoBtnPause);
}

void MusicSoundKMicroWidget::setStateButtonStyle(bool style)  const
{
    m_ui->stateButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnOrigin :
                                             MusicUIObject::MKGVideoBtnOriginOff);
}

void MusicSoundKMicroWidget::stateButtonChanged()
{
    m_stateButtonOn = !m_stateButtonOn;
    setStateButtonStyle(m_stateButtonOn);
}

void MusicSoundKMicroWidget::closeEvent(QCloseEvent *event)
{
    MusicAbstractMoveWidget::closeEvent(event);
    delete m_searchWidget;
    m_searchWidget = nullptr;
}

void MusicSoundKMicroWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractMoveWidget::paintEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, 0) );
}

void MusicSoundKMicroWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractMoveWidget::mouseMoveEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, 0) );
}
