#include "userpanel.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

UserPanel::UserPanel(const QString &studentName, QWidget *parent) :
    QDialog(parent),
    studentName(studentName),
    hws(nullptr),
    gradesTable(new QTableWidget(this)),
    homeworkButton(new QPushButton("View Homework", this))
{
    if (!gradesTable || !homeworkButton) {
        qCritical() << "Failed to create widgets";
        return;
    }

    setupUI();
    loadStudentGrades();
}

UserPanel::~UserPanel()
{
    delete hws;
}

void UserPanel::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    if (!mainLayout) {
        qCritical() << "Failed to create main layout";
        return;
    }

    gradesTable->setEditTriggers(QTableWidget::NoEditTriggers);
    gradesTable->setSelectionBehavior(QTableWidget::SelectRows);
    gradesTable->verticalHeader()->setVisible(false);
    gradesTable->setAlternatingRowColors(true);

    homeworkButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #007bff;"
        "  color: white;"
        "  border: none;"
        "  padding: 8px 16px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0069d9;"
        "}"
    );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(homeworkButton);

    mainLayout->addWidget(gradesTable);
    mainLayout->addLayout(buttonLayout);

    connect(gradesTable, &QTableWidget::cellDoubleClicked, this, &UserPanel::showSubjectDetails);
    connect(homeworkButton, &QPushButton::clicked, this, &UserPanel::showHomework);

    setWindowTitle("Gradebook - " + studentName);
    resize(800, 600);
}

void UserPanel::loadStudentGrades()
{
    if (!gradesTable) return;

    gradesTable->clear();
    gradesTable->setRowCount(0);
    gradesTable->setColumnCount(2);
    gradesTable->setHorizontalHeaderLabels({"Subject", "Average Grade"});

    try {
        QStringList subjects = Database::getSubjects();
        foreach (const QString &subject, subjects) {
            int row = gradesTable->rowCount();
            gradesTable->insertRow(row);

            QTableWidgetItem *subjectItem = new QTableWidgetItem(subject);
            subjectItem->setData(Qt::UserRole, subject);
            gradesTable->setItem(row, 0, subjectItem);

            double average = Database::getAverageGrade(studentName, subject);
            QTableWidgetItem *avgItem = new QTableWidgetItem(QString::number(average, 'f', 2));
            avgItem->setTextAlignment(Qt::AlignCenter);

            QColor bgColor;
            if (average >= 85) bgColor = QColor(200, 230, 200);
            else if (average >= 60) bgColor = QColor(255, 255, 200);
            else bgColor = QColor(255, 200, 200);

            avgItem->setBackground(bgColor);
            gradesTable->setItem(row, 1, avgItem);
        }

        gradesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        gradesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        gradesTable->resizeRowsToContents();
    } catch (const std::exception &e) {
        qCritical() << "Error loading grades:" << e.what();
    }
}

void UserPanel::showSubjectDetails(int row, int column)
{
    Q_UNUSED(column);

    if (!gradesTable || row < 0 || row >= gradesTable->rowCount()) return;

    QString subject = gradesTable->item(row, 0)->data(Qt::UserRole).toString();
    QMap<QDate, int> grades = Database::getGrades(studentName, subject);

    QDialog detailsDialog(this);
    detailsDialog.setWindowTitle("Grades for " + subject);
    detailsDialog.resize(400, 300);

    QTableWidget detailsTable(&detailsDialog);
    detailsTable.setColumnCount(2);
    detailsTable.setHorizontalHeaderLabels({"Date", "Grade"});
    detailsTable.verticalHeader()->setVisible(false);
    detailsTable.setEditTriggers(QTableWidget::NoEditTriggers);

    detailsTable.setRowCount(grades.size());
    int currentRow = 0;
    for (auto it = grades.begin(); it != grades.end(); ++it) {
        detailsTable.setItem(currentRow, 0, new QTableWidgetItem(it.key().toString("dd.MM.yyyy")));
        QTableWidgetItem *gradeItem = new QTableWidgetItem(QString::number(it.value()));
        gradeItem->setTextAlignment(Qt::AlignCenter);
        detailsTable.setItem(currentRow, 1, gradeItem);
        currentRow++;
    }

    detailsTable.resizeColumnsToContents();
    detailsTable.horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout layout(&detailsDialog);
    layout.addWidget(&detailsTable);
    detailsDialog.setLayout(&layout);

    detailsDialog.exec();
}

void UserPanel::showHomework()
{
    if (hws) {
        hws->deleteLater();
    }
    hws = new HomeWorkStudent(studentName);
    hws->setAttribute(Qt::WA_DeleteOnClose);
    hws->exec();
}
