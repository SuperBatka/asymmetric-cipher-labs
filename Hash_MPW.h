#ifndef HASH_MPW_H
#define HASH_MPW_H


#include <QWidget>
#include "md5.h"
#include "sha1.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

namespace Ui {
class Hash_MPW;
}

class Hash_MPW : public QWidget
{
    Q_OBJECT

public:
    explicit Hash_MPW(QWidget *parent = nullptr);
    ~Hash_MPW();

signals:
    void hash_ended_signal(QByteArray res);
    void progress_changed_signal(int barProgress);
    void fail_signal(QString info);

private slots:
    void textHashSlot();
    void fileHashSlot();
    void hashEndedSlot(QByteArray res);
    void failSlot(QString info);

private:
    Ui::Hash_MPW *ui;
    QFuture<void> f1;
    QFutureWatcher<void> FuturWatcher;

};

#endif // HASH_MPW_H
