#ifndef REMOVESTUDENT_H
#define REMOVESTUDENT_H

#include <QDialog>
#include "database.h"

namespace Ui {
class RemoveStudent;
}

class RemoveStudent : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveStudent(QWidget *parent = nullptr);
    ~RemoveStudent();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_removestudent_clicked();

private:
    Ui::RemoveStudent *ui;
};

#endif // REMOVESTUDENT_H
