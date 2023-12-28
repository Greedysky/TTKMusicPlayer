#include "musicabstracttranslationrequest.h"

MusicAbstractTranslationRequest::MusicAbstractTranslationRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicAbstractTranslationRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    deleteLater();
}
