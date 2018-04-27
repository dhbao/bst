// Copyright (c) 2018 Slawek Mozdzonek
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <qt/datapage.h>
#include <qt/forms/ui_datapage.h>
#include <qt/platformstyle.h>

#ifdef ENABLE_WALLET
#include <wallet/wallet.h>
#endif

#include "../data/processunspent.h"
#include "../data/retrievedatatxs.h"


class FileWriter
{
public:
	FileWriter(const QString& fileName_) : isOpen(false), file(fileName_) 
    {
        if(!fileName_.isEmpty())
        {
            isOpen=file.open(QIODevice::WriteOnly);
        }
    }

	~FileWriter()
	{
        if(isOpen)
        {
            file.close();
        }
	}

	void write(const QByteArray &byteArray)
	{
        if(isOpen)
        {
            qint64 bytesWritten=file.write(byteArray);
            if(bytesWritten==-1 || bytesWritten<byteArray.size())
            {
                throw std::runtime_error(std::string("FileWriter.write() failed"));
            }
        }
	}

private:
    bool isOpen;
	QFile file;	
};


DataPage::DataPage(const PlatformStyle *platformStyle, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataPage),
    blockSizeDisplay(64)
{
    ui->setupUi(this);
    ui->txidStoreEdit->setEnabled(false);
    ui->storeMessageRadioButton->setChecked(true);
    connect(ui->storeButton, SIGNAL(clicked()), this, SLOT(store()));

    ui->messageRetrieveEdit->setReadOnly(true);
    ui->stringRadioButton->setChecked(true);
    connect(ui->retrieveButton, SIGNAL(clicked()), this, SLOT(retrieve()));
    connect(ui->hexRadioButton, SIGNAL(clicked()), this, SLOT(hexRadioClicked()));
    connect(ui->stringRadioButton, SIGNAL(clicked()), this, SLOT(stringRadioClicked()));
    connect(ui->fileRetrieveButton, SIGNAL(clicked()), this, SLOT(fileRetrieveClicked()));
}

DataPage::~DataPage()
{
    delete ui;
}

void DataPage::fileRetrieveClicked()
{
    fileToRetrieveName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Files (*.*)"));
    ui->fileRetrieveEdit->setText(fileToRetrieveName);
}

void DataPage::displayInBlocks(QPlainTextEdit* textEdit, const QString& inStr, int blockSize)
{
    textEdit->clear();
    int beg=0;
    while(1)
    {
        QStringRef str=inStr.midRef(beg, blockSize);
        if(str.isNull())
        {
            break;
        }
        textEdit->appendPlainText(str.toString());
        beg+=blockSize;
    }
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    textEdit->setTextCursor(textCursor);
}

void DataPage::hex2bin(const QString& hex, QByteArray& bin)
{
    bin=QByteArray::fromHex(hex.toUtf8());    
}

void DataPage::retrieve()
{
    try
    {
        std::string txid=ui->txidRetrieveEdit->text().toStdString();
        RetrieveDataTxs retrieveDataTxs(txid);
        std::string txData=retrieveDataTxs.getTxData();

        hexaValue = QString::fromStdString(txData);
        textValue = QString::fromLocal8Bit(QByteArray::fromHex(hexaValue.toLatin1()));
        
        if(ui->hexRadioButton->isChecked())
        {
            displayInBlocks(ui->messageRetrieveEdit, hexaValue, blockSizeDisplay);
        }
        else
        {
            ui->messageRetrieveEdit->setPlainText(textValue);
        }
        
        QByteArray data;
        hex2bin(hexaValue, data);
        
        FileWriter fileWriter(ui->fileRetrieveEdit->text());
        fileWriter.write(data);
    }
    catch(std::exception const& e)
    {
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
    catch(...)
    {
        QMessageBox msgBox;
        msgBox.setText("Unknown exception occured");
        msgBox.exec();
    }
}

void DataPage::hexRadioClicked()
{
    displayInBlocks(ui->messageRetrieveEdit, hexaValue, blockSizeDisplay);
}

void DataPage::stringRadioClicked()
{
    ui->messageRetrieveEdit->setPlainText(textValue);
}

void DataPage::store()
{
#ifdef ENABLE_WALLET
    if(!vpwallets.empty())
    {
        try
        {
            std::vector<std::string> addresses;
            ProcessUnspent processUnspent(vpwallets[0], addresses);
            UniValue utx(UniValue::VARR);
            processUnspent.getUtxForAmount(utx, 0.0);
            
            const QString str=QString::fromStdString(getChangeAddress(vpwallets[0]));
            ui->txidStoreEdit->setText(str);
            ui->txidStoreEdit->displayText();
        }
        catch(std::exception const& e)
        {
            QMessageBox msgBox;
            msgBox.setText(e.what());
            msgBox.exec();
        }
        catch(...)
        {
            QMessageBox msgBox;
            msgBox.setText("Unknown exception occured");
            msgBox.exec();
        }
    }
#endif
}
