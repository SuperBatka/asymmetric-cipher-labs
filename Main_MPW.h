#ifndef MAIN_MENUPAGE_WIDGET_H
#define MAIN_MENUPAGE_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>


class Main_MPW : public QWidget
{
    Q_OBJECT
public:
    explicit Main_MPW(QWidget *parent = nullptr);



signals:
    void test_f();

private slots:
    void handleButton1( );

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;


    QVBoxLayout *layout;
};

#endif // MAIN_MENUPAGE_WIDGET_H
