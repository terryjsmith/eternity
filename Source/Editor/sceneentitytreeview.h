#ifndef SCENEENTITYTREEVIEW_H
#define SCENEENTITYTREEVIEW_H

#include <QTreeView>

class SceneEntityTreeView : public QTreeView
{
public:
    SceneEntityTreeView(QWidget* parent = 0);

    void SetInitialized(bool initialized) { m_initialized = initialized; }

private slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
    bool m_initialized;
};

#endif // SCENEENTITYTREEVIEW_H
