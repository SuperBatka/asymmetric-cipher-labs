#include "Elgamal_MPW.h"

Elgamal_MPW::Elgamal_MPW(QWidget *parent) : QWidget(parent)
{
    input_text_edit     = new QTextEdit;
    output_text_edit    = new QTextEdit;
    log_edit            = new QTextEdit;
    prime_line_edit     = new QLineEdit;
    generator_line_edit    = new QLineEdit;
    alice_private_key_line_edit   = new QLineEdit;
    alice_open_key_line_edit  = new QLineEdit;
    bob_private_key_line_edit= new QLineEdit;
    bob_open_key_line_edit  = new QLineEdit;

    log_edit->setReadOnly(true);
    prime_line_edit->setReadOnly(true);
    generator_line_edit->setReadOnly(true);
    alice_private_key_line_edit->setReadOnly(true);
    alice_open_key_line_edit->setReadOnly(true);
    bob_private_key_line_edit->setReadOnly(true);
    bob_open_key_line_edit->setReadOnly(true);


    encrypt_text_button = new QPushButton("Зашифровать");
    decrypt_text_button = new QPushButton("Расшифровать");
    encrypt_file_button = new QPushButton("Зашифровать файл");
    decrypt_file_button = new QPushButton("Расшифровать файл");
    //open_key_file_button = new QPushButton("Открыть файл с ключом");
    //save_key_file_button = new QPushButton("Сохранить ключи в файл");

    generate_keys_numbers_button = new QPushButton("Сгенерировать числа");
    new_session_key_button = new QPushButton("Новые случайные числа Алисы");
    digits_line_edit = new QLineEdit;
    digits_line_edit->setValidator( new QIntValidator(2, 100, this) );

    prime_label        = new QLabel("Простое число \"p\"");
    generator_label        = new QLabel("Генератор \"g\"");
    alice_private_key_label       = new QLabel("Сеансовый ключ Алисы \"k\"");
    alice_open_key_label          = new QLabel("Открытый ключ Алисы \"a\"");
    bob_private_key_label         = new QLabel("Секретный ключ Боба \"x\"");
    bob_open_key_label           = new QLabel("Открытый ключ Боба \"y\"");
    gen_prime_label     = new QLabel("Количество знаков в простом числе: ");
    task_label          = new QLabel;


    file_layout         = new QGridLayout;
    param_layout        = new QVBoxLayout;
    text_layout         = new QVBoxLayout;
    glayout             = new QGridLayout;
    gen_layout          = new QHBoxLayout;
    param_main_layout   = new QVBoxLayout;
    buttons_layout      = new QHBoxLayout;
    log_layout          = new QHBoxLayout;

    param_group_box     = new QGroupBox("Параметры");
    text_group_box      = new QGroupBox("Шифрование текста");
    file_group_box      = new QGroupBox("Шифрование файла");
    log_box             = new QGroupBox("Log");
    progressBar         = new QProgressBar;

    param_layout->addWidget(prime_label);
    param_layout->addWidget(prime_line_edit);
    param_layout->addWidget(generator_label);
    param_layout->addWidget(generator_line_edit);
    param_layout->addWidget(bob_private_key_label);
    param_layout->addWidget(bob_private_key_line_edit);
    param_layout->addWidget(bob_open_key_label);
    param_layout->addWidget(bob_open_key_line_edit);
    param_layout->addWidget(alice_private_key_label);
    param_layout->addWidget(alice_private_key_line_edit);
    param_layout->addWidget(alice_open_key_label);
    param_layout->addWidget(alice_open_key_line_edit);
    param_layout->addWidget(new_session_key_button);
    //buttons_layout->addWidget(calculate_param_button);
    //buttons_layout->addWidget(open_key_file_button);
    //buttons_layout->addWidget(save_key_file_button);

    param_layout->addLayout(buttons_layout);
    gen_layout->addWidget(gen_prime_label);
    gen_layout->addWidget(digits_line_edit);
    gen_layout->addWidget(generate_keys_numbers_button);
    param_main_layout->addLayout(param_layout);
    param_main_layout->addLayout(gen_layout);
    param_group_box->setLayout(param_main_layout);

    file_layout->addWidget(encrypt_file_button,0,0,1,1);
    file_layout->addWidget(decrypt_file_button,0,1,1,1);
    file_group_box->setLayout(file_layout);


    text_layout->addWidget(input_text_edit);
    text_layout->addWidget(output_text_edit);
    text_layout->addWidget(encrypt_text_button);
    text_layout->addWidget(decrypt_text_button);
    text_group_box->setLayout(text_layout);

    log_layout->addWidget(log_edit);
    log_box->setLayout(log_layout);

    glayout->addWidget(param_group_box,0,0,1,1);
    glayout->addWidget(file_group_box,1,0,1,1);
    glayout->addWidget(text_group_box,0,1,3,1);
    glayout->addWidget(log_box,2,0,1,1);
    glayout->addWidget(task_label,3,0,1,2);
    glayout->addWidget(progressBar,4,0,1,2);




    setLayout(glayout);
    connect(encrypt_text_button, SIGNAL(clicked()),this, SLOT(encryptTextButtonHandler()));
    connect(decrypt_text_button, SIGNAL(clicked()),this, SLOT(decryptTextButtonHandler()));
    connect(encrypt_file_button, SIGNAL(clicked()),this, SLOT(encryptFileButtonHandler()));
    connect(decrypt_file_button, SIGNAL(clicked()),this, SLOT(decryptFileButtonHandler()));
    //connect(open_key_file_button, SIGNAL(clicked()), this, SLOT(readKeyFromFile()));
    //connect(save_key_file_button, SIGNAL(clicked()), this, SLOT(saveKeysToFiles()));

    connect(this, SIGNAL(failed_to_open_emit()), this, SLOT(failed_to_open()));
    connect(generate_keys_numbers_button, SIGNAL(clicked()),this, SLOT(generatePrimeNumbersButtonHandler()));
    connect(new_session_key_button, SIGNAL(clicked()),this, SLOT(NewSessionKeyButtonHandler()));
    connect(this, SIGNAL(progressChanged_emit(int)), progressBar, SLOT(setValue(int)));
    connect(&FuturWatcher, SIGNAL(finished()), this, SLOT(endProcessSlot()));

}



void Elgamal_MPW::endProcessSlot()
{
    progressBar->setValue(100);
    task_label->setText("Текущий процесс: Готово!");

}
void Elgamal_MPW::failed_to_open()
{
    QMessageBox::warning(this,"Warning", "Ошибка открытия файла");
    task_label->setText("Текущий процесс: Ошибка открытия файла!");
}

void Elgamal_MPW::encryptTextButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }
    elgamalText(Operation::ENCRYPT);
}
void Elgamal_MPW::decryptTextButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }

    elgamalText(Operation::DECRYPT);
}

void Elgamal_MPW::encryptFileButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }
    elgamalFile(Operation::ENCRYPT);
}
void Elgamal_MPW::decryptFileButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }
    elgamalFile(Operation::DECRYPT);
}

void Elgamal_MPW::NewSessionKeyButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }

        size_t t;
    elgamalCore.generateAliceKeys();
    QString aO(elgamalCore.getStrAliceOpenKey(&t));
    QString aC(elgamalCore.getStrAliceClosedKey(&t));
    alice_open_key_line_edit->setText(aO);
    alice_private_key_line_edit->setText(aC);
    log_edit->append("Алиса выбирает новое случайное число k = " + aC + " и вычисляет открытый ключ a = " + aO + " для передачи следующего сообщения");
}

void Elgamal_MPW::generatePrimeNumbersButtonHandler()
{
    const QValidator *v = digits_line_edit->validator();
    QString digits_number = digits_line_edit->text();
    int pos = 0;
    if (v->validate(digits_number, pos) != QValidator::Acceptable){
            QMessageBox::warning(this,"Warning", "Выбирете размер простого числа в промежутке от 2 до 100");
            return;
        }

    elgamalCore.generatePrimeNumbers(digits_number.toInt());
    elgamalCore.generateGeneratorNumber();
    elgamalCore.generateAliceKeys();
    elgamalCore.generateBobKeys();
    size_t t;
    QString prime(elgamalCore.getStrModule(&t));
    QString generator(elgamalCore.getStrGenerator(&t));
    QString aO(elgamalCore.getStrAliceOpenKey(&t));
    QString aC(elgamalCore.getStrAliceClosedKey(&t));
    QString bO(elgamalCore.getStrBobOpenKey(&t));
    QString bC(elgamalCore.getStrBobClosedKey(&t));

    prime_line_edit->setText(prime);
    generator_line_edit->setText(generator);
    alice_open_key_line_edit->setText(aO);
    alice_private_key_line_edit->setText(aC);
    bob_open_key_line_edit->setText(bO);
    bob_private_key_line_edit->setText(bC);
    log_edit->append("0. START");
    log_edit->append("1. Боб хочет получить сообщение от Алисы");
    log_edit->append("2. Боб выбирает простое число p = " + prime + " и число g = " + generator);
    log_edit->append("3. Боб выбирает случайное число x = " + bC + " и вычисляет открытый ключ y = " + bO);
    log_edit->append("4. Боб передаёт Алисе числа p = " + prime + ", g = " + generator + " и y = " + bO);
    log_edit->append("5. Алиса получает числа p, g, y ");
    log_edit->append("6. Алиса выбирает случайное число k = " + aC + " и вычисляет открытый ключ a = " + aO);
}

//void saveKeysToFiles();
//void readKeyFromFile();


void Elgamal_MPW::elgamalText(Operation OP)
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }
    QString text = input_text_edit->toPlainText();
    if (text.length() == 0){
        if(OP == Operation::ENCRYPT)
            QMessageBox::warning(this,"Warning", "Нечего шифровать");
        if(OP == Operation::DECRYPT)
            QMessageBox::warning(this,"Warning", "Нечего расшифровывать");
        return;
    }

    if(OP == Operation::ENCRYPT)
    {
        if (text.length() > 5000){
            QMessageBox::warning(this,"Warning", "Слишком большой шифротекст.");
            return;
        }
    }

    if(OP == Operation::DECRYPT)
    {
        if (text.length() > 1023){
            QMessageBox::warning(this,"Warning", "Воспользуйтесь функцией шифрования файла. Сохраните данные в файл и зашифруйте этот файл.");
            return;
        }
    }


    char *res;
    size_t size = 0;

    if(OP == Operation::ENCRYPT)
    {
        QByteArray textByteArray( text.toUtf8() );
        elgamalCore.setBufferSize(textByteArray.size());
        size_t firstBlockSize = 0;
        char *firstBlock = elgamalCore.writeFirstBlock(&firstBlockSize);


        QByteArray firstBlockArray(firstBlock,firstBlockSize);
        res = elgamalCore.encrypt(textByteArray.constData(),textByteArray.size(),&size,false);
        QByteArray resultByteArray(res,size);
        log_edit->append("Алиса шифрует сообщение и отправляют Бобу (a,b) = (" +firstBlockArray.toBase64() + ", " + resultByteArray.toBase64() + ")");
        resultByteArray = firstBlockArray + resultByteArray;
        output_text_edit->clear();
        output_text_edit->insertPlainText(resultByteArray.toBase64());

    }

    if(OP == Operation::DECRYPT)
    {
        QByteArray textByteArray = QByteArray::fromBase64(text.toUtf8());
        if (textByteArray.size() < elgamalCore.getModuleByteCount())
        {
            QMessageBox::warning(this,"Warning", "Входные данные не совпадают с размерами ключа");
           return;
        }

        if (textByteArray.size() % elgamalCore.getModuleByteCount() != 0)
        {

           QMessageBox::warning(this,"Warning", "Входные данные не совпадают с размерами ключа");
           return;
        }

        elgamalCore.readFirstBlock(textByteArray.constData(), elgamalCore.getModuleByteCount());
        elgamalCore.setBufferSize(textByteArray.size());
        res = elgamalCore.decrypt(textByteArray.constData() + elgamalCore.getModuleByteCount(),textByteArray.size() - elgamalCore.getModuleByteCount(), &size);
        QString result  = QString::fromUtf8(res);
        log_edit->append("Боб расшифровывает полученный шифротекст от Алисы и получает сообщение M = " + result);
        output_text_edit->clear();
        output_text_edit->insertPlainText(result);
    }

    delete[] res;
}


void Elgamal_MPW::elgamalFile(Operation OP)
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }
    progressBar->setValue(0);


    QMessageBox::information(this,"Info", "Выберете входной файл");
    inputFileName = QFileDialog::getOpenFileName(this,"Open the file");
    if(inputFileName == "")
    {
        return;
    }



    QMessageBox::information(this,"Info", "Выберете результирующий файл");
    resultFileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    if(resultFileName == "")
    {
        return;
    }
    QString prime(prime_line_edit->text());
    QString generator(generator_line_edit->text());
    QString aO(alice_open_key_line_edit->text());
    QString aC(alice_private_key_line_edit->text());
    QString bO(bob_open_key_line_edit->text());
    QString bC(bob_private_key_line_edit->text());

    if(OP == Operation::DECRYPT)
    {
            task_label->setText("Текущий процесс: Расшифровывание файла");
            log_edit->append("Боб решил расшифровать файл  Алисы используя открытые ключи p = " + prime +", g = " +generator +  ", a = " + aO + " и свой секретный ключ x = " + bC);
    }

    if(OP == Operation::ENCRYPT)
    {
            task_label->setText("Текущий процесс: Шифрование файла");
            log_edit->append("Алиса решила зашифровать файл  для Боба, используя открытые ключи p = " + prime +", g = " +generator + ", y = " + bO + ", a = " + aO + " и сессионый ключ k = " + aC);
    }


   // readKeys();
    f1 = QtConcurrent::run([=]() {
        calculateRsaFile(OP);
    });


    this->FuturWatcher.setFuture(f1);
}

void Elgamal_MPW::calculateRsaFile(Operation OP)
{
    quint64 fileSize = QFileInfo(inputFileName).size();
    if(fileSize < elgamalCore.getModuleByteCount())
    {
        return;
    }
    quint64 blcSize;
    if(OP == Operation::DECRYPT)
    {
        blcSize = elgamalCore.getModuleByteCount();
    }

    if(OP == Operation::ENCRYPT)
    {
        blcSize = elgamalCore.getModuleByteCount() - 1;
    }

    size_t bufferSize = (4096 / blcSize) * blcSize;
    quint64 bufferBlocksCount = fileSize / bufferSize +1;
    if(fileSize % bufferSize == 0)
    {
        bufferBlocksCount--;
    }


    QFile file(inputFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit failed_to_open_emit();
        return;
    }
    QDataStream in(&file);

    QFile fileEnc(resultFileName);
    if(!fileEnc.open(QIODevice::WriteOnly))
    {
        emit failed_to_open_emit();
        return;
    }
    QDataStream outFile(&fileEnc);

    quint64 ByteCounter = 0;
    quint64 writeByteCounter = 0;
    char *buffer = new char[bufferSize];
    char *result ;
    quint64 barProgress = 0;
    int bytesRead;
    quint64 bytesWrite = 0;
    elgamalCore.setBufferSize(bufferSize);
    if(OP == Operation::DECRYPT)
    {
        int a;
        a = in.readRawData(buffer,elgamalCore.getModuleByteCount());
        elgamalCore.readFirstBlock(buffer, elgamalCore.getModuleByteCount());
    }

    if(OP == Operation::ENCRYPT)
    {
        int a;
        size_t s = 0;
        result = elgamalCore.writeFirstBlock(&s);
        a = outFile.writeRawData(result,s);

    }

   for (quint64 i = 0; i < bufferBlocksCount; i++)
   {
       bytesRead = in.readRawData(buffer,bufferSize);
       size_t res_size = 0;

       if(OP == Operation::DECRYPT)
       {
           result = elgamalCore.decrypt(buffer,bytesRead, &res_size);
       }

       if(OP == Operation::ENCRYPT)
       {
           result = elgamalCore.encrypt(buffer,bytesRead, &res_size,i==(bufferBlocksCount-1));
       }

       bytesWrite += outFile.writeRawData(result,res_size);
       ByteCounter+=bytesRead;
       writeByteCounter+=bytesWrite;

       if ((barProgress < (ByteCounter * 100/ fileSize )) && barProgress < 101)
       {
           barProgress = ByteCounter * 100 / fileSize ;
           emit progressChanged_emit(barProgress);
       }
   }
   delete[] result;

   file.close();
   fileEnc.close();

    delete[] buffer;
    //delete[] result;
}
