#include <QTimer>
#include <QAction>
#include <QCloseEvent>
#include <QSettings>
#include <QFileInfo>
#include <QPluginLoader>
#include "visual.h"
#include "statehandler.h"
#include "visualfactory.h"
#include "visualbuffer_p.h"

class VisualPrivate
{
public:
    static void createVisualization(VisualFactory *factory, QWidget *parent)
    {
        Visual* visual = factory->create(parent);
        if(receiver && member)
            QObject::connect(visual, SIGNAL(closedByUser()), receiver, member);

        visual->setWindowFlags(Qt::Window);
        visualMap.insert(factory, visual);
        const Qmmp::State st = StateHandler::instance()->state();
        if(st == Qmmp::Playing || st == Qmmp::Buffering || st == Qmmp::Paused)
            visual->start();
        visuals.append(visual);
    //    visual->show();
    }

    static void checkFactories()
    {
        if(factories)
        {
            return;
        }

        factories = new QList<VisualFactory *>;
        files = new QHash<const VisualFactory*, QString>;

        for(const QString &filePath : Qmmp::findPlugins("Visual"))
        {
            QPluginLoader loader(filePath);
            QObject *plugin = loader.instance();
            if(loader.isLoaded())
                qDebug("Visual: loaded plugin %s", qPrintable(QFileInfo(filePath).fileName()));
            else
                qWarning("Visual: %s", qPrintable(loader.errorString()));

            VisualFactory *factory = nullptr;
            if(plugin)
                factory = qobject_cast<VisualFactory*>(plugin);

            if(factory)
            {
                factories->append(factory);
                files->insert(factory, filePath);
            }
        }
    }

    static QList<VisualFactory*> *factories;
    static QHash<const VisualFactory*, QString> *files;
    static QList<Visual*> visuals;
    static QHash<VisualFactory*, Visual*> visualMap; //internal visualization
    static QWidget *parentClass;
    static QObject *receiver;
    static const char *member;
    static VisualBuffer buffer;

};

QList<VisualFactory*> *VisualPrivate::factories = nullptr;
QHash <const VisualFactory*, QString> *VisualPrivate::files = nullptr;
QList<Visual*> VisualPrivate::visuals;
QHash<VisualFactory*, Visual*> VisualPrivate::visualMap;
QWidget *VisualPrivate::parentClass = nullptr;
QObject *VisualPrivate::receiver = nullptr;
const char *VisualPrivate::member = nullptr;
VisualBuffer VisualPrivate::buffer;


Visual::Visual(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, false);

    m_timer = new QTimer(this);
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    connect(m_timer, SIGNAL(timeout()), SLOT(updateVisual()));

    m_screenAction = new QAction(tr("Fullscreen"), this);
    m_screenAction->setCheckable(true);
    connect(m_screenAction, SIGNAL(triggered(bool)), this, SLOT(changeFullScreen(bool)));
}

Visual::~Visual()
{
    delete[] m_visualData;
}

void Visual::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void Visual::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void Visual::closeEvent(QCloseEvent *event)
{
    VisualPrivate::visuals.removeAll(this);
    if(event->spontaneous() && VisualPrivate::visualMap.key(this))
    {
        VisualFactory *factory = VisualPrivate::visualMap.key(this);
        VisualPrivate::visualMap.remove(factory);
        Visual::setEnabled(factory, false);
        emit closedByUser();
    }
    else
    {
        if(VisualPrivate::visualMap.key(this))
        {
            VisualFactory *factory = VisualPrivate::visualMap.key(this);
            VisualPrivate::visualMap.remove(factory);
        }
    }
    QWidget::closeEvent(event);
}

bool Visual::takeData(float *left, float *right)
{
    VisualPrivate::buffer.mutex()->lock();
    VisualNode *node = VisualPrivate::buffer.take();
    if(node)
    {
        if(left && right)
        {
            memcpy(left, node->data[0], QMMP_VISUAL_NODE_SIZE * sizeof(float));
            memcpy(right, node->data[1], QMMP_VISUAL_NODE_SIZE * sizeof(float));
        }
        else if(left && !right)
        {
            for(int i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
                left[i] = qBound(-1.0f, (node->data[0][i] + node->data[1][i]) / 2, 1.0f);
        }
    }
    VisualPrivate::buffer.mutex()->unlock();
    return node != nullptr;
}

void Visual::processState(bool v)
{
    Q_UNUSED(v);
}

void Visual::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

QList<VisualFactory *> Visual::factories()
{
    VisualPrivate::checkFactories();
    return *VisualPrivate::factories;
}

QString Visual::file(const VisualFactory *factory)
{
    VisualPrivate::checkFactories();
    return VisualPrivate::files->value(factory);
}

void Visual::setEnabled(VisualFactory *factory, bool enable)
{
    VisualPrivate::checkFactories();
    if(!VisualPrivate::factories->contains(factory))
        return;

    const QString &name = factory->properties().shortName;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QStringList visList = settings.value("Visualization/enabled_plugins").toStringList();

    if(enable)
    {
        if(!visList.contains(name))
            visList << name;
        if(!VisualPrivate::visualMap.value(factory) && VisualPrivate::parentClass)
        {
            VisualPrivate::createVisualization(factory, VisualPrivate::parentClass);
        }
    }
    else
    {
        visList.removeAll(name);
        if(VisualPrivate::visualMap.value(factory))
        {
            VisualPrivate::visuals.removeAll(VisualPrivate::visualMap.value(factory));
            VisualPrivate::visualMap.value(factory)->close();
            VisualPrivate::visualMap.remove(factory);
        }
    }
    settings.setValue("Visualization/enabled_plugins", visList);
}

bool Visual::isEnabled(const VisualFactory *factory)
{
    VisualPrivate::checkFactories();
    const QString &name = factory->properties().shortName;
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QStringList &visList = settings.value("Visualization/enabled_plugins").toStringList();
    return visList.contains(name);
}

void Visual::add(Visual *visual)
{
    if(!VisualPrivate::visuals.contains(visual))
    {
        const Qmmp::State st = StateHandler::instance() ? StateHandler::instance()->state() : Qmmp::Stopped;
        if(st == Qmmp::Playing || st == Qmmp::Buffering || st == Qmmp::Paused)
            visual->start();
        VisualPrivate::visuals.append(visual);
    }
}

void Visual::remove(Visual *visual)
{
    VisualPrivate::visuals.removeAll(visual);
}

void Visual::initialize(QWidget *parent, QObject *receiver, const char *member)
{
    VisualPrivate::receiver = receiver;
    VisualPrivate::member = member;
    VisualPrivate::parentClass = parent;

//    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
//    settings.setValue("Visualization/enabled_plugins", QStringList());
}

const QList<Visual *> &Visual::visuals()
{
    return VisualPrivate::visuals;
}

void Visual::addAudio(float *pcm, int samples, int channels, qint64 ts, qint64 delay)
{
    VisualPrivate::buffer.mutex()->lock();
    VisualPrivate::buffer.add(pcm, samples, channels, ts, delay);
    VisualPrivate::buffer.mutex()->unlock();
}

void Visual::clearBuffer()
{
    VisualPrivate::buffer.mutex()->lock();
    VisualPrivate::buffer.clear();
    VisualPrivate::buffer.mutex()->unlock();
}

void Visual::start()
{
    if(m_running)
    {
        return;
    }
    //
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void Visual::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void Visual::updateVisual()
{
    float left[QMMP_VISUAL_NODE_SIZE];
    float right[QMMP_VISUAL_NODE_SIZE];

    if(takeData(left, right))
    {
        processState(true);
        processData(left, right);
        update();
    }
    else
    {
        processState(false);
    }
}

void Visual::changeFullScreen(bool state)
{
    emit fullscreenByUser(this, state);
}
