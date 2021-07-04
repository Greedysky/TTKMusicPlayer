#include "musictranslationrequest.h"

MusicTranslationRequest::MusicTranslationRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_reply = nullptr;
}
