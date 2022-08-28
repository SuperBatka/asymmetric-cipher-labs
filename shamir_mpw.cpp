#include "shamir_mpw.h"

Shamir_MPW::Shamir_MPW(QWidget *parent) : QWidget(parent)
{

    alice_layout  = new QVBoxLayout();
    bob_layout    = new QVBoxLayout();
    log_layout    = new QVBoxLayout();
    main_layout   = new QHBoxLayout();

    alice_box     = new QGroupBox("Alice");
    bob_box       = new QGroupBox("Bob");
    log_box       = new QGroupBox("Log");
    log_edit      = new QTextEdit;

    alice_message_edit = new QLineEdit;
    QRegularExpression re("^[1-9][0-9]{0,}");
    QValidator *validator = new QRegularExpressionValidator(re, this);
    alice_message_edit->setValidator(validator);
    alice_random_C_edit = new QLineEdit;
    alice_random_D_edit = new QLineEdit;
    alice_calculated1_edit = new QLineEdit;
    alice_got_edit = new QLineEdit;
    alice_calculated2_edit = new QLineEdit;


    bob_message_edit = new QLineEdit;
    bob_random_C_edit = new QLineEdit;
    bob_random_D_edit = new QLineEdit;
    bob_calculated1_edit = new QLineEdit;
    bob_got_edit = new QLineEdit;
    bob_got2_edit = new QLineEdit;


    alice_message_label = new QLabel("Сообщение");
    alice_random_C_label = new QLabel("Первое число из случайной пары Алисы");
    alice_random_D_label = new QLabel("Второе число из случайной пары Алисы");
    alice_calculated1_label = new QLabel("Вычисленное число x1");
    alice_got_label = new QLabel("Полученное от Боба число x2");
    alice_calculated2_label = new QLabel("Вычисленное число x3");

    bob_random_C_label = new QLabel("Первое число из случайной пары Боба");
    bob_random_D_label = new QLabel("Второе число из случайной пары Боба");
    bob_got_label = new QLabel("Полученное от Алисы число x1");
    bob_calculated1_label = new QLabel("Вычисленное число x2");
    bob_got2_label = new QLabel("Полученное от Алисы число x3");
    bob_message_label = new QLabel("Изначальное сообщение Алисы");

    send_message_button = new QPushButton("Отправить сообщение");
    message_layout = new QHBoxLayout;
    message_layout->addWidget(alice_message_edit);
    message_layout->addWidget(send_message_button);

    alice_layout->addWidget(alice_message_label);
    alice_layout->addLayout(message_layout);
    alice_layout->addWidget(alice_random_C_label);
    alice_layout->addWidget(alice_random_C_edit);

    alice_layout->addWidget(alice_random_D_label);
    alice_layout->addWidget(alice_random_D_edit);
    alice_layout->addWidget(alice_calculated1_label);
    alice_layout->addWidget(alice_calculated1_edit);
    alice_layout->addWidget(alice_got_label);
    alice_layout->addWidget(alice_got_edit);
    alice_layout->addWidget(alice_calculated2_label);
    alice_layout->addWidget(alice_calculated2_edit);



    bob_layout->addWidget(bob_random_C_label);
    bob_layout->addWidget(bob_random_C_edit);

    bob_layout->addWidget(bob_random_D_label);
    bob_layout->addWidget(bob_random_D_edit);
    bob_layout->addWidget(bob_got_label);
    bob_layout->addWidget(bob_got_edit);
    bob_layout->addWidget(bob_calculated1_label);
    bob_layout->addWidget(bob_calculated1_edit);
    bob_layout->addWidget(bob_got2_label);
    bob_layout->addWidget(bob_got2_edit);
    bob_layout->addWidget(bob_message_label);
    bob_layout->addWidget(bob_message_edit);


    log_layout->addWidget(log_edit);


    alice_box->setLayout(alice_layout);
    bob_box->setLayout(bob_layout);
    log_box->setLayout(log_layout);

    main_layout->addWidget(alice_box);
    main_layout->addWidget(log_box);
    main_layout->addWidget(bob_box);

    setLayout(main_layout);

    connect(send_message_button, SIGNAL(clicked()),this, SLOT(sendMessageHandler()));
}


void Shamir_MPW::sendMessageHandler()
{
    QString message = alice_message_edit->text();
    if (message.length() == 0)
    {
        QMessageBox::warning(this,"Warning", "Вы ввели некорретные данные");
        return;
    }
    if (message.length() > 80)
    {
        QMessageBox::warning(this,"Warning", "Введите сообщение поменьше");
        return;
    }
    log_edit->append("0. START\n");
    shamir shm;
    char *prime_str = shm.generatePrimeNumber(message.toLocal8Bit().constData(), message.length());
    log_edit->append("1. Алиса хочет отправить сообщение Бобу, для этого она генерирует случайное число p = " + QString(prime_str) + " и передает это число Бобу\n");
    char *first_alice_str;
    char *second_alice_str;
    char *first_bob_str;
    char *second_bob_str;
    char *x1;
    char *x2;
    char *x3;
    char *result;
    shm.generatePair(first_alice_str,second_alice_str,prime_str);
    alice_random_C_edit->setText(QString(first_alice_str));
    alice_random_D_edit->setText(QString(second_alice_str));
    log_edit->append("2. Алиса выбирает два секретныйх числа c = " + QString(first_alice_str) + " и b = " + QString(second_alice_str) +"\n");
    shm.generatePair(first_bob_str,second_bob_str,prime_str);
    bob_random_C_edit->setText(QString(first_bob_str));
    bob_random_D_edit->setText(QString(second_bob_str));
    log_edit->append("3. Боб выбирает два секретныйх числа c = " + QString(first_bob_str) + " и b = " + QString(second_bob_str)+ "\n");


    x1 = shm.calculate(message.toLocal8Bit().constData(),first_alice_str,prime_str);
    log_edit->append("4. Алиса вычисляет x1 = " + QString(x1) + " и отправляет Бобу\n");
    alice_calculated1_edit->setText(QString(x1));
    bob_got_edit->setText(QString(x1));
    x2 = shm.calculate(x1,first_bob_str,prime_str);
    bob_calculated1_edit->setText(QString(x2));
    alice_got_edit->setText(QString(x2));
    log_edit->append("5. Боб получил от Алисы x1 = " + QString(x1) + ", вычислил x2 = " + QString(x2) + " и отправил результат Алисе\n");

    x3 = shm.calculate(x2,second_alice_str,prime_str);
    alice_calculated2_edit->setText(QString(x3));
    bob_got2_edit->setText(QString(x3));
    log_edit->append("6. Алиса получила от Боба x2 = " + QString(x2) + ", вычислила x3 = " + QString(x3) + " и отправила результат Бобу\n");

    result = shm.calculate(x3,second_bob_str,prime_str);
    bob_message_edit->setText(QString(result));
    log_edit->append("7. Боб получил от Алисы x3 = " + QString(x3) + ", вычислил исходное сообщение Алисы = " + QString(result)+"\n");
    log_edit->append("8. END\n");


}
