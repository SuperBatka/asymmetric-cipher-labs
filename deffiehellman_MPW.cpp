#include "deffiehellman_MPW.h"


DeffieHellman_MPW::DeffieHellman_MPW(QWidget *parent) : QWidget(parent)
{
    alice_layout  = new QVBoxLayout();
    bob_layout    = new QVBoxLayout();
    log_layout    = new QVBoxLayout();
    main_layout   = new QHBoxLayout();

    alice_box     = new QGroupBox("Alice");
    bob_box       = new QGroupBox("Bob");
    log_box       = new QGroupBox("Log");
    log_edit      = new QTextEdit;




    alice_start_button = new QPushButton("Начать общение");
    bob_start_button   = new QPushButton("Начать общение");

    g_bob_edit         = new QLineEdit;
    p_bob_edit         = new QLineEdit;

    g_alice_edit       = new QLineEdit;
    p_alice_edit       = new QLineEdit;

    random_bob_edit     = new QLineEdit;
    calculated_bob_edit = new QLineEdit;

    random_alice_edit   = new QLineEdit;
    calculated_alice_edit = new QLineEdit;

    got_alice_edit          = new QLineEdit;
    session_key_alice_edit  = new QLineEdit;

    got_bob_edit            = new QLineEdit;
    session_key_bob_edit    = new QLineEdit;



    g_alice_label           = new QLabel("Число g");
    p_alice_label           = new QLabel("Число p");
    random_alice_label      = new QLabel("Случайное число \"а\" Алисы");
    calculated_alice_label  = new QLabel("Вычисленное число \"А\" Алисы");
    got_alice_label         = new QLabel("Полученное число \"B\" от Боба");
    session_key_alice_label = new QLabel("Выработанный общий ключ \"К\"");

    g_bob_label             = new QLabel("Число g");
    p_bob_label             = new QLabel("Число p");
    random_bob_label        = new QLabel("Случайное число \"b\" Боба");
    calculated_bob_label    = new QLabel("Вычисленное число \"B\" Боба");
    got_bob_label           = new QLabel("Полученное число \"A\" от Алисы");
    session_key_bob_label   = new QLabel("Выработанный общий ключ \"К\"");


    alice_layout->addWidget(alice_start_button);
    alice_layout->addWidget(g_alice_label);
    alice_layout->addWidget(g_alice_edit);
    alice_layout->addWidget(p_alice_label);
    alice_layout->addWidget(p_alice_edit);
    alice_layout->addWidget(random_alice_label);
    alice_layout->addWidget(random_alice_edit);
    alice_layout->addWidget(calculated_alice_label);
    alice_layout->addWidget(calculated_alice_edit);
    alice_layout->addWidget(got_alice_label);
    alice_layout->addWidget(got_alice_edit);
    alice_layout->addWidget(session_key_alice_label);
    alice_layout->addWidget(session_key_alice_edit);

    bob_layout->addWidget(bob_start_button);
    bob_layout->addWidget(g_bob_label);
    bob_layout->addWidget(g_bob_edit);
    bob_layout->addWidget(p_bob_label);
    bob_layout->addWidget(p_bob_edit);
    bob_layout->addWidget(random_bob_label);
    bob_layout->addWidget(random_bob_edit);
    bob_layout->addWidget(calculated_bob_label);
    bob_layout->addWidget(calculated_bob_edit);
    bob_layout->addWidget(got_bob_label);
    bob_layout->addWidget(got_bob_edit);
    bob_layout->addWidget(session_key_bob_label);
    bob_layout->addWidget(session_key_bob_edit);

    log_layout->addWidget( log_edit);


    alice_box->setLayout(alice_layout);
    bob_box->setLayout(bob_layout);
    log_box->setLayout(log_layout);

    main_layout->addWidget(alice_box);
    main_layout->addWidget(log_box);
    main_layout->addWidget(bob_box);

    setLayout(main_layout);

    connect(alice_start_button, SIGNAL(clicked()),this, SLOT(aliceStartHandler()));
    connect(bob_start_button, SIGNAL(clicked()), this, SLOT(bobStartHandler()));
}

/*void DeffieHellman_MPW::aliceStartHandler()
{
    size_t digits_count =10;
    Deffiehellman dh;
    char *q_prime_str;
    char *p_prime_str;
    char *p_generator_str;
    log_edit->append("0. -- START -- \n");
    log_edit->append("1. Алиса хочет начать общение \n");
    p_prime_str = dh.generatePrimeNumbers(q_prime_str,digits_count);
    p_generator_str = dh.generateGeneratorNumber(p_prime_str,q_prime_str);
    QString p(p_prime_str);
    QString g(p_generator_str);
    p_alice_edit->setText(p);
    g_alice_edit->setText(g);
    log_edit->append("2. Алиса выбрала g =  " + g + " и p = " + p +"\n");


    QString a(dh.generateNumber(digits_count-1));
    QString A(dh.calculate(g.toLocal8Bit().constData(),a.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    random_alice_edit->setText(a);
    calculated_alice_edit->setText(A);
    log_edit->append("3. Алиса сгенерировала случайное число а =  " + a + "\n");
    log_edit->append("4. Алиса вычислила число А =  " + A  +"\n");

    log_edit->append("5. Алиса передала Бобу числа g, p, A \n");

    p_bob_edit->setText(p);
    g_bob_edit->setText(g);
    got_bob_edit->setText(A);

    QString b(dh.generateNumber(digits_count-1));
    QString B(dh.calculate(g.toLocal8Bit().constData(),b.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    random_bob_edit->setText(b);
    calculated_bob_edit->setText(B);
    log_edit->append("6. Боб сгенерировал случайное число b =  " + b + "\n");
    log_edit->append("7. Боб вычислил число B =  " + B  +"\n");
    log_edit->append("8. Боб передал Алисе число B\n");
    got_alice_edit->setText(B);

    QString alice_K(dh.calculate(B.toLocal8Bit().constData(),a.toLocal8Bit().constData(),p.toLocal8Bit().constData()));
    QString bob_K(dh.calculate(A.toLocal8Bit().constData(),b.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    log_edit->append("9. Алиса вычислила общий сессионый ключ К = "+ alice_K +"\n"
                     + "Боб вычислил общий сессионый ключ К = "+ bob_K +"\n");
    log_edit->append("10. -- END --\n");

    session_key_alice_edit->setText(alice_K);
    session_key_bob_edit->setText(bob_K);



} */

void DeffieHellman_MPW::aliceStartHandler()
{
    size_t digits_count =10;
    Deffiehellman dh;
    char *q_prime_str;
    char *p_prime_str;
    char *p_generator_str;
    log_edit->append("0. -- START -- \n");
    log_edit->append("1. Алиса хочет начать общение \n");
    p_prime_str = dh.generatePrimeNumbers(q_prime_str,digits_count);
    p_generator_str = dh.generateGeneratorNumber(p_prime_str,q_prime_str);
    p_alice_edit->setText(p_prime_str);
    g_alice_edit->setText( p_generator_str);
    log_edit->append("2. Алиса выбрала g =  " + QString((const char*)p_generator_str) + " и p = " + QString((const char*)p_prime_str) +"\n");


    char *a = (dh.generateNumber(digits_count-1));
    char *A = (dh.calculate(p_generator_str,a,p_prime_str));


     random_alice_edit->setText(a);
    calculated_alice_edit->setText(A);
    log_edit->append("3. Алиса сгенерировала случайное число а =  " + QString((const char*)a) + "\n");
    log_edit->append("4. Алиса вычислила число А =  " + QString((const char*)A)  +"\n");

    log_edit->append("5. Алиса передала Бобу числа g, p, A \n");

    p_bob_edit->setText(p_prime_str);
    g_bob_edit->setText(p_generator_str);
    got_bob_edit->setText(A);

    char *b = (dh.generateNumber(digits_count-1));
    char *B = (dh.calculate(p_generator_str,b,p_prime_str));

    random_bob_edit->setText(b);
    calculated_bob_edit->setText(B);
    log_edit->append("6. Боб сгенерировал случайное число b =  " + QString((const char*)b) + "\n");
    log_edit->append("7. Боб вычислил число B =  " + QString((const char*)B)  +"\n");
    log_edit->append("8. Боб передал Алисе число B\n");
    got_alice_edit->setText(B);

    char *alice_K = (dh.calculate(B,a,p_prime_str));
    char *bob_K   = (dh.calculate(A,b,p_prime_str));

    log_edit->append("9. Алиса вычислила общий сессионый ключ К = "+ QString((const char*)alice_K) +"\n"
                     + "Боб вычислил общий сессионый ключ К = "+ QString((const char*)bob_K) +"\n");
    log_edit->append("10. -- END --\n");

    session_key_alice_edit->setText(alice_K);
    session_key_bob_edit->setText(bob_K);



}
void DeffieHellman_MPW::bobStartHandler()
{
    size_t digits_count =10;
    Deffiehellman dh;
    char *q_prime_str;
    char *p_prime_str;
    char *p_generator_str;
    log_edit->append("0. -- START -- \n");
    log_edit->append("1. Боб хочет начать общение \n");
    p_prime_str = dh.generatePrimeNumbers(q_prime_str,digits_count);
    p_generator_str = dh.generateGeneratorNumber(p_prime_str,q_prime_str);
    QString p(p_prime_str);
    QString g(p_generator_str);
    p_bob_edit->setText(p);
    g_bob_edit->setText(g);
    log_edit->append("2. Боб выбрал g =  " + g + " и p = " + p +"\n");


    QString a(dh.generateNumber(digits_count-1));
    QString A(dh.calculate(g.toLocal8Bit().constData(),a.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    random_bob_edit->setText(a);
    calculated_bob_edit->setText(A);
    log_edit->append("3. Боб сгенерировал случайное число b =  " + a + "\n");
    log_edit->append("4. Боб вычислил число B =  " + A  +"\n");

    log_edit->append("5. Боб передал Алисе числа g, p, B \n");

    p_alice_edit->setText(p);
    g_alice_edit->setText(g);
    got_alice_edit->setText(A);

    QString b(dh.generateNumber(digits_count-1));
    QString B(dh.calculate(g.toLocal8Bit().constData(),b.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    random_alice_edit->setText(b);
    calculated_alice_edit->setText(B);
    log_edit->append("6. Алиса сгенерировала случайное число ф =  " + b + "\n");
    log_edit->append("7. Алиса вычислила число A =  " + B  +"\n");
    log_edit->append("8. Алиса передала Бобу число A\n");
    got_bob_edit->setText(B);

    QString alice_K(dh.calculate(B.toLocal8Bit().constData(),a.toLocal8Bit().constData(),p.toLocal8Bit().constData()));
    QString bob_K(dh.calculate(A.toLocal8Bit().constData(),b.toLocal8Bit().constData(),p.toLocal8Bit().constData()));

    log_edit->append("9. Алиса вычислила общий сессионый ключ К = "+ alice_K +"\n"
                     + "Боб вычислил общий сессионый ключ К = "+ bob_K +"\n");
    log_edit->append("10. -- END --\n");

    session_key_alice_edit->setText(alice_K);
    session_key_bob_edit->setText(bob_K);
}
