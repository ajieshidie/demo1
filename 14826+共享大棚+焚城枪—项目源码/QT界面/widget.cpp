#include "widget.h"
#include "ui_widget.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QMessageBox>

QByteArray  Token;
double voltageIntValue;
int t;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , networkManager(new QNetworkAccessManager(this))
    , timer(new QTimer(this))
    , displayOK(0) //摄像头flag
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);//创建网络管理对象
    request.setUrl(QUrl("http://192.168.175.185/capture?_cb=8888"));//设置请求地址
    timer1 = new QTimer(this);
    connect(timer1,&QTimer::timeout,this,&Widget::onTimeout);
    timer1->start(100);

    this->ppage2 = new Form;
    connect(ui->pushButton_13,&QPushButton::clicked,[=](){
        //切换窗口，实质上是隐藏主窗口，显示副窗口
            this->hide();//仅仅是隐藏，主窗口没有释放

            this->ppage2->show();
        });
        connect(this->ppage2,&Form::back,[=](){
            this->ppage2->hide();
            this->show();
        });
    connect(ppage2, &Form::dataEntered, this, &Widget::handleData);

    sendRequest();
    timer->start(3000);
    connect(timer,&QTimer::timeout,this,&Widget::fetchDeviceShadow);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Widget::onNetworkReplyFinished);
    connect(timer, &QTimer::timeout, this, &Widget::updateAndRedrawCurve);

    QwtDialSimpleNeedle *out_temp_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    out_temp_needle->setWidth(5);
    ui->out_tempDial->setNeedle(out_temp_needle);

    QwtDialSimpleNeedle *out_humi_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    out_humi_needle->setWidth(5);
    ui->out_humiDial->setNeedle(out_humi_needle);

    QwtDialSimpleNeedle *out_ligth_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    out_ligth_needle->setWidth(5);
    ui->out_ligthDial->setNeedle(out_ligth_needle);

    QwtDialSimpleNeedle *temp_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    temp_needle->setWidth(5);
    ui->temp_Dial->setNeedle(temp_needle);

    QwtDialSimpleNeedle *humi_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    humi_needle->setWidth(5);
    ui->humi_Dial->setNeedle(humi_needle);

    QwtDialSimpleNeedle *ligth_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    ligth_needle->setWidth(5);
    ui->ligth_Dial->setNeedle(ligth_needle);

    QwtDialSimpleNeedle *soiltemp_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    soiltemp_needle->setWidth(5);
    ui->soiltempDial->setNeedle(soiltemp_needle);

    QwtDialSimpleNeedle *soilhumi_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    soilhumi_needle->setWidth(5);
    ui->soilhumi_Dial->setNeedle(soilhumi_needle);

    QwtDialSimpleNeedle *DD_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    DD_needle->setWidth(5);
    ui->DD_Dial->setNeedle(DD_needle);

    QwtDialSimpleNeedle *PH_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    PH_needle->setWidth(5);
    ui->PH_Dial->setNeedle(PH_needle);

    QwtDialSimpleNeedle *CO2_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    CO2_needle->setWidth(5);
    ui->CO2_Dial->setNeedle(CO2_needle);

    QwtDialSimpleNeedle *V_needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Style::Arrow, true,Qt::red, Qt::gray);
    V_needle->setWidth(5);
    ui->V_Dial->setNeedle(V_needle);

    ui->qwtPlot->setTitle("V");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"time(t)");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"V(V)");
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,24.0);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,0.0,12.0);
}

void Widget::sendRequest() {
    // 创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 设置请求URL
    QUrl url("https://iam.myhuaweicloud.com/v3/auth/tokens");

    // 构建JSON请求体
    QJsonObject passwordObj;
    passwordObj["domain"] = QJsonObject{{"name", "huangdong2002"}};
    passwordObj["name"] = "huangdong";
    passwordObj["password"] = "ab727506";

    QJsonObject identityObj;
    identityObj["methods"] = QJsonArray{"password"};
    identityObj["password"] = QJsonObject{{"user", passwordObj}};

    QJsonObject scopeObj;
    scopeObj["domain"] = QJsonObject{{"name", "huangdong2002"}};
    scopeObj["project"] = QJsonObject{{"name", "cn-north-4"}};

    QJsonObject authObj;
    authObj["identity"] = identityObj;
    authObj["scope"] = scopeObj;

    QJsonObject payloadObj;
    payloadObj["auth"] = authObj;

    QJsonDocument payloadDoc(payloadObj);
    QByteArray payloadData = payloadDoc.toJson();

    // 创建请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送POST请求
    QNetworkReply* reply = manager->post(request, payloadData);

    // 连接信号和槽
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 获取响应头中的X-Subject-Token
            QVariant token = reply->rawHeader("X-Subject-Token");
            QByteArray tokenByteArray = token.toByteArray(); // 将QVariant转换为QByteArray
            QString tokenQString = QString::fromUtf8(tokenByteArray);
            Token = tokenByteArray;
            QMessageBox::information(nullptr,"成功","获取token成功");
            //qDebug() << "X-Subject-Token:" << token.toString();
        } else {
            QMessageBox::critical(nullptr,"失败","获取token失败");
           // qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });

    QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
}


Widget::~Widget()
{
    delete ui;
}


// 假设这是您类的一个成员函数


void Widget::fetchDeviceShadow() {
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/shadow");
    QNetworkRequest request(url);

    // 设置请求头
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 发送GET请求
    QNetworkReply *reply = networkManager->get(request);
}

void Widget::onNetworkReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据，并假设它是 UTF-8 编码的
        QString responseString = QString::fromUtf8(reply->readAll());
        qDebug() << "Response:" << responseString;
        // 在这里你可以解析JSON数据，或者将其显示在UI上
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseString.toUtf8());
        if (!jsonDoc.isNull()) {
            // 假设JSON文档的顶层是一个对象
            if (jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();

                // 从对象中获取"shadow"键对应的值，它应该是一个数组
                QJsonValue shadowValue = jsonObject["shadow"];

                // 检查"shadow"是否是一个数组
                if (shadowValue.isArray()) {
                    QJsonArray shadowArray = shadowValue.toArray();

                    // 假设我们只关心数组中的第一个元素（如果有多个）
                    if (!shadowArray.isEmpty()) {
                        QJsonObject shadowObject = shadowArray.at(0).toObject();

                        // 从"reported"键中获取值，它应该是一个对象
                        QJsonValue reportedValue = shadowObject["reported"];

                        // 检查"reported"是否是一个对象
                        if (reportedValue.isObject()) {
                            QJsonObject reportedObject = reportedValue.toObject();

                            // 从"properties"键中获取值，它应该包含"light"键
                            QJsonValue propertiesValue = reportedObject["properties"];

                            // 检查"properties"是否是一个对象
                            if (propertiesValue.isObject()) {
                                QJsonObject propertiesObject = propertiesValue.toObject();

                                // 从"properties"对象中获取"light"的值
                                QJsonValue out_humiValue = propertiesObject["humidity"];
                                QJsonValue out_tempValue = propertiesObject["temperature"];
                                QJsonValue out_ligthValue = propertiesObject["light"];
                                QJsonValue tempValue = propertiesObject["temp_value"];
                                QJsonValue humiValue = propertiesObject["humi_value"];
                                QJsonValue ligthValue = propertiesObject["lux_value"];
                                QJsonValue CO2Value = propertiesObject["CO2_value"];
                                QJsonValue soiltempValue = propertiesObject["soil_temp"];
                                QJsonValue soilhumiValue = propertiesObject["water"];
                                QJsonValue conductivityValue = propertiesObject["conductivity"];
                                QJsonValue phValue = propertiesObject["ph"];
                                QJsonValue voltageValue = propertiesObject["voltage"];
                                // 检查"light"是否是一个有效的值（比如一个整数）

                                    int out_humiIntValue = out_humiValue.toInt(); // 或者 lightValue.toDouble() 如果需要浮点数
                                    int out_tempIntValue = out_tempValue.toInt();
                                    int out_ligthIntValue = out_ligthValue.toInt();
                                    int tempIntValue = tempValue.toInt();
                                    int humiIntValue = humiValue.toInt();
                                    int ligthIntValue = ligthValue.toInt();
                                    int CO2IntValue = CO2Value.toInt();
                                    int soiltempIntValue = soiltempValue.toInt();
                                    int soilhumiIntValue = soilhumiValue.toInt();
                                    int conductivityIntValue = conductivityValue.toInt();
                                    int phIntValue = phValue.toInt();

                                    voltageIntValue = voltageValue.toDouble();

                                    ui->out_humiDial->setValue(out_humiIntValue);
                                    ui->out_tempDial->setValue(out_tempIntValue);
                                    ui->out_ligthDial->setValue(out_ligthIntValue);
                                    ui->temp_Dial->setValue(tempIntValue);
                                    ui->humi_Dial->setValue(humiIntValue);
                                    ui->ligth_Dial->setValue(ligthIntValue);
                                    ui->CO2_Dial->setValue(CO2IntValue);
                                    ui->soiltempDial->setValue(soiltempIntValue);
                                    ui->soilhumi_Dial->setValue(soilhumiIntValue);
                                    ui->DD_Dial->setValue(conductivityIntValue);
                                    ui->PH_Dial->setValue(phIntValue);
                                    ui->V_Dial->setValue(voltageIntValue);

                                    ui->out_humilabel->setNum(out_humiIntValue);
                                    ui->out_templabel->setNum(out_tempIntValue);
                                    ui->out_ligthlabel->setNum(out_ligthIntValue);
                                    ui->templabel->setNum(tempIntValue);
                                    ui->humilabel->setNum(humiIntValue);
                                    ui->ligthlabel->setNum(ligthIntValue);
                                    ui->CO2label->setNum(856);
                                    ui->soiltemplabel->setNum(soiltempIntValue);
                                    ui->soilhumilabel->setNum(soilhumiIntValue);
                                    ui->DDlabel->setNum(conductivityIntValue);
                                    ui->PHlabel->setNum(phIntValue);
                                    ui->V_label->setNum(voltageIntValue);



                                   // Warning(ligthIntValue,soilhumiIntValue);

                                    if(tempIntValue > 25 )
                                    {
                                        ui->templabel->setStyleSheet("color: blue; font-family: '微软雅黑'; font-size: 24pt;");
                                    }
                                    if(soilhumiIntValue > 30)
                                    {

                                        ui->soilhumilabel->setStyleSheet("color: red; font-family: '微软雅黑'; font-size: 24pt;");
                                    }



                            }
                        }
                    }
                }
            }
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    // 清理
    reply->deleteLater();
}

void Widget::LEDON_sendDeviceMessage() {
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "lamp";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
void Widget::LEDOFF_sendDeviceMessage() {
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "lamp";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
}



void Widget::on_pushButton_clicked()
{
    LEDOFF_sendDeviceMessage();
    ui->label_23->setStyleSheet("image: url(:/pic/icon_1/a-buguangdengxian_huaban1_huaban12.png);");
}

void Widget::on_pushButton_2_clicked()
{
    LEDON_sendDeviceMessage();
    ui->label_23->setStyleSheet("image: url(:/pic/icon_1/a-buguangdengxian_huaban1_huaban1.png);");
}

void Widget::on_fan_onpushButton_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "FAN_D3";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_25->setStyleSheet("image: url(:/pic/icon_1/fengshan.png);");
}


void Widget::on_fan_offpushButton_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "FAN_D3";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_25->setStyleSheet("image: url(:/pic/icon_1/fengshan1.png);");
}

void Widget::on_pushButton_8_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "pump_D1";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_26->setStyleSheet("image: url(:/pic/icon_1/shuibeng1.png);");
}

void Widget::on_pushButton_7_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "pump_D1";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_26->setStyleSheet("image: url(:/pic/icon_1/shuibeng12.png);");
}

void Widget::on_pushButton_4_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "lamp_D2";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_24->setStyleSheet("image: url(:/pic/icon_1/bulb1.png);");
}

void Widget::on_pushButton_3_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "lamp_D2";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_24->setStyleSheet("image: url(:/pic/icon_1/bulb12.png);");
}

void Widget::on_pushButton_10_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "Elc_D4";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });

    ui->label_27->setStyleSheet("image: url(:/pic/icon_1/bf-icon-battery_empty.png);");
}

void Widget::on_pushButton_9_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "Elc_D4";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_27->setStyleSheet("image: url(:/pic/icon_1/bf-icon-battery_charging.png);");
}

void Widget::onTimeout()
{
    if(displayOK == 0){
        reply = manager->get(request);
        connect(reply,&QNetworkReply::finished,[=](){
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            ui->shipinglabel->setPixmap(pixmap.scaled(600,500));
            displayOK=0;
            reply->deleteLater();
        });
    }
    displayOK=1;
}

void Widget::handleData(const QString &data1,const QString &data2,const QString &data3,const QString &data4,const QString &data5,const QString &data6,const QString &data7,const QString &data8,const QString &data9,const QString &data10,const QString &data11,const QString &data12) {
    // 在这里处理接收到的数据，比如更新 UI 部件
    QString tempmin = data1;
    QString temp1 =data2;
    QString humimin = data3;
    QString humi1 =data4;
    QString ligthmin = data5;
    QString light1 =data6;
    QString co2min = data7;
    QString co21 = data8;
    QString soiltempmin = data9;
    QString soiltemp1 = data10;
    QString soilhumimin = data11;
    QString soilhumi1 =data12;
    ui->spinBox->setValue(tempmin.toInt());
    ui->spinBox_2->setValue(temp1.toInt());
    ui->spinBox_3->setValue(humimin.toInt());
    ui->spinBox_4->setValue(humi1.toInt());
    ui->spinBox_5->setValue(ligthmin.toInt());
    ui->spinBox_6->setValue(light1.toInt());
    ui->spinBox_8->setValue(co2min.toInt());
    ui->spinBox_7->setValue(co21.toInt());
    ui->spinBox_11->setValue(soiltempmin.toInt());
    ui->spinBox_12->setValue(soiltemp1.toInt());
    ui->spinBox_9->setValue(soilhumimin.toInt());
    ui->spinBox_10->setValue(soilhumi1.toInt());
}

void Widget::Warning(int ligthIntValue,int soilhumiIntValue)
{
    if(ligthIntValue > 80)
    {
        QMessageBox::warning(nullptr,"警告","光照过高");
    }
    if(soilhumiIntValue > 30)
    {
        QMessageBox::warning(nullptr,"警告","土壤湿度过高");

    }else if(soilhumiIntValue < 15)
    {
        QMessageBox::warning(nullptr,"警告","土壤湿度过低");
    }

    if(voltageIntValue <= 7)
    {
        QMessageBox::warning(nullptr,"警告","电量过低！请及时充电");
    }
}

void Widget::on_pushButton_12_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "auto_mode";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "ON";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_28->setStyleSheet("image: url(:/pic/icon_1/shoudongmoshi.png);");
}

void Widget::on_pushButton_11_clicked()
{
    QUrl url("https://8edf64f153.st1.iotda-app.cn-north-4.myhuaweicloud.com:443/v5/iot/a54521efa11d43b0bb39bbc1a7052eb1/devices/664dfc606bc31504f06b78f0_smartLamp/commands"); // 替换为你的实际API地址
    QNetworkRequest request(url);
    const QString messageContent;
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Auth-Token", Token);
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("User-Agent", "Your-Qt-App/1.0"); // 替换为你的应用信息

    // 构造JSON消息
    QJsonObject message;
    message["service_id"] = "control";

        // 添加"command_name"字段
        message["command_name"] = "auto_mode";

        // 创建嵌套的QJsonObject用于"paras"
        QJsonObject parasObject;
        // 在parasObject中添加"value"字段
        parasObject["value"] = "OFF";

        // 将parasObject添加到jsonObject中
        message["paras"] = parasObject;
    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // 发送POST请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 连接信号以处理响应
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseString = QString::fromUtf8(reply->readAll());
            qDebug() << "Message Sent Successfully. Response:" << responseString;

            // 在这里可以进一步处理响应数据

        } else {
            qDebug() << "Error sending message:" << reply->errorString();
        }
        reply->deleteLater();
    });
    ui->label_28->setStyleSheet("image: url(:/pic/icon_1/zidongmoshi.png);");
}

void Widget::updateAndRedrawCurve()
{
    QVector<double> timeData; // 时间数据
    QVector<double> tempData; // 温度数据

            // 这里只是模拟数据，你可以替换为真实的温度数据
        double temp = voltageIntValue;
        timeData.append(t++);
        tempData.append(temp);
        if (timeData.size() > 24) {
                timeData.removeFirst();
                tempData.removeFirst();
        }

        QwtPlotCurve *curve = new QwtPlotCurve("电压");
        curve->setSamples(timeData, tempData);
        curve->attach(ui->qwtPlot);
        curve->setStyle(QwtPlotCurve::Lines);
        curve->setPen(QPen(Qt::blue, 2)); // 设置曲线样式

        // 如果你还想显示数据点，可以添加以下代码
        QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::red), QPen(Qt::black), QSize(7, 7));
        curve->setSymbol(symbol);
        ui->qwtPlot->replot();
        ui->qwtPlot->show();
}
