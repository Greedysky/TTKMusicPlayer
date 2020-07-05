#include "qkugouurl.h"

QKugouUrl::QKugouUrl()
{

}

QString QKugouUrl::getSongYuekuUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/default.html?ver=9173";
}

QString QKugouUrl::getSongRecommendUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/home.html?ver=9173";
}

QString QKugouUrl::getSongRankUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/rank/home/1-6666.html?ver=9173";
}

QString QKugouUrl::getSongSingerUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/singer/index/1-all-0-0_0.html?ver=9173";
}

QString QKugouUrl::getSongCategoryUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/categoryv2/index.html?ver=9173";
}



QString QKugouUrl::getRadioPublicUrl()
{
    return "http://www2.kugou.kugou.com/fm2/app/public/html/recommend/index.html?ver=9173";
}

QString QKugouUrl::getRadioHighFmUrl()
{
    return "http://www2.kugou.kugou.com/fm2/html/highFm.html?ver=9173";
}

QString QKugouUrl::getRadioFxUrl()
{
    return "http://fanxing.kugou.com";
}



QString QKugouUrl::getListUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/special/index/getData.js?cdn=cdn&t=5&c=&ver=9173";
}



QString QKugouUrl::getMVRadioUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/html/mvradio.html?ver=9173";
}

QString QKugouUrl::getMVRecommendUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/mtv/index/getData.js?cdn=cdn&ver=9173";
}

QString QKugouUrl::getMVFanxingUrl()
{
    return "http://fanxing.kugou.com/index.php?action=embedPageMv&ver=9173";
}



QString QKugouUrl::getWebPlayerUrl()
{
    return "http://web.kugou.com/index.html?ver=9173";
}



QString QKugouUrl::getKuiSheUrl()
{
    return "http://huodong.kugou.com/2014/html/kuishe.html?ver=9173";
}
