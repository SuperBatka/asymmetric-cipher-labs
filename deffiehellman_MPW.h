#ifndef DEFFIEHELLMAN_MPW_H
#define DEFFIEHELLMAN_MPW_H

#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include "deffiehellman.h"

class DeffieHellman_MPW : public QWidget
{

    Q_OBJECT


public:
    explicit DeffieHellman_MPW(QWidget *parent = nullptr);


private slots:
    void aliceStartHandler();
    void bobStartHandler();




private:
    QTextEdit             *log_edit;
    QVBoxLayout       *alice_layout;
    QVBoxLayout         *bob_layout;
    QVBoxLayout         *log_layout;
    QHBoxLayout        *main_layout;

    QGroupBox             *alice_box;
    QGroupBox               *bob_box;
    QGroupBox               *log_box;

    QPushButton         *alice_start_button;
    QPushButton         *bob_start_button;

    QLineEdit           *g_bob_edit;
    QLineEdit           *p_bob_edit;

    QLineEdit           *g_alice_edit;
    QLineEdit           *p_alice_edit;

    QLineEdit           *random_bob_edit;
    QLineEdit           *calculated_bob_edit;

    QLineEdit           *random_alice_edit;
    QLineEdit           *calculated_alice_edit;

    QLineEdit           *got_alice_edit;
    QLineEdit           *session_key_alice_edit;

    QLineEdit           *got_bob_edit;
    QLineEdit           *session_key_bob_edit;

    QLabel *g_alice_label;
    QLabel *p_alice_label;
    QLabel *random_alice_label;
    QLabel *calculated_alice_label;
    QLabel *got_alice_label;
    QLabel *session_key_alice_label;

    QLabel *g_bob_label;
    QLabel *p_bob_label;
    QLabel *random_bob_label;
    QLabel *calculated_bob_label;
    QLabel *got_bob_label;
    QLabel *session_key_bob_label;





};


#endif // DEFFIEHELLMAN_MPW_H
