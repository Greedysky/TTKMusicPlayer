#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicinlinelrcuiobject.h"
#include "musicmessagebox.h"
#include "musictime.h"
#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musiccoreutils.h"
#include "musicapplication.h"
#include "musiclrcanalysis.h"
#include "musiclrcmanagerforinline.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"
#include "musicotherdefine.h"

#include <QPainter>
#include <QTextBlock>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#define ITEM_HEIGHT     40

MusicLrcMakerWidgetItem::MusicLrcMakerWidgetItem(QWidget *ui, QObject *parent)
    : QLabel(ui), m_parentObject(parent)
{
    setStyleSheet(MusicUIObject::MBorderStyle02 + MusicUIObject::MBackgroundStyle17);
    setFixedSize(650, ITEM_HEIGHT);

    reset();

    QFont ft = font();
    ft.setPointSize(18);
    setFont(ft);
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

    move(0, ITEM_HEIGHT*m_currentIndex);
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

    move(0, ITEM_HEIGHT*m_currentIndex);
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
    const int w = QFontMetrics(font()).width(text());
    m_paintIndex += m_itemDelta;

    if(m_paintIndex >= w)
    {
        m_painetLineDone = true;
        m_paintIndex = w;
        QTimer::singleShot(MT_MS, m_parentObject, SLOT(currentLineFinished()));
    }
    update();
}

void MusicLrcMakerWidgetItem::setText(const QString &string)
{
    QLabel::setText(string);
    const int len = string.isEmpty() ? 1 : string.length();
    m_itemDelta = QFontMetrics(font()).width(string)/len;
}

void MusicLrcMakerWidgetItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const int w = QFontMetrics(font()).width(text());
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
      m_ui(new Ui::MusicLrcMakerWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);

    m_ui->stackedWidget->setFocusPolicy(Qt::StrongFocus);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(QString("#stackedWidget{%1}").arg(MusicUIObject::MBackgroundStyle17));

    m_currentLine = 0;
    m_intervalTime = 0;

    createMainWidget();
    createFirstWidget();
    createSecondWidget();
    createThirdWidget();

    m_playMode = MusicApplication::instance()->getPlayMode();
    MusicApplication::instance()->musicPlayOneLoop();

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicPlayer::getClassName(), getClassName());
}

MusicLrcMakerWidget::~MusicLrcMakerWidget()
{
    qDeleteAll(m_musicLrcContainer);
    delete m_lineItem;
    delete m_analysis;

    resetToOriginPlayMode();
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_ui;
}

void MusicLrcMakerWidget::setCurrentSongName(const QString &name)
{
    m_plainText.clear();
    m_analysis->setCurrentFileName(QString("%1%2%3").arg(MusicUtils::String::lrcPrefix()).arg(name).arg(LRC_FILE));
    m_ui->songNameEdit->setText(MusicUtils::String::songName(name));
    m_ui->artNameEdit->setText(MusicUtils::String::artistName(name));
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

    const QString &t = QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position))
                                       .arg(MusicTime::msecTime2LabelJustified(m_ui->timeSlider_F->maximum()));
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
            if(currentLrc != m_musicLrcContainer[m_analysis->getMiddle()]->text())
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

void MusicLrcMakerWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicLrcMakerWidget::timeSliderValueChanged(int value)
{
    MusicApplication::instance()->musicPlayAnyTimeAt(value);
    m_analysis->setSongSpeedChanged(value);
}

void MusicLrcMakerWidget::saveButtonClicked()
{
    if(m_times.count() != m_plainText.count())
    {
        return;
    }

    m_analysis->saveLrcTimeChanged();
    MusicMessageBox message;
    message.setText(tr("save file finished"));
    message.exec();
}

void MusicLrcMakerWidget::reMakeButtonClicked()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure you want to remake lyrics?"));
    if(message.exec())
    {
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
        setCurrentFirstWidget();
    }
}

void MusicLrcMakerWidget::backToMakeLrcWidget()
{
    MusicMessageBox message;
    message.setText(tr("Go back and lyrics will be lost!"));
    if(message.exec())
    {
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
        setCurrentSecondWidget();
    }
}

void MusicLrcMakerWidget::firstWidgetStateButtonClicked()
{
    const QString &text = (m_ui->stateButton_F->text() == tr("Play")) ? tr("Stop") : tr("Play");
    m_ui->stateButton_F->setText(text);
    m_ui->stateButton_T->setText(text);

    MusicApplication::instance()->musicStatePlay();
}

void MusicLrcMakerWidget::thirdWidgetStateButtonClicked()
{
    const bool state = m_ui->stateButton_T->text() == tr("Play");
    const QString &text = state ? tr("Stop") : tr("Play");
    m_ui->stateButton_F->setText(text);
    m_ui->stateButton_T->setText(text);

    MusicApplication::instance()->musicStatePlay();
    if(state)
    {
        m_musicLrcContainer[m_analysis->getMiddle()]->startLrcMask(m_intervalTime);
    }
    else
    {
        m_musicLrcContainer[m_analysis->getMiddle()]->stopLrcMask();
        m_ui->lrcViewer->stop();
    }
}

void MusicLrcMakerWidget::setCurrentMainWidget()
{
    m_ui->stackedWidget->setCurrentIndex(LRC_MAKER_INDEX_0);
}

void MusicLrcMakerWidget::setCurrentFirstWidget()
{
    m_ui->stackedWidget->setCurrentIndex(LRC_MAKER_INDEX_1);
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
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
    }

    m_plainText = m_ui->lrcTextEdit->toPlainText().trimmed().split("\n");
    m_times.clear();
    m_currentLine = 0;
    m_lineItem->reset();
    m_times[m_currentLine] = 0;

    m_ui->makeTextEdit->setText(m_ui->lrcTextEdit->toPlainText().trimmed() + "\n");
    QTextCursor cursor = m_ui->makeTextEdit->textCursor();
    for(int i=0; i<m_plainText.count(); ++i)
    {
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setBottomMargin(18);
        cursor.setBlockFormat(textBlockFormat);
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.movePosition(QTextCursor::Start);
    m_ui->makeTextEdit->setTextCursor(cursor);
    m_ui->makeTextEdit->setCurrentCharFormat(QTextCharFormat());
    m_lineItem->setText( cursor.block().text() );

    m_ui->stackedWidget->setCurrentIndex(LRC_MAKER_INDEX_2);
}

void MusicLrcMakerWidget::setCurrentThirdWidget()
{
    if(m_ui->stackedWidget->currentIndex() == 2 &&
       m_plainText.count() > m_ui->makeTextEdit->textCursor().blockNumber())
    {
        MusicMessageBox message;
        message.setText(tr("Lyrics make has not been completed!"));
        message.exec();
        return;
    }

    MusicApplication::instance()->musicPlayAnyTimeAt(0);
    m_ui->stackedWidget->setCurrentIndex(LRC_MAKER_INDEX_3);

    if(m_times.count() == m_plainText.count())
    {
        MIntStringMap data;
        for(int i=0; i<m_times.count(); ++i)
        {
            data.insert(m_times.value(i), m_plainText[i]);
        }
        m_analysis->setLrcData(data);

        for(int i=0; i<m_analysis->getLineMax(); ++i)
        {
            m_musicLrcContainer[i]->setText( QString() );
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
    for(int i=0; i<m_analysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText(m_analysis->getText(i));
    }
    m_analysis->setCurrentIndex(m_analysis->getCurrentIndex() + 1);
    m_musicLrcContainer[m_analysis->getMiddle()]->startLrcMask(m_intervalTime);
}

void MusicLrcMakerWidget::lrcSpeedSlower()
{
    m_analysis->revertLrcTime(MT_S2MS);
    updateCurrentLrc( m_analysis->setSongSpeedChanged(m_ui->timeSlider_T->value()));
}

void MusicLrcMakerWidget::lrcSpeedFaster()
{
    m_analysis->revertLrcTime(-MT_S2MS);
    updateCurrentLrc( m_analysis->setSongSpeedChanged(m_ui->timeSlider_T->value()));
}

void MusicLrcMakerWidget::keyPressEvent(QKeyEvent* event)
{
    MusicAbstractMoveWidget::keyPressEvent(event);
}

void MusicLrcMakerWidget::keyReleaseEvent(QKeyEvent* event)
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
    if(m_ui->stackedWidget->currentIndex() == 2 && m_plainText.count() > m_currentLine )
    {
        switch(key)
        {
            case Qt::Key_Left:
                {
                    MusicApplication::instance()->musicPlayAnyTimeAt(m_ui->timeSlider_F->value() - 2222);
                    m_lineItem->moveLeft(); break;
                }
            case Qt::Key_Up:
                {
                    cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);
                    m_lineItem->setText( cursor.block().text() );

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
                    m_lineItem->setText( cursor.block().text() );

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
    m_ui->nextButton_M->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton_M->setStyleSheet(MusicUIObject::MPushButtonStyle04);
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
    m_ui->stateButton_F->setText(!MusicApplication::instance()->isPlaying() ? tr("Play") : tr("Stop"));

    m_ui->artNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->songNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->authorNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->introductionTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 );
    m_ui->lrcTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->lrcTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->lrcTextEdit->setStyleSheet(MusicUIObject::MTextEditStyle01 + MusicUIObject::MScrollBarStyle01);
    m_ui->lrcTextEdit->setAcceptRichText(false);
    m_ui->lrcTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    QTextBlockFormat fmt;
    fmt.setLineHeight(30, QTextBlockFormat::FixedHeight);
    QTextCursor cur = m_ui->lrcTextEdit->textCursor();
    cur.setBlockFormat(fmt);
    m_ui->lrcTextEdit->setTextCursor(cur);

    m_ui->timeSlider_F->setFocusPolicy(Qt::NoFocus);
    m_ui->timeSlider_F->setStyleSheet(MusicUIObject::MSliderStyle09);
    m_ui->stateButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->previousButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->nextButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);

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
    m_lineItem = new  MusicLrcMakerWidgetItem(m_ui->makeTextEdit, this);
    m_lineItem->show();

    m_ui->makeTextEdit->setReadOnly(true);
    m_ui->makeTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    m_ui->makeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->makeTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->makeTextEdit->setStyleSheet(MusicUIObject::MTextEditStyle01 + MusicUIObject::MScrollBarStyle01);
    m_ui->makeTextEdit->setAcceptRichText(false);
    m_ui->makeTextEdit->setAutoFormatting(QTextEdit::AutoNone);
    m_ui->makeTextEdit->setEnabled(false);

    QFont font = m_ui->makeTextEdit->font();
    font.setPointSize(15);
    m_ui->makeTextEdit->setFont( font );

    m_ui->timeSlider_S->setFocusPolicy(Qt::NoFocus);
    m_ui->timeSlider_S->setStyleSheet(MusicUIObject::MSliderStyle09);
    m_ui->previousButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->nextButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);

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
    for(int i=0; i<m_analysis->getLineMax(); ++i)
    {
        MusicLrcManagerForInline *w = new MusicLrcManagerForInline(this);
        w->setLrcPerWidth(-20);
        m_ui->lrcViewer->addWidget(w);
        m_musicLrcContainer.append(w);
    }

    m_ui->stateButton_T->setText(!MusicApplication::instance()->isPlaying() ? tr("Play") : tr("Stop"));
    m_ui->timeSlider_T->setFocusPolicy(Qt::NoFocus);
    m_ui->lrc_make_up_T->setToolTip(tr("Before 1s"));
    m_ui->lrc_make_down_T->setToolTip(tr("After 1s"));
    m_ui->lrc_make_up_T->setStyleSheet(MusicUIObject::MKGInlineMakeUp);
    m_ui->lrc_make_down_T->setStyleSheet(MusicUIObject::MKGInlineMakeDown);
    connect(m_ui->lrc_make_up_T, SIGNAL(clicked()), SLOT(lrcSpeedSlower()));
    connect(m_ui->lrc_make_down_T, SIGNAL(clicked()), SLOT(lrcSpeedFaster()));

    m_ui->timeSlider_T->setStyleSheet(MusicUIObject::MSliderStyle09);
    m_ui->stateButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->remakeButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->uploadButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->previousButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);

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
    bool errorFlag = false;
    QString msg;
    if(m_ui->artNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = tr("Art name is empty!");
    }
    if(m_ui->songNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = tr("song name is empty!");
    }
    if(m_ui->lrcTextEdit->toPlainText().isEmpty())
    {
        errorFlag = true;
        msg = tr("lrc text is empty!");
    }

    if(errorFlag)
    {
        MusicMessageBox message;
        message.setText(msg);
        message.exec();
        return false;
    }
    return true;
}

void MusicLrcMakerWidget::setControlEnable(bool enable) const
{
    m_ui->artNameEdit->setEnabled(enable);
    m_ui->songNameEdit->setEnabled(enable);
    m_ui->authorNameEdit->setEnabled(enable);
    m_ui->lrcTextEdit->setEnabled(enable);
    m_ui->introductionTextEdit->setEnabled(enable);
}

QString MusicLrcMakerWidget::translateTimeString(qint64 time)
{
    const MusicTime t(time, MusicTime::All_Msec);
    return QString("[%1.%2]").arg(t.toString("mm:ss")).arg(QString::number(t.getMillionSecond()).rightJustified(3, '0'));
}

void MusicLrcMakerWidget::resetToOriginPlayMode()
{
    MusicApplication *w = MusicApplication::instance();
    switch(m_playMode)
    {
        case MusicObject::PM_PlayOrder:
            w->musicPlayOrder(); break;
        case MusicObject::PM_PlayRandom:
            w->musicPlayRandom(); break;
        case MusicObject::PM_PlaylistLoop:
            w->musicPlaylistLoop(); break;
        case MusicObject::PM_PlayOneLoop:
            w->musicPlayOneLoop(); break;
        case MusicObject::PM_PlayOnce:
            w->musicPlayItemOnce(); break;
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
    MusicLrcManagerForInline *w = m_musicLrcContainer[index];
    w->setFontSize(size);

    const int value = qBound<int>(0, 100 - transparent, 100);
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = MStatic_cast(MusicLrcColor::LrcColorType, M_SETTING_PTR->value("LrcColorChoiced").toInt());
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(index);
        w->setLinearGradientColor(cl);
    }
    else
    {
        const MusicLrcColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFrontgroundColorChoiced").toString()),
                               MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBackgroundColorChoiced").toString()));
        w->setLinearGradientColor(cl);
    }
}
