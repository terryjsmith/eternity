#include "sceneentitytreeview.h"
#include "mainwindow.h"
#include <QLineEdit>

SceneEntityTreeView::SceneEntityTreeView(QWidget* parent) : QTreeView(parent) {
    m_initialized = false;
}

void SceneEntityTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
    if(m_initialized == false) return;

    MainWindow* window = MainWindow::GetInstance();
    QGroupBox* propertiesWidget = window->GetPropertiesWidget();

    // Clear properties window
    for (auto widget: propertiesWidget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly))
      delete widget;

    EntityTreeModel* model = (EntityTreeModel*)current.model();
    TreeItem* item = model->getItem(current);
    GigaObject* obj = (GigaObject*)item->GetInternalPointer();

    Component* c = dynamic_cast<Component*>(obj);
    if(c) {
        QFormLayout* layout = window->GetFormLayout(obj->GetGigaName(), obj, 0);
        delete propertiesWidget->layout();
        propertiesWidget->setLayout(layout);
    }
}
