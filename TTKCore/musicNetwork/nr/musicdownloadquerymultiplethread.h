#ifndef MUSICDOWNLOADQUERYMULTIPLETHREAD_H
#define MUSICDOWNLOADQUERYMULTIPLETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_REQUERY_MULTI_WY = "V2ZZckt0dGdZS1JmSStMcmZONDNTSm1mNk5DKzcxSmY5bktjQXlpOWNHOWwwZnJoZTB3OVp2aDBEZkpScHJidUs3c2Y2SWhSbWgwQUtyNDg2VWk3RnNESUZLU2hWaGJQVzd2TFloZmIxWms5OGxTMGZBdlc5NXlLWVhURXZEMGc3UUd6ZHNDd2JQQUwvYWYx";    //wangyiMusic
const QString MUSIC_REQUERY_MULTI_DX = "c1dCWStiL1ZqbXhiekg0NHp0bnRlQXVDOEI2QXk5MnhsY1hXMlBpbk9WTlZDaUZRWW9sS1FVRndGUTBkd3F1a25XOUhOR3RjUHlOUXdFTUpHMHkvREJVTHB1QWxTZmRNZERMYVkyT0E2V01nbFlwNTFnK1pvdXErbS9Od3hMSWhPYXFjaU9odlJ3Ym0xYXg1";    //dianxinMusic
const QString MUSIC_REQUERY_MULTI_QQ = "ZFZOdkVRVGI4SUZLdXJYRThKdlQrR3pLSEVRYUtBSmxDVWlLcml2THJzRzh0TkRSVmt3QkFXOW5PRko3SVpNeVplMzhsMzNSQnlOOWhjT3pNQmE2bjc4ZTVwc1BsTTB4SUdrbVlkUEZ5TlJPZjFBbk0zbTN0RHY4UElsd3BFSDZEb293dU82UEVaUHkvNW16";    //qqMusic
const QString MUSIC_REQUERY_MULTI_XM = "MTZuajNqeUNYYzhVSllVUUFsQWdPcFpTck1ZODNnYk9DT1JVSEw5ZnZzc1NNdkZ5ekNhOGc0d2FYUi9GNXVrQSt5QmJWc0UwckM2QmV2bkRMK2I5enBEVjBraDRTSGxYYVVhbGtkV0IyOUdVR1lUZ0t3cHgvd3ViT05CTjJoSE9YV0l4Wm9LQytqcUVabnJi";    //xiamiMusic
const QString MUSIC_REQUERY_MULTI_TT = "ZWx3dUZQSlJleGZQV2pDNTdiRHE2dmJkMlRDR2VuMkorMDZmRUpHU0ZMbVovL2hmcHVZbjZaZ1djcC9GVDU1M3pXSGkvR2Jaa2lCZGtOMXVTd3p6SnJsdFZuT1E3TlB5aXBKYlJiSFJyeTJzTlZyODNuVXBaNDdabnVhSWRhd25ENE1MV3Vpa0Zud21QeVpx";    //ttpodMusic
const QString MUSIC_REQUERY_MULTI_BD = "SHdlYWszQTV3Rm43SlhiMkJQT0pWazJWV1FjbE4xZ3N5cmlWMlRFSGtRdEkyS21kRHZ3bkdTMTNQMkFHS3dYUVRNREM0V2VvME1Ec245bTluSFpndTIySU8yRWM2bEdEblFpQ05MYll1REtLKzdzOW1XM3NYNm5CbDJaL3VsWWYzdGNuUzlLbHU0NFhIUUsy";    //baiduMusic
const QString MUSIC_REQUERY_MULTI_KW = "OXh0T2V2c0N4RzlKQzFFWDc0bm9lRE52ajdsRlpLVllTRE9rTDVLYXh2b3ZjRmpGVmxCbndOS3lIMVhya0pFSXZRdUhvRmhOdWE4Z1dSWUNZbFFTcTR6UERkN3FDNjNRcmxmMXZTV2dIRFcvTTJsdnBDUHhINlNuSk9RWm56aTU1RGpvVVVxT1ZVUy8raDhw";    //kuwoMusic
const QString MUSIC_REQUERY_MULTI_KG = "WWo4Ly9CdjE1d0hXUkJvb3ZXT0d6NjdKWEU2K3BMNVNaQ2MvTEIxVmdKVzkyTWFzbEpCTnBpeDZ2ZWRFZkNzUExjZThwTGh3Yzg5aktFanZILy94dkJacWhTejBvQ3g1MU9QWE9MZ3RIbW03VWs4QTk2M1NvNGVQcDNEbHNlQlR1UHJUUnk5d3pIYkovSEd2";    //kuhouMusic
const QString MUSIC_REQUERY_MULTI_DM = "SnRKMDVla2RwTXl5ZS9zM0Mvbk1Yb05pU0hVR0g4bHJ5R0JpaTBZWGZQUkVDa3N1YUVNemZMMXRka0hFT1JOWWxtSlpRK3lMMHpEdW5NNkFxdnZWdkYwRWFOMWJIVmlSbVBOQlU2MG5NamVCODFPeCtIL3B2Tnducm9xWXBISmhYUG5VU3lmTmx2TGxLc2hZ";    //duomiMusic
const QString MUSIC_REQUERY_MULTI_MG = "Ty9qM0s0TmpOT3phdExPSjJPQS84TUFxbFNuSmovSkFjSk9DTWxpRmZYc255bGVpdisxUWg0aWI4aEtNTm5PMk9HNVY2Mmc2UkFtS1lBdHkwSHZ6elJrWklYVDVMTDkxN2xaUm83QmR5Z0VWM0RVVDFCQWhhbFE2bWlaWjVCdDBuSlNKOWpJZXZvWVluRXh5";    //miguMusic
const QString MUSIC_REQUERY_MULTI_MU = "c2lTOWtKNXNJMWU1UVQ4NCtwR2N3V1pTaDAvcEZLM05PSnNWeTR4YzBGUFI2VHBHQXE5R2tUNEQwS3I0aFFFMk9XRWpQcVJCYzNEQ3dDbUk2SENzNVp2My9FVU1mTmEyNnFISS9jT3RXaVhwRU1qaG85TjFYei9IZUllMWxLMVVLWXRkdzU1THdOVzZpTnNs";    //dingdongMusic
const QString MUSIC_REQUERY_MULTI_EC = "c2NTbXFCVXhQOTFDUFBtU20wd3AvV3hrRVVKdGdrbGluMFM1YUhnRStNendQQk5qd0t4eE1jZjBCa090MUJXcmxTMDBNbjZLWWd3Wm9TVHZTMDdUUHZsNFM5dS95dlppV1hqNWRXVmxvV2RmWWtWamx2L2MxWnRUUkh1elE2NExVMzByMkpPdm5qaTA5Ni9a";    //echoMusic
const QString MUSIC_REQUERY_MULTI_YY = "UEZUbGt3cmtNaytWamgrbG0xRFplNXphR3Z3a2xuWVpWckFiNGR4a2FiN3lkSTlXcmc2eTZxaitxOUg4T2ZobURlckd4L3AxeEttNE5JMnNrMVVNOXBoQjlMcDlJdGZZYnYyZnN6MmVXVmNKckVFRzdiZWVCdmJRaFZ5NkxpeWhjbEs1czcvWEs2Mnlzb3Fy";    //yinyuetaiMusic


/*! @brief The class to multiple query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryMultipleThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryMultipleThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startSearchSong(QueryType type, const QString &text) override;
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    QString getCurrentURL() const;
    /*!
     * Get current url the user selected from mutiple queue containers.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info, const QString &size,
                                    int bit, const QString &url);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */

};

#endif // MUSICDOWNLOADQUERYMULTIPLETHREAD_H
