#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QDialog>

namespace Ui {
class ColorWidget;
}

/*! @brief The class of the color picker widget.
 * @author Greedysky <greedysky@163.com>
 */
class ColorWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~ColorWidget();

    static QList<QColor> readColorConfig(const QString &value);
    /*!
     * Read color config.
     */
    static QString writeColorConfig(const QList<QColor> &colors);
    /*!
     * Write color config.
     */

    void setColors(const QList<QColor> &colors);
    /*!
     * Set current colors.
     */
    QList<QColor> getColors() const;
    /*!
     * Get selected colors.
     */

public Q_SLOTS:
    void addButtonClicked();
    /*!
     * Add button clicked.
     */
    void deleteButtonClicked();
    /*!
     * Delete button clicked.
     */
    void modifyButtonClicked();
    /*!
     * Modify button clicked.
     */
    void upButtonClicked();
    /*!
     * Up button clicked.
     */
    void downButtonClicked();
    /*!
     * Down button clicked.
     */

protected:
    Ui::ColorWidget *m_ui;

};

#endif // ColorWidget_H
