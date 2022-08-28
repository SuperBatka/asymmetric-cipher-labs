#include "Rsa_MPW.h"

Rsa_MPW::Rsa_MPW(QWidget *parent) : QWidget(parent)
{

    input_text_edit     = new QTextEdit;
    output_text_edit    = new QTextEdit;
    pPrime_line_edit     = new QLineEdit;
    qPrime_line_edit    = new QLineEdit;
    openExp_line_edit   = new QLineEdit;
    closeExp_line_edit  = new QLineEdit;
    openedFile_line_edit= new QLineEdit;
    saveFile_line_edit  = new QLineEdit;

    openedFile_line_edit->setReadOnly(true);
    saveFile_line_edit->setReadOnly(true);


    nModule_line_edit   = new QLineEdit;

    encrypt_text_button = new QPushButton("Зашифровать");
    decrypt_text_button = new QPushButton("Расшифровать");
    encrypt_file_button = new QPushButton("Зашифровать файл");
    decrypt_file_button = new QPushButton("Расшифровать файл");
    open_key_file_button = new QPushButton("Открыть файл с ключом");
    save_key_file_button = new QPushButton("Сохранить ключи в файл");

    generate_prime_numbers_button = new QPushButton("Сгенерировать числа");
    calculate_param_button = new QPushButton("Вычислить ключи");
    digits_line_edit = new QLineEdit;
    digits_line_edit->setValidator( new QIntValidator(2, 500, this) );

    pPrime_label        = new QLabel("Простое число \"p\"");
    qPrime_label        = new QLabel("Простое число \"q\"");
    openExp_label       = new QLabel("Открытая экспонента \"e\"");
    closeExp_label      = new QLabel("Закрытая экспонента \"d\"");
    nModule_label       = new QLabel("Модуль \"N\"");
    gen_prime_label     = new QLabel("Количество знаков в простом числе: ");
    task_label          = new QLabel;


    file_layout         = new QGridLayout;
    param_layout        = new QVBoxLayout;
    text_layout         = new QVBoxLayout;
    glayout             = new QGridLayout;
    gen_layout          = new QHBoxLayout;
    param_main_layout   = new QVBoxLayout;
    buttons_layout      = new QHBoxLayout;

    param_group_box     = new QGroupBox("Параметры");
    text_group_box      = new QGroupBox("Шифрование текста");
    file_group_box      = new QGroupBox("Шифрование файла");
    progressBar         = new QProgressBar;

    param_layout->addWidget(pPrime_label);
    param_layout->addWidget(pPrime_line_edit);
    param_layout->addWidget(qPrime_label);
    param_layout->addWidget(qPrime_line_edit);
    param_layout->addWidget(openExp_label);
    param_layout->addWidget(openExp_line_edit);
    param_layout->addWidget(closeExp_label);
    param_layout->addWidget(closeExp_line_edit);
    param_layout->addWidget(nModule_label);
    param_layout->addWidget(nModule_line_edit);
    buttons_layout->addWidget(calculate_param_button);
    buttons_layout->addWidget(open_key_file_button);
    buttons_layout->addWidget(save_key_file_button);

    param_layout->addLayout(buttons_layout);
    gen_layout->addWidget(gen_prime_label);
    gen_layout->addWidget(digits_line_edit);
    gen_layout->addWidget(generate_prime_numbers_button);
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

    glayout->addWidget(param_group_box,0,0,1,1);
    glayout->addWidget(file_group_box,1,0,1,1);
    glayout->addWidget(text_group_box,0,1,2,1);
    glayout->addWidget(task_label,2,0,1,2);
    glayout->addWidget(progressBar,3,0,1,2);




    setLayout(glayout);
    connect(encrypt_text_button, SIGNAL(clicked()),this, SLOT(encryptTextButtonHandler()));
    connect(decrypt_text_button, SIGNAL(clicked()),this, SLOT(decryptTextButtonHandler()));
    connect(encrypt_file_button, SIGNAL(clicked()),this, SLOT(encryptFileButtonHandler()));
    connect(decrypt_file_button, SIGNAL(clicked()),this, SLOT(decryptFileButtonHandler()));
    connect(open_key_file_button, SIGNAL(clicked()), this, SLOT(readKeyFromFile()));
    connect(save_key_file_button, SIGNAL(clicked()), this, SLOT(saveKeysToFiles()));

    connect(this, SIGNAL(failed_to_open_emit()), this, SLOT(failed_to_open()));
    connect(generate_prime_numbers_button, SIGNAL(clicked()),this, SLOT(generatePrimeNumbersButtonHandler()));
    connect(calculate_param_button, SIGNAL(clicked()),this, SLOT(calculateNumbersButtonHandler()));

    connect(this, SIGNAL(progressChanged_emit(int)), progressBar, SLOT(setValue(int)));
    connect(&FuturWatcher, SIGNAL(finished()), this, SLOT(endProcessSlot()));
    setValidator();
}


void Rsa_MPW::endProcessSlot()
{
    progressBar->setValue(100);
    task_label->setText("Текущий процесс: Готово!");

}
void Rsa_MPW::failed_to_open()
{
    QMessageBox::warning(this,"Warning", "Ошибка открытия файла");
    task_label->setText("Текущий процесс: Ошибка открытия файла!");
}

void Rsa_MPW::calculateNumbersButtonHandler()
{
    int pos = 0;

    QString pPrimeStr = pPrime_line_edit->text();
    QString qPrimeStr = qPrime_line_edit->text();

    if (validator->validate(pPrimeStr,pos) != QValidator::Acceptable || validator->validate(qPrimeStr,pos) != QValidator::Acceptable){
            QMessageBox::warning(this,"Warning", "Вы ввели некорретные данные");
            return;
        }
    rsaCore.clear();
    if(!rsaCore.setPrimeNumbers(pPrimeStr.toLocal8Bit().constData(), qPrimeStr.toLocal8Bit().constData())) {
        QMessageBox::warning(this,"Warning", "P и Q должны быть простыми и больше 29");
        return;
    }
    printParamToForm();
    QMessageBox::information(this,"Info", "Ключи были успешно вычислены и установлены");
    return;
}


void Rsa_MPW::encryptTextButtonHandler()
{
    try {
        rsaText(Operation::ENCRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }

}

void Rsa_MPW::decryptTextButtonHandler()
{
    try {
        rsaText(Operation::DECRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }
}


void Rsa_MPW::encryptFileButtonHandler()
{
    try {
        rsaFile(Operation::ENCRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }
}

void Rsa_MPW::decryptFileButtonHandler()
{
    try {
        rsaFile(Operation::DECRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }


}

void Rsa_MPW::generatePrimeNumbersButtonHandler()
{
    const QValidator *v = digits_line_edit->validator();
    QString digits_number = digits_line_edit->text();
    int pos = 0;
    if (v->validate(digits_number, pos) != QValidator::Acceptable){
            QMessageBox::warning(this,"Warning", "Выбирете размер простого числа в промежутке от 2 до 500");
            return;
        }

    rsaCore.generatePrimeNumbers(digits_number.toInt());
    printParamToForm();

}

void Rsa_MPW::printParamToForm()
{
    size_t size = 0;
    char *module = nullptr;
    char *pNumber = nullptr;
    char *qNumber = nullptr;
    char *openExpNumber = nullptr;
    char *closeExpNumber = nullptr;
    module = rsaCore.getStrModule(&size);
    pNumber = rsaCore.getStrPPrime(&size);
    qNumber = rsaCore.getStrQPrime(&size);
    openExpNumber = rsaCore.getStrOpenExp(&size);
    closeExpNumber = rsaCore.getStrClosedExp(&size);

    if(!(module && pNumber && qNumber && openExpNumber && closeExpNumber))
    {
        return;
    }

    clearForm();

    nModule_line_edit->insert(QString(module));
    pPrime_line_edit->insert(QString(pNumber));
    qPrime_line_edit->insert(QString(qNumber));
    openExp_line_edit->insert(QString(openExpNumber));
    closeExp_line_edit->insert(QString(closeExpNumber));

    delete [] module;
    delete [] pNumber;
    delete [] qNumber;
    delete [] openExpNumber;
    delete [] closeExpNumber;
}

bool Rsa_MPW::readKeys()
{

    QString openExpStr = openExp_line_edit->text();
    QString closeExpStr = closeExp_line_edit->text();
    QString moduleStr = nModule_line_edit->text();

    rsaCore.clear();
    rsaCore.setParametrs(openExpStr.toLocal8Bit().constData(),closeExpStr.toLocal8Bit().constData(),moduleStr.toLocal8Bit().constData());
    return true;
}

void Rsa_MPW::clearForm()
{
    openExp_line_edit->clear();
    closeExp_line_edit->clear();
    nModule_line_edit->clear();
    qPrime_line_edit->clear();
     pPrime_line_edit->clear();
}

void Rsa_MPW::setValidator()
{
    QRegularExpression re("^[1-9][0-9]{0,}");
    validator = new QRegularExpressionValidator(re, this);
    pPrime_line_edit->setValidator(validator);
    qPrime_line_edit->setValidator(validator);

    openExp_line_edit->setValidator(validator);
    closeExp_line_edit->setValidator(validator);
    nModule_line_edit->setValidator(validator);

}

bool Rsa_MPW::isPrimeNumbersEntered()
{
    if( pPrime_line_edit->hasAcceptableInput() && qPrime_line_edit->hasAcceptableInput())
    {
        return  true;
    }
    return false;
}

bool Rsa_MPW::isKeyParametrsEntered(Operation OP)
{
    if(OP == Operation::ENCRYPT)
    {
        if( openExp_line_edit->hasAcceptableInput()
                && nModule_line_edit->hasAcceptableInput())
        {
            return  true;
        }
    }

    if(OP == Operation::DECRYPT)
    {
        if( closeExp_line_edit->hasAcceptableInput()
                && nModule_line_edit->hasAcceptableInput())
        {
            return  true;
        }
    }


    return false;
}

void Rsa_MPW::rsaText(Operation OP)
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



    if(!isKeyParametrsEntered(OP))
    {
        if(!isPrimeNumbersEntered())
        {
            QMessageBox::warning(this,"Warning", "Введите ключи");
            return;
        } else
        {
            calculateNumbersButtonHandler();
            return;
        }
    }

    readKeys();
    char *res;
    size_t size = 0;

    if(OP == Operation::ENCRYPT)
    {
        QByteArray textByteArray( text.toUtf8() );
        rsaCore.setBufferSize(textByteArray.size());
        res = rsaCore.encrypt(textByteArray.constData(),textByteArray.size(),&size,false);
        QByteArray resultByteArray(res,size);
        output_text_edit->clear();
        output_text_edit->insertPlainText(resultByteArray.toBase64());
    }

    if(OP == Operation::DECRYPT)
    {
        QByteArray textByteArray = QByteArray::fromBase64(text.toUtf8());
        if (textByteArray.size() < rsaCore.getModuleByteCount())
        {
           delete[] res;
           return;
        }

        rsaCore.setBufferSize(textByteArray.size());
        res = rsaCore.decrypt(textByteArray.constData(),textByteArray.size(), &size);
        QString result  = QString::fromUtf8(res);
        output_text_edit->clear();
        output_text_edit->insertPlainText(result);
    }

    delete[] res;
}

void Rsa_MPW::rsaFile(Operation OP)
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }
    progressBar->setValue(0);
    if(!isKeyParametrsEntered(Operation::DECRYPT))
    {
        if(!isPrimeNumbersEntered())
        {
            QMessageBox::warning(this,"Warning", "Введите или сгенерируйте ключи");
            return;
        } else
        {
            calculateNumbersButtonHandler();
            return;
        }
    }



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

    if(OP == Operation::DECRYPT)
    {
            task_label->setText("Текущий процесс: Расшифровывание файла");
    }

    if(OP == Operation::ENCRYPT)
    {
            task_label->setText("Текущий процесс: Шифрование файла");
    }


    readKeys();

    f1 = QtConcurrent::run([=]() {
        calculateRsaFile(OP);
    });
    this->FuturWatcher.setFuture(f1);
}

void Rsa_MPW::calculateRsaFile(Operation OP)
{
    quint64 fileSize = QFileInfo(inputFileName).size();
    if(fileSize < rsaCore.getModuleByteCount())
    {
        return;
    }
    quint64 blcSize;
    if(OP == Operation::DECRYPT)
    {
        blcSize = rsaCore.getModuleByteCount();
    }

    if(OP == Operation::ENCRYPT)
    {
        blcSize = rsaCore.getModuleByteCount() - 1;
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
    rsaCore.setBufferSize(bufferSize);

   for (quint64 i = 0; i < bufferBlocksCount; i++)
   {
       bytesRead = in.readRawData(buffer,bufferSize);
       size_t res_size = 0;

       if(OP == Operation::DECRYPT)
       {
           result = rsaCore.decrypt(buffer,bytesRead, &res_size);
       }

       if(OP == Operation::ENCRYPT)
       {
           result = rsaCore.encrypt(buffer,bytesRead, &res_size,i==(bufferBlocksCount-1));
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
   saveFile_line_edit->clear();

    delete[] buffer;
    //delete[] result;
}
void Rsa_MPW::saveKeysToFiles()
{
    if(!isKeyParametrsEntered(Operation::ENCRYPT) || !isKeyParametrsEntered(Operation::DECRYPT))
    {
        QMessageBox::warning(this,"Warning", "Некорректные данные или не заполнены ключевые поля");
        return;
    }
    QString keyFileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    QFile fileOpen(keyFileName+".pub_key");
    if(!fileOpen.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для записи");
        return;
    }
    QTextStream outOpen(&fileOpen);
    outOpen << "======== Public Rsa key ======== " << "\n";
    outOpen<<openExp_line_edit->text()<<"\n";
    outOpen<<nModule_line_edit->text()<<"\n";
    outOpen << "========      End       ======== " << "\n";
    fileOpen.close();

    QFile fileClose(keyFileName+".priv_key");
    if(!fileClose.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для записи");
        return;
    }
    QTextStream outClose(&fileClose);
    outClose << "======== Private Rsa key ======== " << "\n";
    outClose<<closeExp_line_edit->text()<<"\n";
    outClose<<nModule_line_edit->text()<<"\n";
    outClose << "========      End        ======== " << "\n";
    fileClose.close();


}

void Rsa_MPW::readKeyFromFile()
{
    QString keyFileName = QFileDialog::getOpenFileName(this,"Open the keyfile");
    QFile file(keyFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для чтения");
        return;
    }
    QTextStream in(&file);
    QString first_line = in.readLine();
    if (first_line == "======== Public Rsa key ======== ")
    {
        openExp_line_edit->setText(in.readLine());
        nModule_line_edit->setText(in.readLine());
    } else if (first_line == "======== Private Rsa key ======== ")
    {
        closeExp_line_edit->setText(in.readLine());
        nModule_line_edit->setText(in.readLine());
    } else
    {
        QMessageBox::warning(this,"Warning", "В данном файле нет ключей");
        return;
    }

    file.close();
}
