#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graphs/qcustomplot.h"
#include "Graphs/qcustombars.h"
#include "Completion/getcomplsuggestions.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

private:
    Ui::MainWindow *ui;
    QCPItemText *textItem;
    QCustomBars cbars;
    GetComplSuggestions getSuggestions;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

private slots:
    void on_pbn_database_clicked();
    void on_pbn_graphs_clicked();
    void on_pbn_plot_clicked();
    void on_pbn_createDb_clicked();
    void onMouseMove(QMouseEvent* event);
    void on_pbn_clear_clicked();
};
#endif // MAINWINDOW_H
