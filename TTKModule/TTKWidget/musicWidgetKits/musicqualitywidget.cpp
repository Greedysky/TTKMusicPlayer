#include "musicqualitywidget.h"
#include "musicqualitywidgetuiobject.h"
#include "musicnumberutils.h"

#include <qmmp/soundcore.h>

MusicQualityWidget::MusicQualityWidget(QWidget *parent)
    : QLabel(parent)
{
    setToolTip(tr("Quality"));
    setFixedSize(48, 20);
    setStyleSheet(TTK::UI::LabelQuality);
}

void MusicQualityWidget::updateQuality()
{
    if(!SoundCore::instance())
    {
        return;
    }

    const auto &properties = SoundCore::instance()->trackInfo().properties();
    if(!properties.contains(Qmmp::BITRATE))
    {
        return;
    }

    QString style;
    switch(TTK::Number::bitrateToLevel(properties[Qmmp::BITRATE].toInt()))
    {
        case TTK::QueryQuality::Standard:
        {
            style = "margin-left:-48px;";
            break;
        }
        case TTK::QueryQuality::High:
        {
            style = "margin-left:-96px;";
            break;
        }
        case TTK::QueryQuality::Super:
        {
            style = "margin-left:-144px;";
            break;
        }
        case TTK::QueryQuality::Lossless:
        {
            style = "margin-left:-192px;";
            break;
        }
        default: break;
    }

    setStyleSheet(styleSheet() + style);
}
