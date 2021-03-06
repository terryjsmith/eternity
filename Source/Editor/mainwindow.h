#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "entitytreemodel.h"
#include "eternityopenglwidget.h"
#include <Core/DataRecord.h>

#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>
#include <QTreeView>
#include <QFormLayout>
#include <QItemSelection>
#include <QStatusBar>

Q_DECLARE_METATYPE(DataRecord*);

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
    QGroupBox* GetPropertiesWidget() { return m_propertiesGroupBox; }
    QStatusBar* GetStatusBar() { return m_statusBar; }

    // Menu item handlers
    void btnOpenProject_clicked();
    void btnOpenScene_clicked();
    void btnSaveScene_clicked();

    // Create properties window from meta data fields
    QFormLayout* GetFormLayout(std::string className, GigaObject* object, QWidget* parent = 0);

private slots:
    void on_propertiesLayout_clicked();
    void cbStateChange(int value);
    void textEditFinished();
    void browseAssets();
    void textResourceChange(QString path);

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

    // Status bar
    QStatusBar* m_statusBar;
};

#endif // MAINWINDOW_H
