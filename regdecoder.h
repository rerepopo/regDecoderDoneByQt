#ifndef REGDECODER_H
#define REGDECODER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegDecoder; }
QT_END_NAMESPACE

class RegDecoder : public QWidget
{
    Q_OBJECT
    uint displayingDecValue = 0xFF;
    void cleanSelection(void);
    void showToTableWidget(void);
    void showToLineEditOut(uint displayingDecValue);
    //ref: https://doc.qt.io/qt-5/qtablewidgetitem.html
    //ref: https://www.regexpal.com/93640
    uint setBit(uint value, uint bit);
    uint clearBit(uint value, uint bit);
    void keyPressEvent(QKeyEvent *event);
public:
    RegDecoder(QWidget *parent = nullptr);
    ~RegDecoder();

private slots:
    void on_pushButtonCleanSelection_clicked();
    void on_pushButton_clicked();

    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::RegDecoder *ui;
};
#endif // REGDECODER_H
