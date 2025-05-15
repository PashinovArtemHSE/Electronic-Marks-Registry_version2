#include "ratings.h"
#include "ui_ratings.h"
#include <QTableWidgetItem>
#include <QHeaderView>

Ratings::Ratings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ratings)
{
    ui->setupUi(this);
    this->setStyleSheet("QDialog { background-color: #f5f5f5; }");
    loadAndDisplayRatings();
}

Ratings::~Ratings()
{
    delete ui;
}

void Ratings::loadAndDisplayRatings()
{
    QMap<QString, double> studentAverages;
    //Загрузка информации об оценках студентов из БД
    foreach (const QString &studentName, Database::getAllStudents()) {
        double total = 0;
        int count = 0;

        foreach (const QString &subject, Database::getSubjects()) {
            double avg = Database::getAverageGrade(studentName, subject);
            if (avg > 0) {
                total += avg;
                count++;
            }
        }

        if (count > 0) {
            studentAverages[studentName] = total / count;
        }
    }

    //Сортировка студентов по среднему баллу
    QList<QPair<QString, double>> sortedStudents;
    for (auto it = studentAverages.begin(); it != studentAverages.end(); ++it) {
        sortedStudents.append(qMakePair(it.key(), it.value()));
    }

    std::sort(sortedStudents.begin(), sortedStudents.end(),
        [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
            return a.second > b.second;
        });

    ui->Rate->clear();
    ui->Rate->setRowCount(sortedStudents.size());
    ui->Rate->setColumnCount(2);

    ui->Rate->setStyleSheet(
        "QTableWidget {"
        " background-color: white;"
        " border: 1px solid #ddd;"
        " border-radius: 4px;"
        " gridline-color: #eee;"
        "}"
        "QTableWidget::item {"
        " padding: 5px;"
        "}"
        "QHeaderView::section {"
        " background-color: #4CAF50;"
        " color: white;"
        " padding: 5px;"
        " border: none;"
        " font-weight: bold;"
        "}"
        "QTableWidget::item:nth-child(even) {"
        " background-color: #f2f2f2;"
        "}"
    );

    QFont headerFont;
    headerFont.setBold(true);
    headerFont.setPointSize(10);
    ui->Rate->horizontalHeader()->setFont(headerFont);

    QFont cellFont;
    cellFont.setPointSize(9);
    ui->Rate->setFont(cellFont);

    //Настройка столбцов в таблице
    QStringList headers;
    headers << "Student" << "GPA";
    ui->Rate->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const auto &pair : sortedStudents) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(pair.first);
        QTableWidgetItem *gradeItem = new QTableWidgetItem(QString::number(pair.second, 'f', 2));

        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        gradeItem->setFlags(gradeItem->flags() & ~Qt::ItemIsEditable);

        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        gradeItem->setTextAlignment(Qt::AlignCenter);

        if (row % 2 == 0) {
            nameItem->setBackground(QColor(245, 245, 245));
            gradeItem->setBackground(QColor(245, 245, 245));
        }

        ui->Rate->setItem(row, 0, nameItem);
        ui->Rate->setItem(row, 1, gradeItem);
        row++;
    }

    ui->Rate->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->Rate->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->Rate->verticalHeader()->setDefaultSectionSize(25);
    ui->Rate->setSortingEnabled(true);

    connect(ui->Rate->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &Ratings::on_Rate_horizontalHeader_clicked);
}

//Обработка сортировки при клике на заголовок GPA или Student
void Ratings::on_Rate_horizontalHeader_clicked(int column)
{
    if (column == 1) {
        static bool ascending = true;
        ui->Rate->sortItems(column, ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
        ascending = !ascending;
    }
}
