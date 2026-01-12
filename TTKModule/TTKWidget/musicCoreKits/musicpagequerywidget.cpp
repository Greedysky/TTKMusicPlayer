#include "musicpagequerywidget.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

static constexpr int MAX_PAGE_SIZE = 10;

MusicPageQueryWidget::MusicPageQueryWidget(QObject *parent)
    : QObject(parent),
      m_currentPage(0),
      m_pageWidget(nullptr)
{

}

MusicPageQueryWidget::~MusicPageQueryWidget()
{
    qDeleteAll(m_pageItems);
    delete m_pageWidget;
}

QWidget* MusicPageQueryWidget::createPageWidget(QWidget *parent, int total)
{
    m_pageWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(m_pageWidget);
    layout->setContentsMargins(0, 20, 0, 20);
    layout->setSpacing(12);

    for(int i = 1; i <= MAX_PAGE_SIZE; ++i)
    {
        m_pageItems << (new TTKClickedLabel(QString::number(i), m_pageWidget));
    }
    m_pageItems << (new TTKClickedLabel("<<", m_pageWidget)) << (new TTKClickedLabel(">>", m_pageWidget));

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SIGNAL(clicked(int)));

    for(TTKClickedLabel *label : qAsConst(m_pageItems))
    {
        QFont font(label->font());
        font.setPixelSize(14);
        label->setFont(font);
        label->setStyleSheet(TTK::UI::ColorStyle04);
        label->setFixedWidth(TTK::Widget::fontTextWidth(font, label->text()));
        clickedGroup->mapped(label);
    }

    m_pageItems[MAX_PAGE_SIZE]->hide();
    if(total <= MAX_PAGE_SIZE)
    {
        m_pageItems[MAX_PAGE_SIZE + 1]->hide();

        for(int i = MAX_PAGE_SIZE - 1; i >= total; --i)
        {
            m_pageItems[i]->hide();
        }
    }

    m_pageItems[0]->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle01);
    layout->addStretch(1);

    if(total != 0)
    {
        layout->addWidget(m_pageItems[MAX_PAGE_SIZE]);

        for(int i = 0; i < MAX_PAGE_SIZE; ++i)
        {
            layout->addWidget(m_pageItems[i]);
        }

        layout->addWidget(m_pageItems[MAX_PAGE_SIZE + 1]);
    }
    else
    {
        QWidget *func = new QWidget(m_pageWidget);
        QVBoxLayout *funcLayout = new QVBoxLayout(func);

        QLabel *icon = new QLabel(func);
        icon->setPixmap(QPixmap(":/lrc/lb_no_results"));
        QLabel *text = new QLabel(tr("There is empty"), func);
        text->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle04);

        text->setAlignment(Qt::AlignCenter);
        funcLayout->addWidget(icon, 0, Qt::AlignCenter);
        funcLayout->addWidget(text, 0, Qt::AlignCenter);
        func->setLayout(funcLayout);
        layout->addWidget(func);
    }

    layout->addStretch(1);
    m_pageWidget->setLayout(layout);
    return m_pageWidget;
}

void MusicPageQueryWidget::reset(int total)
{
    if(m_pageItems.isEmpty())
    {
        return;
    }

    m_currentPage = 0;
    for(TTKClickedLabel *label : qAsConst(m_pageItems))
    {
        label->hide();
    }

    if(total > MAX_PAGE_SIZE)
    {
        total = MAX_PAGE_SIZE;
        m_pageItems[MAX_PAGE_SIZE + 1]->show();
    }

    for(int i = 0; i < total; ++i)
    {
        m_pageItems[i]->setText(QString::number(i + 1));
        m_pageItems[i]->show();
    }

    updateStatus();
}

void MusicPageQueryWidget::page(int index, int total)
{
    if(total <= 0 || m_pageItems.isEmpty())
    {
        return;
    }

    int page = m_pageItems[0]->text().toInt();

    switch(index)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            m_currentPage = index;
            break;
        }
        case 10:
        {
            page -= MAX_PAGE_SIZE;
            TTKClickedLabel *label = m_pageItems[MAX_PAGE_SIZE];

            if(total <= MAX_PAGE_SIZE)
            {
                label->hide();
            }
            else
            {
                for(int i = 0; i < MAX_PAGE_SIZE; ++i)
                {
                    m_pageItems[i]->setText(QString::number(page + i));
                    m_pageItems[i]->show();
                }

                m_pageItems[0]->text().toInt() < MAX_PAGE_SIZE ? label->hide() : label->show();
            }

            m_currentPage = 0;
            m_pageItems[MAX_PAGE_SIZE + 1]->show();
            break;
        }
        case 11:
        {
            page += MAX_PAGE_SIZE;
            TTKClickedLabel *label = m_pageItems[MAX_PAGE_SIZE + 1];
            int boundary = total - page + 1;
                boundary = boundary < MAX_PAGE_SIZE ? boundary : MAX_PAGE_SIZE;

            for(int i = 0; i < boundary; ++i)
            {
                m_pageItems[i]->setText(QString::number(page + i));
            }

            if(total - page >= MAX_PAGE_SIZE)
            {
                label->show();
            }
            else
            {
                label->hide();
                //
                for(int i = MAX_PAGE_SIZE - 1; i > total - page; --i)
                {
                    m_pageItems[i]->hide();
                }
            }

            m_currentPage = 0;
            m_pageItems[MAX_PAGE_SIZE]->show();
            break;
        }
        default: break;
    }

    updateStatus();
}

int MusicPageQueryWidget::currentIndex() const
{
    return m_pageItems[m_currentPage]->text().toInt();
}

void MusicPageQueryWidget::updateStatus()
{
    for(int i = 0; i < m_pageItems.count() - 2; ++i)
    {
        TTKClickedLabel *label = m_pageItems[i];
        label->setStyleSheet(TTK::UI::ColorStyle04);
        label->setFixedWidth(TTK::Widget::fontTextWidth(label->font(), label->text()));
    }

    TTKClickedLabel *label = m_pageItems[m_currentPage];
    label->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle01);
    label->setFixedWidth(label->width() + 5);
}
