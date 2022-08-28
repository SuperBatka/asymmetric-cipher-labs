#ifndef ELGAMAL_EDS_MPW_H
#define ELGAMAL_EDS_MPW_H

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
#include "md5.h"

class Elgamal_eds_MPW : public QWidget
{
    Q_OBJECT
public:
    explicit Elgamal_eds_MPW(QWidget *parent = nullptr);

signals:
    void progress_changed_signal(int progress);
    void failed_signal(QString info);
    void result_signal(QString result);

private slots:
   void encryptFileButtonHandler();
   void decryptFileButtonHandler();
   void generatePrimeNumbersButtonHandler();
   void saveKeysToFiles();
   void readKeyFromFile();
   void failed_slot(QString info);

   void endProcessSlot();
   void resultSlot(QString result);

private:

    enum class Operation{
        ENCRYPT,
        DECRYPT

    };



    QLineEdit       *prime_line_edit;
    QLineEdit      *generator_line_edit;
    QLineEdit     *alice_private_key_line_edit;
    QLineEdit    *alice_open_key_line_edit;
    QLineEdit    *digits_line_edit;
    QLineEdit     *bob_private_key_line_edit;
    QLineEdit    *bob_open_key_line_edit;



    QPushButton    *open_file_button;
    QPushButton    *save_file_button;
    QPushButton *encrypt_file_button;
    QPushButton *decrypt_file_button;
    QPushButton *generate_keys_numbers_button;
    QPushButton *open_key_file_button;
    QPushButton *save_key_file_button;

    QGroupBox       *param_group_box;
    QGroupBox        *file_group_box;

    QGridLayout         *file_layout;
    QVBoxLayout        *param_layout;
    QVBoxLayout        *param_main_layout;
    QHBoxLayout          *gen_layout;
    QHBoxLayout      *buttons_layout;
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
    QString signatureFileName;
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

#endif // ELGAMAL_EDS_MPW_H
