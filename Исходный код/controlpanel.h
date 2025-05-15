#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include "addstudent.h"
#include "removestudent.h"
#include "marks.h"
#include "ratings.h"
#include "homework.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private slots:
    void on_addstudent_clicked();
    void on_removestudent_clicked();
    void on_selectsubject_clicked();
    void on_ratings_clicked();
    void on_add_homework_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::ControlPanel *ui;
    AddStudent *add_student;
    Marks *marks;
    RemoveStudent *rs;
    Ratings *rt;
    Homework *homework;

    void setupButtonAnimations();
    void updateBackground();

    QTimer *bgTimer;
    float bgOffset = 0;
    QColor bgColor1 = QColor(70, 130, 180);
    QColor bgColor2 = QColor(50, 205, 50);
};

#endif // CONTROLPANEL_H
