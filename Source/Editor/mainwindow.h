#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>

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

    // Open a project from a directory
    void OpenProject(QString directory);

    // Menu item handlers
    void btnOpenProject_clicked();
    void btnOpenScene_clicked();

private slots:
    void on_propertiesLayout_clicked();

private:
    Ui::MainWindow *ui;

    // Singleton
    static MainWindow* m_instance;

    // Asset location dropdown
    QComboBox* m_assetLocationDropdown;

    // Current project directory
    QString m_currentProjectDirectory;

    QGroupBox* m_propertiesGroupBox;
    bool m_propertiesExpanded;
};

#endif // MAINWINDOW_H
