#include "musicitemdelegate.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicwidgetheaders.h"

MusicRadioButtonDelegate::MusicRadioButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_treeMode = false;
    m_radioButton  = new QRadioButton;
    m_radioButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
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

void MusicRadioButtonDelegate::setTreeModel(bool tree)
{
    m_treeMode = tree;
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

    if(m_treeMode && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    const int minSize = qMin(option.rect.width(), option.rect.height());
    m_radioButton->resize(minSize, minSize);
    m_radioButton->setChecked(TTKStatic_cast(Qt::CheckState, index.data(MUSIC_CHECK_ROLE).toInt()) == Qt::Checked);
    painter->translate((option.rect.width() - 16) / 2, 0); // top left

    m_radioButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}


MusicCheckBoxDelegate::MusicCheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_background = false;
    m_textMode = false;
    m_treeMode = false;
    m_checkBox  = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
#endif
}

MusicCheckBoxDelegate::~MusicCheckBoxDelegate()
{
    delete m_checkBox;
}

void MusicCheckBoxDelegate::setStyleSheet(const QString &style)
{
    m_checkBox->setStyleSheet(style);
}

void MusicCheckBoxDelegate::showTextMode(bool mode)
{
    m_textMode = mode;
}

void MusicCheckBoxDelegate::setTreeModel(bool tree)
{
    m_treeMode = tree;
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

    if(m_treeMode && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    if(m_textMode)
    {
        m_checkBox->resize(option.rect.size());
        m_checkBox->setText(index.data(MUSIC_TEXT_ROLE).toString());
        m_checkBox->setEnabled(index.data(MUSIC_ENABLE_ROLE).toBool());
    }
    else
    {
        const int minSize = qMin(option.rect.width(), option.rect.height());
        m_checkBox->resize(minSize, minSize);
    }

    const bool state = m_checkBox->isChecked();
    m_checkBox->setChecked(TTKStatic_cast(Qt::CheckState, index.data(MUSIC_CHECK_ROLE).toInt()) == Qt::Checked);
    if(m_checkBox->isEnabled() && state != m_checkBox->isChecked())
    {
        Q_EMIT TTKConst_cast(MusicCheckBoxDelegate*, this)->buttonChecked();
    }
    painter->translate(m_textMode ? 0 : (option.rect.width() - 16) / 2, 0);

    m_checkBox->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicProgressBarDelegate::MusicProgressBarDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_treeMode = false;
    m_progress = new QProgressBar;
    m_progress->setStyleSheet(MusicUIObject::MQSSProgressBar01);
}

MusicProgressBarDelegate::~MusicProgressBarDelegate()
{
    delete m_progress;
}

void MusicProgressBarDelegate::setStyleSheet(const QString &style)
{
    m_progress->setStyleSheet(style);
}

void MusicProgressBarDelegate::setTreeModel(bool tree)
{
    m_treeMode = tree;
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

    if(m_treeMode && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    m_progress->resize(option.rect.width() - 21, option.rect.height() - 21);
    m_progress->setValue(index.data(MUSIC_PROGRESS_ROLE).toInt());
    painter->translate(10, 10);

    m_progress->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}


MusicLabelDelegate::MusicLabelDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_treeMode = false;
    m_label  = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet(MusicUIObject::MQSSBackgroundStyle13);
}

MusicLabelDelegate::~MusicLabelDelegate()
{
    delete m_label;
}

void MusicLabelDelegate::setStyleSheet(const QString &style)
{
    m_label->setStyleSheet(style);
}

void MusicLabelDelegate::setAlignment(Qt::Alignment alignment)
{
    m_label->setAlignment(alignment);
}

void MusicLabelDelegate::setTreeModel(bool tree)
{
    m_treeMode = tree;
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

    if(m_treeMode && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    const QColor &color = index.data(Qt::TextColorRole).value<QColor>();
    if(color.isValid())
    {
        QPalette pal(m_label->palette());
        pal.setColor(QPalette::WindowText, color);
        m_label->setPalette(pal);
    }

    m_label->setText(index.data(MUSIC_TEXT_ROLE).toString());
    m_label->resize(option.rect.size());
    painter->translate(0, 0);

    m_label->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicPushButtonDelegate::MusicPushButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_treeMode = false;
    m_pushButton  = new QPushButton;
    m_pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_pushButton->setStyleSheet(MusicUIObject::MQSSBorderStyle03 + MusicUIObject::MQSSBorderStyle06 + MusicUIObject::MQSSBackgroundStyle12);
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

void MusicPushButtonDelegate::setTreeModel(bool tree)
{
    m_treeMode = tree;
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

    if(m_treeMode && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    m_pushButton->setText(index.data(MUSIC_TEXT_ROLE).toString());
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
