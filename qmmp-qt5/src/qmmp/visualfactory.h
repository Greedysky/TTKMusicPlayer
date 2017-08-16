#ifndef VISUALFACTORY_H
#define VISUALFACTORY_H

#include <QObject>

class QWidget;
class QTranslator;
class QDialog;
class Visual;

/*! @brief Helper class to store visual plugin properies.
 *  @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT VisualProperties
{
public:
    /*!
     * Constructor
     */
    VisualProperties()
    {
        hasAbout = false;
        hasSettings = false;
    }
    QString name;      /*!< Effect plugin full name */
    QString shortName; /*!< Effect plugin short name for internal usage */
    bool hasAbout;     /*!< Should be \b true if plugin has about dialog, otherwise returns \b false */
    bool hasSettings;  /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};
/*! @brief %Visual plugin interface (visual factory).
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class VisualFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~VisualFactory() {}
     /*!
     * Returns visual plugin properties.
     */
    virtual const VisualProperties properties() const = 0;
    /*!
     * Creates visualization provided by plugin.
     * @param parent Parent object.
     */
    virtual Visual *create(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(VisualFactory, "VisualFactory/1.0")

#endif
