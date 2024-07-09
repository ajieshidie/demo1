#ifndef WIDGET_H
#define WIDGET_H
#include "form.h"
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include <qwt_dial_needle.h>
#include <qwt_abstract_scale.h>
#include <qwt_dial.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Form *ppage2 = NULL;

    void fetchDeviceShadow();
    void onNetworkReplyFinished(QNetworkReply *reply);
    void LEDON_sendDeviceMessage();
    void LEDOFF_sendDeviceMessage();
    void sendRequest();
    void Warning(int ligthIntValue,int soilhumiIntValue);



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_fan_onpushButton_clicked();

    void on_fan_offpushButton_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void onTimeout();

    void handleData(const QString &data1,const QString &data2,const QString &data3,const QString &data4,const QString &data5,const QString &data6,const QString &data7,const QString &data8,const QString &data9,const QString &data10,const QString &data11,const QString &data12);

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void  updateAndRedrawCurve();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *networkManager;
     QString m_token;
     QTimer *timer;
     QTimer *timer1;
     QNetworkAccessManager *manager;
     QNetworkRequest request;
     int displayOK;
     QNetworkReply *reply;

};
#endif // WIDGET_H
