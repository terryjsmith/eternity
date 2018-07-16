#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>

class TreeItem {
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    bool setData(int column, const QVariant &value);
    void removeChild(int row);

    // Get/set internal pointer
    void* GetInternalPointer() { return m_internalPointer; }
    void SetInternalPointer(void* pointer) { m_internalPointer = pointer; }

    // Get/set editable status
    void SetEditable(bool editable) { m_editable = editable; }
    bool GetEditable() { return m_editable; }

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
    void* m_internalPointer;
    bool m_editable;
};

#endif // TREEITEM_H
