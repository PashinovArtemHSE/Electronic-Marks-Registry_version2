#include "homework.h"
#include "ui_homework.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

Homework::Homework(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Homework),
    isUpdating(false)
{
    ui->setupUi(this);
    setupUI();
    loadHomework();
}

Homework::~Homework()
{
    delete ui;
}

void Homework::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setDisplayFormat("dd.MM.yyyy");
    dateEdit->setCalendarPopup(true);

    QPushButton *addButton = new QPushButton("Add Date", this);
    QPushButton *removeButton = new QPushButton("Remove Date", this);

    buttonLayout->addWidget(dateEdit);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(ui->hw);

    connect(addButton, &QPushButton::clicked, this, &Homework::onAddDateClicked);
    connect(removeButton, &QPushButton::clicked, this, &Homework::onRemoveDateClicked);
    connect(ui->hw, &QTableWidget::cellChanged, this, &Homework::onCellChanged);

    ui->hw->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->hw->verticalHeader()->setVisible(false);
}

//Добавление существующего дз из бд
void Homework::loadHomework()
{
    isUpdating = true;

    QStringList subjects = Database::getSubjects();
    ui->hw->setColumnCount(subjects.size());
    ui->hw->setHorizontalHeaderLabels(subjects);

    updateHomeworkTable();

    isUpdating = false;
}

void Homework::updateHomeworkTable()
{
    ui->hw->setRowCount(0);

    QSet<QDate> allDates;
    foreach (const QString &subject, Database::getSubjects()) {
        QMap<QDate, QString> hw = Database::getHomework(subject);
        for (auto it = hw.begin(); it != hw.end(); ++it) {
            allDates.insert(it.key());
        }
    }

    QList<QDate> sortedDates = allDates.values();
    std::sort(sortedDates.begin(), sortedDates.end());

    foreach (const QDate &date, sortedDates) {
        addNewDateRow(date);
    }
}

//Добавление поля новой даты для ДЗ
void Homework::addNewDateRow(const QDate &date)
{
    for (int row = 0; row < ui->hw->rowCount(); ++row) {
        QTableWidgetItem *dateItem = ui->hw->item(row, 0);
        if (dateItem && QDate::fromString(dateItem->text(), "dd.MM.yyyy") == date) {
            return;
        }
    }

    int row = ui->hw->rowCount();
    ui->hw->insertRow(row);

    QTableWidgetItem *dateItem = new QTableWidgetItem(date.toString("dd.MM.yyyy"));
    dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
    ui->hw->setItem(row, 0, dateItem);

    for (int col = 1; col < ui->hw->columnCount(); ++col) {
        QString subject = ui->hw->horizontalHeaderItem(col)->text();
        QMap<QDate, QString> hw = Database::getHomework(subject);

        if (hw.contains(date)) {
            ui->hw->setItem(row, col, new QTableWidgetItem(hw[date]));
        } else {
            ui->hw->setItem(row, col, new QTableWidgetItem(""));
        }
    }
}

//Обработка изменения данных в ячейке
void Homework::onCellChanged(int row, int column)
{
    if (isUpdating || column == 0) return;

    QDate date = QDate::fromString(ui->hw->item(row, 0)->text(), "dd.MM.yyyy");
    QString subject = ui->hw->horizontalHeaderItem(column)->text();
    QString description = ui->hw->item(row, column)->text();

    if (!Database::updateHomework(subject, date, description)) {
        QMessageBox::warning(this, "Error", "Failed to save homework");
    }
}

//Обработка нажатия кнопки add row для добавления поля новой даты для ДЗ
void Homework::onAddDateClicked()
{
    addNewDateRow(dateEdit->date());
}

//Удаление ДЗ на всей выбранной дате
void Homework::onRemoveDateClicked()
{
    int row = ui->hw->currentRow();
    if (row >= 0) {
        QDate date = QDate::fromString(ui->hw->item(row, 0)->text(), "dd.MM.yyyy");

        foreach (const QString &subject, Database::getSubjects()) {
            Database::updateHomework(subject, date, "");
        }

        ui->hw->removeRow(row);
    }
}
