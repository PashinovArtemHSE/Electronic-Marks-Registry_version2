#ifndef ADDSTUDENT_H
#define ADDSTUDENT_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddStudent;
}

class AddStudent : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudent(QWidget *parent = nullptr);
    ~AddStudent();

private slots:
    void on_addst_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::AddStudent *ui;
};

#endif // ADDSTUDENT_H
