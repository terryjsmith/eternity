#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include "openscenedialog.h"

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
    m_entityTreeView = ui->sceneTreeView;
    m_openglWidget = ui->openGLWidget;

    // Set up our default asset locations
    QStringList assetTypes;
    assetTypes << "Assets" << "File System";
    m_assetLocationDropdown->addItems(assetTypes);

    ui->contentBrowser->setTabText(0, "Content Browser");
    ui->contentBrowser->setTabText(1, "Console");

    // Initialize our views
    m_entityTreeModel = new EntityTreeModel(0);
    m_entityTreeView->setModel(m_entityTreeModel);

    // Connect menu items to functions
    connect(ui->actionOpen_Project, &QAction::triggered, this, &MainWindow::btnOpenProject_clicked);
    connect(ui->actionOpen_Scene, &QAction::triggered, this, &MainWindow::btnOpenScene_clicked);
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
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project", QDir::currentPath(), "Eternity Project (*.gpf)");
    OpenProject(filePath);
}

void MainWindow::OpenProject(QString projectFile) {
    if(projectFile.length() == 0) return;

    QString path = projectFile;
    QString directory = path = path.mid(0, path.lastIndexOf("/"));

    QFile projFile(projectFile);
    if(!projFile.open(QIODevice::ReadWrite)) {
        qWarning("Couldn't open project settings file.");
        return;
    }

    QByteArray jsonData = projFile.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonObject settingsObject = jsonDoc.object();

    setWindowTitle("Eternity Editor - " + settingsObject["name"].toString());

    m_currentProjectDirectory = directory;
    QDir::setCurrent(m_currentProjectDirectory);

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

    Application* application = Application::GetInstance();
    SQLiteDataLoader* dataloader = application->CreateAppService<SQLiteDataLoader>();

    // Look for a game.db file
    QString gamedbFile = directory + "/game.db";
    if(QFile::exists(gamedbFile)) {
        dataloader->Open(gamedbFile.toStdString());
    }

    // Enable menu items
    ui->actionClose_Project->setEnabled(true);
    ui->actionNew_Scene->setEnabled(true);
    ui->actionOpen_Scene->setEnabled(true);
}

void MainWindow::btnOpenScene_clicked() {
    OpenSceneDialog* dlg = new OpenSceneDialog(this);
    dlg->show();
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
