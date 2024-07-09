#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPainter>
#include <QPixmap>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);


    this->setWindowTitle("智慧种植");
    ui->comboBox->addItem("胡萝卜");
    ui->comboBox->addItem("西红柿");
    ui->comboBox->addItem("黄瓜");
    ui->comboBox->addItem("青椒");
    ui->comboBox->addItem("土豆");
    ui->comboBox->addItem("菠菜");
    ui->comboBox->addItem("茄子");
    ui->comboBox->addItem("南瓜");
    ui->comboBox->addItem("花菜");
    ui->comboBox->addItem("生菜");

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setDatabaseName("greenhouse");
    db.setHostName("localhost");

    db.setUserName("root");
    db.setPassword("123456");

    if(db.open())
    {
        QMessageBox::information(this,"连接提示","连接成功");

        m = new QSqlTableModel;
        m->setTable("vegetable");
        ui->tableView->setModel(m);
    }
    else
    {
        QMessageBox::warning(this,"连接提示","连接失败");
    }
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleComboBoxSelection(int)));





    connect(ui->pushButton1,&QPushButton::clicked,[=](){
           //发个信号
           emit this->back();
       });
}

Form::~Form()
{
    delete ui;
}
void Form::on_pushButton_clicked()
{
    m->select();
}

/*void Widget::on_pushButton_2_clicked()
{
    QVariant Tempdata = ui->tableView->model()->index(0, 3).data();
    QVariant Humidata = ui->tableView->model()->index(0, 5).data();
    QVariant Lightdata = ui->tableView->model()->index(0, 7).data();
    QVariant SoilHumidata = ui->tableView->model()->index(0, 13).data();

    int Tnum = Tempdata.toInt();
    int Hnum = Humidata.toInt();
    double Lnum = Lightdata.toDouble();
    double SHnum = SoilHumidata.toDouble();
   // ui->Namelabel->setText("胡萝卜");


     ui->TspinBox->setValue(Tnum);
     ui->HspinBox->setValue(Hnum);
     ui->LspinBox->setValue(Lnum);
     ui->SHspinBox->setValue(SHnum);
}

void Widget::on_pushButton_3_clicked()
{
    QVariant Tempdata = ui->tableView->model()->index(1, 3).data();
    QVariant Humidata = ui->tableView->model()->index(1, 5).data();
    QVariant Lightdata = ui->tableView->model()->index(1, 7).data();
    QVariant SoilHumidata = ui->tableView->model()->index(1, 13).data();

    int Tnum = Tempdata.toInt();
    int Hnum = Humidata.toInt();
    double Lnum = Lightdata.toDouble();
    double SHnum = SoilHumidata.toDouble();
   // ui->Namelabel->setText("西红柿");


     ui->TspinBox->setValue(Tnum);
     ui->HspinBox->setValue(Hnum);
     ui->LspinBox->setValue(Lnum);
     ui->SHspinBox->setValue(SHnum);
}*/

void Form::on_pushButton_4_clicked()
{
    QString id = ui->IDlineEdit->text();
    QString name = ui->NamelineEdit->text();
    QString Tmin = ui->TMinlineEdit->text();
    QString Tmax = ui->TMaxlineEdit->text();
    QString Hmin = ui->HMinlineEdit->text();
    QString Hmax = ui->HMaxlineEdit->text();
    QString Lmin = ui->LMinlineEdit->text();
    QString Lmax = ui->LMaxlineEdit->text();
    QString Cmin = ui->CMinlineEdit->text();
    QString Cmax = ui->CMaxlineEdit->text();
    QString STmin = ui->STMinlineEdit->text();
    QString STmax = ui->STMaxlineEdit->text();
    QString SHmin = ui->SHMinlineEdit->text();
    QString SHmax = ui->SHMaxlineEdit->text();
    QString Dmin = ui->DMinlineEdit->text();
    QString Dmax = ui->DMaxlineEdit->text();
    QString Pmin = ui->PMinlineEdit->text();
    QString Pmax = ui->PMaxlineEdit->text();
    QString sql = QString("insert into vegetable values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18');").arg(id).arg(name).arg(Tmin).arg(Tmax).arg(Hmin).arg(Hmax).arg(Lmin).arg(Lmax).arg(Cmin).arg(Cmax).arg(STmin).arg(STmax).arg(SHmin).arg(SHmax).arg(Dmin).arg(Dmax).arg(Pmin).arg(Pmax);
    QSqlQuery query;
    if(query.exec(sql))
    {
        QMessageBox::information(this,"插入提示","插入成功");
    }
    else
    {
        QMessageBox::information(this,"插入提示","插入失败");
    }
    QList<QLineEdit*> lineEditList = {
        ui->IDlineEdit, ui->NamelineEdit, ui->TMinlineEdit, ui->TMaxlineEdit, ui->HMinlineEdit,
        ui->HMaxlineEdit, ui->LMinlineEdit, ui->LMaxlineEdit, ui->CMinlineEdit, ui->CMaxlineEdit,
        ui->STMinlineEdit, ui->STMaxlineEdit, ui->SHMinlineEdit, ui->SHMaxlineEdit, ui->DMinlineEdit,
        ui->DMaxlineEdit, ui->PMinlineEdit, ui->PMaxlineEdit
    };

    for (QLineEdit* lineEdit : lineEditList) {
        lineEdit->clear();
    }

}
void Form::handleComboBoxSelection(int index)
{
    QVariant Tempdata0_0 = ui->tableView->model()->index(0, 2).data();
    QVariant Tempdata0 = ui->tableView->model()->index(0, 3).data();
    QVariant Humidata0_0 = ui->tableView->model()->index(0, 4).data();
    QVariant Humidata0 = ui->tableView->model()->index(0, 5).data();
    QVariant Lightdata0_0 = ui->tableView->model()->index(0, 6).data();
    QVariant Lightdata0 = ui->tableView->model()->index(0, 7).data();
    QVariant CO2data0_0 = ui->tableView->model()->index(0, 8).data();
    QVariant CO2data0 = ui->tableView->model()->index(0, 9).data();
    QVariant STempdata0_0 = ui->tableView->model()->index(0, 10).data();
    QVariant STempdata0 = ui->tableView->model()->index(0, 11).data();
    QVariant SoilHumidata0_0 = ui->tableView->model()->index(0, 12).data();
    QVariant SoilHumidata0 = ui->tableView->model()->index(0, 13).data();


    QVariant Tempdata1_1 = ui->tableView->model()->index(1, 2).data();
    QVariant Tempdata1 = ui->tableView->model()->index(1, 3).data();
    QVariant Humidata1_1 = ui->tableView->model()->index(1, 4).data();
    QVariant Humidata1 = ui->tableView->model()->index(1, 5).data();
    QVariant Lightdata1_1 = ui->tableView->model()->index(1, 6).data();
    QVariant Lightdata1 = ui->tableView->model()->index(1, 7).data();
    QVariant CO2data1_1 = ui->tableView->model()->index(1, 8).data();
    QVariant CO2data1 = ui->tableView->model()->index(1, 9).data();
    QVariant STempdata1_1 = ui->tableView->model()->index(1, 10).data();
    QVariant STempdata1 = ui->tableView->model()->index(1, 11).data();
    QVariant SoilHumidata1_1 = ui->tableView->model()->index(1, 12).data();
    QVariant SoilHumidata1 = ui->tableView->model()->index(1, 13).data();


    QVariant Tempdata2_2 = ui->tableView->model()->index(2, 2).data();
    QVariant Tempdata2 = ui->tableView->model()->index(2, 3).data();
    QVariant Humidata2_2 = ui->tableView->model()->index(2, 4).data();
    QVariant Humidata2 = ui->tableView->model()->index(2, 5).data();
    QVariant Lightdata2_2 = ui->tableView->model()->index(2, 6).data();
    QVariant Lightdata2 = ui->tableView->model()->index(2, 7).data();
    QVariant CO2data2_2 = ui->tableView->model()->index(2, 8).data();
    QVariant CO2data2 = ui->tableView->model()->index(2, 9).data();
    QVariant STempdata2_2 = ui->tableView->model()->index(2, 10).data();
    QVariant STempdata2 = ui->tableView->model()->index(2, 11).data();
    QVariant SoilHumidata2_2 = ui->tableView->model()->index(2, 12).data();
    QVariant SoilHumidata2 = ui->tableView->model()->index(2, 13).data();


    QVariant Tempdata3_3 = ui->tableView->model()->index(3, 2).data();
    QVariant Tempdata3 = ui->tableView->model()->index(3, 3).data();
    QVariant Humidata3_3 = ui->tableView->model()->index(3, 4).data();
    QVariant Humidata3 = ui->tableView->model()->index(3, 5).data();
    QVariant Lightdata3_3 = ui->tableView->model()->index(3, 6).data();
    QVariant Lightdata3 = ui->tableView->model()->index(3, 7).data();
    QVariant CO2data3_3 = ui->tableView->model()->index(3, 8).data();
    QVariant CO2data3 = ui->tableView->model()->index(3, 9).data();
    QVariant STempdata3_3 = ui->tableView->model()->index(3, 10).data();
    QVariant STempdata3 = ui->tableView->model()->index(3, 11).data();
    QVariant SoilHumidata3_3 = ui->tableView->model()->index(3, 12).data();
    QVariant SoilHumidata3 = ui->tableView->model()->index(3, 13).data();


    QVariant Tempdata4_4 = ui->tableView->model()->index(4, 2).data();
    QVariant Tempdata4 = ui->tableView->model()->index(4, 3).data();
    QVariant Humidata4_4 = ui->tableView->model()->index(4, 4).data();
    QVariant Humidata4 = ui->tableView->model()->index(4, 5).data();
    QVariant Lightdata4_4 = ui->tableView->model()->index(4, 6).data();
    QVariant Lightdata4 = ui->tableView->model()->index(4, 7).data();
    QVariant CO2data4_4 = ui->tableView->model()->index(4, 8).data();
    QVariant CO2data4 = ui->tableView->model()->index(4, 9).data();
    QVariant STempdata4_4 = ui->tableView->model()->index(4, 10).data();
    QVariant STempdata4 = ui->tableView->model()->index(4, 11).data();
    QVariant SoilHumidata4_4 = ui->tableView->model()->index(4, 12).data();
    QVariant SoilHumidata4 = ui->tableView->model()->index(4, 13).data();


    QVariant Tempdata5_5 = ui->tableView->model()->index(5, 2).data();
    QVariant Tempdata5 = ui->tableView->model()->index(5, 3).data();
    QVariant Humidata5_5 = ui->tableView->model()->index(5, 4).data();
    QVariant Humidata5 = ui->tableView->model()->index(5, 5).data();
    QVariant Lightdata5_5 = ui->tableView->model()->index(5, 6).data();
    QVariant Lightdata5 = ui->tableView->model()->index(5, 7).data();
    QVariant CO2data5_5 = ui->tableView->model()->index(5, 8).data();
    QVariant CO2data5 = ui->tableView->model()->index(5, 9).data();
    QVariant STempdata5_5 = ui->tableView->model()->index(5, 10).data();
    QVariant STempdata5 = ui->tableView->model()->index(5, 11).data();
    QVariant SoilHumidata5_5 = ui->tableView->model()->index(5, 12).data();
    QVariant SoilHumidata5 = ui->tableView->model()->index(5, 13).data();


    QVariant Tempdata6_6 = ui->tableView->model()->index(6, 2).data();
    QVariant Tempdata6 = ui->tableView->model()->index(6, 3).data();
    QVariant Humidata6_6 = ui->tableView->model()->index(6, 4).data();
    QVariant Humidata6 = ui->tableView->model()->index(6, 5).data();
    QVariant Lightdata6_6 = ui->tableView->model()->index(6, 6).data();
    QVariant Lightdata6 = ui->tableView->model()->index(6, 7).data();
    QVariant CO2data6_6 = ui->tableView->model()->index(6, 8).data();
    QVariant CO2data6 = ui->tableView->model()->index(6, 9).data();
    QVariant STempdata6_6 = ui->tableView->model()->index(6, 10).data();
    QVariant STempdata6 = ui->tableView->model()->index(6, 11).data();
    QVariant SoilHumidata6_6 = ui->tableView->model()->index(6, 12).data();
    QVariant SoilHumidata6 = ui->tableView->model()->index(6, 13).data();


    QVariant Tempdata7_7 = ui->tableView->model()->index(7, 2).data();
    QVariant Tempdata7 = ui->tableView->model()->index(7, 3).data();
    QVariant Humidata7_7 = ui->tableView->model()->index(7, 4).data();
    QVariant Humidata7 = ui->tableView->model()->index(7, 5).data();
    QVariant Lightdata7_7 = ui->tableView->model()->index(7, 6).data();
    QVariant Lightdata7 = ui->tableView->model()->index(7, 7).data();
    QVariant CO2data7_7 = ui->tableView->model()->index(7, 8).data();
    QVariant CO2data7 = ui->tableView->model()->index(7, 9).data();
    QVariant STempdata7_7 = ui->tableView->model()->index(7, 10).data();
    QVariant STempdata7 = ui->tableView->model()->index(7, 11).data();
    QVariant SoilHumidata7_7 = ui->tableView->model()->index(7, 12).data();
    QVariant SoilHumidata7 = ui->tableView->model()->index(7, 13).data();


    QVariant Tempdata8_8 = ui->tableView->model()->index(8, 2).data();
    QVariant Tempdata8 = ui->tableView->model()->index(8, 3).data();
    QVariant Humidata8_8 = ui->tableView->model()->index(8, 4).data();
    QVariant Humidata8 = ui->tableView->model()->index(8, 5).data();
    QVariant Lightdata8_8 = ui->tableView->model()->index(8, 6).data();
    QVariant Lightdata8 = ui->tableView->model()->index(8, 7).data();
    QVariant CO2data8_8 = ui->tableView->model()->index(8, 8).data();
    QVariant CO2data8 = ui->tableView->model()->index(8, 9).data();
    QVariant STempdata8_8 = ui->tableView->model()->index(8, 10).data();
    QVariant STempdata8 = ui->tableView->model()->index(8, 11).data();
    QVariant SoilHumidata8_8 = ui->tableView->model()->index(8, 12).data();
    QVariant SoilHumidata8 = ui->tableView->model()->index(8, 13).data();


    QVariant Tempdata9_9 = ui->tableView->model()->index(9, 2).data();
    QVariant Tempdata9 = ui->tableView->model()->index(9, 3).data();
    QVariant Humidata9_9 = ui->tableView->model()->index(9, 4).data();
    QVariant Humidata9 = ui->tableView->model()->index(9, 5).data();
    QVariant Lightdata9_9 = ui->tableView->model()->index(9, 6).data();
    QVariant Lightdata9 = ui->tableView->model()->index(9, 7).data();
    QVariant CO2data9_9 = ui->tableView->model()->index(9, 8).data();
    QVariant CO2data9 = ui->tableView->model()->index(9, 9).data();
    QVariant STempdata9_9 = ui->tableView->model()->index(9, 10).data();
    QVariant STempdata9 = ui->tableView->model()->index(9, 11).data();
    QVariant SoilHumidata9_9 = ui->tableView->model()->index(9, 12).data();
    QVariant SoilHumidata9 = ui->tableView->model()->index(9, 13).data();


    int Tnum0_0 = Tempdata0_0.toInt();
    int Tnum0 = Tempdata0.toInt();
    int Hnum0_0 = Humidata0_0.toInt();
    int Hnum0 = Humidata0.toInt();
    double Lnum0_0 = Lightdata0_0.toDouble();
    double Lnum0 = Lightdata0.toDouble();
    double CO2num0_0 = CO2data0_0.toDouble();
    double CO2num0 = CO2data0.toDouble();
    double STempnum0_0 = STempdata0_0.toDouble();
    double STempnum0 = STempdata0.toDouble();
    double SHnum0_0 = SoilHumidata0_0.toDouble();
    double SHnum0 = SoilHumidata0.toDouble();


    int Tnum1_1 = Tempdata1_1.toInt();
    int Tnum1 = Tempdata1.toInt();
    int Hnum1_1 = Humidata1_1.toInt();
    int Hnum1 = Humidata1.toInt();
    double Lnum1_1 = Lightdata1_1.toDouble();
    double Lnum1 = Lightdata1.toDouble();
    double CO2num1_1 = CO2data1_1.toDouble();
    double CO2num1 = CO2data1.toDouble();
    double STempnum1_1 = STempdata1_1.toDouble();
    double STempnum1 = STempdata1.toDouble();
    double SHnum1_1 = SoilHumidata1_1.toDouble();
    double SHnum1 = SoilHumidata1.toDouble();

    int Tnum2_2 = Tempdata2_2.toInt();
    int Tnum2 = Tempdata2.toInt();
    int Hnum2_2 = Humidata2_2.toInt();
    int Hnum2 = Humidata0.toInt();
    double Lnum2_2 = Lightdata2_2.toDouble();
    double Lnum2 = Lightdata2.toDouble();
    double CO2num2_2 = CO2data2_2.toDouble();
    double CO2num2 = CO2data2.toDouble();
    double STempnum2_2 = STempdata2_2.toDouble();
    double STempnum2 = STempdata2.toDouble();
    double SHnum2_2 = SoilHumidata2_2.toDouble();
    double SHnum2 = SoilHumidata2.toDouble();

    int Tnum3_3 = Tempdata3_3.toInt();
    int Tnum3 = Tempdata3.toInt();
    int Hnum3_3 = Humidata3_3.toInt();
    int Hnum3 = Humidata3.toInt();
    double Lnum3_3 = Lightdata3_3.toDouble();
    double Lnum3 = Lightdata3.toDouble();
    double CO2num3_3 = CO2data3_3.toDouble();
    double CO2num3 = CO2data3.toDouble();
    double STempnum3_3 = STempdata3_3.toDouble();
    double STempnum3 = STempdata3.toDouble();
    double SHnum3_3 = SoilHumidata3_3.toDouble();
    double SHnum3 = SoilHumidata3.toDouble();

    int Tnum4_4 = Tempdata4_4.toInt();
    int Tnum4 = Tempdata4.toInt();
    int Hnum4_4 = Humidata4_4.toInt();
    int Hnum4 = Humidata4.toInt();
    double Lnum4_4 = Lightdata4_4.toDouble();
    double Lnum4 = Lightdata4.toDouble();
    double CO2num4_4 = CO2data4_4.toDouble();
    double CO2num4 = CO2data4.toDouble();
    double STempnum4_4 = STempdata4_4.toDouble();
    double STempnum4 = STempdata4.toDouble();
    double SHnum4_4 = SoilHumidata4_4.toDouble();
    double SHnum4 = SoilHumidata4.toDouble();

    int Tnum5_5 = Tempdata5_5.toInt();
    int Tnum5 = Tempdata5.toInt();
    int Hnum5_5 = Humidata5_5.toInt();
    int Hnum5 = Humidata5.toInt();
    double Lnum5_5 = Lightdata5_5.toDouble();
    double Lnum5 = Lightdata5.toDouble();
    double CO2num5_5 = CO2data5_5.toDouble();
    double CO2num5 = CO2data5.toDouble();
    double STempnum5_5 = STempdata5_5.toDouble();
    double STempnum5 = STempdata5.toDouble();
    double SHnum5_5 = SoilHumidata5_5.toDouble();
    double SHnum5 = SoilHumidata5.toDouble();

    int Tnum6_6 = Tempdata6_6.toInt();
    int Tnum6 = Tempdata6.toInt();
    int Hnum6_6 = Humidata6_6.toInt();
    int Hnum6 = Humidata6.toInt();
    double Lnum6_6 = Lightdata6_6.toDouble();
    double Lnum6 = Lightdata6.toDouble();
    double CO2num6_6 = CO2data6_6.toDouble();
    double CO2num6 = CO2data6.toDouble();
    double STempnum6_6 = STempdata6_6.toDouble();
    double STempnum6 = STempdata6.toDouble();
    double SHnum6_6 = SoilHumidata6_6.toDouble();
    double SHnum6 = SoilHumidata6.toDouble();

    int Tnum7_7 = Tempdata7_7.toInt();
    int Tnum7 = Tempdata7.toInt();
    int Hnum7_7 = Humidata7_7.toInt();
    int Hnum7 = Humidata7.toInt();
    double Lnum7_7 = Lightdata7_7.toDouble();
    double Lnum7 = Lightdata7.toDouble();
    double CO2num7_7 = CO2data7_7.toDouble();
    double CO2num7 = CO2data7.toDouble();
    double STempnum7_7 = STempdata7_7.toDouble();
    double STempnum7 = STempdata7.toDouble();
    double SHnum7_7 = SoilHumidata7_7.toDouble();
    double SHnum7 = SoilHumidata7.toDouble();

    int Tnum8_8 = Tempdata8_8.toInt();
    int Tnum8 = Tempdata8.toInt();
    int Hnum8_8 = Humidata8_8.toInt();
    int Hnum8 = Humidata8.toInt();
    double Lnum8_8 = Lightdata8_8.toDouble();
    double Lnum8 = Lightdata8.toDouble();
    double CO2num8_8 = CO2data8_8.toDouble();
    double CO2num8 = CO2data8.toDouble();
    double STempnum8_8 = STempdata8_8.toDouble();
    double STempnum8 = STempdata8.toDouble();
    double SHnum8_8 = SoilHumidata8_8.toDouble();
    double SHnum8 = SoilHumidata8.toDouble();

    int Tnum9_9 = Tempdata9_9.toInt();
    int Tnum9 = Tempdata9.toInt();
    int Hnum9_9 = Humidata9_9.toInt();
    int Hnum9 = Humidata9.toInt();
    double Lnum9_9 = Lightdata9_9.toDouble();
    double Lnum9 = Lightdata9.toDouble();
    double CO2num9_9 = CO2data9_9.toDouble();
    double CO2num9 = CO2data0.toDouble();
    double STempnum9_9 = STempdata9_9.toDouble();
    double STempnum9 = STempdata9.toDouble();
    double SHnum9_9 = SoilHumidata9_9.toDouble();
    double SHnum9 = SoilHumidata9.toDouble();
    /*QString selectedOption = ui->comboBox->itemText(index);
    qDebug() << "User selected: " << selectedOption;*/
    if(index == 0) {
        ui->spinBox->setValue(Tnum0_0);
        ui->TspinBox->setValue(Tnum0);
        ui->spinBox_2->setValue(Hnum0_0);
        ui->HspinBox->setValue(Hnum0);
        ui->spinBox_3->setValue(CO2num0_0);
        ui->spinBox_7->setValue(CO2num0);
        ui->spinBox_4->setValue(Lnum0_0);
        ui->LspinBox->setValue(Lnum0);
        ui->spinBox_5->setValue(STempnum0_0);
        ui->spinBox_8->setValue(STempnum0);
        ui->spinBox_6->setValue(SHnum0_0);
        ui->SHspinBox->setValue(SHnum0);
    } else if(index == 1) {
        ui->spinBox->setValue(Tnum1_1);
        ui->TspinBox->setValue(Tnum1);
        ui->spinBox_2->setValue(Hnum1_1);
        ui->HspinBox->setValue(Hnum1);
        ui->spinBox_3->setValue(CO2num1_1);
        ui->spinBox_7->setValue(CO2num1);
        ui->spinBox_4->setValue(Lnum1_1);
        ui->LspinBox->setValue(Lnum1);
        ui->spinBox_5->setValue(STempnum1_1);
        ui->spinBox_8->setValue(STempnum1);
        ui->spinBox_6->setValue(SHnum1_1);
        ui->SHspinBox->setValue(SHnum1);
    }else if(index == 2){
        ui->spinBox->setValue(Tnum2_2);
        ui->TspinBox->setValue(Tnum2);
        ui->spinBox_2->setValue(Hnum2_2);
        ui->HspinBox->setValue(Hnum2);
        ui->spinBox_3->setValue(CO2num2_2);
        ui->spinBox_7->setValue(CO2num2);
        ui->spinBox_4->setValue(Lnum2_2);
        ui->LspinBox->setValue(Lnum2);
        ui->spinBox_5->setValue(STempnum2_2);
        ui->spinBox_8->setValue(STempnum2);
        ui->spinBox_6->setValue(SHnum2_2);
        ui->SHspinBox->setValue(SHnum2);
    }else if(index == 3){
        ui->spinBox->setValue(Tnum3_3);
        ui->TspinBox->setValue(Tnum3);
        ui->spinBox_2->setValue(Hnum3_3);
        ui->HspinBox->setValue(Hnum3);
        ui->spinBox_3->setValue(CO2num3_3);
        ui->spinBox_7->setValue(CO2num3);
        ui->spinBox_4->setValue(Lnum3_3);
        ui->LspinBox->setValue(Lnum3);
        ui->spinBox_5->setValue(STempnum3_3);
        ui->spinBox_8->setValue(STempnum3);
        ui->spinBox_6->setValue(SHnum3_3);
        ui->SHspinBox->setValue(SHnum3);
    }else if(index == 4){
        ui->spinBox->setValue(Tnum4_4);
        ui->TspinBox->setValue(Tnum4);
        ui->spinBox_2->setValue(Hnum4_4);
        ui->HspinBox->setValue(Hnum4);
        ui->spinBox_3->setValue(CO2num4_4);
        ui->spinBox_7->setValue(CO2num4);
        ui->spinBox_4->setValue(Lnum4_4);
        ui->LspinBox->setValue(Lnum4);
        ui->spinBox_5->setValue(STempnum4_4);
        ui->spinBox_8->setValue(STempnum4);
        ui->spinBox_6->setValue(SHnum4_4);
        ui->SHspinBox->setValue(SHnum4);
    }else if(index == 5){
        ui->spinBox->setValue(Tnum5_5);
        ui->TspinBox->setValue(Tnum5);
        ui->spinBox_2->setValue(Hnum5_5);
        ui->HspinBox->setValue(Hnum5);
        ui->spinBox_3->setValue(CO2num5_5);
        ui->spinBox_7->setValue(CO2num5);
        ui->spinBox_4->setValue(Lnum5_5);
        ui->LspinBox->setValue(Lnum5);
        ui->spinBox_5->setValue(STempnum5_5);
        ui->spinBox_8->setValue(STempnum5);
        ui->spinBox_6->setValue(SHnum5_5);
        ui->SHspinBox->setValue(SHnum5);
    }else if(index == 6){
        ui->spinBox->setValue(Tnum6_6);
        ui->TspinBox->setValue(Tnum6);
        ui->spinBox_2->setValue(Hnum6_6);
        ui->HspinBox->setValue(Hnum6);
        ui->spinBox_3->setValue(CO2num6_6);
        ui->spinBox_7->setValue(CO2num6);
        ui->spinBox_4->setValue(Lnum6_6);
        ui->LspinBox->setValue(Lnum6);
        ui->spinBox_5->setValue(STempnum6_6);
        ui->spinBox_8->setValue(STempnum6);
        ui->spinBox_6->setValue(SHnum6_6);
        ui->SHspinBox->setValue(SHnum6);
    }else if(index == 7){
        ui->spinBox->setValue(Tnum7_7);
        ui->TspinBox->setValue(Tnum7);
        ui->spinBox_2->setValue(Hnum7_7);
        ui->HspinBox->setValue(Hnum7);
        ui->spinBox_3->setValue(CO2num7_7);
        ui->spinBox_7->setValue(CO2num7);
        ui->spinBox_4->setValue(Lnum7_7);
        ui->LspinBox->setValue(Lnum7);
        ui->spinBox_5->setValue(STempnum7_7);
        ui->spinBox_8->setValue(STempnum7);
        ui->spinBox_6->setValue(SHnum7_7);
        ui->SHspinBox->setValue(SHnum7);
    }else if(index == 8){
        ui->spinBox->setValue(Tnum8_8);
        ui->TspinBox->setValue(Tnum8);
        ui->spinBox_2->setValue(Hnum8_8);
        ui->HspinBox->setValue(Hnum8);
        ui->spinBox_3->setValue(CO2num8_8);
        ui->spinBox_7->setValue(CO2num8);
        ui->spinBox_4->setValue(Lnum8_8);
        ui->LspinBox->setValue(Lnum8);
        ui->spinBox_5->setValue(STempnum8_8);
        ui->spinBox_8->setValue(STempnum8);
        ui->spinBox_6->setValue(SHnum8_8);
        ui->SHspinBox->setValue(SHnum8);
    }else if(index == 9){
        ui->spinBox->setValue(Tnum9_9);
        ui->TspinBox->setValue(Tnum9);
        ui->spinBox_2->setValue(Hnum9_9);
        ui->HspinBox->setValue(Hnum9);
        ui->spinBox_3->setValue(CO2num9_9);
        ui->spinBox_7->setValue(CO2num9);
        ui->spinBox_4->setValue(Lnum9_9);
        ui->LspinBox->setValue(Lnum9);
        ui->spinBox_5->setValue(STempnum9_9);
        ui->spinBox_8->setValue(STempnum9);
        ui->spinBox_6->setValue(SHnum9_9);
        ui->SHspinBox->setValue(SHnum9);
    }
}




void Form::on_pushButton_2_clicked()
{

    int tempmin = ui->spinBox->value();
    int temp = ui->TspinBox->value();
    int humimin = ui->spinBox_2->value();
    int humi = ui->HspinBox->value();
    int CO2min = ui->spinBox_3->value();
    int CO2 = ui->spinBox_7->value();
    int ligthmin = ui->spinBox_4->value();
    int light = ui->LspinBox->value();
    int soiltempmin = ui->spinBox_5->value();
    int soiltemp = ui->spinBox_8->value();
    int soilhumimin = ui->spinBox_6->value();
    int soilhumi = ui->SHspinBox->value();

    QString tempmin1 = QString::number(tempmin);
    QString temp1 = QString::number(temp);
    QString humimin1 = QString::number(humimin);
    QString humi1 = QString::number(humi);
    QString ligthmin1 = QString::number(ligthmin);
    QString light1 = QString::number(light);
    QString CO2min1 = QString ::number(CO2min);
    QString CO21 = QString::number(CO2);
    QString soiltempmin1 = QString::number(soiltempmin);
    QString soiltemp1 = QString::number(soiltemp);
    QString soilhumimin1 = QString::number(soilhumimin);
    QString soilhumi1 = QString::number(soilhumi);

   // QString enteredData = ui->lineEdit->text(); // 获取输入的数据
    emit dataEntered(tempmin1,temp1,humimin1,humi1,ligthmin1,light1,CO21,CO2min1,soiltempmin1,soiltemp1,soilhumimin1,soilhumi1); // 发射信号，传递数据

}
