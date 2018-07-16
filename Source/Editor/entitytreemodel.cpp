#include "entitytreemodel.h"

EntityTreeModel::EntityTreeModel(QObject *parent) : TreeModel(parent) {
    QList<QVariant> rootData;
    rootData << "Entities";
    m_rootItem = new TreeItem(rootData, 0);
}

void EntityTreeModel::addItem(Entity* entity) {
    // Add this entity
    QList<QVariant> rootData;
    rootData << QString::fromStdString(entity->Name());
    TreeItem* item = new TreeItem(rootData, m_rootItem);
    item->SetInternalPointer((void*)entity);
    item->SetEditable(true);

    beginInsertRows(QModelIndex(), m_rootItem->childCount(), m_rootItem->childCount() + 1);
    m_rootItem->appendChild(item);
    endInsertRows();
}

void EntityTreeModel::addChildItem(Entity* entity, Component* component) {
    // Find the entity
    for(int i = 0; i < m_rootItem->childCount(); i++) {
        TreeItem* item = m_rootItem->child(i);
        Entity* pointer = (Entity*)item->GetInternalPointer();
        if(pointer == entity) {
            QList<QVariant> rootData;
            rootData << QString::fromStdString(component->GetGigaName());

            TreeItem* childItem = new TreeItem(rootData, item);
            childItem->SetInternalPointer((void*)component);

            beginInsertRows(QModelIndex(), m_rootItem->childCount(), m_rootItem->childCount() + 1);
            item->appendChild(childItem);
            endInsertRows();

            return;
        }
    }
}

bool EntityTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return(false);
    }

    // Convert to a string and check validity
    if(value.canConvert(QVariant::String) == false) {
        return(false);
    }

    QString newName = value.toString();
    if(newName.length() == 0) {
        return(false);
    }

    // Change the name internally
    TreeItem *item = getItem(index);
    Entity* entity = (Entity*)item->GetInternalPointer();
    entity->Name(newName.toStdString());

    // Update the tree view
    bool result = item->setData(index.column(), QString::fromStdString(entity->Name()));
    if (result)
        emit dataChanged(index, index);

    return result;
}

int EntityTreeModel::GetRowNumber(Entity* entity) {
    for(int i = 0; i < m_rootItem->childCount(); i++) {
        TreeItem* item = m_rootItem->child(i);
        Entity* pointer = (Entity*)item->GetInternalPointer();
        if(pointer == entity) {
            return(i);
        }
    }

    return(-1);
}
