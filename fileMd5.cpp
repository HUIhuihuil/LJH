#include "fileMd5.h"
#include<QFile>
#include<QCryptographicHash>
#include<QMessageBox>
#include<QDebug>
#include<QCoreApplication>
#include<QDir>
#include<QFileInfo>
fileMd5::fileMd5(QObject *parent) : QObject(parent)
{

}

void fileMd5::checkDumplate(const QString &path)
{

    QHash<QByteArray,QStringList> res;
    QStringList fileName = getFile(path);

    for(int i=0;i<fileName.count();i++){
        QString aFile= fileName.at(i);
        QByteArray md5 = getFileMd5(aFile);
        res[md5.toHex()].append(aFile);//注意存的时候应该存toHex格式的
        emit progress(i,fileName.count()-1);
    }

    emit gotDumplate(res);
}

QStringList fileMd5::getFile(const QString &path)
{
    QStringList res;//返回值
    QDir dir(path);//实例化一个文件类
    QFileInfoList infolist= dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);//找path的文件和文件夹，但是不包括当前和上个目录的文件
    for(int i =0;i<infolist.count();i++){
        QFileInfo aFile=infolist.at(i);//单个文件
        if(aFile.isDir()){//如果是目录则递归寻找
            QString subDir=aFile.absoluteFilePath();
            QStringList files = getFile(subDir);
            res.append(files);
        }else{
            QString subDir=aFile.absoluteFilePath();//获取文件的详细路径
            res.append(subDir);//结果加入到返回值

        }
    }
    return res;
}

QByteArray fileMd5::getFileMd5(const QString &path)
{
    QFile file(path);//打开文件夹
    if(file.open(QIODevice::ReadOnly)){//读取方式：只读
        QCryptographicHash hash(QCryptographicHash::Md5);
        //以防文件过大，每次只读100Mb
        while(!file.atEnd()){
            QByteArray content = file.read(100*1024*1024);//只读一小部分
            hash.addData(content);
            qApp->processEvents();//在循环中执行事件循环，防止卡顿头文件是#include<QCoreApplication>
        }
        QByteArray md = hash.result();

        //QByteArray content = file.readAll();//读取所有内容，注意是用QByteArray格式
        //QMessageBox::information(this," ",content);//消息框，指定父窗口，标题，和内容
        //QByteArray md=QCryptographicHash::hash(content,QCryptographicHash::Md5);//运算md5
        //qDebug() << md.toHex();//打印md5
        file.close();
        return md;

    }
    return QByteArray();
}
