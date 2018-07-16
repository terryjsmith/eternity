#include "propertycheckbox.h"

PropertyCheckBox::PropertyCheckBox(QWidget* parent) : QCheckBox(parent) {
    record = 0;
}

void PropertyCheckBox::checkStateSet() {
    bool value = checkState() == Qt::Checked;
    if(record) {
        record->Set(field, new Variant(value));
        record->GetObject()->Deserialize(record);
    }
}
