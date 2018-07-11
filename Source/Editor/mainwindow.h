#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* GetInstance();

private:
    Ui::MainWindow *ui;

    // Singleton
    static MainWindow* m_instance;
};

#endif // MAINWINDOW_H
