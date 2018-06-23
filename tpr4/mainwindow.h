#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QStandardItem>
#include <QTableView>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);



    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    QPoint mousPos;
    Ui::MainWindow *ui;
    void build();
    void rebuild();

    QSize getSize();
    void calculate();
    QTableView* tableRate;//таблица
    QStandardItemModel  *model;
    bool valueChanged;
    void run();

    float valCrash;
    float valRepear;
    int numNods;
    int colRow;
    int colColumn;
    QVector<float> cur_to_next;
    QVector<float> cur_to_previous;
    QVector<float> transition_ratio;
    QVector<float> probability_of_event;
    float sumRatio;
    float sumProbability;



    QVector< QString >headerName;
    QVector< QVector<float> > tableContent;//содержимое таблици
};

#endif // MAINWINDOW_H
