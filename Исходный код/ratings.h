#ifndef RATINGS_H
#define RATINGS_H

#include <QDialog>
#include "database.h"

namespace Ui {
class Ratings;
}

class Ratings : public QDialog
{
    Q_OBJECT

public:
    explicit Ratings(QWidget *parent = nullptr);
    ~Ratings();

private slots:
    void on_Rate_horizontalHeader_clicked(int column);

private:
    Ui::Ratings *ui;
    void loadAndDisplayRatings();
};

#endif // RATINGS_H
