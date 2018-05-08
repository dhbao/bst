// Copyright (c) 2018 Slawek Mozdzonek
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_DATAPAGE_H
#define BITCOIN_QT_DATAPAGE_H

#include <QWidget>
#include <univalue.h>

class WalletModel;
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
    void setModel(WalletModel *model);

public Q_SLOTS:


Q_SIGNALS:


private:
    Ui::DataPage *ui;
    WalletModel *walletModel;
    const int blockSizeDisplay;
    std::string changeAddress;
    QString hexaValue;
    QString textValue;
    QString fileToRetrieveName;
    QString fileToStoreName;

    void displayInBlocks(QPlainTextEdit* textEdit, const QString& inStr, int blockSize);
    void hex2bin(const QString& hex, QByteArray& bin);
    void unlockWallet();
    std::string byte2str(const QByteArray& binaryData);
    std::string getHexStr();
    double computeFee(size_t dataSize);
    std::string double2str(double val);
    std::string computeChange(const UniValue& inputs, double fee);

private Q_SLOTS:
    void retrieve();
    void store();
    void hexRadioClicked();
    void stringRadioClicked();
    void fileRetrieveClicked();
    void fileStoreClicked();    
    void storeMessageRadioClicked();
    void storeFileRadioClicked();
    void storeFileHashRadioClicked();
};

#endif // BITCOIN_QT_DATAPAGE_H
