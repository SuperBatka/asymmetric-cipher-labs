#ifndef GOST3410_12_MPW_H
#define GOST3410_12_MPW_H

#include <QWidget>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QFileInfo>
#include <QFileDialog>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include "gost3410_12.h"
#include "streebog.h"

namespace Ui {
class Gost3410_12_MPW;
}

class Gost3410_12_MPW : public QWidget
{
    Q_OBJECT

public:
    explicit Gost3410_12_MPW(QWidget *parent = nullptr);
    ~Gost3410_12_MPW();

signals:
    void progress_changed_signal(int progress);
    void failed_signal(QString info);
    void result_signal(QString result);

private slots:
    void saveKeysToFilesSlot();
    void readKeyFromFileSlot();
    void signFileButtonSlot();
    void checkSignFileButtonSlot();
    void failed_slot(QString info);
    void endProcessSlot();
    void resultSlot(QString result);
    void generateSignatureKeySlot();

private:
    Ui::Gost3410_12_MPW *ui;

    QFuture<void> f1;
    QFutureWatcher<void> FuturWatcher;
    QString inputFileName;
    QString signatureFileName;
    gost3410_12 gostSignCore;


    enum class Operation{
        ENCRYPT,
        DECRYPT

    };

   void signatureFile(Operation OP);
   void calculateSignatureFile(Operation OP);
};

#endif // GOST3410_12_MPW_H
