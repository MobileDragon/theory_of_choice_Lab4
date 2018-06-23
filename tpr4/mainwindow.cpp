#include "mainwindow.h"
#include "ui_mainwindow.h"
//аналитическая модель
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    build();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::build()
{
    headerName<<"k"<<"λk.k+1"<<"λk+1.k"<<"П"<<"Вероятность события";
    model=0;
    valueChanged=false;
    colRow=0;
    colColumn=5;
    valCrash=0;
    valRepear=0;

    tableRate=new QTableView();
    tableRate->setFont(QFont("Arial",10,5000));
    tableRate->verticalHeader()->hide();
    tableRate->horizontalHeader()->hide();
    ui->verticalLayout_9->addWidget(tableRate);
    ui->comboBox->setFixedWidth(50);
    for(int i=2; i<11; ++i)
    {
        ui->comboBox->addItem(QString().setNum(i,10) );
    }

}

void MainWindow::rebuild()
{
    if(model!=0)
    {
        model->deleteLater();
    }
    model = new QStandardItemModel;
    //


    //
    for(int i=0;i<colColumn;i++)
    {
        QStandardItem *item = new QStandardItem(headerName[i]);
        item->setEditable(false);
        item->setCheckable(false);
        //item->setEnabled(false);

        item->setBackground(QBrush(QColor(250,230,170)) );
        item->setFont(QFont("Arial",10,6500));
        model->setItem(0, i, item);//
    }
    for(int i=0;i <= numNods;i++)
    {
        QStandardItem *item = new QStandardItem(QString("%1").arg(i) );
        item->setEditable(false);
        item->setCheckable(false);
        //item->setEnabled(false);

        item->setBackground(QBrush(QColor(250,230,170)) );
        item->setFont(QFont("Arial",10,6500));
        model->setItem(i+1, 0, item);//
    }
    QStandardItem *itemS = new QStandardItem("Σ");
    itemS->setEditable(false);
    itemS->setCheckable(false);
    itemS->setBackground(QBrush(QColor(250,230,170)) );
    itemS->setFont(QFont("Arial",10,6500));
    model->setItem(numNods+2, 0, itemS);//
    //
    itemS = new QStandardItem( QString("%1").arg(sumRatio) );
    itemS->setEditable(false);
    itemS->setCheckable(false);
    itemS->setBackground(QBrush(QColor(250,230,170)) );
    itemS->setFont(QFont("Arial",10,6500));
    model->setItem(numNods+2, 3, itemS);//
    //
    itemS = new QStandardItem( QString("%1").arg(sumProbability) );
    itemS->setEditable(false);
    itemS->setCheckable(false);
    itemS->setBackground(QBrush(QColor(250,230,170)) );
    itemS->setFont(QFont("Arial",10,6500));
    model->setItem(numNods+2, 4, itemS);//


    //2,3,5 столбцы:
    for(int i=0; i<=numNods ;i++)
    {
        //2
        QStandardItem *item = new QStandardItem(QString("%1").arg(cur_to_next[i]) );
        item->setEditable(false);
        item->setFont(QFont("Arial",10,6500));
        model->setItem(i+1, 1, item);//
        //3
        item = new QStandardItem(QString("%1").arg(cur_to_previous[i]) );
        item->setEditable(false);
        item->setFont(QFont("Arial",10,6500));
        model->setItem(i+1, 2, item);//
        //5
        item = new QStandardItem(QString("%1").arg(probability_of_event[i]) );
        item->setEditable(false);
        item->setFont(QFont("Arial",10,6500));
        model->setItem(i+1, 4, item);//
    }
    //4 столбец:
    for(int i=0; i<numNods; i++)
    {
        QStandardItem *item = new QStandardItem(QString("%1").arg(transition_ratio[i]) );
        item->setEditable(false);
        item->setFont(QFont("Arial",10,6500));
        model->setItem(i+1, 3, item);
    }

    //
    tableRate->setModel(model);
    for(int i=0; i < colColumn; i++)//
    {
        tableRate->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }

    tableRate->setFixedSize(getSize());

}

QSize MainWindow::getSize()
{
    QSize tableSize;
    int i=0;
    int height=0;
    int currentHeight=tableRate->rowHeight(i);
    while(currentHeight > 0)
    {
        height+=currentHeight;
        ++i;
        currentHeight=tableRate->rowHeight(i);
    }
    height+=i/2+1;
    i=0;
    int width=0;
    int currentWidth=tableRate->columnWidth(i);
    while(currentWidth > 0)
    {
        width+=currentWidth;
        ++i;
        currentWidth=tableRate->columnWidth(i);
    }
    width+=i/2+1;
    //width = tableCreterion->columnWidth(0)+20;
    tableSize.setHeight(height);
    tableSize.setWidth(width);
    return tableSize;
}

void MainWindow::calculate()
{
    float sum=0;//сумма оценок компетентности экспертов
    cur_to_next.clear();
    cur_to_previous.clear();
    transition_ratio.clear();
    probability_of_event.clear();

    for(int i=0; i<=numNods; i++)
    {
        //расчет второго столбца:
        cur_to_next.append( ( numNods-i )*valCrash );
        //расчет третьего столбца:
        cur_to_previous.append( i * valRepear );
    }
    //расчет четвертого столбца:
    float sum1=1;
    float sum2=1;
    sumRatio=0;
    sumProbability=0;
    for(int i=0; i<numNods; i++)
    {
        sum1 *= cur_to_next[i];
        sum2 *= cur_to_previous[i+1];
        transition_ratio.append( sum1 / sum2 );
        sumRatio += transition_ratio[i];
    }

    //расчет пятого столбца:
    probability_of_event.append( 1 / (sumRatio+1) );
    for(int i=0; i<numNods; i++)
    {
        probability_of_event.append( probability_of_event[i]*cur_to_next[i]/cur_to_previous[i+1] );
        sumProbability += probability_of_event[i];
    }
    sumProbability += probability_of_event[numNods];


}


void MainWindow::run()
{
    if(ui->lineEdit->text().length()>0 &&
            ui->lineEdit_2->text().length()>0)
    {
        //
        calculate();
        rebuild();
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int kol=arg1.toInt();
    numNods=kol;
    colRow=numNods+3;

    //
    run();

    this->adjustSize();//передел размера окна
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(valueChanged)
        return;
    bool ok;
    float f=ui->lineEdit->text().toFloat(&ok);
    if(ui->lineEdit->text().length() > 0 && ok)
    {
        valCrash=f;
        run();
    }
    else
    {
        valCrash=0;
        valueChanged=true;
        ui->lineEdit->setText("");
        valueChanged=false;
    }
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(valueChanged)
        return;
    bool ok;
    float f=ui->lineEdit_2->text().toFloat(&ok);
    if(ui->lineEdit_2->text().length() > 0 && ok)
    {
        valRepear=f;
        run();
    }
    else
    {
        valRepear=0;
        valueChanged=true;
        ui->lineEdit_2->setText("");
        valueChanged=false;
    }
}
