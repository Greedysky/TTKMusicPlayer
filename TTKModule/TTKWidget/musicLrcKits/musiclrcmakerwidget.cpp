#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicinteriorlrcuiobject.h"
#include "musicmessagebox.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musicapplication.h"
#include "musiclrcanalysis.h"
#include "musiclrcmanagerforinterior.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"
#include "ttktime.h"

#include <QTextBlock>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

MusicLrcMakerWidgetItem::MusicLrcMakerWidgetItem(QWidget *ui, QObject *parent)
    : QLabel(ui),
      m_parent(parent)
{
    setStyleSheet(TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle10);
    setFixedSize(650, TTK_ITEM_SIZE_L);

    reset();

    QFont font = this->font();
    font.setPointSize(18);
    setFont(font);
}

bool MusicLrcMakerWidgetItem::biggerThan(int value) const
{
    return m_currentIndex > value;
}

bool MusicLrcMakerWidgetItem::done() const
{
    return m_painetLineDone;
}

void MusicLrcMakerWidgetItem::reset()
{
    m_leftDirection = false;
    m_painetLineDone = false;
    m_intervalCount = 5;
    m_currentIndex = 0;
    m_paintIndex = 0;
    m_itemDelta = 0;

    move(0, 0);
}

void MusicLrcMakerWidgetItem::moveUp()
{
    if(--m_currentIndex  < 0)
    {
        m_currentIndex = 0;
        return;
    }

    m_paintIndex = 0;
    m_intervalCount = 5;
    m_painetLineDone = false;

    move(0, TTK_ITEM_SIZE_L * m_currentIndex);
}

void MusicLrcMakerWidgetItem::moveDown()
{
    if(++m_currentIndex >= 6)
    {
        m_currentIndex = 6;
    }
    m_paintIndex = 0;
    m_intervalCount = 5;
    m_painetLineDone = false;

    move(0, TTK_ITEM_SIZE_L * m_currentIndex);
}

void MusicLrcMakerWidgetItem::moveLeft()
{
    m_leftDirection = true;
    m_painetLineDone = false;
    m_paintIndex -= m_itemDelta;

    if(m_paintIndex <= 0)
    {
        m_paintIndex = 0;
    }
    update();
}

void MusicLrcMakerWidgetItem::moveRight()
{
    m_leftDirection = false;
    const int w = TTK::Widget::fontTextWidth(font(), text());
    m_paintIndex += m_itemDelta;

    if(m_paintIndex >= w)
    {
        m_painetLineDone = true;
        m_paintIndex = w;
        QTimer::singleShot(TTK_DN_ONCE, m_parent, SLOT(currentLineFinished()));
    }
    update();
}

void MusicLrcMakerWidgetItem::setText(const QString &string)
{
    QLabel::setText(string);
    const int len = string.isEmpty() ? 1 : string.length();
    m_itemDelta = TTK::Widget::fontTextWidth(font(), string) / len;
}

void MusicLrcMakerWidgetItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const int w = TTK::Widget::fontTextWidth(font(), text());
    if(!m_leftDirection && m_intervalCount + w >= width() && m_paintIndex >= width() / 2)
    {
        m_intervalCount -= m_itemDelta;
    }
    if(m_leftDirection && m_intervalCount < 0)
    {
        m_intervalCount += m_itemDelta;
    }

    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(m_intervalCount, -10, w, 60, Qt::AlignLeft | Qt::AlignVCenter, text());

    painter.setPen(QColor(144, 184, 214));
    painter.drawText(m_intervalCount, -10, m_paintIndex, 60, Qt::AlignLeft | Qt::AlignVCenter, text());
}



MusicLrcMakerWidget::MusicLrcMakerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicLrcMakerWidget),
      m_currentLine(0),
      m_intervalTime(0)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->stackedWidget->setFocusPolicy(Qt::StrongFocus);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(QString("#stackedWidget{%1}").arg(TTK::UI::BackgroundStyle10));

    createMainWidget();
    createFirstWidget();
    createSecondWidget();
    createThirdWidget();

    m_playMode = MusicApplication::instance()->playMode();
    MusicApplication::instance()->playOneLoop();

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(MusicPlayer::className(), className());
}

MusicLrcMakerWidget::~MusicLrcMakerWidget()
{
    resetToOriginPlayMode();
    G_CONNECTION_PTR->removeValue(this);
    G_SINGLE_MANAGER_PTR->removeObject(className());
    qDeleteAll(m_lrcContainer);
    delete m_lineItem;
    delete m_analysis;
    delete m_ui;
}

void MusicLrcMakerWidget::setCurrentSongName(const QString &name)
{
    m_plainText.clear();
    m_analysis->setCurrentFilePath(QString("%1%2%3").arg(TTK::String::lrcDirPrefix(), name, LRC_FILE));
    m_ui->songNameEdit->setText(TTK::String::songName(name));
    m_ui->artNameEdit->setText(TTK::String::artistName(name));
}

void MusicLrcMakerWidget::positionChanged(qint64 position)
{
    m_ui->timeSlider_F->blockSignals(true);
    m_ui->timeSlider_S->blockSignals(true);
    m_ui->timeSlider_T->blockSignals(true);

    m_ui->timeSlider_F->setValue(position);
    m_ui->timeSlider_S->setValue(position);
    m_ui->timeSlider_T->setValue(position);

    m_ui->timeSlider_F->blockSignals(false);
    m_ui->timeSlider_S->blockSignals(false);
    m_ui->timeSlider_T->blockSignals(false);

    const QString &t = QString("%1/%2").arg(TTKTime::formatDuration(position),
                                            TTKTime::formatDuration(m_ui->timeSlider_F->maximum()));
    m_ui->labelTime_F->setText(t);
    m_ui->labelTime_S->setText(t);
    m_ui->labelTime_T->setText(t);

    if(m_ui->stackedWidget->currentIndex() == 3)
    {
        if(m_analysis->isEmpty())
        {
            return;
        }

        QString currentLrc, laterLrc;
        if(m_analysis->findText(position, m_ui->timeSlider_F->maximum(), currentLrc, laterLrc, m_intervalTime))
        {
            if(currentLrc != m_lrcContainer[m_analysis->lineMiddle()]->text())
            {
                updateCurrentLrc(m_intervalTime);
            }
        }
    }
}

void MusicLrcMakerWidget::durationChanged(qint64 position)
{
    m_ui->timeSlider_F->setRange(0, position);
    m_ui->timeSlider_S->setRange(0, position);
    m_ui->timeSlider_T->setRange(0, position);
}

void MusicLrcMakerWidget::currentLineFinished()
{
    createCurrentLine(Qt::Key_Down);
}

void MusicLrcMakerWidget::timeSliderValueChanged(int value)
{
    MusicApplication::instance()->playAnyTimeAt(value);
    m_analysis->setSongTimeSpeed(value);
}

void MusicLrcMakerWidget::saveButtonClicked()
{
    if(m_times.count() != m_plainText.count())
    {
        return;
    }
    m_analysis->saveData();

    MusicToastLabel::popup(tr("Save file finished"));
}

void MusicLrcMakerWidget::reMakeButtonClicked()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure that you want to remake lyrics?"));
    if(message.exec())
    {
        MusicApplication::instance()->playAnyTimeAt(0);
        setCurrentFirstWidget();
    }
}

void MusicLrcMakerWidget::backToMakeLrcWidget()
{
    MusicMessageBox message;
    message.setText(tr("Go back and lyrics will be lost"));
    if(message.exec())
    {
        MusicApplication::instance()->playAnyTimeAt(0);
        setCurrentSecondWidget();
    }
}

void MusicLrcMakerWidget::firstWidgetStateButtonClicked()
{
    const QString &text = (m_ui->stateButton_F->text() == tr("Play")) ? tr("Pause") : tr("Play");
    m_ui->stateButton_F->setText(text);
    m_ui->stateButton_T->setText(text);

    MusicApplication::instance()->switchToPlayState();
}

void MusicLrcMakerWidget::thirdWidgetStateButtonClicked()
{
    const bool state = m_ui->stateButton_T->text() == tr("Play");
    const QString &text = state ? tr("Pause") : tr("Play");
    m_ui->stateButton_F->setText(text);
    m_ui->stateButton_T->setText(text);

    MusicApplication::instance()->switchToPlayState();
    if(state)
    {
        m_lrcContainer[m_analysis->lineMiddle()]->startDrawLrcMask(m_intervalTime);
    }
    else
    {
        m_lrcContainer[m_analysis->lineMiddle()]->stopDrawLrc();
        m_ui->lrcViewer->stop();
    }
}

void MusicLrcMakerWidget::setCurrentMainWidget()
{
    m_ui->stackedWidget->setCurrentIndex(0);
}

void MusicLrcMakerWidget::setCurrentFirstWidget()
{
    m_ui->stackedWidget->setCurrentIndex(1);
}

void MusicLrcMakerWidget::setCurrentSecondWidget()
{
    if(m_ui->stackedWidget->currentIndex() == 1 && !checkInputValid())
    {
        return;
    }

    if(!MusicApplication::instance()->isPlaying())
    {
        firstWidgetStateButtonClicked();
    }
    else
    {
        MusicApplication::instance()->playAnyTimeAt(0);
    }

    m_plainText = m_ui->lrcTextEdit->toPlainText().trimmed().split("\n");
    m_times.clear();
    m_currentLine = 0;
    m_lineItem->reset();
    m_times[m_currentLine] = 0;

    m_ui->makeTextEdit->setText(m_ui->lrcTextEdit->toPlainText().trimmed() + "\n");
    QTextCursor cursor = m_ui->makeTextEdit->textCursor();
    for(int i = 0; i < m_plainText.count(); ++i)
    {
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setBottomMargin(18);
        cursor.setBlockFormat(textBlockFormat);
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.movePosition(QTextCursor::Start);
    m_ui->makeTextEdit->setTextCursor(cursor);
    m_ui->makeTextEdit->setCurrentCharFormat(QTextCharFormat());
    m_lineItem->setText(cursor.block().text());

    m_ui->stackedWidget->setCurrentIndex(2);
}

void MusicLrcMakerWidget::setCurrentThirdWidget()
{
    if(m_ui->stackedWidget->currentIndex() == 2 && m_plainText.count() > m_ui->makeTextEdit->textCursor().blockNumber())
    {
        MusicToastLabel::popup(tr("Lyrics make has not been completed"));
        return;
    }

    MusicApplication::instance()->playAnyTimeAt(0);
    m_ui->stackedWidget->setCurrentIndex(3);

    if(m_times.count() == m_plainText.count())
    {
        TTKIntStringMap data;
        for(int i = 0; i < m_times.count(); ++i)
        {
            data.insert(m_times.value(i), m_plainText[i]);
        }
        m_analysis->setData(data);

        for(int i = 0; i < m_analysis->lineMax(); ++i)
        {
            m_lrcContainer[i]->setText({});
        }
        setItemStyleSheet(0, -3, 90);
        setItemStyleSheet(1, -6, 35);
        setItemStyleSheet(2, -10, 0);
        setItemStyleSheet(3, -6, 35);
        setItemStyleSheet(4, -3, 90);
    }
}

void MusicLrcMakerWidget::updateAnimationLrc()
{
    for(int i = 0; i < m_analysis->lineMax(); ++i)
    {
        m_lrcContainer[i]->setText(m_analysis->text(i));
    }

    m_analysis->setCurrentIndex(m_analysis->currentIndex() + 1);
    m_lrcContainer[m_analysis->lineMiddle()]->startDrawLrcMask(m_intervalTime);
}

void MusicLrcMakerWidget::lrcSpeedSlower()
{
    m_analysis->revertTime(TTK_DN_S2MS);
    updateCurrentLrc(m_analysis->setSongTimeSpeed(m_ui->timeSlider_T->value()));
}

void MusicLrcMakerWidget::lrcSpeedFaster()
{
    m_analysis->revertTime(-TTK_DN_S2MS);
    updateCurrentLrc(m_analysis->setSongTimeSpeed(m_ui->timeSlider_T->value()));
}

void MusicLrcMakerWidget::keyPressEvent(QKeyEvent *event)
{
    MusicAbstractMoveWidget::keyPressEvent(event);
}

void MusicLrcMakerWidget::keyReleaseEvent(QKeyEvent *event)
{
    MusicAbstractMoveWidget::keyReleaseEvent(event);

    createCurrentLine(event->key());
    if(event->key() == Qt::Key_Space)
    {
        firstWidgetStateButtonClicked();
    }
}

void MusicLrcMakerWidget::createCurrentLine(int key)
{
    QTextCursor cursor = m_ui->makeTextEdit->textCursor();
    if(m_ui->stackedWidget->currentIndex() == 2 && m_plainText.count() > m_currentLine)
    {
        switch(key)
        {
            case Qt::Key_Left:
            {
                MusicApplication::instance()->playAnyTimeAt(m_ui->timeSlider_F->value() - 2222);
                m_lineItem->moveLeft();
                break;
            }
            case Qt::Key_Up:
            {
                cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);
                m_lineItem->setText(cursor.block().text());

                if(--m_currentLine < 0)
                {
                    m_currentLine = 0;
                }

                m_lineItem->moveUp();
                m_times[m_currentLine] = m_ui->timeSlider_S->value();
                break;
            }
            case Qt::Key_Right:
            {
                m_lineItem->moveRight();
                break;
            }
            case Qt::Key_Down:
            {
                if(!m_lineItem->done())
                {
                    return;
                }

                cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
                m_lineItem->setText(cursor.block().text());

                if(++m_currentLine >= m_plainText.count())
                {
                    m_currentLine = m_currentLine - 1;
                    if(!m_lineItem->biggerThan(m_currentLine))
                    {
                        m_lineItem->moveDown();
                    }
                }
                else
                {
                    m_lineItem->moveDown();
                    m_times[m_currentLine] = m_ui->timeSlider_S->value();
                }
                break;
            }
            default: break;
        }
    }
    m_ui->makeTextEdit->setTextCursor(cursor);
}

void MusicLrcMakerWidget::createMainWidget()
{
    m_ui->nextButton_M->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton_M->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->nextButton_M->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton_M->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->cancelButton_M, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->nextButton_M, SIGNAL(clicked()), SLOT(setCurrentFirstWidget()));

    QPropertyAnimation *anim1 = new QPropertyAnimation(m_ui->label_M5, "geometry", this);
    anim1->setDuration(1500);
    anim1->setStartValue(QRect(630, 280, 0, 30));
    anim1->setEndValue(QRect(400, 280, 230, 30));
    anim1->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim2 = new QPropertyAnimation(m_ui->label_M6, "geometry", this);
    anim2->setDuration(1500);
    anim2->setStartValue(QRect(630, 315, 0, 30));
    anim2->setEndValue(QRect(400, 315, 230, 30));
    anim2->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim3 = new QPropertyAnimation(m_ui->label_M7, "geometry", this);
    anim3->setDuration(1500);
    anim3->setStartValue(QRect(630, 350, 0, 30));
    anim3->setEndValue(QRect(400, 350, 230, 30));
    anim3->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim4 = new QPropertyAnimation(m_ui->label_M8, "geometry", this);
    anim4->setDuration(1500);
    anim4->setStartValue(QRect(630, 385, 0, 30));
    anim4->setEndValue(QRect(400, 385, 230, 30));
    anim4->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim5 = new QPropertyAnimation(m_ui->label_M9, "geometry", this);
    anim5->setDuration(1500);
    anim5->setStartValue(QRect(630, 420, 0, 30));
    anim5->setEndValue(QRect(400, 420, 230, 30));
    anim5->setEasingCurve(QEasingCurve::Linear);

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->addAnimation(anim3);
    group->addAnimation(anim4);
    group->addAnimation(anim5);
    group->start();
}

void MusicLrcMakerWidget::createFirstWidget()
{
    m_ui->stateButton_F->setText(!MusicApplication::instance()->isPlaying() ? tr("Play") : tr("Pause"));

    m_ui->artNameEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->songNameEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->authorNameEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->introductionTextEdit->setStyleSheet(TTK::UI::TextEditStyle01);
    m_ui->lrcTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->lrcTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->lrcTextEdit->setStyleSheet(TTK::UI::TextEditStyle01 + TTK::UI::ScrollBarStyle01);
    m_ui->lrcTextEdit->setAcceptRichText(false);
    m_ui->lrcTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    QTextBlockFormat fmt;
    fmt.setLineHeight(30, QTextBlockFormat::FixedHeight);
    QTextCursor cur = m_ui->lrcTextEdit->textCursor();
    cur.setBlockFormat(fmt);
    m_ui->lrcTextEdit->setTextCursor(cur);

    m_ui->timeSlider_F->setFocusPolicy(Qt::NoFocus);
    m_ui->timeSlider_F->setStyleSheet(TTK::UI::SliderStyle09);
    m_ui->stateButton_F->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->previousButton_F->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->nextButton_F->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton_F->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->stateButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton_F->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->timeSlider_F, SIGNAL(sliderReleasedAt(int)), SLOT(timeSliderValueChanged(int)));
    connect(m_ui->stateButton_F, SIGNAL(clicked(bool)), SLOT(firstWidgetStateButtonClicked()));
    connect(m_ui->cancelButton_F, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->nextButton_F, SIGNAL(clicked()), SLOT(setCurrentSecondWidget()));
    connect(m_ui->previousButton_F, SIGNAL(clicked()), SLOT(setCurrentMainWidget()));
}

void MusicLrcMakerWidget::createSecondWidget()
{
    m_lineItem = new MusicLrcMakerWidgetItem(m_ui->makeTextEdit, this);
    m_lineItem->show();

    m_ui->makeTextEdit->setReadOnly(true);
    m_ui->makeTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    m_ui->makeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->makeTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->makeTextEdit->setStyleSheet(TTK::UI::TextEditStyle01 + TTK::UI::ScrollBarStyle01);
    m_ui->makeTextEdit->setAcceptRichText(false);
    m_ui->makeTextEdit->setAutoFormatting(QTextEdit::AutoNone);
    m_ui->makeTextEdit->setEnabled(false);

    QFont font = m_ui->makeTextEdit->font();
    font.setPointSize(15);
    m_ui->makeTextEdit->setFont(font);

    m_ui->timeSlider_S->setFocusPolicy(Qt::NoFocus);
    m_ui->timeSlider_S->setStyleSheet(TTK::UI::SliderStyle09);
    m_ui->previousButton_S->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->nextButton_S->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton_S->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->previousButton_S->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton_S->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton_S->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->timeSlider_S, SIGNAL(sliderReleasedAt(int)), SLOT(timeSliderValueChanged(int)));
    connect(m_ui->cancelButton_S, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->nextButton_S, SIGNAL(clicked()), SLOT(setCurrentThirdWidget()));
    connect(m_ui->previousButton_S, SIGNAL(clicked()), SLOT(setCurrentFirstWidget()));
}

void MusicLrcMakerWidget::createThirdWidget()
{
    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(5);
    m_ui->lrcViewer->connectTo(this);

    for(int i = 0; i < m_analysis->lineMax(); ++i)
    {
        MusicLrcManagerForInterior *w = new MusicLrcManagerForInterior(this);
        w->setLrcPerWidth(-20);
        m_ui->lrcViewer->addWidget(w);
        m_lrcContainer.append(w);
    }

    m_ui->stateButton_T->setText(!MusicApplication::instance()->isPlaying() ? tr("Play") : tr("Pause"));
    m_ui->timeSlider_T->setFocusPolicy(Qt::NoFocus);
    m_ui->lrc_make_up_T->setToolTip(tr("Before 1.0s"));
    m_ui->lrc_make_down_T->setToolTip(tr("After 1.0s"));
    m_ui->lrc_make_up_T->setStyleSheet(TTK::UI::InteriorMakeUp);
    m_ui->lrc_make_down_T->setStyleSheet(TTK::UI::InteriorMakeDown);
    connect(m_ui->lrc_make_up_T, SIGNAL(clicked()), SLOT(lrcSpeedSlower()));
    connect(m_ui->lrc_make_down_T, SIGNAL(clicked()), SLOT(lrcSpeedFaster()));

    m_ui->timeSlider_T->setStyleSheet(TTK::UI::SliderStyle09);
    m_ui->stateButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->remakeButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->uploadButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->saveButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->previousButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton_T->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->stateButton_T->setFocusPolicy(Qt::NoFocus);
    m_ui->remakeButton_T->setFocusPolicy(Qt::NoFocus);
    m_ui->uploadButton_T->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton_T->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton_T->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton_T->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->timeSlider_T, SIGNAL(sliderReleasedAt(int)), SLOT(timeSliderValueChanged(int)));
    connect(m_ui->remakeButton_T, SIGNAL(clicked()), SLOT(reMakeButtonClicked()));
    connect(m_ui->saveButton_T, SIGNAL(clicked()), SLOT(saveButtonClicked()));
    connect(m_ui->stateButton_T, SIGNAL(clicked()), SLOT(thirdWidgetStateButtonClicked()));
    connect(m_ui->cancelButton_T, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->previousButton_T, SIGNAL(clicked()), SLOT(backToMakeLrcWidget()));
}

bool MusicLrcMakerWidget::checkInputValid()
{
    bool error = false;
    QString msg;
    if(m_ui->artNameEdit->text().isEmpty())
    {
        error = true;
        msg = tr("Art name is empty");
    }

    if(m_ui->songNameEdit->text().isEmpty())
    {
        error = true;
        msg = tr("Song name is empty");
    }

    if(m_ui->lrcTextEdit->toPlainText().isEmpty())
    {
        error = true;
        msg = tr("Lrc text is empty");
    }

    if(error)
    {
        MusicToastLabel::popup(msg);
        return false;
    }
    return true;
}

void MusicLrcMakerWidget::setControlEnabled(bool enabled) const
{
    m_ui->artNameEdit->setEnabled(enabled);
    m_ui->songNameEdit->setEnabled(enabled);
    m_ui->authorNameEdit->setEnabled(enabled);
    m_ui->lrcTextEdit->setEnabled(enabled);
    m_ui->introductionTextEdit->setEnabled(enabled);
}

QString MusicLrcMakerWidget::translateTimeString(qint64 time)
{
    const TTKTime t(time);
    return QString("[%1.%2]").arg(t.toString("mm:ss"), QString::number(t.millionSecond()).rightJustified(3, '0'));
}

void MusicLrcMakerWidget::resetToOriginPlayMode()
{
    MusicApplication *w = MusicApplication::instance();
    switch(m_playMode)
    {
        case TTK::PlayMode::Order: w->playOrder(); break;
        case TTK::PlayMode::Random: w->playRandom(); break;
        case TTK::PlayMode::ListLoop: w->playlistLoop(); break;
        case TTK::PlayMode::OneLoop: w->playOneLoop(); break;
        case TTK::PlayMode::Once: w->playOnce(); break;
        default: break;
    }
}

void MusicLrcMakerWidget::updateCurrentLrc(qint64 time)
{
    if(m_analysis->isValid())
    {
        m_intervalTime = time;
        m_ui->lrcViewer->start();
    }
}

void MusicLrcMakerWidget::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInterior *w = m_lrcContainer[index];
    w->setFontSize(size);

    const int value = qBound<int>(TTK_RN_MIN, TTK_RN_MAX - transparent, TTK_RN_MAX);
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(G_SETTING_PTR->value("LrcColor").toInt() != -1)
    {
        const MusicLrcColor::Color index = TTKStaticCast(MusicLrcColor::Color, G_SETTING_PTR->value("LrcColor").toInt());
        w->setLinearGradientColor(TTK::mapIndexToColor(index));
    }
    else
    {
        const MusicLrcColor cl(TTK::readColorConfig(G_SETTING_PTR->value("LrcFrontgroundColor").toString()),
                               TTK::readColorConfig(G_SETTING_PTR->value("LrcBackgroundColor").toString()));
        w->setLinearGradientColor(cl);
    }
}
