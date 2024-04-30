#include "qkugouurl.h"

QString QKugouUrl::makeSongYuekuUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/default.html?ver=10053";
}

QString QKugouUrl::makeSongRecommendUrl()
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/home.html?ver=10053";
}

QString QKugouUrl::makeRadioPublicUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/html/mvradio.html?ver=10053";
}

QString QKugouUrl::makeMovieRecommendUrl()
{
    return "http://www2.kugou.kugou.com/mv/v9/mtv/index/getData.js?cdn=cdn&ver=10053";
}

QString QKugouUrl::makeKuiSheUrl()
{
    return "http://huodong.kugou.com/2014/html/kuishe.html?ver=10053";
}
