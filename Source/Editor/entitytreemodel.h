#ifndef ENTITYTREEVIEW_H
#define ENTITYTREEVIEW_H

#include "treemodel.h"
#include <Core/Entity.h>
#include <Core/Component.h>

class EntityTreeModel : public TreeModel
{
public:
    EntityTreeModel(QObject *parent);

    void addItem(Entity* entity);
    void addChildItem(Entity* entity, Component* component);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int GetRowNumber(Entity* entity);
};

#endif // ENTITYTREEVIEW_H
