#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QPainter>
#include <QPixmap>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

signals:
    void back();
    void dataEntered(const QString &data1,const QString &data2,const QString &data3,const QString &data4,const QString &data5,const QString &data6,const QString &data7,const QString &data8,const QString &data9,const QString &data10,const QString &data11,const QString &data12); // 定义一个信号来传递数据

private slots:
    void on_pushButton_clicked();



    void on_pushButton_4_clicked();

    void handleComboBoxSelection(int index);
    void on_pushButton_2_clicked();



private:
    Ui::Form *ui;
    QSqlTableModel *m;

};

#endif // FORM_H
