#include "regdecoder.h"
#include "ui_regdecoder.h"
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QString>
#include <QKeyEvent>

void RegDecoder::cleanSelection()
{
   QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
   for (uint item=0; item<list.size();item++ )
   {
      list[item]->setSelected(false);
   }
   showToLineEditOut(this->displayingDecValue);
}

void RegDecoder::showToTableWidget()
{  //show to tableWidget from this->displayingDecValue
   QString binaryStr= QString::number(this->displayingDecValue, 2).rightJustified(32, '0');//QString with zero padding (leading zeroes)
   //qDebug() << "The number in binary" << binaryStr;

   for (int index = 0; index<32;index++)
   {
      ui->tableWidget->item(0, index)->setText(binaryStr.at(index));
   }
   cleanSelection();
}

void RegDecoder::showToLineEditOut(uint displayingDecValue)
{
   ui->lineEditOutBin->setText("0b"+QString::number(displayingDecValue, 2).rightJustified(32, '0'));
   ui->lineEditOutDec->setText(QString::number(displayingDecValue, 10));
   ui->lineEditOutHex->setText("0x"+(QString::number(displayingDecValue, 16)).toUpper());
}

uint RegDecoder::setBit(uint value, uint bit)
{
   return value | (1 << bit);
}

uint RegDecoder::clearBit(uint value, uint bit)
{
    return value & ~(1 << bit);
}

void RegDecoder::keyPressEvent(QKeyEvent *event)
{
    if(( event->key() == Qt::Key_Return)||( event->key() == Qt::Key_Enter))
    {
       on_pushButton_clicked();
    }
    /*else
    {
       QDialog::keyPressEvent(event);
    }  */
}

RegDecoder::RegDecoder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegDecoder)
{
   ui->setupUi(this);
   on_pushButton_clicked();
}

RegDecoder::~RegDecoder()
{
   delete ui;
}


void RegDecoder::on_pushButtonCleanSelection_clicked()
{
   cleanSelection();
}

void RegDecoder::on_pushButton_clicked()
{
   QString inputStr = ui->lineEdit->text();
   // ref https://vimsky.com/zh-tw/examples/detail/python-method-PyQt4.QtGui.QFont.html
   QPalette *palettePass = new QPalette();
   palettePass->setColor(QPalette::Text,QColor("black"));//  setColor( QPalette.Text, QtCore.Qt.black);

   QFont *fontPass = new QFont();
   fontPass->setPointSize(12);
   fontPass->setBold(false);

   QPalette *paletteFail = new QPalette();
   paletteFail->setColor(QPalette::Text,QColor("red"));//  setColor( QPalette.Text, QtCore.Qt.black);

   QFont *fontFail = new QFont();
   fontFail->setPointSize(16);
   fontFail->setBold(true);
   //1. decode
   inputStr = inputStr.trimmed();
   //qDebug() << "buttonClicked: text is " << inputStr;
   bool ok = false;
   uint  inputInt;
   if (inputStr.startsWith("0x")) // if hex
   {
       uint hexStrToDec = inputStr.toUInt(&ok,16);
       qDebug() << "hex "<< hexStrToDec << ok;
       inputInt=hexStrToDec;
   }
   else if (inputStr.startsWith("0b")) // if bin
   {
       uint binStrToDec=inputStr.right(inputStr.size()-2).toUInt(&ok,2);
       qDebug() << "bin "<< binStrToDec << ok;
       inputInt=binStrToDec;
   }
   else
   {
       uint decStrToDec=inputStr.toUInt(&ok,10);
       qDebug() << "dec "<< decStrToDec << ok;
       inputInt=decStrToDec;
   }
   //2. check and show if need
   if (ok==false)
   {
      //show error
       qDebug() << "show error";
       ui->lineEdit->setPalette(*paletteFail);
       ui->lineEdit->setFont(*fontFail);
       ui->labelInputCheckResult->setText("invalid input. Only Support 0~0xFFFFFFFF. \nNOTE: \"^0x[0-9A-Fa-f]+$\" for hex. \"^0b[0-1]+$\" for binary. \"^[0-9]+$\" for decimal.");
   }
   /****************** No need due to uint type ******************
   else if (inputInt>0xFFFFFFFF)
   {
      inputInt=0xFFFFFFFF;
      ui->lineEdit->setPalette(*paletteFail);
      ui->lineEdit->setFont(*fontFail);
      ui->labelInputCheckResult->setText("is invalid input. Take it as 0xFFFFFFFF.  \nNOTE: Only Support 0~2^32-1.");
      this->displayingDecValue=(uint)inputInt;
      showToTableWidget();
   }
   else if (inputInt<0)
   {
      inputInt=0;
      ui->lineEdit->setPalette(*paletteFail);
      ui->lineEdit->setFont(*fontFail);
      ui->labelInputCheckResult->setText("is invalid input. Take it as 0.  \nNOTE: Only Support 0~2^32-1.");
      this->displayingDecValue=(uint)inputInt;
      showToTableWidget();
   }
   */
   else
   {//everything is good
      ui->labelInputCheckResult->setText("Input Check Result: PASS.");
      qDebug() << "Input Check Result: PASS.";
      ui->lineEdit->setPalette(*palettePass);
      ui->lineEdit->setFont(*fontPass);
      ui->labelInputCheckResult->setText("Input Check Result: PASS.");
      this->displayingDecValue=(uint)inputInt;
      showToTableWidget();
   }
   //3. end
   delete palettePass;delete fontPass;delete paletteFail;delete fontFail;
}

void RegDecoder::on_tableWidget_itemSelectionChanged()
{
    QList<QTableWidgetItem*> indexList = ui->tableWidget->selectedItems();
    QString selectedStr = QString("");
    QString selectedIndexStr = QString("");

    std::vector<int> selectedIndexStrlist;
    //get all index
    for (uint i=0;i<indexList.size() ;i++ )
    {
       selectedIndexStrlist.push_back(indexList[i]->column());
       //qDebug() << "indexList[i]->column(): " << indexList[i]->column();
    }
    std::sort( selectedIndexStrlist.begin(), selectedIndexStrlist.end() );

    //show all after sorting
    for (uint i=0;i<selectedIndexStrlist.size() ;i++ )
    {
       //qDebug() << "[selectedIndexStrlist[i]: " << selectedIndexStrlist[i];
       selectedStr = selectedStr + ui->tableWidget->item(0,selectedIndexStrlist[i])->text();
       selectedIndexStr=selectedIndexStr + QString::number(31-ui->tableWidget->item(0,selectedIndexStrlist[i])->column(), 10) +", ";
    }


    //qDebug() << "selectedStr: " << selectedStr;
    selectedIndexStr.resize(selectedIndexStr.size()-2);
    bool ok;
    uint binStrToDec=selectedStr.toUInt(&ok,2);

    if (ok)
    {
       ui->labelOutputResult->setText("partial Selection: [" + selectedIndexStr + "]");
       showToLineEditOut(binStrToDec);
    }
    else
    {
       ui->labelOutputResult->setText("Output Result: ");
       showToLineEditOut(this->displayingDecValue);
       qDebug()  << ok << "on_tableWidget_itemSelectionChanged: use displayingDecValue ";
    }

}

void RegDecoder::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    bool ok;
    int indexColumn = index.column();
    if( ui->tableWidget->item(0, indexColumn)->text().toUInt(&ok,10)==0)
    {
       ui->tableWidget->item(0, indexColumn)->setText("1");
       this->displayingDecValue = setBit(this->displayingDecValue, (uint) (31-indexColumn));
    }
    else
    {
       ui->tableWidget->item(0, indexColumn)->setText("0");
       this->displayingDecValue = clearBit(this->displayingDecValue, (uint) (31-indexColumn));
    }
    showToTableWidget();
    cleanSelection();
}
