#include "Elgamal_eds_MPW.h"

Elgamal_eds_MPW::Elgamal_eds_MPW(QWidget *parent) : QWidget(parent)
{
    prime_line_edit     = new QLineEdit;
    generator_line_edit    = new QLineEdit;
    alice_private_key_line_edit   = new QLineEdit;
    alice_open_key_line_edit  = new QLineEdit;
    bob_private_key_line_edit= new QLineEdit;
    bob_open_key_line_edit  = new QLineEdit;

    prime_line_edit->setReadOnly(true);
    generator_line_edit->setReadOnly(true);
    alice_private_key_line_edit->setReadOnly(true);
    alice_open_key_line_edit->setReadOnly(true);
    bob_private_key_line_edit->setReadOnly(true);
    bob_open_key_line_edit->setReadOnly(true);


    encrypt_file_button = new QPushButton("Подписать файл");
    decrypt_file_button = new QPushButton("Проверить подпись файла");
    open_key_file_button = new QPushButton("Открыть файл с ключом");
    save_key_file_button = new QPushButton("Сохранить ключи в файл");

    generate_keys_numbers_button = new QPushButton("Сгенерировать числа");
    digits_line_edit = new QLineEdit;
    digits_line_edit->setValidator( new QIntValidator(2, 100, this) );

    prime_label        = new QLabel("Простое число \"p\"");
    generator_label        = new QLabel("Генератор \"g\"");
    alice_private_key_label       = new QLabel("число \"k\"");
    alice_open_key_label          = new QLabel("число \"r\"");
    bob_private_key_label         = new QLabel("число \"x\"");
    bob_open_key_label            = new QLabel("число \"y\"");
    gen_prime_label     = new QLabel("Количество знаков в простом числе: ");
    task_label          = new QLabel;


    file_layout         = new QGridLayout;
    param_layout        = new QVBoxLayout;
    glayout             = new QGridLayout;
    gen_layout          = new QHBoxLayout;
    param_main_layout   = new QVBoxLayout;
    buttons_layout      = new QHBoxLayout;


    param_group_box     = new QGroupBox("Параметры");
    file_group_box      = new QGroupBox("Подпись файла");
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
    //buttons_layout->addWidget(calculate_param_button);
    buttons_layout->addWidget(open_key_file_button);
    buttons_layout->addWidget(save_key_file_button);

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




    glayout->addWidget(param_group_box,0,0,1,1);
    glayout->addWidget(file_group_box,1,0,1,1);
    glayout->addWidget(task_label,3,0,1,2);
    glayout->addWidget(progressBar,4,0,1,2);




    setLayout(glayout);
        connect(encrypt_file_button, SIGNAL(clicked()),this, SLOT(encryptFileButtonHandler()));
    connect(decrypt_file_button, SIGNAL(clicked()),this, SLOT(decryptFileButtonHandler()));
    connect(open_key_file_button, SIGNAL(clicked()), this, SLOT(readKeyFromFile()));
    connect(save_key_file_button, SIGNAL(clicked()), this, SLOT(saveKeysToFiles()));

    connect(generate_keys_numbers_button, SIGNAL(clicked()),this, SLOT(generatePrimeNumbersButtonHandler()));
    connect(this, SIGNAL(progress_changed_signal(int)), progressBar, SLOT(setValue(int)));

    connect(this, SIGNAL(result_signal(QString)), this, SLOT(resultSlot(QString)));
    connect(this, SIGNAL(failed_signal(QString)), this, SLOT(failed_slot(QString)));


    connect(&FuturWatcher, SIGNAL(finished()), this, SLOT(endProcessSlot()));

}



void Elgamal_eds_MPW::resultSlot(QString result)
{
    QMessageBox::warning(this,"info", result);
}

void Elgamal_eds_MPW::endProcessSlot()
{
    progressBar->setValue(100);
    task_label->setText("Текущий процесс: Готово!");

}
void Elgamal_eds_MPW::failed_slot(QString info)
{
    QMessageBox::warning(this,"Warning", info);
    task_label->setText("Текущий процесс: " + info);
}


void Elgamal_eds_MPW::encryptFileButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }
    elgamalFile(Operation::ENCRYPT);
}
void Elgamal_eds_MPW::decryptFileButtonHandler()
{
    QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    }
    elgamalFile(Operation::DECRYPT);
}



void Elgamal_eds_MPW::generatePrimeNumbersButtonHandler()
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
}



void Elgamal_eds_MPW::elgamalFile(Operation OP)
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
            QMessageBox::warning(this,"Warning", "Недостаточно ключевых данных для проверки подписи");
            return;
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
            QMessageBox::warning(this,"Warning", "Недостаточно ключевых данных для подписания файла");
            return;
        }
        QMessageBox::information(this,"Info", "Выберете файл для подписи");
        inputFileName = QFileDialog::getOpenFileName(this,"Open the file");
        if(inputFileName == "")
        {
            return;
        }

        task_label->setText("Текущий процесс: Подпись файла");
    }


   // readKeys();
    f1 = QtConcurrent::run([=]() {
        calculateRsaFile(OP);
    });


    this->FuturWatcher.setFuture(f1);
}

void Elgamal_eds_MPW::calculateRsaFile(Operation OP)
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
       elgamalCore.setBufferSize(hashByteArray.size());
       size_t firstBlockSize = 0;
       char *firstBlock = elgamalCore.writeSignatureFirstBlock(&firstBlockSize);
       size_t size = 0;



       QByteArray firstBlockArray(firstBlock,firstBlockSize);
       char *res = elgamalCore.sign(hashByteArray.constData(),hashByteArray.size(),&size);
       QByteArray resultByteArray(res,size);
       resultByteArray = firstBlockArray + resultByteArray;

       QFile resultFile(inputFileName+".signature");
       if(!resultFile.open(QIODevice::WriteOnly))
       {
           emit failed_signal("Ошибка открытия или чтения файла.");
           return;
       }

       QDataStream out(&resultFile);
       out.writeRawData(resultByteArray.constData(), resultByteArray.size());
       delete[] res;
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

         if (readSize % elgamalCore.getModuleByteCount() != 0)
         {
             emit failed_signal("Входные данные не совпадают с размерами ключа");
             return;
         }

         elgamalCore.readSignatureFirstBlock(sigBuf, elgamalCore.getModuleByteCount());
         elgamalCore.setBufferSize(readSize);
         bool res = elgamalCore.checkSign(sigBuf + elgamalCore.getModuleByteCount(),readSize - elgamalCore.getModuleByteCount(),hashByteArray.constData(),hashByteArray.size(), &size);


         if(res)
         {
                emit result_signal("Подпись верна");
         } else
         {
                emit result_signal("Подпись не верна");
         }

         delete[] sigBuf;
   }




   delete[] result;
   delete[] buffer;
   delete hashCore;
   file.close();

}

void Elgamal_eds_MPW::saveKeysToFiles()
{
    if(!isKeyParametrsEntered(Operation::ENCRYPT) || !isKeyParametrsEntered(Operation::DECRYPT))
    {
        QMessageBox::warning(this,"Warning", "Не заполнены ключевые поля");
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
    outOpen << "======== Public Elgamal Signature key ======== " << "\n";
    outOpen<<prime_line_edit->text()<<"\n";
    outOpen<<generator_line_edit->text()<<"\n";
    outOpen<<alice_open_key_line_edit->text()<<"\n";
    outOpen << "========      End       ======== " << "\n";
    fileOpen.close();

    QFile fileClose(keyFileName+".priv_key");
    if(!fileClose.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для записи");
        return;
    }
    QTextStream outClose(&fileClose);
    outClose << "======== Private Elgamal Signature key ======== " << "\n";
    outClose<<prime_line_edit->text()<<"\n";
    outClose<<generator_line_edit->text()<<"\n";
    outClose<<bob_private_key_line_edit->text()<<"\n";
    outClose<<alice_private_key_line_edit->text()<<"\n";
    outClose << "========      End        ======== " << "\n";
    fileClose.close();


}

void Elgamal_eds_MPW::readKeyFromFile()
{
    QString keyFileName = QFileDialog::getOpenFileName(this,"Open the keyfile");
    QFile file(keyFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для чтения");
        return;
    }
    prime_line_edit->clear();
    generator_line_edit->clear();
    alice_open_key_line_edit->clear();
    alice_private_key_line_edit->clear();
    bob_private_key_line_edit->clear();
    bob_open_key_line_edit->clear();


    QTextStream in(&file);
    QString first_line = in.readLine();
    if (first_line == "======== Public Elgamal Signature key ======== ")
    {
        prime_line_edit->setText(in.readLine());
        generator_line_edit->setText(in.readLine());
        alice_open_key_line_edit->setText(in.readLine());
        QString prime = prime_line_edit->text();
        QString generator = generator_line_edit->text();
        QString y = alice_open_key_line_edit->text();
        bool res = elgamalCore.setOpenKeyParametrs(y.toLocal8Bit().constData(),generator.toLocal8Bit().constData(),prime.toLocal8Bit().constData());
        if(!res)
        {
            QMessageBox::warning(this,"Warning", "Не корректный ключ");
            prime_line_edit->clear();
            generator_line_edit->clear();
            alice_open_key_line_edit->clear();
            alice_private_key_line_edit->clear();
            bob_private_key_line_edit->clear();
            bob_open_key_line_edit->clear();
            return;
        }
    } else if (first_line == "======== Private Elgamal Signature key ======== ")
    {

        prime_line_edit->setText(in.readLine());
        generator_line_edit->setText(in.readLine());
        bob_private_key_line_edit->setText(in.readLine());
        alice_private_key_line_edit->setText(in.readLine());
        QString prime = prime_line_edit->text();
        QString generator = generator_line_edit->text();
        QString k = bob_private_key_line_edit->text();
        QString x = alice_private_key_line_edit->text();
        bool res = elgamalCore.setPrivateKeyParametrs(x.toLocal8Bit().constData(),k.toLocal8Bit().constData(),generator.toLocal8Bit().constData(),prime.toLocal8Bit().constData());
        if(!res)
        {
            QMessageBox::warning(this,"Warning", "Не корректный ключ");
            prime_line_edit->clear();
            generator_line_edit->clear();
            alice_open_key_line_edit->clear();
            alice_private_key_line_edit->clear();
            bob_private_key_line_edit->clear();
            bob_open_key_line_edit->clear();
            return;
        }
    } else
    {
        QMessageBox::warning(this,"Warning", "В данном файле нет ключей");
        return;
    }

    file.close();
}

bool Elgamal_eds_MPW::isKeyParametrsEntered(Operation OP)
{
    if(OP == Operation::ENCRYPT)
    {
        if( (prime_line_edit->text() != "")
                && (generator_line_edit->text() != "")
                && (bob_private_key_line_edit->text() != "")
                && (alice_private_key_line_edit->text() != ""))

        {
            return  true;
        }
    }

    if(OP == Operation::DECRYPT)
    {
        if( (prime_line_edit->text() != "")
                && (generator_line_edit->text() != "")
                && (alice_open_key_line_edit->text() != ""))

        {
            return  true;
        }
    }


    return false;
}

