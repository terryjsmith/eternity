#include "openscenedialog.h"
#include "ui_openscenedialog.h"
#include "mainwindow.h"

#include <Core/Application.h>
#include <IO/SQLiteDataLoader.h>
#include <Core/Scene.h>
#include <Core/World.h>

OpenSceneDialog::OpenSceneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenSceneDialog)
{
    ui->setupUi(this);

    SQLiteDataLoader* loader = GetAppService<SQLiteDataLoader>();
    m_scenes = loader->GetRecords("Scene");

    // Add scenes to the dropdown
    ui->sceneDropdown->clear();
    for(std::vector<DataRecord*>::iterator i = m_scenes.begin(); i != m_scenes.end(); i++) {
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
    // Clear any previous entities out of our system
    World* world = Application::GetInstance()->GetWorld();
    world->Clear();

    // Get the selected scene ID
    QVariant selectedSceneID = ui->sceneDropdown->currentData();

    std::map<std::string, std::string> args;
    args["m_sceneID"] = std::to_string(selectedSceneID.toInt());

    SQLiteDataLoader* loader = GetAppService<SQLiteDataLoader>();
    std::vector<DataRecord*> entities = loader->GetRecords("Entity");
    EntityTreeModel* model = MainWindow::GetInstance()->GetEntityTreeModel();

    for(size_t i = 0; i < entities.size(); i++) {
        Entity* entity = (Entity*)entities[i]->GetObject();
        world->AddEntity(entity);
        model->addItem(entity);
    }

    this->close();
}
