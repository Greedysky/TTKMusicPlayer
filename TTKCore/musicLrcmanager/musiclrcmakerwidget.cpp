#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musictime.h"
#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musicutils.h"
#include "musicapplication.h"

#include <QPainter>
#include <QTextBlock>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#define ITEM_DELTA      40
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

QString MusicLrcMakerWidgetItem::getClassName()
{
    return staticMetaObject.className();
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
    m_paintIndex -= ITEM_DELTA;

    if(m_paintIndex  < 0)
    {
        m_paintIndex = 0;
    }
    update();
}

void MusicLrcMakerWidgetItem::moveRight()
{
    m_leftDirection = false;
    int w = QFontMetrics(font()).width(text());
    m_paintIndex += ITEM_DELTA;

    if(m_paintIndex > w)
    {
        m_painetLineDone = true;
        m_paintIndex = w;
        QTimer::singleShot(1, m_parentObject, SLOT(currentLineFinished()));
    }
    update();
}

void MusicLrcMakerWidgetItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int w = QFontMetrics(font()).width(text());
    if(!m_leftDirection && m_intervalCount + w >= width() && m_paintIndex >= width() / 2)
    {
        m_intervalCount -= ITEM_DELTA;
    }
    if( m_leftDirection && m_intervalCount < 0)
    {
        m_intervalCount += ITEM_DELTA;
    }

    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(m_intervalCount, -10, w, 60, Qt::AlignLeft | Qt::AlignVCenter, text());

    painter.setPen(QColor(144, 184, 214));
    painter.drawText(m_intervalCount, -10, m_paintIndex, 60, Qt::AlignLeft | Qt::AlignVCenter, text());
    painter.end();
}



MusicLrcMakerWidget::MusicLrcMakerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicLrcMakerWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    ui->stackedWidget->setFocusPolicy(Qt::StrongFocus);
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->stackedWidget->setStyleSheet(QString("#stackedWidget{%1}").arg(MusicUIObject::MBackgroundStyle17));

    m_currentLine = 0;

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
    delete m_lineItem;
    resetToOriginPlayMode();
    M_CONNECTION_PTR->removeValue(getClassName());
}

QString MusicLrcMakerWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcMakerWidget::setCurrentSongName(const QString &name)
{
    m_plainText.clear();
    m_file.setFileName(QString("%1%2%3").arg(MusicUtils::Core::lrcPrefix()).arg(name).arg(LRC_FILE));
    ui->songNameEdit->setText(MusicUtils::Core::songName(name));
    ui->artNameEdit->setText(MusicUtils::Core::artistName(name));
}

void MusicLrcMakerWidget::positionChanged(qint64 position)
{
    ui->timeSlider_F->blockSignals(true);
    ui->timeSlider_S->blockSignals(true);
    ui->timeSlider_T->blockSignals(true);

    ui->timeSlider_F->setValue(position);
    ui->timeSlider_S->setValue(position);
    ui->timeSlider_T->setValue(position);

    ui->timeSlider_F->blockSignals(false);
    ui->timeSlider_S->blockSignals(false);
    ui->timeSlider_T->blockSignals(false);

    QString t = QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position))
     .arg(MusicTime::msecTime2LabelJustified(ui->timeSlider_F->maximum()));
    ui->labelTime_F->setText(t);
    ui->labelTime_S->setText(t);
    ui->labelTime_T->setText(t);
}

void MusicLrcMakerWidget::durationChanged(qint64 position)
{
    ui->timeSlider_F->setRange(0, position);
    ui->timeSlider_S->setRange(0, position);
    ui->timeSlider_T->setRange(0, position);
}

void MusicLrcMakerWidget::currentLineFinished()
{
    createCurrentLine(Qt::Key_Down);
}

void MusicLrcMakerWidget::show()
{
    setBackgroundPixmap(ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicLrcMakerWidget::timeSliderValueChanged(int value)
{
    MusicApplication::instance()->musicPlayAnyTimeAt(value);
}

void MusicLrcMakerWidget::saveButtonClicked()
{
    if(m_times.count() != m_plainText.count())
    {
        return;
    }

    if( m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
    {
        QByteArray array;
        array.append(QString("[ti:%1]\n[ar:%2]\n[by:%3]\n")
                .arg(ui->songNameEdit->text()).arg(ui->artNameEdit->text())
                .arg(ui->authorNameEdit->text()));
        for(int i=0; i<m_times.count(); ++i)
        {
            array.append(m_times.value(i) + m_plainText[i] + "\n");
        }

        QTextStream outstream(&m_file);
        outstream.setCodec("utf-8");
        outstream << array << endl;
        m_file.close();

        MusicMessageBox message;
        message.setText(tr("save file finished"));
        message.exec();
    }
}

void MusicLrcMakerWidget::reMakeButtonClicked()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure you want to remake lyrics?"));
    if(message.exec() == 0)
    {
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
        setCurrentFirstWidget();
    }
}

void MusicLrcMakerWidget::backToMakeLrcWidget()
{
    MusicMessageBox message;
    message.setText(tr("Go back and lyrics will be lost!"));
    if(message.exec() == 0)
    {
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
        setCurrentSecondWidget();
    }
}

void MusicLrcMakerWidget::firstWidgetStateButtonClicked()
{
    QString text = (ui->stateButton_F->text() == tr("Play")) ? tr("Stop") : tr("Play");
    ui->stateButton_F->setText(text);
    ui->stateButton_T->setText(text);

    MusicApplication::instance()->musicStatePlay();
}

void MusicLrcMakerWidget::thirdWidgetStateButtonClicked()
{
    QString text = (ui->stateButton_T->text() == tr("Play")) ? tr("Stop") : tr("Play");
    ui->stateButton_F->setText(text);
    ui->stateButton_T->setText(text);

    MusicApplication::instance()->musicStatePlay();
}

void MusicLrcMakerWidget::setCurrentMainWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicLrcMakerWidget::setCurrentFirstWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicLrcMakerWidget::setCurrentSecondWidget()
{
    if(ui->stackedWidget->currentIndex() == 1 && !checkInputValid())
    {
        return;
    }

    if(MusicApplication::instance()->getPlayState() != MusicPlayer::PlayingState)
    {
        firstWidgetStateButtonClicked();
    }
    else
    {
        MusicApplication::instance()->musicPlayAnyTimeAt(0);
    }

    m_plainText = ui->lrcTextEdit->toPlainText().trimmed().split("\n");
    m_times.clear();
    m_currentLine = 0;
    m_lineItem->reset();
    m_times[m_currentLine] = translateTimeString(0);

    ui->makeTextEdit->setText(ui->lrcTextEdit->toPlainText().trimmed() + "\n");
    QTextCursor cursor = ui->makeTextEdit->textCursor();
    for(int i=0; i<m_plainText.count(); ++i)
    {
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setBottomMargin(18);
        cursor.setBlockFormat(textBlockFormat);
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.movePosition(QTextCursor::Start);
    ui->makeTextEdit->setTextCursor(cursor);
    ui->makeTextEdit->setCurrentCharFormat(QTextCharFormat());
    m_lineItem->setText( cursor.block().text() );

    ui->stackedWidget->setCurrentIndex(2);
}

void MusicLrcMakerWidget::setCurrentThirdWidget()
{
    if(ui->stackedWidget->currentIndex() == 2 &&
       m_plainText.count() > ui->makeTextEdit->textCursor().blockNumber())
    {
        MusicMessageBox message;
        message.setText(tr("Lyrics make has not been completed!"));
        message.exec();
        return;
    }

    MusicApplication::instance()->musicPlayAnyTimeAt(0);
    ui->stackedWidget->setCurrentIndex(3);
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
    QTextCursor cursor = ui->makeTextEdit->textCursor();
    if(ui->stackedWidget->currentIndex() == 2 && m_plainText.count() > m_currentLine )
    {
        switch(key)
        {
            case Qt::Key_Left:
                {
                    MusicApplication::instance()->musicPlayAnyTimeAt(ui->timeSlider_F->value() - 2222);
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

                    m_times[m_currentLine] = translateTimeString(ui->timeSlider_S->value());
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
                    }

                    m_times[m_currentLine] = translateTimeString(ui->timeSlider_S->value());
                    break;
                }
        }
    }
    ui->makeTextEdit->setTextCursor(cursor);
}

void MusicLrcMakerWidget::createMainWidget()
{
    ui->nextButton_M->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton_M->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->cancelButton_M, SIGNAL(clicked()), SLOT(close()));
    connect(ui->nextButton_M, SIGNAL(clicked()), SLOT(setCurrentFirstWidget()));

    QPropertyAnimation *anim1 = new QPropertyAnimation(ui->label_M5, "geometry", this);
    anim1->setDuration(1500);
    anim1->setStartValue(QRect(630, 280, 0, 30));
    anim1->setEndValue(QRect(400, 280, 230, 30));
    anim1->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim2 = new QPropertyAnimation(ui->label_M6, "geometry", this);
    anim2->setDuration(1500);
    anim2->setStartValue(QRect(630, 315, 0, 30));
    anim2->setEndValue(QRect(400, 315, 230, 30));
    anim2->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim3 = new QPropertyAnimation(ui->label_M7, "geometry", this);
    anim3->setDuration(1500);
    anim3->setStartValue(QRect(630, 350, 0, 30));
    anim3->setEndValue(QRect(400, 350, 230, 30));
    anim3->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim4 = new QPropertyAnimation(ui->label_M8, "geometry", this);
    anim4->setDuration(1500);
    anim4->setStartValue(QRect(630, 385, 0, 30));
    anim4->setEndValue(QRect(400, 385, 230, 30));
    anim4->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim5 = new QPropertyAnimation(ui->label_M9, "geometry", this);
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
    ui->stateButton_F->setText(MusicApplication::instance()->getPlayState() != MusicPlayer::PlayingState ? tr("Play") : tr("Stop"));

    ui->artNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->songNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->authorNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->introductionTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 );
    ui->lrcTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lrcTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lrcTextEdit->setStyleSheet(MusicUIObject::MTextEditStyle01 + MusicUIObject::MScrollBarStyle01);
    ui->lrcTextEdit->setAcceptRichText(false);
    ui->lrcTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    QTextBlockFormat fmt;
    fmt.setLineHeight(30, QTextBlockFormat::FixedHeight);
    QTextCursor cur = ui->lrcTextEdit->textCursor();
    cur.setBlockFormat(fmt);
    ui->lrcTextEdit->setTextCursor(cur);

    ui->timeSlider_F->setFocusPolicy(Qt::NoFocus);
    ui->timeSlider_F->setStyleSheet(MusicUIObject::MSliderStyle07);
    ui->stateButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->previousButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->nextButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->timeSlider_F, SIGNAL(valueChanged(int)), SLOT(timeSliderValueChanged(int)));
    connect(ui->stateButton_F, SIGNAL(clicked(bool)), SLOT(firstWidgetStateButtonClicked()));
    connect(ui->cancelButton_F, SIGNAL(clicked()), SLOT(close()));
    connect(ui->nextButton_F, SIGNAL(clicked()), SLOT(setCurrentSecondWidget()));
    connect(ui->previousButton_F, SIGNAL(clicked()), SLOT(setCurrentMainWidget()));
}

void MusicLrcMakerWidget::createSecondWidget()
{
    m_lineItem = new  MusicLrcMakerWidgetItem(ui->makeTextEdit, this);
    m_lineItem->show();

    ui->makeTextEdit->setReadOnly(true);
    ui->makeTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui->makeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->makeTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->makeTextEdit->setStyleSheet(MusicUIObject::MTextEditStyle01 + MusicUIObject::MScrollBarStyle01);
    ui->makeTextEdit->setAcceptRichText(false);
#ifdef MUSIC_GREATER_NEW
    ui->makeTextEdit->setAutoFormatting(false);
#else
    ui->makeTextEdit->setAutoFormatting(QTextEdit::AutoNone);
#endif
    ui->makeTextEdit->setEnabled(false);

    QFont font = ui->makeTextEdit->font();
    font.setPointSize(15);
    ui->makeTextEdit->setFont( font );

    ui->timeSlider_S->setFocusPolicy(Qt::NoFocus);
    ui->timeSlider_S->setStyleSheet(MusicUIObject::MSliderStyle07);
    ui->previousButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->nextButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->timeSlider_S, SIGNAL(valueChanged(int)), SLOT(timeSliderValueChanged(int)));
    connect(ui->cancelButton_S, SIGNAL(clicked()), SLOT(close()));
    connect(ui->nextButton_S, SIGNAL(clicked()), SLOT(setCurrentThirdWidget()));
    connect(ui->previousButton_S, SIGNAL(clicked()), SLOT(setCurrentFirstWidget()));
}

void MusicLrcMakerWidget::createThirdWidget()
{
    ui->stateButton_T->setText(MusicApplication::instance()->getPlayState() != MusicPlayer::PlayingState  ? tr("Play") : tr("Stop"));

    ui->timeSlider_T->setFocusPolicy(Qt::NoFocus);
    ui->timeSlider_T->setStyleSheet(MusicUIObject::MSliderStyle07);
    ui->stateButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->remakeButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->uploadButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->saveButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->previousButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->timeSlider_T, SIGNAL(valueChanged(int)), SLOT(timeSliderValueChanged(int)));
    connect(ui->remakeButton_T, SIGNAL(clicked()), SLOT(reMakeButtonClicked()));
    connect(ui->saveButton_T, SIGNAL(clicked()), SLOT(saveButtonClicked()));
    connect(ui->stateButton_T, SIGNAL(clicked()), SLOT(thirdWidgetStateButtonClicked()));
    connect(ui->cancelButton_T, SIGNAL(clicked()), SLOT(close()));
    connect(ui->previousButton_T, SIGNAL(clicked()), SLOT(backToMakeLrcWidget()));
}

bool MusicLrcMakerWidget::checkInputValid()
{
    bool errorFlag = false;
    QString msg;
    if(ui->artNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = tr("Art name is empty!");
    }
    if(ui->songNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = tr("song name is empty!");
    }
    if(ui->lrcTextEdit->toPlainText().isEmpty())
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
    ui->artNameEdit->setEnabled(enable);
    ui->songNameEdit->setEnabled(enable);
    ui->authorNameEdit->setEnabled(enable);
    ui->lrcTextEdit->setEnabled(enable);
    ui->introductionTextEdit->setEnabled(enable);
}

QString MusicLrcMakerWidget::translateTimeString(qint64 time)
{
    MusicTime t(time, MusicTime::All_Msec);
    return QString("[%1.%2]").arg(t.toString("mm:ss")).arg(
           QString::number(t.getMillionSecond()).rightJustified(3, '0'));
}

void MusicLrcMakerWidget::resetToOriginPlayMode()
{
    MusicApplication *w = MusicApplication::instance();
    switch(m_playMode)
    {
        case MusicObject::MC_PlayOrder:
            w->musicPlayOrder(); break;
        case MusicObject::MC_PlayRandom:
            w->musicPlayRandom(); break;
        case MusicObject::MC_PlayListLoop:
            w->musicPlayListLoop(); break;
        case MusicObject::MC_PlayOneLoop:
            w->musicPlayOneLoop(); break;
        case MusicObject::MC_PlayOnce:
            w->musicPlayItemOnce(); break;
    }
}
