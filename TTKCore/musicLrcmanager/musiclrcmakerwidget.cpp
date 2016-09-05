#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musictime.h"
#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musicutils.h"

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

MusicLrcMakerWidget::MusicLrcMakerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicLrcMakerWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->stackedWidget->setStyleSheet(QString("#stackedWidget{%1}").arg(MusicUIObject::MBackgroundStyle17));

    ui->lrcTextEdit->setFontPointSize(11);
    ui->lrcTextEdit->setAlignment(Qt::AlignLeft);

    ui->artNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->songNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->authorNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->introductionTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 );
    ui->lrcTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 + \
                                    MusicUIObject::MScrollBarStyle01 );

    QPropertyAnimation *anim1 = new QPropertyAnimation(ui->label_M5, "pos", this);
    anim1->setDuration(500);
    anim1->setStartValue(QPoint(630, 280));
    anim1->setEndValue(QPoint(400, 280));
    anim1->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim2 = new QPropertyAnimation(ui->label_M6, "pos", this);
    anim2->setDuration(500);
    anim2->setStartValue(QPoint(630, 315));
    anim2->setEndValue(QPoint(400, 315));
    anim2->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim3 = new QPropertyAnimation(ui->label_M7, "pos", this);
    anim3->setDuration(500);
    anim3->setStartValue(QPoint(630, 350));
    anim3->setEndValue(QPoint(400, 350));
    anim3->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim4 = new QPropertyAnimation(ui->label_M8, "pos", this);
    anim4->setDuration(500);
    anim4->setStartValue(QPoint(630, 385));
    anim4->setEndValue(QPoint(400, 385));
    anim4->setEasingCurve(QEasingCurve::Linear);

    QPropertyAnimation *anim5 = new QPropertyAnimation(ui->label_M9, "pos", this);
    anim5->setDuration(500);
    anim5->setStartValue(QPoint(630, 420));
    anim5->setEndValue(QPoint(400, 420));
    anim5->setEasingCurve(QEasingCurve::Linear);

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->addAnimation(anim3);
    group->addAnimation(anim4);
    group->addAnimation(anim5);
    group->start();

    ui->timeSlider_F->setStyleSheet(MusicUIObject::MSliderStyle07);
//    ui->makeButton->setStyleSheet( MusicUIObject::MPushButtonStyle04);
//    ui->saveButton->setStyleSheet( MusicUIObject::MPushButtonStyle04);
//    ui->reviewButton->setStyleSheet( MusicUIObject::MPushButtonStyle04);
//    ui->makeButton->setCursor(QCursor(Qt::PointingHandCursor));
//    ui->saveButton->setCursor(QCursor(Qt::PointingHandCursor));
//    ui->reviewButton->setCursor(QCursor(Qt::PointingHandCursor));
//    connect(ui->makeButton, SIGNAL(clicked()), SLOT(makeButtonClicked()));
//    connect(ui->saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));
//    connect(ui->reviewButton, SIGNAL(clicked()), SLOT(reviewButtonClicked()));

//    ui->saveButton->setEnabled(false);
    m_position = 0;
    m_currentLine = 0;

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicPlayer::getClassName(), getClassName());
}

MusicLrcMakerWidget::~MusicLrcMakerWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName() );
}

QString MusicLrcMakerWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcMakerWidget::setCurrentSongName(const QString& name)
{
    m_plainText.clear();
    m_file.setFileName(QString("%1%2%3").arg(MusicUtils::UCore::lrcPrefix()).arg(name).arg(LRC_FILE));
    QStringList ls = name.split('-');
    if(!ls.isEmpty())
    {
        ui->songNameEdit->setText(ls.back().trimmed());
        ui->artNameEdit->setText(ls.front().trimmed());
    }
}

void MusicLrcMakerWidget::setCurrentPosition(qint64 pos)
{
    m_position = pos;
}

void MusicLrcMakerWidget::makeButtonClicked()
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
        return;
    }

//    ui->makeButton->setEnabled(false);
    setControlEnable(false);
    m_plainText = ui->lrcTextEdit->toPlainText().split("\n");
    QTextCursor cursor = ui->lrcTextEdit->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    ui->lrcTextEdit->setTextCursor(cursor);
}

void MusicLrcMakerWidget::saveButtonClicked()
{
    if( m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
    {
        QByteArray array;
        array.append(QString("[ti:%1]\n[ar:%2]\n[by:%3]\n")
                .arg(ui->songNameEdit->text()).arg(ui->artNameEdit->text())
                .arg(ui->authorNameEdit->text()));
        foreach(const QString &var, m_plainText)
        {
            array.append(var + "\n");
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

void MusicLrcMakerWidget::reviewButtonClicked()
{
    m_currentLine = 0;
    m_plainText.clear();
//    ui->makeButton->setEnabled(true);
    setControlEnable(true);
    ui->lrcTextEdit->clear();
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
           QString::number(t.getMillionSecond()).rightJustified(3, '0').left(2));
}

void MusicLrcMakerWidget::keyPressEvent(QKeyEvent* event)
{
    MusicAbstractMoveWidget::keyPressEvent(event);
//    if(!ui->makeButton->isEnabled() && event->key() == Qt::Key_A)
//    {
//        if(m_plainText.count() > m_currentLine)
//        {
//            m_plainText[m_currentLine++].insert(0, translateTimeString(m_position) );
//            QTextCursor cursor = ui->lrcTextEdit->textCursor();
//            if(!cursor.atEnd())
//            {
//                QTextBlockFormat textBlockFormat = cursor.blockFormat();
//                textBlockFormat.setBottomMargin(5);
//                cursor.setBlockFormat(textBlockFormat);
//                cursor.select(QTextCursor::LineUnderCursor);
//                cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor, 1);
//                ui->lrcTextEdit->setTextCursor(cursor);
//            }
//        }
//        else
//        {
//            ui->saveButton->setEnabled(true);
//            MusicMessageBox message;
//            message.setText(tr("lrc make finished"));
//            message.exec();
//        }
//    }
}

void MusicLrcMakerWidget::keyReleaseEvent(QKeyEvent* event)
{
    MusicAbstractMoveWidget::keyReleaseEvent(event);
}

void MusicLrcMakerWidget::show()
{
    setBackgroundPixmap(ui->background, size());
    MusicAbstractMoveWidget::show();
}
