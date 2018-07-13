#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include <IO/SQLiteDataLoader.h>
#include <IO/ResourceSystem.h>
#include <Scripting/ScriptingSystem.h>
#include <Core/Application.h>

MainWindow* MainWindow::m_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_propertiesExpanded = true;

    // Get UI components
    m_assetLocationDropdown = ui->assetLocationDropdown;
    m_propertiesGroupBox = ui->propertiesLayout;

    // Set up our default asset locations
    QStringList assetTypes;
    assetTypes << "Assets" << "File System";
    m_assetLocationDropdown->addItems(assetTypes);

    ui->contentBrowser->setTabText(0, "Content Browser");
    ui->contentBrowser->setTabText(1, "Console");

    // Connect menu items to functions
    connect(ui->actionOpen_Project, &QAction::triggered, this, &MainWindow::btnOpenProject_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

MainWindow* MainWindow::GetInstance() {
    if(m_instance == 0) {
        m_instance = new MainWindow();
    }

    return(m_instance);
}

void MainWindow::btnOpenProject_clicked() {
    // Have the user choose a project directory
    QString directory = QFileDialog::getExistingDirectory(this, "Choose Project Directory", QDir::currentPath());
    OpenProject(directory);
}

void MainWindow::OpenProject(QString directory) {
    if(directory.length() == 0) return;

    m_currentProjectDirectory = directory;
    QDir::setCurrent(m_currentProjectDirectory);

    SQLiteDataLoader* dataloader = new SQLiteDataLoader();

    // Look for a game.db file
    QString gamedbFile = directory + "/game.db";
    if(QFile::exists(gamedbFile)) {
        dataloader->Open(gamedbFile.toStdString());
    }

    // Load a game.js file if one exists
    QString gamejsFile = directory + "/game.js";
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

    ScriptComponent* gamejs = 0;
    if(QFile::exists(gamejsFile)) {
        scriptingSystem->Lock(0);

        Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
        ScriptComponent* gameComponent = new ScriptComponent();

        gameComponent->Initialize(gamejs);

        scriptingSystem->Unlock();
    }

    // Enable menu items
    ui->actionClose_Project->setEnabled(true);
    ui->actionNew_Scene->setEnabled(true);
    ui->actionOpen_Scene->setEnabled(true);
}

void MainWindow::on_propertiesLayout_clicked() {
    if(m_propertiesExpanded) {
        m_propertiesGroupBox->setMaximumHeight(20);
        m_propertiesExpanded = false;
    }
    else {
        m_propertiesGroupBox->setMaximumHeight(0);
        m_propertiesExpanded = true;
    }
}
