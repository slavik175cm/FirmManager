#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QDate>
class Task
{
public:
    Task(QString project_name, QString task_name, QString worker, QString manager, QDate start, QDate dead_line, QDate end);
    QString project_name, task_name, worker, manager;
    QDate start, dead_line, end;
};

#endif // TASK_H
