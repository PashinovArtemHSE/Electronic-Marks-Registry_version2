#include "database.h"

QSqlDatabase Database::db = QSqlDatabase::addDatabase("QSQLITE");

Database::Database(QObject *parent) : QObject(parent)
{
}

Database::~Database()
{
    if(db.isOpen()) {
        db.close();
    }
}

bool Database::initializeDatabase()
{
    db.setDatabaseName("university.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    QSqlQuery query;

    //Создаём таблицу со студентами
    query.exec("CREATE TABLE IF NOT EXISTS students ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL UNIQUE, "
               "password TEXT NOT NULL, "
               "group_name TEXT NOT NULL)");

    //Создаём таблицу с преподавателями
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT NOT NULL UNIQUE, "
               "password TEXT NOT NULL)");

    // Создаём таблицу с предметами
    query.exec("CREATE TABLE IF NOT EXISTS subjects ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL UNIQUE)");

    // Создаём таблицу с оценками
    query.exec("CREATE TABLE IF NOT EXISTS grades ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER NOT NULL, "
               "subject_id INTEGER NOT NULL, "
               "date TEXT NOT NULL, "
               "grade INTEGER NOT NULL, "
               "FOREIGN KEY(student_id) REFERENCES students(id), "
               "FOREIGN KEY(subject_id) REFERENCES subjects(id))");

    //  Создаём таблицу с ДЗ
    query.exec("CREATE TABLE IF NOT EXISTS homework ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "subject_id INTEGER NOT NULL, "
               "date TEXT NOT NULL, "
               "description TEXT NOT NULL, "
               "FOREIGN KEY(subject_id) REFERENCES subjects(id))");

    QStringList defaultSubjects = {
        "Lineal Algebra", "Calculus", "Computer Science (CS)",
        "Math Statistics", "Geometric analysis", "Data Bases",
        "Machine Learning", "English"
    };

    foreach (const QString &subject, defaultSubjects) {
        query.prepare("INSERT OR IGNORE INTO subjects (name) VALUES (:name)");
        query.bindValue(":name", subject);
        query.exec();
    }

    return true;
}

QSqlDatabase Database::getDatabase()
{
    return db;
}

bool Database::addStudent(const QString &name, const QString &password, const QString &group)
{
    QSqlQuery query;
    query.prepare("INSERT INTO students (name, password, group_name) VALUES (:name, :password, :group)");
    query.bindValue(":name", name);
    query.bindValue(":password", password);
    query.bindValue(":group", group);

    return query.exec();
}

//Обработка удаления студента из БД
bool Database::removeStudent(const QString &name)
{
    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE name = :name");
    query.bindValue(":name", name);

    return query.exec();
}

//Обработка проверки существования студента в БД
bool Database::studentExists(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM students WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

QList<QString> Database::getAllStudents()
{
    QList<QString> students;
    QSqlQuery query("SELECT name FROM students");

    while (query.next()) {
        students.append(query.value(0).toString());
    }

    return students;
}

QList<QString> Database::getGroups()
{
    QList<QString> groups;
    QSqlQuery query("SELECT DISTINCT group_name FROM students");

    while (query.next()) {
        groups.append(query.value(0).toString());
    }

    return groups;
}

bool Database::addUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    return query.exec();
}

bool Database::userExists(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool Database::validateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

QList<QString> Database::getSubjects()
{
    QList<QString> subjects;
    QSqlQuery query("SELECT name FROM subjects");

    while (query.next()) {
        subjects.append(query.value(0).toString());
    }

    return subjects;
}

bool Database::addGrade(const QString &studentName, const QString &subject, const QDate &date, int grade)
{
    QSqlQuery query;

    query.prepare("SELECT id FROM students WHERE name = :name");
    query.bindValue(":name", studentName);
    if (!query.exec() || !query.next()) {
        return false;
    }
    int studentId = query.value(0).toInt();

    query.prepare("SELECT id FROM subjects WHERE name = :name");
    query.bindValue(":name", subject);
    if (!query.exec() || !query.next()) {
        return false;
    }
    int subjectId = query.value(0).toInt();

    query.prepare("INSERT INTO grades (student_id, subject_id, date, grade) "
                  "VALUES (:student_id, :subject_id, :date, :grade)");
    query.bindValue(":student_id", studentId);
    query.bindValue(":subject_id", subjectId);
    query.bindValue(":date", date.toString(Qt::ISODate));
    query.bindValue(":grade", grade);

    return query.exec();
}

QMap<QDate, int> Database::getGrades(const QString &studentName, const QString &subject)
{
    QMap<QDate, int> grades;
    QSqlQuery query;

    query.prepare("SELECT g.date, g.grade FROM grades g "
                  "JOIN students s ON g.student_id = s.id "
                  "JOIN subjects sub ON g.subject_id = sub.id "
                  "WHERE s.name = :student_name AND sub.name = :subject_name "
                  "ORDER BY g.date");
    query.bindValue(":student_name", studentName);
    query.bindValue(":subject_name", subject);

    if (query.exec()) {
        while (query.next()) {
            QDate date = QDate::fromString(query.value(0).toString(), Qt::ISODate);
            int grade = query.value(1).toInt();
            grades[date] = grade;
        }
    }

    return grades;
}

double Database::getAverageGrade(const QString &studentName, const QString &subject)
{
    QSqlQuery query;

    query.prepare("SELECT AVG(g.grade) FROM grades g "
                  "JOIN students s ON g.student_id = s.id "
                  "JOIN subjects sub ON g.subject_id = sub.id "
                  "WHERE s.name = :student_name AND sub.name = :subject_name");
    query.bindValue(":student_name", studentName);
    query.bindValue(":subject_name", subject);

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }

    return 0.0;
}

bool Database::addHomework(const QString &subject, const QDate &date, const QString &description)
{
    QSqlQuery query;

    query.prepare("SELECT id FROM subjects WHERE name = :name");
    query.bindValue(":name", subject);
    if (!query.exec() || !query.next()) {
        return false;
    }
    int subjectId = query.value(0).toInt();

    query.prepare("INSERT OR REPLACE INTO homework (subject_id, date, description) "
                  "VALUES (:subject_id, :date, :description)");
    query.bindValue(":subject_id", subjectId);
    query.bindValue(":date", date.toString(Qt::ISODate));
    query.bindValue(":description", description);

    return query.exec();
}

QMap<QDate, QString> Database::getHomework(const QString &subject)
{
    QMap<QDate, QString> homework;
    QSqlQuery query;

    query.prepare("SELECT h.date, h.description FROM homework h "
                  "JOIN subjects s ON h.subject_id = s.id "
                  "WHERE s.name = :subject_name "
                  "ORDER BY h.date");
    query.bindValue(":subject_name", subject);

    if (query.exec()) {
        while (query.next()) {
            QDate date = QDate::fromString(query.value(0).toString(), Qt::ISODate);
            QString description = query.value(1).toString();
            homework[date] = description;
        }
    }

    return homework;
}

bool Database::updateHomework(const QString &subject, const QDate &date, const QString &description)
{
    return addHomework(subject, date, description);
}
