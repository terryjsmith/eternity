#include "treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent) {
    m_parentItem = parent;
    m_itemData = data;
}

TreeItem::~TreeItem() {
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item) {
    m_childItems.append(item);
}

void TreeItem::removeChild(int row) {
    m_childItems.removeAt(row);
}

TreeItem *TreeItem::child(int row) {
    return m_childItems.value(row);
}

int TreeItem::childCount() const {
    return m_childItems.count();
}

int TreeItem::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const {
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const {
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem() {
    return m_parentItem;
}

bool TreeItem::setData(int column, const QVariant &value) {
    if(column < 0 || column >= m_itemData.size()) {
        return(false);
    }

    m_itemData[column] = value;
    return true;
}
