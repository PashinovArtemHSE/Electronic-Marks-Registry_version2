#include "marks.h"
#include "ui_marks.h"
#include <QDate>
#include <QHeaderView>
#include <QMessageBox>

Marks::Marks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Marks)
{
    ui->setupUi(this);

    QPropertyAnimation *animq = new QPropertyAnimation(this, "windowOpacity");
    animq->setDuration(500);
    animq->setStartValue(0.0);
    animq->setEndValue(1.0);
    animq->setEasingCurve(QEasingCurve::InOutQuad);
    animq->start();

    //Добавление групп
    foreach (const QString &group, Database::getGroups()) {
        ui->cb->addItem(group);
    }

    //Добавление предметов
    foreach (const QString &subject, Database::getSubjects()) {
        ui->Subjects->addItem(subject);
    }

    filltable();
    ui->tableWidget->setSortingEnabled(true);
    connect(ui->tableWidget->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &Marks::on_tableWidget_horizontalHeader_clicked);
}

Marks::~Marks()
{
    delete ui;
}

void Marks::on_tableWidget_horizontalHeader_clicked(int column)
{
    if (column == 0) {
        static bool ascending = true;
        ui->tableWidget->sortItems(column, ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
        ascending = !ascending;
    }
}

//Заполнение таблицы данными
void Marks::filltable()
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(13);

    QDate todayDate = QDate::currentDate();
    QStringList headers = {"ФИО","Группа"};
    for (int i = 0; i < 10; i++) {
        headers.append(todayDate.addDays(i).toString("dd.MM.yyyy"));
    }
    headers.append("Средний балл");
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QString selectedGroup = ui->cb->currentText();
    QString selectedSubject = ui->Subjects->currentText();

    foreach (const QString &studentName, Database::getAllStudents()) {
        QSqlQuery query;
        query.prepare("SELECT group_name FROM students WHERE name = :name");
        query.bindValue(":name", studentName);
        if (!query.exec() || !query.next()) continue;

        QString group = query.value(0).toString();
        if (group != selectedGroup) continue;

        int currentRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(currentRow);

        ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(studentName));
        ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(group));

        QMap<QDate, int> grades = Database::getGrades(studentName, selectedSubject);

        for (int i = 0; i < 10; i++) {
            QDate date = todayDate.addDays(i);
            if (grades.contains(date)) {
                ui->tableWidget->setItem(currentRow, i+2,
                    new QTableWidgetItem(QString::number(grades[date])));
            } else {
                ui->tableWidget->setItem(currentRow, i+2, new QTableWidgetItem(""));
            }
        }

        double average = Database::getAverageGrade(studentName, selectedSubject);
        ui->tableWidget->setItem(currentRow, 12,
            new QTableWidgetItem(QString::number(average, 'f', 2)));
    }

    ui->tableWidget->blockSignals(false);
}

//Рассчёт среднего арифметического
double Marks::calculateAverage(const QString &studentName, const QString &subject)
{
    return Database::getAverageGrade(studentName, subject);
}

//Обработка изменения ячейки
void Marks::on_tableWidget_cellChanged(int row, int column)
{
    if (column < 2 || column > 11) return; // Only process grade columns

    QString studentName = ui->tableWidget->item(row, 0)->text();
    QString subject = ui->Subjects->currentText();
    QDate date = QDate::currentDate().addDays(column-2);

    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if (!item) return;

    bool ok;
    int grade = item->text().toInt(&ok);
    if (!ok || grade < 0 || grade > 100) {
        QMessageBox::warning(this, "Ошибка", "Оценка должна быть числом от 0 до 100");
        return;
    }

    if (!Database::addGrade(studentName, subject, date, grade)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить оценку");
    }
}

void Marks::on_Subjects_currentTextChanged(const QString &arg1)
{
    filltable();
}

//Обработка изменения текста в окне
void Marks::on_cb_currentTextChanged()
{
    filltable();
}
