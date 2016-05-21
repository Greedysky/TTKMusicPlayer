#include "KugouUrl.h"

KugouUrl::KugouUrl()
{

}

QString KugouUrl::getYuekuUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/default.html?ver=8010";
}

QString KugouUrl::getRecommendUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/home.html";
}

QString KugouUrl::getRankUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/rank/home/1-22943.html?close=1";
}

QString KugouUrl::getSingerUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/singer/index/1-all-0-0_0.html";
}

QString KugouUrl::getCategoryUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/category/songs/1-0-0-0-0.html";
}

QString KugouUrl::getShowUrl()
{
    return "http://www2.kugou.kugou.com/show/v9/html/index.html";
}
