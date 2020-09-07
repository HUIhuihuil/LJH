#ifndef WIDGET_H
#define WIDGET_H
#include "fileMd5.h"
#include <QWidget>
#include<QPoint>
#include<QHash>
#include<QMap>
#include<QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    ~Widget();
signals:
    void checkDumplate(const QString& path);
private slots:
    void on_ButtonClose_clicked();

    void on_ButtonMax_clicked();

    void on_ButtonMin_clicked();

    void on_pushButton_2_clicked();
    void onGotDumplate(const QHash<QByteArray,QStringList>& dumps);

    void on_progress(int start,int end);

    void on_listWidget_currentTextChanged(const QString &currentText);

private:
    Ui::Widget *ui;
    QPoint z;
    fileMd5 md5;
    QThread thread;
    QHash<QByteArray, QStringList> dumpss;



};
#endif // WIDGET_H
