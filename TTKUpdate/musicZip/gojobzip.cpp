#include "gojobzip.h"

#include <QDir>
#include <QDebug>

PackZip::PackZip(QObject *parent)
    : QObject(parent)
{
    startnow = QDir::currentPath();
}

PackZip::~PackZip()
{
    QDir::setCurrent(startnow);
}

QString PackZip::getClassName()
{
    return staticMetaObject.className();
}

void PackZip::setJob(const QString &packfolder, const QString &zipoutfolder)
{
    startdir = packfolder;   /* grab all this file */
    zipdir = zipoutfolder;   /* save zip to this dir */
    startnow = QDir::currentPath();
    init();
}

QString PackZip::biteorMega(int peso)
{
    QString humanread;
    double canno = peso / 1024;
    int sale = canno;
    if(peso > 0)
    {
        if(canno < 1)
        {
            sale = 1;
        }
    }

    if(sale < 1025)
    {
        humanread = QString("Kb.%1").arg(sale);
        return humanread;
    }

    float megad = sale / 1024;
    if(megad < 1025)
    {
        humanread = QString("MB.%1").arg(megad);
        return humanread;
    }
    else
    {
        humanread = QString("GB.1+");
    }
    return humanread;
}

bool PackZip::isFile(const QString &fullFileName)
{
    if(!fullFileName.size() > 0)
    {
        return false;
    }

    QDir diro(fullFileName);
    if(diro.exists())
    {
        return false;
    }

    QFile f( fullFileName );
    if( f.exists(fullFileName) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PackZip::gzCompress(FILE *in, gzFile out)
{
    local char buf[BUFLEN];
    int len;
    ///////int err;
    for(;;)
    {
        len = (int)fread(buf, 1, sizeof(buf), in);
        if(ferror(in))
        {
            perror("fread");
            exit(1);
        }
        if(len == 0)
        {
            break;
        }

        if(gzwrite(out, buf, (unsigned)len) != len)
        {
            return;
        }
    }

    fclose(in);
    if(gzclose(out) != Z_OK)
    {
        return;
    }
}

bool PackZip::makeGzipOneFile(QString filelocation)
{
    local char outfile[MAX_NAME_LEN];
    FILE *in;
    gzFile out;
    QByteArray cgofile = filelocation.toUtf8();
    char* file = cgofile.data();
    QString const busgo = filelocation.append(GZ_SUFFIX);
    strcpy(outfile, file);
    strcat(outfile, GZ_SUFFIX);   /* attach suffix */
    in = fopen(file, "rb");

    if(in == NULL)
    {
        return false;
    }
    out = gzopen(outfile,"wb6");
    if(out == NULL)
    {
        fclose( in );
        return false;
    }
    gzCompress(in, out);

    QFile f( busgo );
    return f.exists(busgo);
}

bool PackZip::unZipTo(const QString &zipfile, const QString &outputdir)
{
    QuaZip zip(zipfile);
    bool extractsuccess = false;
    zip.open(QuaZip::mdUnzip);
    QuaZipFile file(&zip);

    int count, value = 0;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        ++value;
    }

    count = value;
    value = 0;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        file.open(QIODevice::ReadOnly);
        QString name = zip.getCurrentFileName();
        name.prepend(outputdir);   /* extract to path ....... */
        QFile meminfo(name);
        QFileInfo infofile(meminfo);
        QDir dira(infofile.absolutePath());
        if( dira.mkpath(infofile.absolutePath()) )
        {
            ++value;
            QString fileqinfo = zip.getCurrentFileName();
            qDebug() <<  "Processing..... " << fileqinfo << "\n";
            emit process(value*1.0/count, fileqinfo);

            if(meminfo.open(QIODevice::ReadWrite))
            {
                meminfo.write(file.readAll());   /* write */
                meminfo.close();
                extractsuccess = true;
            }
        }
        else
        {
            file.close();
            return false;
        }
        file.close(); // do not forget to close!
    }
    zip.close();
    return extractsuccess;
}

void PackZip::init()
{
    canmake = false;
    if(!zipdir.contains("/"))
    {
        resetDir();
        return;
    }
    if(!zipdir.endsWith("/"))
    {
        resetDir();
        return;
    }
    if(!startdir.contains("/"))
    {
        resetDir();
        return;
    }
    if(!startdir.endsWith("/"))
    {
        resetDir();
        return;
    }
    canmake = true;

    QDir dir(startdir);
    char c;
    qreal cento;
    qreal percentuale;
    QString dirname = dir.dirName();
    selfname = dirname + ".zip";
    zipfile = zipdir;
    zipfile.append(dirname);
    zipfile.append(".zip");

    QFile zcheck(zipfile);
    if(zcheck.exists())
    {
        zcheck.remove();
    }

    dirandfile.clear();
    indexDir(startdir);
    if(dirandfile.size() > 0)
    {
        QuaZip zip(zipfile);
        if(!zip.open(QuaZip::mdCreate))
        {
            resetDir();
            return;
        }

        QuaZipFile outFile(&zip);
        int totalf = dirandfile.size();

        ///////qDebug() << "### totalfile " <<  dirandfile.size();
        for(int i = 0; i < dirandfile.size(); ++i)
        {
            QString filepak = dirandfile.at(i);
            const QString abse = filepak;
            const QString goname = filepak.replace(startdir, "");

            QFile inFile(abse);
            inFile.setFileName(abse);

            if(!inFile.open(QIODevice::ReadOnly))
            {
                resetDir();
                return;
            }

            if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(goname, inFile.fileName())))
            {
                resetDir();
                return;
            }

            while(inFile.getChar(&c)&&outFile.putChar(c));
            if(outFile.getZipError() != UNZ_OK)
            {
                resetDir();
                return;
            }

            outFile.close();
            if(outFile.getZipError()!=UNZ_OK)
            {
                resetDir();
                return;
            }
            inFile.close();

            cento = 100;
            percentuale = i*cento/totalf;
            emit status(percentuale);

            QString fileqinfo = goname;
            fileqinfo.append(" ");
            fileqinfo.append(biteorMega(inFile.size()));
            qDebug() <<  "Status. " << percentuale << "% " << fileqinfo << "\n";
        }
        zip.close();

        if(zip.getZipError() != 0)
        {
            QDir::setCurrent(startnow);
        }

        QFile re(zipfile);
        QString fr = re.fileName();
        fr.append(" ");
        fr.append(biteorMega(re.size()));
        qDebug() <<  "End file " << fr << "\n";

        emit status(100);
        emit zipEnd(zipfile);
        return;
    }
    emit zipError();
}

void PackZip::resetDir()
{
    emit zipError();
    QDir::setCurrent(startnow);
}

void PackZip::indexDir(const QString &dir)
{
    QDir directory(dir);
    if(directory.exists())
    {
        const QFileInfoList list = directory.entryInfoList();
        QFileInfo fi;
        for(int l = 0; l < list.size(); l++)
        {
            fi = list.at(l);
            if(fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                indexDir(fi.absoluteFilePath());
            }
            else if(fi.isFile() && fi.fileName() != selfname )
            {
                dirandfile.append(fi.absoluteFilePath());
            }
        }
    }
}

