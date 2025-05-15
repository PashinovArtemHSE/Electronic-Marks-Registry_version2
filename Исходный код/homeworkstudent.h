#ifndef HOMEWORKSTUDENT_H
#define HOMEWORKSTUDENT_H

#include <QDialog>
#include "database.h"

namespace Ui {
class HomeWorkStudent;
}

class HomeWorkStudent : public QDialog
{
    Q_OBJECT

public:
    explicit HomeWorkStudent(const QString &studentName, QWidget *parent = nullptr);
    ~HomeWorkStudent();

private:
    Ui::HomeWorkStudent *ui;
    QString studentGroup;

    void loadHomework();
    void setupTable();
};

#endif // HOMEWORKSTUDENT_H
