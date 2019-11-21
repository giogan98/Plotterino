#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustombars.h"
#include "IScanLogDatabase.h"
#include "getcomplsuggestions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbn_database_clicked();

    void on_pbn_graphs_clicked();

    void on_pbn_plot_clicked();

    void on_pbn_createDb_clicked();

    void onMouseMove(QMouseEvent* event);

    void on_pbn_clear_clicked();

private:
    Ui::MainWindow *ui;
    QCustomBars custombars;
    IScanLogDatabase scanlogdatabase;
    getComplSuggestions getsuggestions;
    QCPItemText *textItem;



};
#endif // MAINWINDOW_H
