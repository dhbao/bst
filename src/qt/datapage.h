// Copyright (c) 2018 Slawek Mozdzonek
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_DATAPAGE_H
#define BITCOIN_QT_DATAPAGE_H

#include <QWidget>

class QPlainTextEdit;
class PlatformStyle;

namespace Ui {
    class DataPage;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Overview ("home") page widget */
class DataPage : public QWidget
{
    Q_OBJECT

public:
    explicit DataPage(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~DataPage();


public Q_SLOTS:


Q_SIGNALS:


private:
    Ui::DataPage *ui;
    QString hexaValue;
    QString textValue;
    QString fileToRetrieveName;
    
    const int blockSizeDisplay;
    void displayInBlocks(QPlainTextEdit* textEdit, const QString& inStr, int blockSize);
    void hex2bin(const QString& hex, QByteArray& bin);

private Q_SLOTS:
    void retrieve();
    void store();
    void hexRadioClicked();
    void stringRadioClicked();
    void fileRetrieveClicked();
};

#endif // BITCOIN_QT_DATAPAGE_H
