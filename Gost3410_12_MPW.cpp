#include "Gost3410_12_MPW.h"
#include "ui_Gost3410_12_MPW.h"

Gost3410_12_MPW::Gost3410_12_MPW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gost3410_12_MPW)
{
    ui->setupUi(this);
    connect(this, SIGNAL(progress_changed_signal(int)), ui->progressBar, SLOT(setValue(int)));

    connect(this, SIGNAL(result_signal(QString)), this, SLOT(resultSlot(QString)));
    connect(this, SIGNAL(failed_signal(QString)), this, SLOT(failed_slot(QString)));

    connect(ui->sign_file_button, SIGNAL(clicked()), this, SLOT(signFileButtonSlot()));
    connect(ui->signcheck_file_button, SIGNAL(clicked()), this, SLOT(checkSignFileButtonSlot()));
    connect(ui->generate_sign_keys_button, SIGNAL(clicked()), this, SLOT(generateSignatureKeySlot()));
    connect(ui->save_keys_file_button, SIGNAL(clicked()), this, SLOT(saveKeysToFilesSlot()));
    connect(ui->load_keys_file_button, SIGNAL(clicked()), this, SLOT(readKeyFromFileSlot()));

    connect(&FuturWatcher, SIGNAL(finished()), this, SLOT(endProcessSlot()));
    gostSignCore.set_default_Elleptic_Curve();
    gostSignCore.generate_signature_keys();
    ui->elleptic_curve_p_edit->setText("3623986102229003635907788753683874306021320925534678605086546150450856166624002482588482022271496854025090823603058735163734263822371964987228582907372403");
    ui->elleptic_curve_a_edit->setText("7");
    ui->elleptic_curve_b_edit->setText("1518655069210828534508950034714043154928747527740206436194018823352809982443793732829756914785974674866041605397883677596626326413990136959047435811826396");
    ui->elleptic_curve_m_edit->setText("3623986102229003635907788753683874306021320925534678605086546150450856166623969164898305032863068499961404079437936585455865192212970734808812618120619743");
    ui->elleptic_curve_q_edit->setText("3623986102229003635907788753683874306021320925534678605086546150450856166623969164898305032863068499961404079437936585455865192212970734808812618120619743");
    generateSignatureKeySlot();
}

Gost3410_12_MPW::~Gost3410_12_MPW()
{
    delete ui;
}



void Gost3410_12_MPW::resultSlot(QString result)
{
    QMessageBox::warning(this,"info", result);
}

void Gost3410_12_MPW::endProcessSlot()
{
    ui->progressBar->setValue(100);
    ui->task_label->setText("Текущий процесс: Готово!");

}
void Gost3410_12_MPW::failed_slot(QString info)
{
    QMessageBox::warning(this,"Warning", info);
    ui->task_label->setText("Текущий процесс: " + info);
}


void Gost3410_12_MPW::signFileButtonSlot()
{
   /* QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    } */
    signatureFile(Operation::ENCRYPT);
}

void Gost3410_12_MPW::checkSignFileButtonSlot()
{
 /*   QString temp = prime_line_edit->text();
    if (temp == "") {
        QMessageBox::warning(this,"Warning", "Сначала сгенирируйте ключи");
        return;
    } */
    signatureFile(Operation::DECRYPT);
}



void Gost3410_12_MPW::generateSignatureKeySlot()
{
    gostSignCore.generate_signature_keys();
    ui->sign_key_edit->setText(gostSignCore.get_sign_key_d());
    ui->signcheck_key_x_edit->setText(gostSignCore.get_signcheck_keyQ_x());
    ui->signcheck_key_y_edit->setText(gostSignCore.get_signcheck_keyQ_y());
}



void Gost3410_12_MPW::signatureFile(Operation OP)
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }
    ui->progressBar->setValue(0);

    if(OP == Operation::DECRYPT)
    {
        if(ui->signcheck_key_x_edit->text() == "" || ui->signcheck_key_y_edit->text() == "")
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

        ui->task_label->setText("Текущий процесс: Проверка подписи файла");
    }

    if(OP == Operation::ENCRYPT)
    {
        if(ui->sign_key_edit->text() == "")
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

        ui->task_label->setText("Текущий процесс: Подпись файла");
    }


   // readKeys();
    f1 = QtConcurrent::run([=]() {
        calculateSignatureFile(OP);
    });


    this->FuturWatcher.setFuture(f1);
}

void Gost3410_12_MPW::calculateSignatureFile(Operation OP)
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
    quint64 pos= fileSize - 64;
    quint64 step =  -64;

    HashI *hashCore  = new Streebog();
    char *buffer = new char[bufferSize];
    //hashCore.Init();
    file.seek(pos);
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
       pos +=step;
       file.seek(pos);
   }
   if ( pos > (fileSize + 128))
   {
       pos = 0;
       file.seek(pos);
   }
   if ( pos > fileSize)
   {
       pos = (fileSize / 64) * 64;
       file.seek(pos);
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

       char *res = gostSignCore.sign(hashByteArray.constData());
       QByteArray resultByteArray(res,128);

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
        if(signatureSize != 128)
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

         bool res = gostSignCore.checkSign(sigBuf,hashByteArray.constData());

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

void Gost3410_12_MPW::saveKeysToFilesSlot()
{

    QString keyFileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    QFile fileOpen(keyFileName+".pub_key");
    if(!fileOpen.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для записи");
        return;
    }
    QTextStream outOpen(&fileOpen);
    outOpen << "======== Public GOST R 34.10-2012 Signature key ======== " << "\n";
    outOpen<<ui->signcheck_key_x_edit->text()<<"\n";
    outOpen<<ui->signcheck_key_y_edit->text()<<"\n";
    outOpen << "========      End       ======== " << "\n";
    fileOpen.close();

    QFile fileClose(keyFileName+".priv_key");
    if(!fileClose.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для записи");
        return;
    }
    QTextStream outClose(&fileClose);
    outClose << "======== Private GOST R 34.10-2012 Signature key ======== " << "\n";
    outClose<<ui->sign_key_edit->text()<<"\n";
    outClose << "========      End        ======== " << "\n";
    fileClose.close();


}

void Gost3410_12_MPW::readKeyFromFileSlot()
{
    QString keyFileName = QFileDialog::getOpenFileName(this,"Open the keyfile");
    QFile file(keyFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Warning", "Не удалось открыть файл для чтения");
        return;
    }
    ui->signcheck_key_x_edit->clear();
    ui->signcheck_key_y_edit->clear();
    ui->sign_key_edit->clear();

    QTextStream in(&file);
    QString first_line = in.readLine();
    if (first_line == "======== Public GOST R 34.10-2012 Signature key ======== ")
    {
        ui->signcheck_key_x_edit->setText(in.readLine());
        ui->signcheck_key_y_edit->setText(in.readLine());
        if(!gostSignCore.set_Elleptic_Curve_Q_point(ui->signcheck_key_x_edit->text().toLocal8Bit().constData(),ui->signcheck_key_y_edit->text().toLocal8Bit().constData()))
        {
            QMessageBox::warning(this,"Warning", "Данный ключ проверки подписи не подходит для текущей эллиптической кривой");
            ui->signcheck_key_x_edit->clear();
            ui->signcheck_key_y_edit->clear();
        }

    } else if (first_line == "======== Private GOST R 34.10-2012 Signature key ======== ")
    {

        ui->sign_key_edit->setText(in.readLine());
        if(!gostSignCore.set_d_parameter(ui->sign_key_edit->text().toLocal8Bit().constData()))
        {
            QMessageBox::warning(this,"Warning", "Данный ключ подписи не подходит для текущей эллиптической кривой");
            ui->sign_key_edit->clear();
        }

    } else
    {
        QMessageBox::warning(this,"Warning", "В данном файле нет ключей");
        return;
    }



    file.close();
}
