#ifndef USERPANEL_H
#define USERPANEL_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include "database.h"
#include "homeworkstudent.h"

class UserPanel : public QDialog
{
    Q_OBJECT

public:
    explicit UserPanel(const QString &studentName, QWidget *parent = nullptr);
    ~UserPanel();

private slots:
    void showSubjectDetails(int row, int column);
    void showHomework();

private:
    QTableWidget *gradesTable;
    QPushButton *homeworkButton;
    HomeWorkStudent *hws;
    QString studentName;

    void setupUI();
    void loadStudentGrades();
};

#endif // USERPANEL_H
