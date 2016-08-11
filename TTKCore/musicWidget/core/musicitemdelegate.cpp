#include "musicitemdelegate.h"
#include "musicuiobject.h"
#include "musicobject.h"

#include <QPainter>
#include <QCheckBox>
#include <QRadioButton>
#include <QProgressBar>
#include<QDebug>

MusicRadioButtonDelegate::MusicRadioButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_radioButton  = new QRadioButton;
    m_radioButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
}

MusicRadioButtonDelegate::~MusicRadioButtonDelegate()
{
    delete m_radioButton;
}

QString MusicRadioButtonDelegate::getClassName()
{
    return staticMetaObject.className();
}

void MusicRadioButtonDelegate::setStyleSheet(const QString &style)
{
    m_radioButton->setStyleSheet(style);
}

QSize MusicRadioButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicRadioButtonDelegate::paint(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    int minSize = qMin(option.rect.width(), option.rect.height());
    m_radioButton->resize(minSize, minSize);
    m_radioButton->setChecked( index.data(MUSIC_CHECK_ROLE).toBool() );
    painter->translate((option.rect.width() - 16)/2, 0); // top left
    m_radioButton->render(painter, option.rect.topLeft(), QRegion(),
                          QWidget::DrawChildren);
    painter->restore();
}


MusicCheckBoxDelegate::MusicCheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_checkBox  = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
}

MusicCheckBoxDelegate::~MusicCheckBoxDelegate()
{
    delete m_checkBox;
}

QString MusicCheckBoxDelegate::getClassName()
{
    return staticMetaObject.className();
}

void MusicCheckBoxDelegate::setStyleSheet(const QString &style)
{
    m_checkBox->setStyleSheet(style);
}

QSize MusicCheckBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicCheckBoxDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    int minSize = qMin(option.rect.width(), option.rect.height());
    m_checkBox->resize(minSize, minSize);
    m_checkBox->setChecked( index.data(MUSIC_CHECK_ROLE).toBool() );
    painter->translate((option.rect.width() - 16)/2, 0); // top left
    m_checkBox->render(painter, option.rect.topLeft(), QRegion(),
                       QWidget::DrawChildren);
    painter->restore();
}


MusicQueryTableDelegate::MusicQueryTableDelegate(QObject *parent)
    : MusicCheckBoxDelegate(parent)
{

}

MusicQueryTableDelegate::~MusicQueryTableDelegate()
{

}

QString MusicQueryTableDelegate::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryTableDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    painter->fillRect(option.rect,
                      MStatic_cast(Qt::GlobalColor, index.data(MUSIC_AUDIT_ROLE).toInt()));
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

QString MusicProgressBarDelegate::getClassName()
{
    return staticMetaObject.className();
}

void MusicProgressBarDelegate::setStyleSheet(const QString &style)
{
    m_progress->setStyleSheet(style);
}

QSize MusicProgressBarDelegate::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicProgressBarDelegate::paint(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    painter->save();
    m_progress->resize(option.rect.width() - 21, option.rect.height() - 21);
    m_progress->setValue(index.data(MUSIC_PROCS_ROLE).toInt());
    painter->translate(10, 10);
    m_progress->render(painter, option.rect.topLeft(), QRegion(),
                       QWidget::DrawChildren);
    painter->restore();
}

