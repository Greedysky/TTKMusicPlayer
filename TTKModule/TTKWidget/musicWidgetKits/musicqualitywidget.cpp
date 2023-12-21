#include "musicqualitywidget.h"
#include "musicqualitywidgetuiobject.h"
#include "musicnumberutils.h"

#include <qmmp/statehandler.h>

MusicQualityWidget::MusicQualityWidget(QWidget *parent)
    : QLabel(parent)
{
    setToolTip(tr("Quality"));
    setFixedSize(48, 20);
    setStyleSheet(TTK::UI::LabelQuality + "margin-left:-48px;");

    connect(StateHandler::instance(), SIGNAL(bitrateChanged(int)), SLOT(updateQuality(int)));
}

void MusicQualityWidget::updateQuality(int bitrate)
{
    QString style;
    switch(TTK::Number::bitrateToLevel(bitrate))
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
