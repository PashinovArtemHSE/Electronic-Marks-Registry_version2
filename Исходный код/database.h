#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    static bool initializeDatabase();
    static QSqlDatabase getDatabase();

    // Student operations
    static bool addStudent(const QString &name, const QString &password, const QString &group);
    static bool removeStudent(const QString &name);
    static bool studentExists(const QString &name);
    static QList<QString> getAllStudents();
    static QList<QString> getGroups();

    // User operations
    static bool addUser(const QString &username, const QString &password);
    static bool userExists(const QString &username);
    static bool validateUser(const QString &username, const QString &password);

    // Subject operations
    static QList<QString> getSubjects();
    static bool addGrade(const QString &studentName, const QString &subject, const QDate &date, int grade);
    static QMap<QDate, int> getGrades(const QString &studentName, const QString &subject);
    static double getAverageGrade(const QString &studentName, const QString &subject);

    // Homework operations
    static bool addHomework(const QString &subject, const QDate &date, const QString &description);
    static QMap<QDate, QString> getHomework(const QString &subject);
    static bool updateHomework(const QString &subject, const QDate &date, const QString &description);

private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
