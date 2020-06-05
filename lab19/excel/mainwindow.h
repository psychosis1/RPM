#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <ActiveQt\QAxObject>
#include <QDir>
#include<QDebug>
#include<QTableWidgetItem>
#include<QInputDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QShortcut>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief buttoms Коннекты для кнопок
     */
    void buttoms();
    /**
     * @brief buttoms2 Вторая часть коннектов для кнопок
     */
    void buttoms2();
    /**
     * @brief addToExcel Добавлени значения в ячейку Excel
     * @param list Текущий лист
     * @param row Строка
     * @param column Столбец
     * @param value Значение
     */
    void addToExcel(QString list,int row,int column,QTableWidgetItem* value);
    /**
     * @brief removeFromExcel Удаление строки или столбца в Excel
     * @param list Текущий лист
     * @param comand Комманда(строка или столбец)
     * @param number Номер строки или столбца
     */
    void removeFromExcel(QString list,char *comand,int number);
    /**
     * @brief removeFromExcelCells Удаление ячейки из Excel
     * @param list Текущий лист
     * @param row Строка
     * @param column Столбец
     */
    void removeFromExcelCells(QString list,int row,int column);
    /**
     * @brief appearance Icon для кнопок
     */
    void appearance();
    /**
     * @brief appearance2 Icon для кнопок, главного окна, заголовок главного окна
     */
    void appearance2();
    /**
     * @brief key Кнопка удаления
     */
    void key();
public slots:
    /**
     * @brief saveAs Сохранить как файл
     */
    void saveAs();
    /**
     * @brief save Сохранить файл
     */
    void save();
    /**
     * @brief addRow Добавить строку
     */
    void addRow();
    /**
     * @brief removeRow Удалить строку
     */
    void removeRow();
    /**
     * @brief removeColumn Удалить столбец
     */
    void removeColumn();
    /**
     * @brief addColumn Добавить столбец
     */
    void addColumn();
    /**
     * @brief writeCell Записать в ячейку
     */
    void writeCell();
    /**
     * @brief renameTab Переименовать лист
     */
    void renameTab();
    /**
     * @brief addTab Добавить лист
     */
    void addTab();
    /**
     * @brief removeTab Удалить лист
     */
    void removeTab();
    /**
     * @brief changeTable Смена таблицы
     */
    void changeTable();
    /**
     * @brief deleteItems Удалить выбранные ячейки
     */
    void deleteItems();
    /**
     * @brief questions Полезная информация
     */
    void questions();

private:
    Ui::MainWindow *ui;

    QUrl dir;
    QShortcut *del;

    QInputDialog inputDialog;
    QTableWidget *table;
    QTabWidget *tabWidget;

    QAxObject *mExcel;
    QAxObject *workbooks;
    QAxObject *workbook;
    QAxObject *mSheets;
    QAxObject *workSheet;
    QSet<QString> set;
};
#endif // MAINWINDOW_H
