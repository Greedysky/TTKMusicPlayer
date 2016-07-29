#include "kugouurl.h"

KugouUrl::KugouUrl()
{

}

QString KugouUrl::getClassName()
{
    return "KugouUrl";
}

QString KugouUrl::getSongYuekuUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/default.html?ver=8010";
}

QString KugouUrl::getSongRecommendUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/home.html";
}

QString KugouUrl::getSongRankUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/rank/home/1-22943.html";
}

QString KugouUrl::getSongSingerUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/singer/index/1-all-0-0_0.html";
}

QString KugouUrl::getSongCategoryUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/categoryv2/index.html";
}

QString KugouUrl::getSongShowUrl()
{
    return "http://www2.kugou.kugou.com/show/v9/html/index.html";
}

QString KugouUrl::getSongHeroesUrl()
{
    return "http://huodong.kugou.com/a2016/html/HeroesOfRemix.html";
}



QString KugouUrl::getRadioPublicUrl()
{
    return "http://www2.kugou.kugou.com/fm2/app/publicV2/html/recommend/index.html";
}

QString KugouUrl::getRadioHighFmUrl()
{
    return "http://www2.kugou.kugou.com/fm2/html/highFm.html";
}

QString KugouUrl::getRadioFxUrl()
{
    return "http://fanxing.kugou.com";
}

QString KugouUrl::getRadioHomeUrl()
{
    return "http://ugcfm.kugou.com/fm2/html/ugc/home.html";
}



QString KugouUrl::getListUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/special/index/getData.js?cdn=cdn&t=5&c=";
}



QString KugouUrl::getMVRadioUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/html/mvradio.html";
}

QString KugouUrl::getMVRecommendUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/mtv/index/getData.js?cdn=cdn";
}

QString KugouUrl::getMVFanxingUrl()
{
    return "http://fanxing.kugou.com/index.php?action=embedPageMv";
}

QString KugouUrl::getMVStarMusicUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/mtv/starmusic/getData.js?cdn=cdn";
}
