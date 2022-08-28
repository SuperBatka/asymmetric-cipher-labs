#include "Rsa_eds_MPW.h"

Rsa_eds_MPW::Rsa_eds_MPW(QWidget *parent) : QWidget(parent)
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


    encrypt_file_button = new QPushButton("Подписать");
    decrypt_file_button = new QPushButton("Проверить подпись");
    open_key_file_button = new QPushButton("Открыть файл с ключом");
    save_key_file_button = new QPushButton("Сохранить ключи в файл");

    generate_prime_numbers_button = new QPushButton("Сгенерировать числа");
    calculate_param_button = new QPushButton("Вычислить ключи");
    digits_line_edit = new QLineEdit;
    digits_line_edit->setValidator( new QIntValidator(2, 100, this) );

    pPrime_label        = new QLabel("Простое число \"p\"");
    qPrime_label        = new QLabel("Простое число \"q\"");
    openExp_label       = new QLabel("Открытая экспонента \"d\"");
    closeExp_label      = new QLabel("Закрытая экспонента \"e\"");
    nModule_label       = new QLabel("Модуль \"N\"");
    gen_prime_label     = new QLabel("Количество знаков в простом числе: ");
    task_label          = new QLabel;


    file_layout         = new QGridLayout;
    param_layout        = new QVBoxLayout;
    glayout             = new QGridLayout;
    gen_layout          = new QHBoxLayout;
    param_main_layout   = new QVBoxLayout;
    buttons_layout      = new QHBoxLayout;

    param_group_box     = new QGroupBox("Параметры");
    file_group_box      = new QGroupBox("Подписание и проверка подписи файла");
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


    glayout->addWidget(param_group_box,0,0,1,1);
    glayout->addWidget(file_group_box,1,0,1,1);
    glayout->addWidget(task_label,2,0,1,2);
    glayout->addWidget(progressBar,3,0,1,2);




    setLayout(glayout);
    connect(encrypt_file_button, SIGNAL(clicked()),this, SLOT(encryptFileButtonHandler()));
    connect(decrypt_file_button, SIGNAL(clicked()),this, SLOT(decryptFileButtonHandler()));
    connect(open_key_file_button, SIGNAL(clicked()), this, SLOT(readKeyFromFile()));
    connect(save_key_file_button, SIGNAL(clicked()), this, SLOT(saveKeysToFiles()));

    connect(this, SIGNAL(failed_signal(QString)), this, SLOT(failed_slot(QString)));
    connect(generate_prime_numbers_button, SIGNAL(clicked()),this, SLOT(generatePrimeNumbersButtonHandler()));
    connect(calculate_param_button, SIGNAL(clicked()),this, SLOT(calculateNumbersButtonHandler()));

    connect(this, SIGNAL(progress_changed_signal(int)), progressBar, SLOT(setValue(int)));
    connect(this, SIGNAL(result_signal(QString)), this, SLOT(resultSlot(QString)));
    connect(&FuturWatcher, SIGNAL(finished()), this, SLOT(endProcessSlot()));
    setValidator();
}


void Rsa_eds_MPW::resultSlot(QString result)
{
    QMessageBox::warning(this,"info", result);
}

void Rsa_eds_MPW::endProcessSlot()
{
    progressBar->setValue(100);
    task_label->setText("Текущий процесс: Готово!");

}
void Rsa_eds_MPW::failed_slot(QString info)
{
    QMessageBox::warning(this,"Warning", info);
    task_label->setText("Текущий процесс: " + info);
}

void Rsa_eds_MPW::calculateNumbersButtonHandler()
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



void Rsa_eds_MPW::encryptFileButtonHandler()
{
    try {
        rsaFile(Operation::ENCRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }
}

void Rsa_eds_MPW::decryptFileButtonHandler()
{
    try {
        rsaFile(Operation::DECRYPT);
    }  catch (...) {
        QMessageBox::warning(this,"Warning", "Вы сделали что-то плохое :(");
    }


}

void Rsa_eds_MPW::generatePrimeNumbersButtonHandler()
{
    const QValidator *v = digits_line_edit->validator();
    QString digits_number = digits_line_edit->text();
    int pos = 0;
    if (v->validate(digits_number, pos) != QValidator::Acceptable){
            QMessageBox::warning(this,"Warning", "Выбирете размер простого числа в промежутке от 2 до 100");
            return;
        }

    rsaCore.generatePrimeNumbers(digits_number.toInt());
    printParamToForm();

}

void Rsa_eds_MPW::printParamToForm()
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
    openExpNumber = rsaCore.getStrClosedExp(&size);  // поменял местами
    closeExpNumber = rsaCore.getStrOpenExp(&size);   //т.к. подпись

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

bool Rsa_eds_MPW::readKeys()
{

    QString openExpStr = openExp_line_edit->text();
    QString closeExpStr = closeExp_line_edit->text();
    QString moduleStr = nModule_line_edit->text();

    rsaCore.clear();
    rsaCore.setParametrs(closeExpStr.toLocal8Bit().constData(),openExpStr.toLocal8Bit().constData(),moduleStr.toLocal8Bit().constData());  //открытый, закрытый, модуль
    return true;
}

void Rsa_eds_MPW::clearForm()
{
    openExp_line_edit->clear();
    closeExp_line_edit->clear();
    nModule_line_edit->clear();
    qPrime_line_edit->clear();
     pPrime_line_edit->clear();
}

void Rsa_eds_MPW::setValidator()
{
    QRegularExpression re("^[1-9][0-9]{0,}");
    validator = new QRegularExpressionValidator(re, this);
    pPrime_line_edit->setValidator(validator);
    qPrime_line_edit->setValidator(validator);

    openExp_line_edit->setValidator(validator);
    closeExp_line_edit->setValidator(validator);
    nModule_line_edit->setValidator(validator);

}

bool Rsa_eds_MPW::isPrimeNumbersEntered()
{
    if( pPrime_line_edit->hasAcceptableInput() && qPrime_line_edit->hasAcceptableInput())
    {
        return  true;
    }
    return false;
}

bool Rsa_eds_MPW::isKeyParametrsEntered(Operation OP)
{
    if(OP == Operation::ENCRYPT)
    {
        if( closeExp_line_edit->hasAcceptableInput()                         // поменял местами
                && nModule_line_edit->hasAcceptableInput())                  //т.к. подпись
        {
            return  true;
        }
    }

    if(OP == Operation::DECRYPT)
    {
        if( openExp_line_edit->hasAcceptableInput()                        // поменял местами
                && nModule_line_edit->hasAcceptableInput())                //т.к. подпись
        {
            return  true;
        }
    }


    return false;
}



void Rsa_eds_MPW::rsaFile(Operation OP)
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }
    progressBar->setValue(0);



    if(OP == Operation::DECRYPT)
    {
        if(!isKeyParametrsEntered(OP))
        {

            if(!isPrimeNumbersEntered())
            {
                QMessageBox::warning(this,"Warning", "Недостаточно ключевых данных для проверки подписи");
                return;
            } else
            {
                calculateNumbersButtonHandler();
                return;
            }
        }
        QMessageBox::information(this,"Info", "Выберете файл для проверки");
        inputFileName = QFileDialog::getOpenFileName(this,"Open the file");
        if(inputFileName == "")
        {
            return;
        }

        QMessageBox::information(this,"Info", "Выберете файл с подписью");
        signatureFileName = QFileDialog::getOpenFileName(this,"Open the file");
        if(inputFileName == "")
        {
            return;
        }

        task_label->setText("Текущий процесс: Проверка подписи файла");
    }

    if(OP == Operation::ENCRYPT)
    {
        if(!isKeyParametrsEntered(OP))
        {
            if(!isPrimeNumbersEntered())
            {
                QMessageBox::warning(this,"Warning", "Недостаточно ключевых данных для подписания файла");
                return;
            } else
            {
                calculateNumbersButtonHandler();
                return;
            }
        }

        QMessageBox::information(this,"Info", "Выберете файл для подписи");
        inputFileName = QFileDialog::getOpenFileName(this,"Open the file");
        if(inputFileName == "")
        {
            return;
        }

        task_label->setText("Текущий процесс: Подпись файла");
    }


    readKeys();

    f1 = QtConcurrent::run([=]() {
        calculateRsaFile(OP);
    });
    this->FuturWatcher.setFuture(f1);
}

void Rsa_eds_MPW::calculateRsaFile(Operation OP)
{
    quint64 fileSize = QFileInfo(inputFileName).size();

    size_t bufferSize = 64;
    quint64 bufferBlocksCount = fileSize / bufferSize;
    if(fileSize % bufferSize == 0)
    {
        bufferBlocksCount--;
    }

    QFile file(inputFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit failed_signal("Ошибка открытия или чтения файла.");
        return;
    }
    QDataStream in(&file);

    quint64 ByteCounter = 0;
    char *result ;
    quint64 barProgress = 0;
    size_t bytesRead;

    HashI *hashCore = new MD5();

    char *buffer = new char[bufferSize];
    //hashCore.Init();

   for (quint64 i = 0; i < bufferBlocksCount; i++)
   {
       bytesRead = in.readRawData(buffer,bufferSize);

       hashCore->hashBlock((unsigned char*)buffer);

       ByteCounter+=bytesRead;


       if ((barProgress < (ByteCounter * 100/ fileSize )) && barProgress < 101)
       {
           barProgress = ByteCounter * 100 / fileSize ;
           emit progress_changed_signal(barProgress);
       }
   }


   bytesRead = in.readRawData(buffer,fileSize - bufferBlocksCount*bufferSize);  //
   hashCore->hashFinaleBlock((unsigned char*)buffer,bytesRead,fileSize);
   result = (char*)hashCore->getHashResult();

   ByteCounter+=bytesRead;

   if(ByteCounter != fileSize)
   {
       emit failed_signal( "Произошла ошибка чтения файла. Хэш файла может быть посчитан неправильно.");
   }

   QByteArray hashByteArray(result,hashCore->GetMode());

   if(OP == Operation::ENCRYPT)
   {
       size_t size = 0;
       char *hashRes;
       rsaCore.setBufferSize(hashByteArray.size());
       hashRes = rsaCore.encrypt(hashByteArray.constData(),hashByteArray.size(),&size,false);
       QByteArray resultByteArray(hashRes,size);



       QFile resultFile(inputFileName+".signature");
       if(!resultFile.open(QIODevice::WriteOnly))
       {
           emit failed_signal("Ошибка открытия или чтения файла.");
           return;
       }

       QDataStream out(&resultFile);
       out.writeRawData(resultByteArray.constData(), resultByteArray.size());
       delete[] hashRes;
       resultFile.close();
   }

   if(OP == Operation::DECRYPT)
   {
        quint64 signatureSize = QFileInfo(signatureFileName).size();
        if(signatureSize > 100)
        {
            emit failed_signal("Файл не является подписью");
            return;
        }
         QFile signatureFile(signatureFileName);
         if(!signatureFile.open(QIODevice::ReadOnly))
         {
             emit failed_signal("Ошибка открытия или чтения файла.");
             return;
         }

         QDataStream sigIn(&signatureFile);
         char *sigBuf = new char[signatureSize];
         size_t readSize = sigIn.readRawData(sigBuf,signatureSize);
         size_t size = 0;
         rsaCore.setBufferSize(readSize);
         char *res = rsaCore.decrypt(sigBuf,readSize, &size);
         QByteArray signatureByteArray(res,16 );
         if(hashByteArray == signatureByteArray)
         {
                emit result_signal("Подпись верна");
         } else
         {
                emit result_signal("Подпись не верна");
         }
         delete[] res;
         delete[] sigBuf;
   }




   delete[] result;
   delete[] buffer;  
   delete hashCore;
   file.close();



}
void Rsa_eds_MPW::saveKeysToFiles()
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
    outOpen << "======== Public Rsa Signature key ======== " << "\n";
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
    outClose << "======== Private Rsa Signature key ======== " << "\n";
    outClose<<closeExp_line_edit->text()<<"\n";
    outClose<<nModule_line_edit->text()<<"\n";
    outClose << "========      End        ======== " << "\n";
    fileClose.close();


}

void Rsa_eds_MPW::readKeyFromFile()
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
    if (first_line == "======== Public Rsa Signature key ======== ")
    {
        openExp_line_edit->setText(in.readLine());
        nModule_line_edit->setText(in.readLine());
    } else if (first_line == "======== Private Rsa Signature key ======== ")
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
