#include "Main_MPW.h"

Main_MPW::Main_MPW(QWidget *parent) : QWidget(parent)
{
    button1 = new QPushButton("RSA");
    button2 = new QPushButton("заглушка");
    button3 = new QPushButton("заглушка");
    button4 = new QPushButton("заглушка");
    button5 = new QPushButton("заглушка");

    layout = new QVBoxLayout;
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);

    setLayout(layout);

    connect(button1, SIGNAL (clicked()), this, SLOT (handleButton1()));
}

void Main_MPW::handleButton1()
{
    emit test_f();
}
