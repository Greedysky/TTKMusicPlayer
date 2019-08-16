#include "musicitemdelegate.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicwidgetheaders.h"

#include <QPainter>

MusicRadioButtonDelegate::MusicRadioButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_radioButton  = new QRadioButton;
    m_radioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_radioButton->setFocusPolicy(Qt::NoFocus);
#endif
}

MusicRadioButtonDelegate::~MusicRadioButtonDelegate()
{
    delete m_radioButton;
}

void MusicRadioButtonDelegate::setStyleSheet(const QString &style)
{
    m_radioButton->setStyleSheet(style);
}

QSize MusicRadioButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicRadioButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    const int minSize = qMin(option.rect.width(), option.rect.height());
    m_radioButton->resize(minSize, minSize);
    m_radioButton->setChecked( index.data(MUSIC_CHECK_ROLE).toBool() );
    painter->translate((option.rect.width() - 16)/2, 0); // top left
    m_radioButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}


MusicCheckBoxDelegate::MusicCheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_checkBox  = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_checkBox, SIGNAL(stateChanged(int)), SIGNAL(buttonChecked()));
}

MusicCheckBoxDelegate::~MusicCheckBoxDelegate()
{
    delete m_checkBox;
}

void MusicCheckBoxDelegate::setStyleSheet(const QString &style)
{
    m_checkBox->setStyleSheet(style);
}

QSize MusicCheckBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    const int minSize = qMin(option.rect.width(), option.rect.height());
    m_checkBox->resize(minSize, minSize);
    m_checkBox->setChecked( index.data(MUSIC_CHECK_ROLE).toBool() );
    painter->translate((option.rect.width() - 16)/2, 0); // top left
    m_checkBox->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}


MusicQueryTableDelegate::MusicQueryTableDelegate(QObject *parent)
    : MusicCheckBoxDelegate(parent)
{

}

MusicQueryTableDelegate::~MusicQueryTableDelegate()
{

}

void MusicQueryTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->fillRect(option.rect, MStatic_cast(Qt::GlobalColor, index.data(MUSIC_AUDIT_ROLE).toInt()));
    MusicCheckBoxDelegate::paint(painter, option, index);
}



MusicProgressBarDelegate::MusicProgressBarDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_progress  = new QProgressBar;
    m_progress->setStyleSheet(MusicUIObject::MProgressBar01);
}

MusicProgressBarDelegate::~MusicProgressBarDelegate()
{
    delete m_progress;
}

void MusicProgressBarDelegate::setStyleSheet(const QString &style)
{
    m_progress->setStyleSheet(style);
}

QSize MusicProgressBarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    m_progress->resize(option.rect.width() - 21, option.rect.height() - 21);
    m_progress->setValue(index.data(MUSIC_PROCS_ROLE).toInt());
    painter->translate(10, 10);
    m_progress->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}


MusicLabelDelegate::MusicLabelDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_label  = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet(MusicUIObject::MBackgroundStyle18);
}

MusicLabelDelegate::~MusicLabelDelegate()
{
    delete m_label;
}

void MusicLabelDelegate::setStyleSheet(const QString &style)
{
    m_label->setStyleSheet(style);
}

QSize MusicLabelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicLabelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    m_label->setText(index.data(MUSIC_TEXTS_ROLE).toString());
    m_label->resize(option.rect.size());
    painter->translate(0, 0);
    m_label->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicPushButtonDelegate::MusicPushButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_pushButton  = new QPushButton;
    m_pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_pushButton->setStyleSheet(MusicUIObject::MBorderStyle04 + MusicUIObject::MBorderStyle07 + MusicUIObject::MBackgroundStyle17);
#ifdef Q_OS_UNIX
    m_pushButton->setFocusPolicy(Qt::NoFocus);
#endif
}

MusicPushButtonDelegate::~MusicPushButtonDelegate()
{
    delete m_pushButton;
}

void MusicPushButtonDelegate::setStyleSheet(const QString &style)
{
    m_pushButton->setStyleSheet(style);
}

QSize MusicPushButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicPushButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    m_pushButton->setText(index.data(MUSIC_TEXTS_ROLE).toString());
    m_pushButton->resize(option.rect.size() - QSize(10, 10));
    painter->translate(5, 5);
    m_pushButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicRenameLineEditDelegate::MusicRenameLineEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

QWidget *MusicRenameLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QString text;
    const QAbstractItemModel *model = index.model();
    if(model)
    {
        text = model->data(index, Qt::DisplayRole).toString();
    }

    MusicSongsToolItemRenamedWidget *edit = new MusicSongsToolItemRenamedWidget(text, parent);
    return edit;
}
