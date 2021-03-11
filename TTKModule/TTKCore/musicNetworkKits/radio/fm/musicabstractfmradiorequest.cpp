#include "musicabstractfmradiorequest.h"

MusicAbstractFMRadioRequest::MusicAbstractFMRadioRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

MusicAbstractFMRadioRequest::~MusicAbstractFMRadioRequest()
{
    deleteAll();
}
