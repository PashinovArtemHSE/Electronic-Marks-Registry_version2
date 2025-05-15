#include "homeworkstudent.h"
#include "ui_homeworkstudent.h"
#include <QHeaderView>

HomeWorkStudent::HomeWorkStudent(const QString &studentName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeWorkStudent)
{
    ui->setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT group_name FROM students WHERE name = :name");
    query.bindValue(":name", studentName);
    if (query.exec() && query.next()) {
        studentGroup = query.value(0).toString();
    }

    setWindowTitle("Homework for " + studentName + " (Group: " + studentGroup + ")");
    setupTable();
    loadHomework();
}

HomeWorkStudent::~HomeWorkStudent()
{
    delete ui;
}

//Создание таблицы в окне домашнего задания для студента
void HomeWorkStudent::setupTable()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Set columns: Date + Subjects
    QStringList subjects = Database::getSubjects();
    ui->tableWidget->setColumnCount(subjects.size() + 1);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Date"));

    for (int i = 0; i < subjects.size(); ++i) {
        ui->tableWidget->setHorizontalHeaderItem(i+1, new QTableWidgetItem(subjects[i]));
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

//Загрузка ДЗ
void HomeWorkStudent::loadHomework()
{
    ui->tableWidget->setRowCount(0);

    QSet<QDate> allDates;
    foreach (const QString &subject, Database::getSubjects()) {
        QMap<QDate, QString> hw = Database::getHomework(subject);
        allDates.unite(QSet<QDate>::fromList(hw.keys()));
    }

    foreach (const QDate &date, allDates) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Add date in first column
        QTableWidgetItem *dateItem = new QTableWidgetItem(date.toString("dd.MM.yyyy"));
        ui->tableWidget->setItem(row, 0, dateItem);

        // Add homework for each subject
        for (int col = 1; col < ui->tableWidget->columnCount(); ++col) {
            QString subject = ui->tableWidget->horizontalHeaderItem(col)->text();
            QMap<QDate, QString> hw = Database::getHomework(subject);

            if (hw.contains(date)) {
                ui->tableWidget->setItem(row, col, new QTableWidgetItem(hw[date]));
            }
        }
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}
