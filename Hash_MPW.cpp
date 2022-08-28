#include "Hash_MPW.h"
#include "ui_Hash_MPW.h"
#include "streebog.h"
#include "HashI.h"

Hash_MPW::Hash_MPW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hash_MPW)
{
    ui->setupUi(this);
    connect(ui->file_hash_Button, SIGNAL(clicked()),this, SLOT(fileHashSlot()));
    connect(ui->text_hash_Button, SIGNAL(clicked()),this, SLOT(textHashSlot()));
    connect(this, SIGNAL(hash_ended_signal(QByteArray)),this, SLOT(hashEndedSlot(QByteArray)));
    connect(this, SIGNAL(progress_changed_signal(int)), ui->progressBar, SLOT(setValue(int)));
    connect(this, SIGNAL(fail_signal(QString)), this, SLOT(failSlot(QString)));
}

Hash_MPW::~Hash_MPW()
{
    delete ui;
}

/* uint8_t in[] = {0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,
              0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,
              0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,
              0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30};
 unsigned char *res = hashFinal(in,63);
 QByteArray resultByteArray((char*)res,64);
 ui->result_hash_Edit->clear();
 ui->result_hash_Edit->insertPlainText(resultByteArray.toHex());*/


void Hash_MPW::textHashSlot()
{


    QString text = ui->input_text_Edit->toPlainText();
    if (text.length() > 5000){
        QMessageBox::warning(this,"Warning", "Слишком большой входной текст, попробуйте хеширование файлов.");
        return;
    }
    int hashNum = ui->hash_Box->currentIndex();

    HashI *hashCore;
    int d;
    int startPos;


    char *res;
    size_t count = 0;
    QByteArray textByteArray( text.toUtf8() );
    size_t textSize = textByteArray.size();
    //hashCore->Init();


    if(hashNum == 0)
    {
        hashCore = new MD5();
        d = 1;

    }
    if (hashNum == 1)
    {
        hashCore = new SHA1();
        d = 1;


    }
    if (hashNum == 2)
    {
        hashCore = new Streebog();
        d = -1;
        startPos = textSize - 64;
    }

    if ( textSize > 64)
    {
        count = textSize/64;
        if( textSize % 64 == 0)
            count--;
        if (hashNum == 2)
        {
            for(size_t i = 0; i < count; i ++)
            {
                hashCore->hashBlock((unsigned char*)(textByteArray.constData() + startPos - i*64));
            }
            hashCore->hashFinaleBlock((unsigned char*)(textByteArray.constData()),textByteArray.size() - count*64,textByteArray.size());

        } else {
            for(size_t i = 0; i < count; i ++)
            {
                hashCore->hashBlock((unsigned char*)(textByteArray.constData() + i*64));
            }
            hashCore->hashFinaleBlock((unsigned char*)(textByteArray.constData() + count*64),textByteArray.size() - count*64,textByteArray.size());

        }
        res = (char*)hashCore->getHashResult();

    } else {

        hashCore->hashFinaleBlock((unsigned char*)textByteArray.constData(),textByteArray.size(),textByteArray.size());
        res = ( char*)hashCore->getHashResult();
    }

    QByteArray resultByteArray(res,hashCore->GetMode());
    delete hashCore;
    ui->result_hash_Edit->clear();
    ui->result_hash_Edit->insertPlainText(resultByteArray.toHex());


}

/*void Hash_MPW::textHashSlot()
{


    QString text = ui->input_text_Edit->toPlainText();
    if (text.length() > 5000){
        QMessageBox::warning(this,"Warning", "Слишком большой входной текст, попробуйте хеширование файлов.");
        return;
    }
    HashI *hashCore = new SHA1();
    char *res;
    size_t count = 0;
    QByteArray textByteArray( text.toUtf8() );
    size_t textSize = textByteArray.size();
    //hashCore->Init();
    if ( textSize > 64)
    {
        count = textSize/64;
        if( textSize % 64 == 0)
            count--;

        for(size_t i = 0; i < count; i ++)
        {
            hashCore->hashBlock((unsigned char*)textByteArray.constData() + i*64);
        }
        hashCore->hashFinaleBlock((unsigned char*)textByteArray.constData() + count*64,textByteArray.size() - count*64,textByteArray.size());
        res = (char*)hashCore->getHashResult();

    } else {

        hashCore->hashFinaleBlock((unsigned char*)textByteArray.constData(),textByteArray.size(),textByteArray.size());
        res = ( char*)hashCore->getHashResult();
    }

    QByteArray resultByteArray(res,20);
    ui->result_hash_Edit->clear();
    ui->result_hash_Edit->insertPlainText(resultByteArray.toHex());


} */

void Hash_MPW::fileHashSlot()
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }

    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    ui->filename_Edit->setText(inputFileName);

    int hashNum = ui->hash_Box->currentIndex();

    f1 = QtConcurrent::run([=]() {

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
            emit fail_signal("Ошибка открытия или чтения файла.");
            return;
        }
        QDataStream in(&file);

        quint64 ByteCounter = 0;
        char *result ;
        quint64 barProgress = 0;
        size_t bytesRead;
        quint64 pos;
        quint64 step;

        HashI *hashCore;
        if(hashNum == 0)
        {
            hashCore = new MD5();
            pos = 0;
            step = 64;
        }
        if (hashNum == 1)
        {
            hashCore = new SHA1();
            pos = 0;
            step = 64;
        }
        if (hashNum == 2)
        {
            hashCore = new Streebog();
            pos = fileSize - 64;
            step = -64;
        }


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
           emit fail_signal( "Произошла ошибка чтения файла. Хэш файла может быть посчитан неправильно.");
       }

       QByteArray resultByteArray(result,hashCore->GetMode());
       emit hash_ended_signal(resultByteArray);

       delete[] result;
       delete[] buffer;
       delete hashCore;
       file.close();

   });
}

/*
void Hash_MPW::fileHashSlot()
{
    if(f1.isRunning())
    {
        QMessageBox::warning(this,"Warning", "Подождите, выполняется другая операция");
        return;
    }

    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    ui->filename_Edit->setText(inputFileName);

    f1 = QtConcurrent::run([=]() {

        quint64 fileSize = QFileInfo(inputFileName).size();
        size_t count =0;

        size_t bufferSize = 4096;
        quint64 bufferBlocksCount = fileSize / bufferSize;
        if(fileSize % bufferSize == 0)
        {
            bufferBlocksCount--;
        }


        QFile file(inputFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            emit fail_signal("Ошибка открытия или чтения файла.");
            return;
        }
        QDataStream in(&file);


        quint64 ByteCounter = 0;
        char *buffer = new char[bufferSize];
        char *result ;
        quint64 barProgress = 0;
        size_t bytesRead;

        HashI *hashCore = new MD5();
        //hashCore.Init();

       for (quint64 i = 0; i < bufferBlocksCount; i++)
       {
           bytesRead = in.readRawData(buffer,bufferSize);


           for( size_t i = 0; i < bytesRead / 64; i++)
           {
               hashCore->hashBlock((unsigned char*)buffer + i*64);
           }

           ByteCounter+=bytesRead;


           if ((barProgress < (ByteCounter * 100/ fileSize )) && barProgress < 101)
           {
               barProgress = ByteCounter * 100 / fileSize ;
               emit progress_changed_signal(barProgress);
           }
       }

       bytesRead = in.readRawData(buffer,bufferSize);

       if ( bytesRead > 64)
       {
           count = bytesRead/64;
           if( bytesRead % 64 == 0)
               count--;

           for(size_t i = 0; i < count; i ++)
           {
               hashCore->hashBlock((unsigned char*)buffer + i*64);
           }
           hashCore->hashFinaleBlock((unsigned char*)buffer + count*64,bytesRead - count*64,fileSize);
           result = (char*)hashCore->getHashResult();

       } else {

           hashCore->hashFinaleBlock((unsigned char*)buffer,bytesRead,fileSize);
           result = (char*)hashCore->getHashResult();
       }

       ByteCounter+=bytesRead;


       if(ByteCounter != fileSize)
       {
           emit fail_signal( "Произошла ошибка чтения файла. Хэш файла может быть посчитан неправильно.");
       }

       QByteArray resultByteArray(result,20);
       emit hash_ended_signal(resultByteArray);

       delete[] result;
       delete[] buffer;
       file.close();

   });
}
*/
void Hash_MPW::hashEndedSlot(QByteArray res)
{
    ui->progressBar->setValue(100);
    ui->result_hash_Edit->clear();
    ui->result_hash_Edit->insertPlainText(res.toHex());
}

void Hash_MPW::failSlot(QString info)
{
    QMessageBox::warning(this,"Warning",info);
}
