#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "entitytreemodel.h"
#include "eternityopenglwidget.h"

#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>
#include <QTreeView>

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
    void OpenProject(QString projectFile);

    // Getters for view models
    EntityTreeModel* GetEntityTreeModel() { return m_entityTreeModel; }
    EternityOpenGLWidget* GetOpenGLWidget() { return m_openglWidget; }

    // Menu item handlers
    void btnOpenProject_clicked();
    void btnOpenScene_clicked();

private slots:
    void on_propertiesLayout_clicked();

private:
    // UI
    Ui::MainWindow *ui;

    // OpenGL window
    EternityOpenGLWidget* m_openglWidget;

    // Singleton
    static MainWindow* m_instance;

    // Asset location dropdown
    QComboBox* m_assetLocationDropdown;

    // Current project directory
    QString m_currentProjectDirectory;

    QGroupBox* m_propertiesGroupBox;
    bool m_propertiesExpanded;

    // Entity view
    QTreeView* m_entityTreeView;
    EntityTreeModel* m_entityTreeModel;
};

#endif // MAINWINDOW_H
