#ifndef ELGAMAL_MPW_H
#define ELGAMAL_MPW_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QIntValidator>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QMessageBox>
#include <QProgressBar>
#include <QRegularExpression>
#include "Elgamal.h"

class Elgamal_MPW : public QWidget
{
    Q_OBJECT
public:
    explicit Elgamal_MPW(QWidget *parent = nullptr);

signals:
    void progressChanged_emit(int progress);
    void failed_to_open_emit();

private slots:
   void encryptTextButtonHandler();
   void decryptTextButtonHandler();
   void encryptFileButtonHandler();
   void decryptFileButtonHandler();
   void generatePrimeNumbersButtonHandler();
   void NewSessionKeyButtonHandler();
   //void saveKeysToFiles();
   //void readKeyFromFile();
   void failed_to_open();

   void endProcessSlot();

private:

    enum class Operation{
        ENCRYPT,
        DECRYPT

    };

    QTextEdit       *input_text_edit;
    QTextEdit      *output_text_edit;
    QTextEdit               *log_edit;
    QLineEdit       *prime_line_edit;
    QLineEdit      *generator_line_edit;
    QLineEdit     *alice_private_key_line_edit;
    QLineEdit    *alice_open_key_line_edit;
    QLineEdit    *digits_line_edit;
    QLineEdit     *bob_private_key_line_edit;
    QLineEdit    *bob_open_key_line_edit;



    QPushButton    *open_file_button;
    QPushButton    *save_file_button;
    QPushButton *encrypt_text_button;
    QPushButton *decrypt_text_button;
    QPushButton *encrypt_file_button;
    QPushButton *decrypt_file_button;
    QPushButton *generate_keys_numbers_button;
    QPushButton *new_session_key_button;
    QPushButton *open_key_file_button;
    QPushButton *save_key_file_button;

    QGroupBox       *param_group_box;
    QGroupBox        *text_group_box;
    QGroupBox        *file_group_box;
    QGroupBox       *log_box;


    QGridLayout         *file_layout;
    QVBoxLayout        *param_layout;
    QVBoxLayout        *param_main_layout;
    QVBoxLayout         *text_layout;
    QHBoxLayout          *gen_layout;
    QHBoxLayout      *buttons_layout;
    QHBoxLayout           *log_layout;
    QGridLayout             *glayout;

    QLabel *prime_label;
    QLabel *generator_label;
    QLabel *alice_private_key_label;
    QLabel *alice_open_key_label;
    QLabel *bob_private_key_label;
    QLabel  *bob_open_key_label;
    QLabel *task_label;
    QLabel *gen_prime_label;

    QProgressBar *progressBar;


    QString inputFileName;
    QString resultFileName;
    QValidator *validator;
    void printParamToForm();
    void clearForm();
    void setValidator();
    bool isPrimeNumbersEntered();
    bool isKeyParametrsEntered(Operation OP);
    void elgamalFile(Operation OP);
    void elgamalText(Operation OP);

    void calculateRsaFile(Operation OP);
    QFuture<void> f1;
    QFutureWatcher<void> FuturWatcher;
    Elgamal elgamalCore;

};

#endif // ELGAMAL_MPW_H
