#ifndef MUSICDOWNLOADQQINTERFACE_H
#define MUSICDOWNLOADQQINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicobject.h"
#include "musicglobaldefine.h"

///////////////////////////////////////////////////////////////////////////////
const QString QQ_ALBUM_URL          = "TERmU2h2ZUVKY1ZVUUtZdzZGb3QweDZQb3BPWmoyT0x5TzhCQXkrVHByZmRmdEcrMERkSnVMNnJvS1ZjamJKMEowbHZpTkhndk5ZZ1hpYWRLRWJIeVNPTTB5TndmWnRQeFJtNi8weVdqNjN2MFdZbVdnYjBIZUZtSk5QT01jNWM0eHEvWUtQdXN1cGVlcWFmUFB1K3hPSHlmTmV2N1JBWTE5QTY1ZFFnRjcrTFJtbXF4Z0x4aTRlNGkxQjFVY3I0enRZQ01EU3FrWDVjUmd1bURHUzl2dHNTTjFONDdTOFY=";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_SONG_SEARCH_URL    = "dksvcVdpRjJYWnlrMnUyUUU4Tkl0OHFzV1hMcVRmcnJib1IxRjRWNzdOWExGaTVmK2pGSUxtQ2pzQU9Tc3JDL0Rmc2NXWWFvQ3JyYzRKZkIrRHN4dWhJTjZLQkQyMkFjVVJOcFJlZENVZnBOUHhwVExrbWtJL0tHRmFmaGdWcnVYc09McWx2enFBSFlBMU9heEViSlRqSFgzSVJnSGlRVGhhTG0zMW5Ib21DWU1JV095UURZdG0zaTQ0V2diNEg3ZGZ4V2MxNkI0YnRMM0RMVnBIdjUremdXLzZld2U1bnBneHg0VG5qYU1wRDdrUG1TaHNHVUMzUVR1cW9yVlg5QWJwbFQ2clo2WmhkdFNYZVZVQngzZVd4T3FIWXpTOEt4SzhqcmNvdWRmNURXOGVzSGkrNVZIbGJxVFVJamY4WlhyZUg0NjZkL3k4bz0=";
const QString QQ_SONG_INFO_URL      = "VTl3Ykp1cUk5TEFvY1JIc1E2aUFneXV4Y1VGRlVxVFM5ZTI2a283bExrNzk3WFl6ZEdneVdVN0JDeC9jUFRwVG9lTDBCVVphNWNlQTVXcnovV0lSZnZFOEZQcDFMdnNOdW1naitVdFJ4MFU9";
const QString QQ_SONG_LRC_URL       = "WG9Cd1NrcEtnaFh5NkhsY0IrWUhhbUVET2Z6Znc0Wk4yVzZDVUhxMFRCcmFvNDRPSHZUZE9rZHhPWVBaS3JDOE10TTJXUUl1Z0VXdGM1Vkg4enNhQU9ROHhRR3g2SGd5alVTTmt3dEtNRVk5YXhjWnN0UFVNdUZYSWJYenM0b08=";
const QString QQ_SONG_PIC_URL       = "SjhCT2UxMEZ5TnZackt2N1ovbmVYQzlUbzNQeXMwMFNSaWE2eGxFR2tBTm9pckk2VWZ1OWN4QzBkVEtYYUIrc2UxKzVHRU1Gd0EzbkZuR0Y=";
const QString QQ_SONG_128_URL       = "bmNTSUx0QWwrdE5uTHl6M1p6Kyt4cXdUWFJraXNCSUtuU1I1WTliTFhjMnlwTzkzYWcyenNSa3N4MGFuUWdMN0ZvTEZKeERHUy85QWRBK3hUWlpjZFh4ZUc4NXlGL1Ay";
const QString QQ_SONG_192_URL       = "cjZONVhBNXF0SnY3ekFROGh2U2o5WmJ2VFk3SGxnWkNPTm1qTEg3TmxZWllYUTVtMWdXMk4yUU9NTjZqWUtJWjZXL2oyeXdzbHFSVndFbW52TzhTaHloMko0QnU5dXJQb3NHdEpRPT0=";
const QString QQ_SONG_320_URL       = "dWRDRWxuNG5YTlRQZUlKUlJmZlNDNFUzOFpidXYrVndtbjNWZVNQMDBzNXlGKzZmeTl3aDZ0Ni9KYStPR3VFZDYrRDdQSEx5dCt4MlIwekVQQTd3dTR6ejFUdnBKYWRPaFFrYnRRPT0=";
const QString QQ_SONG_APE_URL       = "THBYOE1UN1lkcXg5eGNmOWhMVmpzVkVOMEQ0MUUrSTNjdVI4Q0VyU1FSOHFFbnNCZ29SL1ErK0xVSFkycHB0V0hNSEVMZUtZK2l4UGRWcHVzR05pNmxaRjdvL3FDemsxbVoxL1d3PT0=";
const QString QQ_SONG_FLAC_URL      = "bDVvT25YNEVaZ1o1QllHSm5pUzFSejdua3hVaFRZRjlTbGtSdDlnd01TckliTkovVHdWalE2MFlseHhjdG4xNTlYYS84TXMzWWJEWnJZWVI0MFFkNmQ0WkFoZXI2RkV4TnMrZFF3PT0=";
const QString QQ_SONG_KEY_URL       = "N3Jac2NWa0I5QVppeEp2VFI2Tk5MSVVqVEx6WlJuSnpYdXBBT2gvbXU2aHpTQVg3QWQzS3B2ZkVqeklpYmZZU0ZPVXUrL2ZHWTE2UE1rTXU5TUxQK1hKOGVlQXRrbW1VdHVQN1lnPT0=";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_ARTIST_URL         = "em8xbTBWNG5sY1FIRnZEZGJuYjlRM21BSjJyZ0dZQzVDMkQrVGxzeGRJUHgxZnhyQmxGUmZwTFQxRC9SMlVTY0V6Nm9MZ2F1djdtVUlFU3hTNElmSFFvM0NoTGFtMysvWlkvL3VLRkZndy9JRkozQU95dG1lcmg1ZXErMEJ5aXNDY05JNFpHRzRzQWMyVzJjMmVwRFA3cDc4c1hId0YxN3NONHYvOW5McWp5S2JrbzFqOUtyYWJpSEVudnpVbmtINkRUdHU4UTJlSWg0bFc0bW4vSUpmOEcreUdxd0JPWjMrcjI2aHo0UGsxZktvVkdiMFo1azBza2crZmR3QjVNT1h2bWRlcG1EdmZlMWx5SllEWVgrc0NObGZsST0=";
const QString QQ_ARTIST_INFO_URL    = "NnQzYXB4c3hmNHhUc1RNL0tPN3VGMTdrbitmN0ozU0Rta1BtQVVpT3JITE16T2wzbk50R1c2Q0d3MU9vMkZwWGNUaWMxY29OSmpRTDd0RDR5UUFtZmxoWnplNlNNbmhXTUFtVDlHYWdHcmRwOHdtWDNsTFM2UHZYdUwvUzZGSW51dlBuVlV1bE1GOWpQVVdKZDBaNTV3PT0=";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_SONG_TOPLIST_C_URL = "NGtsYzRNNjFCSlhOVUFnMk0rSHFpZll4WUdsY0ZWcERWWXFtbnBIU2JGV0lWLzhNU2NFWUpaS3ZaOTZFdm9VQlBLdzl0S0FOTEhaNDhpQmkzSGt2QXFLcnhDNjNuZHNQOVM2Tk1UMjRReHVFRnQ3eS9JakUwOHBYa2RXUzNtcGQwaTNEdUJEZXlYdllya2xPWXVzRkFqRFFWV29nSkowN2NuNmxxRkhTaFFRZzhCZSs2bXIzK1hzNHhoakppdGM5TTVFNlkwV2pRV3hpbzR4WTFNdU55STZ2UVdnVVRVRlBZMjA2U1pzbVdobWd6S3lEMGZtVWJpbUhwT011U1pJRw==";
const QString QQ_SONG_TOPLIST_A_URL = "eWM4LzBEa0wyRkFkdHhTVDVnb1ZNZHZ0MG5xQTJkS0d0QjQ3dGdxVlBGRlNLb3h1TTBmczVDY3MreC9qNEsxblVrYVg2Y2pBMXVZSUhycnV5cnlNbVhoRms1VVlBVEZKS3A2bHR3MURYYXZLZUtwTTZRZ0xyMWl1ZTVuRXRjeXp2RHRaSGc9PQ==";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_MV_INFO_URL        = "ZjhkZmd4LzNnQi9seFJpUmZ0L0hLQkZ1dFdnME5lOWI5dlgvUFpHUUw5U2xQdzJXZzhxditTWTNTWllKTVp4dFo2bmlqc1Y4TUpoeTFEdUFnbkNVOE5WL0RVYVFzSHR0UFpzN2pBPT0=";
const QString QQ_MV_KEY_URL         = "cEo4MkdYaVNIUEJFTGNnd0hGai8zUFVWQStTQ3NyamM1YmovTzdGQWRwRFU2OExQV0MxMUxSN0NkSmJrT3MzTTNVaFhSUytNWmFrKys3ODdhWldaNWljZXBaRmFHeUg1dHdGb1JSNEJpQ3orZFIwVUIrUURJUFRJZ25SakVpN20=";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_COMMIT_URL         = "Zk9wcGVJRXMxaTg5enBsVEFGc0hGWVpkT0gvVHNCNDBHSXU0anR6bzdTQnpEN2dlZDg4TllZa256aUhZUys1UURDdFV3ZFZmZmRzSFlXVDhlcTM4TFQvNnpWU0VqR1k5WGZZaWpnPT0=";
const QString QQ_SG_COMMIT_DATA_URL = "d0F1WXh6MWt2SXN6WktuU24wUTNXNERCZmJXb296QUhicFFZb29DSjhEeFN2dldqWHgrL0cvSG1sRVdaWHMzbGliaWFzbklEaTQ5UzZxY3RVNlpqU1huSDJCWVRCYU5YQ25HT2ozVE95aGRXUy9malNrNU9GcUVHSTkvMHA5blNxUE10ZW1lNEpLMWM2dFA1cHQzdjdDVDRXb2IxbGVhbXRXQzJRR1FrNWtrb3FmV01KVG5TVkxyUEVCYmxjMC9zOVZSWHVTRThmSzkzalZBd0p2bnRkZXh0QmRTV3N3OVh6alZ3UWJ4ck8xdEZ3U2xCdmNPT0RkRHpRb3phR3Z6TXZDcjd5TzU2cHJoMElLeFE=";
///////////////////////////////////////////////////////////////////////////////
const QString QQ_PL_COMMIT_URL      = "c1BKYmVFa1FRc0trZ0NtVVVMOEMrNXNDWFRiYzJLYXBQaUdXZWZobjlrZy94cVdNNTlJSTQ1S0xjWlpBSEhPWVlzZXREb0ZLSkFoVWJteFpqRlNiRjNRSHRDNVFEbXRyRnFRVW9JTTc1MW11ZFp4cVh2QmxFQXplWHZEeWNRUERBYThYM0dqZFZqeVVTYW1GL2pkeHpkV2EvbnlNakdIc0UrY2MrNVRKWWxKQ3RmTEdSYmt6LzRTU2JFQm9iOG96VXZ6TVlveE5rSXpvQUlaL2JXN2RKWnJXT1BjcjF1MEsrelI0VS9kOVc0SndkQk5LNWhRekp2WTFnd1VBdzJpMytDeENJenN0S0VKUS9yTVk=";
const QString QQ_PLAYLIST_URL       = "RGdWa2ZLeGdjcEhIT3FlVGs5bWsycndRTjFaNStCM0t3MHgvOGYyKzdyVitOZU9qbVM1T25HZzRPeE5IbEtpaHFrQ09GSU5rRlkxTkM4NnZVQ1ZaMXcxZHBmb3FYaXNSZWN0Mmozc3czU0lCUGFsTjY0VlcvWnVkTkVheWtyVTFMbW94dzNPVXUvS0FwR0JFY0YwMHlHeFhPS2VhMkMyOVZhSEJsbVp4ZS9GdEF1TkdPa3Z0bFpsN2oxOWhaRFBsLzlIM1ZqZUUyeGtZRytrTnF4SW1IZUJkbmEwWWVMQmo2ZDI3MTRKNmw5U1hQVHFRYkdETmRET3pOQjNWaGZLUmdPaFpvYWMrd1ZENXB0SldPbG1xdEZDNzdBbS9qR29jVDU3WEVOc2RvTGJHR3hRamVya2dxdDJwVEFVbUFQcEtCSmFsQVBWZk0wK003Rk4zaisrK09OcHd1dTg9";
const QString QQ_PLAYLIST_ATTR_URL  = "QmJ1Vkg0OUNpZUVxQXFBMks3czlzcDZqME9ON2xkMTVwT3E1ekxFejU2N3drL2d6cHdTdzBtemtiaTJ2VHh6QlBsbXdVTDFoQzNLWG5HN3NucmlPR1pjaGRkSkRqU3pSNk5CN1V0YVVGcDdUNUhEeElVd0J4OE5TTjhNSmZZTks4Z1J5RTRxY0MzclUwR0FHOHp1QXk5U29wb3ZGUXlTZThxWWgybEtuRUpsSkkyQWpmN0lRNkh0UWNHN0hvNjV6a25ZdVdIL0pHZGlydjRrdkxibXdCcGdRWkpQZkcwQXMzZEVKbisrYXBsU3RTNzhCS2Jja0Fhc295Yk5kWk1sY09tRzJweDlldXN1OUlYMFNxVUw3MnovRkMvdzZ1L3A2UHdPMHU5V1hENTd5SmFqWEhubnJKK3pyaHR0bW9zSnZ2N3k3S3RNYUlKajJaNFNJR0s0cWl2aEpWUjRGWW5jSThYOFdWSno4VmhvamljVFhKMVM5M2VRTDZWc3NpM1F1TU5waEdRPT0=";
///////////////////////////////////////////////////////////////////////////

class QNetworkAccessManager;

/*! @brief The class to qq query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQQInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    const QString &quality, bool all);
    /*!
     * Get music key.
     */
    QString getMusicKey(const QString &time);

};

#endif // MUSICDOWNLOADQQINTERFACE_H
