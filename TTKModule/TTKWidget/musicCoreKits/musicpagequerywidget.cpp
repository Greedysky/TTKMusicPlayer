#include "musicpagequerywidget.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

static constexpr int PAGE_SIZE = 10;

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

    for(int i = 1; i <= PAGE_SIZE; ++i)
    {
        m_pageItems << (new TTKClickedLabel(QString::number(i), m_pageWidget));
    }
    m_pageItems << (new TTKClickedLabel("<<", m_pageWidget)) << (new TTKClickedLabel(">>", m_pageWidget));

    TTKClickedGroup *group = new TTKClickedGroup(this);
    connect(group, SIGNAL(clicked(int)), SIGNAL(clicked(int)));

    for(TTKClickedLabel *w : qAsConst(m_pageItems))
    {
        QFont font(w->font());
        font.setPixelSize(14);
        w->setFont(font);
        w->setStyleSheet(TTK::UI::ColorStyle04);
        w->setFixedWidth(TTK::Widget::fontTextWidth(font, w->text()));
        group->mapped(w);
    }

    m_pageItems[PAGE_SIZE]->hide();
    if(total <= PAGE_SIZE)
    {
        m_pageItems[PAGE_SIZE + 1]->hide();
        for(int i = PAGE_SIZE - 1; i >= total; --i)
        {
            m_pageItems[i]->hide();
        }
    }
    m_pageItems[0]->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle01);

    layout->addStretch(1);
    if(total != 0)
    {
        layout->addWidget(m_pageItems[PAGE_SIZE]);
        for(int i = 0; i < PAGE_SIZE; ++i)
        {
            layout->addWidget(m_pageItems[i]);
        }
        layout->addWidget(m_pageItems[PAGE_SIZE + 1]);
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

    for(TTKClickedLabel *label : qAsConst(m_pageItems))
    {
        label->hide();
    }
    m_currentPage = 0;

    if(total > PAGE_SIZE)
    {
        total = PAGE_SIZE;
        m_pageItems[PAGE_SIZE + 1]->show();
    }

    for(int i = 0; i < total; ++i)
    {
        m_pageItems[i]->setText(QString::number(i + 1));
        m_pageItems[i]->show();
    }
}

void MusicPageQueryWidget::page(int index, int total)
{
    if(total <= 0)
    {
        return;
    }

    int page = m_pageItems[0]->text().toInt();
    for(int i = 0; i < m_pageItems.count() - 2; ++i)
    {
        m_pageItems[i]->setStyleSheet(TTK::UI::ColorStyle04);
    }

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
            page -= PAGE_SIZE;
            TTKClickedLabel *w = m_pageItems[PAGE_SIZE];

            if(total <= PAGE_SIZE)
            {
                w->hide();
            }
            else
            {
                for(int i = 0; i < PAGE_SIZE; ++i)
                {
                    m_pageItems[i]->setText(QString::number(page + i));
                    m_pageItems[i]->show();
                }
                (m_pageItems[0]->text().toInt() < PAGE_SIZE) ? w->hide() : w->show();
            }

            m_currentPage = 0;
            m_pageItems[PAGE_SIZE + 1]->show();
            break;
        }
        case 11:
        {
            page += PAGE_SIZE;
            TTKClickedLabel *w = m_pageItems[PAGE_SIZE + 1];
            int boundary = total - page + 1;
                boundary = boundary < PAGE_SIZE ? boundary : PAGE_SIZE;

            for(int i = 0; i < boundary; ++i)
            {
                m_pageItems[i]->setText(QString::number(page + i));
            }

            if(total - page >= PAGE_SIZE)
            {
                w->show();
            }
            else
            {
                w->hide();
                for(int i = PAGE_SIZE - 1; i > total - page; --i)
                {
                    m_pageItems[i]->hide();
                }
            }

            m_currentPage = 0;
            m_pageItems[PAGE_SIZE]->show();
            break;
        }
        default: break;
    }

    for(int i = 0; i < m_pageItems.count() - 2; ++i)
    {
        TTKClickedLabel *w = m_pageItems[i];
        w->setFixedWidth(TTK::Widget::fontTextWidth(w->font(), w->text()));
    }

    TTKClickedLabel *w = m_pageItems[m_currentPage];
    w->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle01);
    w->setFixedWidth(w->width() + 5);
}

int MusicPageQueryWidget::currentIndex() const
{
    return m_pageItems[m_currentPage]->text().toInt();
}
