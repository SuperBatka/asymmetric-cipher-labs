#ifndef RSA_EDS_MPW_H
#define RSA_EDS_MPW_H

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
#include "rsa.h"
#include "md5.h"

class Rsa_eds_MPW : public QWidget
{
    Q_OBJECT
public:
    explicit Rsa_eds_MPW(QWidget *parent = nullptr);


signals:
    void progress_changed_signal(int progress);
    void failed_signal(QString info);
    void result_signal(QString result);

private slots:
    void encryptFileButtonHandler();
    void decryptFileButtonHandler();
    void generatePrimeNumbersButtonHandler();
    void calculateNumbersButtonHandler();
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

    QTextEdit       *input_text_edit;
    QTextEdit      *output_text_edit;
    QLineEdit       *pPrime_line_edit;
    QLineEdit      *qPrime_line_edit;
    QLineEdit     *openExp_line_edit;
    QLineEdit    *closeExp_line_edit;
    QLineEdit     *nModule_line_edit;
    QLineEdit  *openedFile_line_edit;
    QLineEdit    *saveFile_line_edit;
    QLineEdit      *digits_line_edit;

    QPushButton    *open_file_button;
    QPushButton    *save_file_button;
    QPushButton *encrypt_file_button;
    QPushButton *decrypt_file_button;
    QPushButton *generate_prime_numbers_button;
    QPushButton *calculate_param_button;
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

    QLabel *pPrime_label;
    QLabel *qPrime_label;
    QLabel *openExp_label;
    QLabel *closeExp_label;
    QLabel *nModule_label;
    QLabel  *openedFile_label;
    QLabel *gen_prime_label;
    QLabel *saveFile_label;
    QLabel *task_label;

    QProgressBar *progressBar;


    QString inputFileName;
    QString signatureFileName;
    QValidator *validator;
    void printParamToForm();
    bool readKeys();
    void clearForm();
    void setValidator();
    bool isPrimeNumbersEntered();
    bool isKeyParametrsEntered(Operation OP);
    void rsaFile(Operation OP);
    void rsaText(Operation OP);

    void calculateRsaFile(Operation OP);

    Rsa rsaCore;
    QFuture<void> f1;
    QFutureWatcher<void> FuturWatcher;
};

#endif // RSA_EDS_MPW_H
