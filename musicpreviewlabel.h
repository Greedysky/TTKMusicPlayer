#ifndef MUSICPREVIEWLABEL_H
#define MUSICPREVIEWLABEL_H

#include <QLabel>
#include "core/musiclibexportglobal.h"

class MUSIC_EXPORT MusicPreviewLabel : public QLabel
{

public:
    MusicPreviewLabel(QWidget *parent = 0);

    void setParameter(const QStringList& para);
    void setLinearGradient(QColor &fg, QColor &bg);
    void setTransparent(int trans) { m_transparent = trans;}

protected:
    virtual void paintEvent(QPaintEvent *);
    QFont m_font;
    QLinearGradient m_linearGradient;
    QLinearGradient m_maskLinearGradient;
    int m_transparent;

};

#endif // MUSICPREVIEWLABEL_H
