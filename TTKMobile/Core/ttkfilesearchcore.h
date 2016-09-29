#ifndef TTKFILESEARCHCORE_H
#define TTKFILESEARCHCORE_H

#include <QFileInfoList>

class TTKFileSearchCore : public QObject
{
    Q_OBJECT
public:
    explicit TTKFileSearchCore(QObject *parent = 0);

    Q_INVOKABLE QString getRoot() const;
    Q_INVOKABLE void search(const QStringList &path);
    Q_INVOKABLE void search(const QString &path);

Q_SIGNALS:
    void finished(const QStringList &path);

private:
    QFileInfoList findFile(const QString &path, const QStringList &filter);

};

#endif // TTKFILESEARCHCORE_H
