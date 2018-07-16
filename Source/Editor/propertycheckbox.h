#ifndef PROPERTYCHECKBOX_H
#define PROPERTYCHECKBOX_H

#include <QCheckBox>
#include <Core/DataRecord.h>

class PropertyCheckBox : public QCheckBox
{
public:
    PropertyCheckBox(QWidget* parent = 0);

    void checkStateSet();

public:
    DataRecord* record;
    std::string field;
};

#endif // PROPERTYCHECKBOX_H
