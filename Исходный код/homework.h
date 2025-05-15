#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QDialog>
#include <QDateEdit>
#include "database.h"

namespace Ui {
class Homework;
}

class Homework : public QDialog
{
    Q_OBJECT

public:
    explicit Homework(QWidget *parent = nullptr);
    ~Homework();

private slots:
    void onCellChanged(int row, int column);
    void onAddDateClicked();
    void onRemoveDateClicked();

private:
    Ui::Homework *ui;
    QDateEdit *dateEdit;
    bool isUpdating;

    void loadHomework();
    void updateHomeworkTable();
    void addNewDateRow(const QDate &date);
    void setupUI();
};

#endif // HOMEWORK_H
