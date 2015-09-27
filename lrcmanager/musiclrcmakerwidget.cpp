#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicbgthememanager.h"
#include "musicmessagebox.h"

#include <QTime>

MusicLrcMakerWidget::MusicLrcMakerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
    ui(new Ui::MusicLrcMakerWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->lrcTextEdit->setFontPointSize(15);
    ui->lrcTextEdit->setFontWeight(QFont::Bold);
    ui->lrcTextEdit->setAlignment(Qt::AlignCenter);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    ui->artNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->songNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->authorNameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->introductionTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 );
    ui->lrcTextEdit->setStyleSheet( MusicUIObject::MTextEditStyle01 );

    ui->makeButton->setStyleSheet( MusicUIObject::MPushButtonStyle05);
    ui->saveButton->setStyleSheet( MusicUIObject::MPushButtonStyle05);
    ui->reviewButton->setStyleSheet( MusicUIObject::MPushButtonStyle05);
    ui->makeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->saveButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->reviewButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->makeButton,SIGNAL(clicked()),SLOT(makeButtonClicked()));
    connect(ui->saveButton,SIGNAL(clicked()),SLOT(saveButtonClicked()));
    connect(ui->reviewButton,SIGNAL(clicked()),SLOT(reviewButtonClicked()));

    ui->saveButton->setEnabled(false);
    m_position = 0;
    m_currentLine = 0;

}

void MusicLrcMakerWidget::setCurrentSongName(const QString& name)
{
    m_plainText.clear();
    m_file.setFileName(QString("%1%2%3").arg(LRC_DOWNLOAD).arg(name).arg(LRC_FILE));
    QStringList ls = name.split('-');
    if(!ls.isEmpty())
    {
        ui->songNameEdit->setText(ls.back().trimmed());
        ui->artNameEdit->setText(ls.front().trimmed());
    }
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

    ui->makeButton->setEnabled(false);
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
        foreach(QString var, m_plainText)
        {
            array.append(var);
            array.append("\n");
        }
        m_file.write(array);
        m_file.close();
    }
}

void MusicLrcMakerWidget::reviewButtonClicked()
{
    m_currentLine = 0;
    m_plainText.clear();
    ui->makeButton->setEnabled(true);
    setControlEnable(true);
    ui->artNameEdit->clear();
    ui->songNameEdit->clear();
    ui->authorNameEdit->clear();
    ui->lrcTextEdit->clear();
    ui->introductionTextEdit->clear();
    ui->lrcTextEdit->setFontPointSize(15);
    ui->lrcTextEdit->setFontWeight(QFont::Bold);
    ui->lrcTextEdit->setAlignment(Qt::AlignCenter);
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
    return QString("[%1.%2]").arg(QTime::fromMSecsSinceStartOfDay(time).toString("mm:ss"))
                             .arg( QString::number(time%1000).left(2) );
}

void MusicLrcMakerWidget::keyPressEvent(QKeyEvent* event)
{
    MusicAbstractMoveWidget::keyPressEvent(event);
    if(!ui->makeButton->isEnabled() && event->key() == Qt::Key_A)
    {
        if(m_plainText.count() > m_currentLine)
        {
            m_plainText[m_currentLine++].insert(0, translateTimeString(m_position) );

            QList<QTextEdit::ExtraSelection> extraSelections;

            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(QColor(Qt::yellow));
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = ui->lrcTextEdit->textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
            ui->lrcTextEdit->setExtraSelections(extraSelections);

            QTextCursor cursor = ui->lrcTextEdit->textCursor();
            if(!cursor.atEnd())
            {
                cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor, 1);
                ui->lrcTextEdit->setTextCursor(cursor);
            }
        }
        else
        {
            ui->saveButton->setEnabled(true);
            MusicMessageBox message;
            message.setText(tr("lrc make finished"));
            message.exec();
        }
    }
}

void MusicLrcMakerWidget::keyReleaseEvent(QKeyEvent* event)
{
    MusicAbstractMoveWidget::keyReleaseEvent(event);
}

void MusicLrcMakerWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
