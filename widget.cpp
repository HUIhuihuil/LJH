#include "widget.h"
#include "ui_widget.h"
#include<QMouseEvent>
#include<QGraphicsDropShadowEffect>
#include<QFile>
#include<QCryptographicHash>
#include<QMessageBox>
#include<QDebug>
#include<QCoreApplication>
#include<QDir>
#include<QFileInfo>
#include<QFileDialog>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    thread.start();//线程启动
    md5.moveToThread(&thread);//将这个类移动到线程
    
    
    
    this->setWindowFlag(Qt::FramelessWindowHint);//去掉系统边框
    QGraphicsDropShadowEffect* shape = new QGraphicsDropShadowEffect();//阴影
    shape->setColor(Qt::black);
    shape->setBlurRadius(10);
    shape->setOffset(0);

    ui->widget->setGraphicsEffect(shape);//新加的内窗口的阴影样式
    this->setAttribute(Qt::WA_TranslucentBackground);//外窗口透明

    qRegisterMetaType<QHash<QByteArray, QStringList>>("QHash<QByteArray, QStringList>");//注册这种QHash类型可以让信号和槽使用
    connect(this,SIGNAL(checkDumplate(QString)),&md5,SLOT(checkDumplate(QString)));
    connect(&md5,SIGNAL(gotDumplate(QHash<QByteArray, QStringList>)),this,
            SLOT(onGotDumplate(QHash<QByteArray, QStringList>)));
    //connect(this,SIGNAL(checkDumplate(QString)),&md5,SLOT(checkDumplate(QString)));
    connect(&md5,SIGNAL(progress(int,int)),this,SLOT(on_progress(int, int)));

}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QPoint y = event->globalPos();//鼠标全局位置
    QPoint x = y-z;
    this->move(x);

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPoint y = event->globalPos();//鼠标的全局位置
    QPoint x = this->geometry().topLeft();//窗口左上角相对于桌面左上角的位置
    z = y-x; //定值不会改变
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    z = QPoint();
}

Widget::~Widget()
{
    thread.exit();//窗口关闭调用析构
    thread.wait(10*1000);//等待线程的退出
    delete ui;
}


void Widget::on_ButtonClose_clicked()
{
    this->close();
}

void Widget::on_ButtonMax_clicked()
{
    this->showMinimized();
}

void Widget::on_ButtonMin_clicked()
{
    if(this->isMaximized()){
        this->showNormal();
    }else{
        this->showMaximized();
    }
}


void Widget::on_pushButton_2_clicked()
{
   // md5.checkDumplate("D:/Qt/My/project_1");
    ui->progressBBar->setValue(0);//每次点击进度条设为0
    QString path = QFileDialog::getExistingDirectory(this,"selct",".");//现实文件夹路径，第二个参数是标题，第三个是当年目录打开
    emit checkDumplate(path);//使用信号将参数发送出去
}

void Widget::onGotDumplate(const QHash<QByteArray, QStringList>& dumps)
{
    ui->listWidget->clear();
    this->dumpss = dumps;
    for(QHash<QByteArray,QStringList>::const_iterator itr=dumps.begin();itr!=dumps.end();itr++){
        qDebug()<<"mds"<<itr.key()<<"count:"<<itr.value();
        if(itr.value().count()>1){
            ui->listWidget->addItem(itr.key());;
        }
    }
}



void Widget::on_progress(int start, int end)
{
    ui->progressBBar->setMaximum(end);
    ui->progressBBar->setValue(start);
}

void Widget::on_listWidget_currentTextChanged(const QString &currentText)
{
    qDebug() << currentText;
    ui->listWidget_2->clear();
    QStringList dlist = this->dumpss[currentText.toLocal8Bit()];
    qDebug()<<currentText.toLocal8Bit();
    ui->listWidget_2->addItems(dlist);

}
