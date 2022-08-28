#ifndef SHAMIR_MPW_H
#define SHAMIR_MPW_H

#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <shamir.h>
#include <QIntValidator>

class Shamir_MPW : public QWidget
{
    Q_OBJECT
public:
    explicit Shamir_MPW(QWidget *parent = nullptr);

private slots:
    void sendMessageHandler();

signals:


private:
    QTextEdit             *log_edit;
    QVBoxLayout       *alice_layout;
    QVBoxLayout         *bob_layout;
    QVBoxLayout         *log_layout;
    QHBoxLayout        *main_layout;

    QGroupBox             *alice_box;
    QGroupBox               *bob_box;
    QGroupBox               *log_box;

    QLineEdit       *alice_message_edit;
    QLineEdit       *alice_random_C_edit;
    QLineEdit       *alice_random_D_edit;
    QLineEdit       *alice_calculated1_edit;
    QLineEdit       *alice_got_edit;
    QLineEdit       *alice_calculated2_edit;


    QLineEdit       *bob_message_edit;
    QLineEdit       *bob_random_C_edit;
    QLineEdit       *bob_random_D_edit;
    QLineEdit       *bob_calculated1_edit;
    QLineEdit       *bob_got_edit;
    QLineEdit       *bob_got2_edit;

    QLabel       *alice_message_label;
    QLabel       *alice_random_C_label;
    QLabel       *alice_random_D_label;
    QLabel       *alice_calculated1_label;
    QLabel       *alice_got_label;
    QLabel       *alice_calculated2_label;

    QLabel       *bob_message_label;
    QLabel       *bob_random_C_label;
    QLabel       *bob_random_D_label;
    QLabel       *bob_calculated1_label;
    QLabel       *bob_got_label;
    QLabel       *bob_got2_label;

    QPushButton *send_message_button;
    QHBoxLayout *message_layout;




};

#endif // SHAMIR_MPW_H
