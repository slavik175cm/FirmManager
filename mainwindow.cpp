#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Все проекты");
    ui->comboBox->addItem("Проекты руководителя");
    ui->comboBox->addItem("Задачи сотрудника");
    ui->comboBox->addItem("Сотрудники проекта");
    ui->comboBox->addItem("Не успели сдать вовремя");

    QFont a;
    a.setWeight(100);
    ui->start->setFont(a);

    ui->comboBox_2->hide();
    file.setFileName("d:\\qt_projects\\FirmManager\\file.txt");
    file.open(QIODevice::ReadWrite);

    init();
    sort();
    showall();
}

void MainWindow::init() {
    tasks.resize(0);
    while (!file.atEnd()) {
        QStringList list = QString(file.readLine()).split(",");
        if (list.size() == 1) continue;
        Task *task = new Task(list[0], list[1], list[2], list[3], transform(list[4]), transform(list[5]), transform(list[6]));
        tasks.push_back(task);
    }
}

void MainWindow::showall() {
    ui->textBrowser->clear();
    for (int i = 0; i < tasks.size(); i++) {
        ui->textBrowser->append("Проект: " + tasks[i]->project_name);
        ui->textBrowser->append("Задание: " + tasks[i]->task_name);
        ui->textBrowser->append("ФИО исполнителя: " + tasks[i]->worker);
        ui->textBrowser->append("ФИО руководителя: " + tasks[i]->manager);
        ui->textBrowser->append("Дата выдачи задания: " + tasks[i]->start.toString());
        ui->textBrowser->append("Срок выполнения: " + tasks[i]->dead_line.toString());
        ui->textBrowser->append("Дата сдачи: " + tasks[i]->end.toString() + '\n');
    }
    ui->textBrowser->moveCursor(QTextCursor::Start);
}

void MainWindow::swap(Task *&a, Task *&b) {
    Task *c = a;
    a = b;
    b = c;
}

void MainWindow::sort() {
    for (int i = 0; i < tasks.size(); i++)
        for (int j = 0; j < tasks.size() - 1; j++)
            if (tasks[j]->start > tasks[j + 1]->start) {
                swap(tasks[j], tasks[j + 1]);
            }
}

QDate MainWindow::transform(QString s) {
    QStringList list = s.split('.');
    return QDate(list[2].toInt(), list[1].toInt(), list[0].toInt());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if (arg1 == "Все проекты") {
        typ = 0;
        ui->comboBox_2->hide();
        showall();
        return;
    }
    QMap<QString, bool> mem; mem.clear();

    if (arg1 == "Не успели сдать вовремя") {
        typ = 4;
        ui->comboBox_2->hide();
        ui->textBrowser->clear();
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i]->dead_line < tasks[i]->end && !mem[tasks[i]->worker]) {
                QString s = QString::number(tasks[i]->dead_line.daysTo(tasks[i]->end));
                ui->textBrowser->append(tasks[i]->worker + " (" + s + " дней)");
                mem[tasks[i]->worker] = 1;
            }
        }
        return;
    }

    ui->comboBox_2->clear();
    ui->comboBox_2->show();

    if (arg1 == "Проекты руководителя") {
        typ = 1;
        for (int i = 0; i < tasks.size(); i++) {
            if (!mem[tasks[i]->manager]) {
                ui->comboBox_2->addItem(tasks[i]->manager);
                mem[tasks[i]->manager] = 1;
            }
        }
    }

    if (arg1 == "Задачи сотрудника") {
        typ = 2;
        for (int i = 0; i < tasks.size(); i++) {
            if (!mem[tasks[i]->worker]) {
                ui->comboBox_2->addItem(tasks[i]->worker);
                mem[tasks[i]->worker] = 1;
            }
        }
    }

    if (arg1 == "Сотрудники проекта") {
        typ = 3;
        for (int i = 0; i < tasks.size(); i++) {
            if (!mem[tasks[i]->project_name]) {
                ui->comboBox_2->addItem(tasks[i]->project_name);
                mem[tasks[i]->project_name] = 1;
            }
        }
    }
    on_comboBox_2_activated(ui->comboBox_2->itemText(0));

}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    QMap<QString, bool> mem; mem.clear();
    ui->textBrowser->clear();
    if (typ == 1) {
        QVector<QString> list(0);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i]->manager != arg1 || mem[tasks[i]->project_name]) continue;
            list.push_back(tasks[i]->project_name);
            mem[tasks[i]->project_name] = 1;
        }
        for (int i = 0; i < list.size(); i++) {
            ui->textBrowser->append(list[i] + ": ");
            for (int j = 0; j < tasks.size(); j++) {
                if (tasks[j]->manager != arg1 || tasks[j]->project_name != list[i]) continue;
                ui->textBrowser->append("    " + tasks[j]->task_name + " (" + tasks[j]->worker + ')');
            }
        }
    }
    if (typ == 2) {
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i]->worker != arg1) continue;
            ui->textBrowser->append(tasks[i]->task_name + " (" + tasks[i]->project_name + ')');
        }
    }
    if (typ == 3) {
        ui->textBrowser->append("Работники: ");
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i]->project_name != arg1) continue;
            if (!mem[tasks[i]->worker]) {
                mem[tasks[i]->worker] = 1;
                ui->textBrowser->append(tasks[i]->worker);
            }
        }
        mem.clear();
        ui->textBrowser->append("\nРуководители: ");
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i]->project_name != arg1) continue;
            if (!mem[tasks[i]->manager]) {
                mem[tasks[i]->manager] = 1;
                ui->textBrowser->append(tasks[i]->manager);
            }
        }
    }
}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    if (focus == 0) ui->start->setDate(date);
    if (focus == 1) ui->deadline->setDate(date);
    if (focus == 2) ui->end->setDate(date);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {

        QFont a;
        a.setWeight(50);
        if (focus == 0) ui->start->setFont(a);
        if (focus == 1) ui->deadline->setFont(a);
        if (focus == 2) ui->end->setFont(a);

        focus = (focus + 1) % 3;

        a.setWeight(100);
        if (focus == 0) ui->start->setFont(a);
        if (focus == 1) ui->deadline->setFont(a);
        if (focus == 2) ui->end->setFont(a);
    }
}

void MainWindow::write_in_file(Task *task) {
    char a[10000] = {};
    strcat(a, task->project_name.toUtf8());
    strcat(a, ", " + task->task_name.toUtf8());
    strcat(a, ", " + task->worker.toUtf8());
    strcat(a, ", " + task->manager.toUtf8());
    strcat(a, ", " + task->start.toString(Qt::SystemLocaleShortDate).toUtf8());
    strcat(a, ", " + task->dead_line.toString(Qt::SystemLocaleShortDate).toUtf8());
    strcat(a, ", " + task->end.toString(Qt::SystemLocaleShortDate).toUtf8() + ".\r\n");
    file.write(a);
}

void MainWindow::on_add_new_project_clicked()
{
    Task *task = new Task(ui->project_name->text(), ui->task_name->text(), ui->worker->text(), ui->manager->text(),
                ui->start->date(), ui->deadline->date(), ui->end->date());
    write_in_file(task);
    tasks.push_back(task);
    sort();
    on_comboBox_activated(ui->comboBox->itemText(typ));
}
