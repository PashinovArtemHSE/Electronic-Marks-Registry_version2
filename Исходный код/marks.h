#ifndef MARKS_H
#define MARKS_H

#include <QDialog>
#include "database.h"

namespace Ui {
class Marks;
}

class Marks : public QDialog
{
    Q_OBJECT

public:
    explicit Marks(QWidget *parent = nullptr);
    ~Marks();

private slots:
    void on_tableWidget_cellChanged(int row, int column);
    void on_Subjects_currentTextChanged(const QString &arg1);
    void on_cb_currentTextChanged();
    void on_tableWidget_horizontalHeader_clicked(int column);

private:
    Ui::Marks *ui;
    void filltable();
    double calculateAverage(const QString &studentName, const QString &subject);
};

#endif // MARKS_H
