#include "openscenedialog.h"
#include "ui_openscenedialog.h"
#include <Core/Application.h>
#include <IO/SQLiteDataLoader.h>
#include <Core/Scene.h>

OpenSceneDialog::OpenSceneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenSceneDialog)
{
    ui->setupUi(this);

    SQLiteDataLoader* loader = GetAppService<SQLiteDataLoader>();
    std::vector<DataRecord*> scenes = loader->GetRecords("Scene");

    // Add scenes to the dropdown
    ui->sceneDropdown->clear();
    for(std::vector<DataRecord*>::iterator i = scenes.begin(); i != scenes.end(); i++) {
        Scene* scene = (Scene*)((*i)->GetObject());
        QVariant sceneID((*i)->GetRecordID());
        ui->sceneDropdown->addItem(QString::fromStdString(scene->GetName()), sceneID);
    }
}

OpenSceneDialog::~OpenSceneDialog()
{
    delete ui;
}

void OpenSceneDialog::accept() {
}
