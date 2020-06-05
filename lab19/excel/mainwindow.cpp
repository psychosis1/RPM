#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *grid = new QGridLayout();
    ui->widget->setLayout(grid);

    tabWidget = new QTabWidget(ui->widget);
    grid->addWidget(tabWidget);
    tabWidget->addTab(new QTableWidget,"List 1");

    QWidget *widget = tabWidget->currentWidget();
    table = qobject_cast<QTableWidget*>(widget);

    appearance();
    appearance2();

    mExcel = new QAxObject( "Excel.Application", this); // Подключение к Excel
    mExcel->setProperty("DisplayAlerts","0");
    workbooks = mExcel->querySubObject( "Workbooks" );
    workbook = workbooks->querySubObject("Add()");
    mSheets = workbook->querySubObject( "Sheets" ); // Все листы
    workSheet = workbook->querySubObject("ActiveSheet()");
    workSheet->setProperty("Name",tabWidget->tabBar()->tabText(tabWidget->currentIndex()));
    set.insert(tabWidget->tabBar()->tabText(tabWidget->currentIndex()));

    inputDialog.setWindowFlags(inputDialog.windowFlags() & (~Qt::WindowContextHelpButtonHint) & (~Qt::WindowCloseButtonHint));

    buttoms();
    buttoms2();
    key();
}

void MainWindow::appearance()
{
    ui->column->setIcon(QIcon(":/add/column.png"));
    ui->row->setIcon(QIcon(":/add/row.png"));
    ui->save->setIcon(QIcon(":/save/Save.png"));
    ui->saveAs->setIcon(QIcon(":/save/Save as.png"));
    ui->newTab->setIcon(QIcon(":/add/tab.png"));
}

void MainWindow::appearance2()
{
    ui->deleteColumn->setIcon(QIcon(":/remove/deleteColumn.png"));
    ui->deleteRow->setIcon(QIcon(":/remove/deleteRow.png"));
    this->setWindowIcon(QIcon(":/main/excel.png"));
    this->setWindowTitle("Excel Work");
    ui->question->setIcon(QIcon(":/main/question.png"));
    ui->deleteTab->setIcon(QIcon(":/remove/deleteTab.png"));
}

void MainWindow::saveAs()
{
    dir = QFileDialog::getSaveFileUrl(this,tr("Сохранение Excel файла"),tr(""),tr("Excel(*.xlsx)"));
    if(!dir.isEmpty())
        workbook->dynamicCall("SaveAs(const QString&,int)",dir,51);
}

void MainWindow::save()
{
    if(dir.isEmpty())
        saveAs();
    else
        workbook->dynamicCall("Save()");
}

void MainWindow::buttoms()
{
    connect(ui->saveAs,&QToolButton::clicked,this,&MainWindow::saveAs);
    connect(ui->save,&QToolButton::clicked,this,&MainWindow::save);
    connect(ui->row,&QToolButton::clicked,this,&MainWindow::addRow);
    connect(ui->deleteRow,&QToolButton::clicked,this,&MainWindow::removeRow);
    connect(ui->deleteColumn,&QToolButton::clicked,this,&MainWindow::removeColumn);
}

void MainWindow::buttoms2()
{
    connect(ui->column,&QToolButton::clicked,this,&MainWindow::addColumn);
    connect(table,&QTableWidget::itemChanged,this,&MainWindow::writeCell);
    connect(tabWidget,&QTabWidget::tabBarDoubleClicked,this,&MainWindow::renameTab);
    connect(ui->newTab,&QToolButton::clicked,this,&MainWindow::addTab);
    connect(tabWidget,&QTabWidget::currentChanged,this,&MainWindow::changeTable);
    connect(ui->question,&QToolButton::clicked,this,&MainWindow::questions);
    connect(ui->deleteTab,&QToolButton::clicked,this,&MainWindow::removeTab);
}

void MainWindow::changeTable()
{
    QWidget *widget = tabWidget->currentWidget();
    table = qobject_cast<QTableWidget*>(widget);
    connect(table,&QTableWidget::itemChanged,this,&MainWindow::writeCell);
    qInfo()<<tabWidget->tabBar()->tabText(tabWidget->currentIndex());
}

void MainWindow::addToExcel(QString list, int row, int column, QTableWidgetItem* value)
{
    QAxObject *StatSheet = mSheets->querySubObject("Item(QString)",list);//текущий лист
    StatSheet->dynamicCall("Select()");
    QAxObject *cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", row, column);
    cell->setProperty("Value",value->text());
    delete cell;
    delete StatSheet;
}

void MainWindow::addRow()
{
    bool ok;
    int count = table->rowCount()+1;
    QString nameRow = inputDialog.getText(this,tr("Строка"),tr("Ввведите название строки:"), QLineEdit::Normal,QString::number(count), &ok,inputDialog.windowFlags());
    if (ok && !nameRow.isEmpty()){
        table->insertRow(table->rowCount());
        table->setVerticalHeaderItem(count-1,new QTableWidgetItem(nameRow));
        addToExcel(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),count+1,1,table->verticalHeaderItem(count-1));
    }
}

void MainWindow::removeFromExcel(QString list,char *comand, int number)
{
    QAxObject *StatSheet = mSheets->querySubObject("Item(QString)",list);
    QAxObject *range = StatSheet->querySubObject(comand);
    StatSheet->dynamicCall("Select()");
    QAxObject *rowOrColumn = range->querySubObject("Item(int)",number);
    rowOrColumn->dynamicCall("Delete");
    delete rowOrColumn;
    delete range;
    delete StatSheet;
}

void MainWindow::removeRow()
{
    table->removeRow(table->rowCount()-1);
    removeFromExcel(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),QString("Rows()").toLatin1().data(),table->rowCount()+2);
}

void MainWindow::removeColumn()
{
    table->removeColumn(table->columnCount()-1);
    removeFromExcel(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),QString("Columns()").toLatin1().data(),table->columnCount()+2);
}

void MainWindow::addColumn()
{
    bool ok;
    int count = table->columnCount()+1;
    QString nameColumn = inputDialog.getText(this,tr("Столбец"),tr("Ввведите название столбца:"), QLineEdit::Normal,QString::number(count), &ok,inputDialog.windowFlags());
    if (ok && !nameColumn.isEmpty()){
        table->insertColumn(table->columnCount());
        table->setHorizontalHeaderItem(count-1,new QTableWidgetItem(nameColumn));
        addToExcel(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),1,count+1,table->horizontalHeaderItem(count-1));
    }
}

void MainWindow::writeCell()
{
    addToExcel(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),table->currentRow()+2,table->currentColumn()+2,table->currentItem());
}

void MainWindow::deleteItems()
{
    QList<QTableWidgetItem *> list = table->selectedItems();
    foreach(QTableWidgetItem *item,list){
        removeFromExcelCells(tabWidget->tabBar()->tabText(tabWidget->currentIndex()),item->row()+2,item->column()+2);
        table->takeItem(item->row(),item->column());
    }
}

void MainWindow::removeFromExcelCells(QString list,int row, int column)
{
    QAxObject *StatSheet = mSheets->querySubObject("Item(QString)",list);
    StatSheet->dynamicCall("Select()");
    QAxObject *cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", row, column);
    cell->dynamicCall("Clear");
    delete cell;
    delete StatSheet;
}

void MainWindow::key()
{
    del = new QShortcut(this);
    del->setKey(Qt::Key_Delete);
    connect(del,&QShortcut::activated,this,&MainWindow::deleteItems);
}
void MainWindow::addTab()
{
    bool ok;
    QString nameTab = inputDialog.getText(this,tr("Лист"),tr("Ввведите название листа:"), QLineEdit::Normal,tabWidget->tabBar()->tabText(tabWidget->currentIndex()), &ok,inputDialog.windowFlags());
    if(set.end()!=set.find(nameTab))
        QMessageBox::critical(0, "Ошибка", "Данное имя уже используется");
    else if (ok && !nameTab.isEmpty()){
        tabWidget->setCurrentIndex(tabWidget->addTab(new QTableWidget,nameTab));
        workSheet = mSheets->querySubObject("Add()");
        workSheet->setProperty("Name",nameTab);
        set.insert(nameTab);
    }
}

void MainWindow::removeTab()
{
    int currentIndexDeleted;//index
    if(tabWidget->count()!=1){
        currentIndexDeleted=tabWidget->currentIndex(); // удаляемый индекс
        workSheet = mSheets->querySubObject("Item(QString)",tabWidget->tabBar()->tabText(currentIndexDeleted));
        workSheet->dynamicCall("Delete"); //удалили из Excel
        set.remove(tabWidget->tabBar()->tabText(currentIndexDeleted));
        tabWidget->removeTab(currentIndexDeleted);
        workSheet = mSheets->querySubObject("Item(QString)",tabWidget->tabBar()->tabText(tabWidget->currentIndex()));
    }
}

void MainWindow::renameTab()
{
    bool ok;
    QString nameTab = inputDialog.getText(this,tr("Лист"),tr("Ввведите название листа:"), QLineEdit::Normal,tabWidget->tabBar()->tabText(tabWidget->currentIndex()), &ok,inputDialog.windowFlags());
    if(set.end()!=set.find(nameTab) )
        QMessageBox::critical(0, "Ошибка", "Данное имя уже используется");
    else if (ok && !nameTab.isEmpty()){
            set.remove(tabWidget->tabBar()->tabText(tabWidget->currentIndex()));
            workSheet = mSheets->querySubObject("Item(QString)",tabWidget->tabBar()->tabText(tabWidget->currentIndex()));
            tabWidget->tabBar()->setTabText(tabWidget->currentIndex(),nameTab);
            set.insert(nameTab);
            workSheet->setProperty("Name",nameTab);
    }
}

void MainWindow::questions()
{
    bool ok;
    QMessageBox::information(0,"Полезная информация","-для удаления ячеек выделите необходимые и нажмите кнопку DEL\n-для переименования листа нажмите на необходимый лист дважды\n-если непонятно назначение кнопки, просто наведите на нее мышью",
                             ok);
}

MainWindow::~MainWindow()
{
    workbook->dynamicCall("Close()"); // Закрываем книгу
    mExcel->dynamicCall("Quit()"); // Закрываем Excel
    delete workSheet;
    delete mSheets;
    delete workbook;
    delete workbooks;
    delete mExcel;
    delete ui;
}

