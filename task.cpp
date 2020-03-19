#include "task.h"
#include <QDebug>
Task::Task(QString project_name, QString task_name, QString worker, QString manager,
           QDate start, QDate dead_line, QDate end)
{
    this->project_name = project_name;
    this->task_name = task_name;
    this->worker = worker;
    this->manager = manager;
    this->start = start;
    this->dead_line = dead_line;
    this->end = end;
}
