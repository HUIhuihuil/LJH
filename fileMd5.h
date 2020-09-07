#ifndef FILEMD5_H
#define FILEMD5_H

#include <QObject>

class fileMd5 : public QObject
{
    Q_OBJECT
public:
    explicit fileMd5(QObject *parent = nullptr);

signals:
    void gotDumplate(const QHash<QByteArray, QStringList>& dumps);
    void progress(int current,int total);
public slots:
    void checkDumplate(const QString& path);


private:
    QStringList getFile(const QString& path);//QStringList是一个字符串列表
    QByteArray getFileMd5(const QString& path);
};

#endif // FILEMD5_H
