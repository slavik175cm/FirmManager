#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <task.h>
#include <QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QDate transform(QString s);
    void write_in_file(Task *task);
    void swap(Task *&a, Task *&b);
    void sort();
    QFile file, savefile;
    QVector<Task*> tasks;
    void init();
    void showall();
    int typ = 0, focus = 0;

private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_comboBox_2_activated(const QString &arg1);

    void keyPressEvent(QKeyEvent *event);

    void on_calendarWidget_clicked(const QDate &date);

    void on_add_new_project_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
