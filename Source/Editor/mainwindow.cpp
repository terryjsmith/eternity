#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include "openscenedialog.h"
#include "sceneentitytreeview.h"

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
    connect(ui->actionSave_Scene, &QAction::triggered, this, &MainWindow::btnSaveScene_clicked);
    connect(ui->sceneTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(MainWindow::entityChanged(QModelIndex)));
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

    SceneEntityTreeView* entityView = (SceneEntityTreeView*)m_entityTreeView;
    entityView->SetInitialized(true);
}

void MainWindow::btnOpenScene_clicked() {
    OpenSceneDialog* dlg = new OpenSceneDialog(this);
    dlg->show();
}

void MainWindow::btnSaveScene_clicked() {
    Application* application = Application::GetInstance();
    SQLiteDataLoader* dataloader = application->GetAppService<SQLiteDataLoader>();
    dataloader->SaveRecords();
}

QFormLayout* MainWindow::GetFormLayout(std::string className, GigaObject *object, QWidget *parent) {
    // Create a new form layout
    QFormLayout* layout = new QFormLayout(0);
    parent = 0;

    // Get our data loader
    Application* application = Application::GetInstance();
    SQLiteDataLoader* dataloader = application->GetAppService<SQLiteDataLoader>();

    // Find our data record
    DataRecord* record = dataloader->FindDataRecord(object);

    DataRecordType* type = DataRecordType::GetType(className);
    std::map<std::string, int> keys = type->GetKeys();
    std::map<std::string, int>::iterator it = keys.begin();
    for(; it != keys.end(); it++) {
        QWidget* widget = 0;
        bool added = false;

        DataRecordType::DataRecordField* details = type->GetKeyDetail(it->first);
        std::string field_name = details->friendly_name.empty() ? it->first : details->friendly_name;
        QString qFieldName = QString::fromStdString(it->first);

        if(details->editable == false)
            continue;

        if(it->second == Variant::VAR_INT32 || it->second == Variant::VAR_UINT32 || it->second == Variant::VAR_INT64 || it->second == Variant::VAR_UINT64) {
            QLineEdit* edit = new QLineEdit(parent);
            edit->setText(QString::fromStdString(record->GetString(it->first)));
            edit->setValidator(new QIntValidator(layout));
            edit->setMaximumWidth(40);
            widget = edit;
        }

        if(it->second == Variant::VAR_BOOL) {
            QCheckBox* cb = new QCheckBox(parent);
            cb->setChecked(record->GetString(it->first).compare("true") == 0);
            connect(cb, SIGNAL(stateChanged(int)), this, SLOT(cbStateChange(int)));
            widget = cb;
        }

        if(it->second == Variant::VAR_FLOAT) {
            QLineEdit* edit = new QLineEdit(parent);
            edit->setText(QString::fromStdString(record->GetString(it->first)));
            edit->setValidator(new QDoubleValidator(layout));
            edit->setMaximumWidth(40);
            widget = edit;
        }

        if(it->second == Variant::VAR_VECTOR2) {
            QHBoxLayout* hbox = new QHBoxLayout();
            vector2 vec2 = record->Get(it->first)->AsVector2();

            QLineEdit* x = new QLineEdit(parent);
            x->setText(QString::number(vec2.x, 'f', 2));
            x->setMaximumWidth(40);
            x->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(x);

            QLineEdit* y = new QLineEdit(parent);
            y->setText(QString::number(vec2.y, 'f', 2));
            y->setMaximumWidth(40);
            y->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(y);

            layout->addRow(QString::fromStdString(field_name),hbox);
            added = true;
        }

        if(it->second == Variant::VAR_VECTOR3) {
            QHBoxLayout* hbox = new QHBoxLayout();
            vector3 vec3 = record->Get(it->first)->AsVector3();

            QLineEdit* x = new QLineEdit(parent);
            x->setText(QString::number(vec3.x, 'f', 2));
            x->setMaximumWidth(40);
            x->setValidator(new QDoubleValidator(layout));
            x->setObjectName(qFieldName + ".x");
            hbox->addWidget(x);

            x->setProperty("fieldRecord", QVariant::fromValue(record));

            QLineEdit* y = new QLineEdit(parent);
            y->setText(QString::number(vec3.y, 'f', 2));
            y->setMaximumWidth(40);
            y->setValidator(new QDoubleValidator(layout));
            y->setObjectName(qFieldName + ".y");
            hbox->addWidget(y);

            y->setProperty("fieldRecord", QVariant::fromValue(record));

            QLineEdit* z = new QLineEdit(parent);
            z->setText(QString::number(vec3.z, 'f', 2));
            z->setMaximumWidth(40);
            z->setValidator(new QDoubleValidator(layout));
            z->setObjectName(qFieldName + ".z");
            hbox->addWidget(z);

            z->setProperty("fieldRecord", QVariant::fromValue(record));

            connect(x, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));
            connect(y, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));
            connect(z, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));

            layout->addRow(QString::fromStdString(field_name),hbox);
            added = true;
        }

        if(it->second == Variant::VAR_VECTOR4) {
            QHBoxLayout* hbox = new QHBoxLayout();
            vector4 vec4 = record->Get(it->first)->AsVector4();

            QLineEdit* x = new QLineEdit(parent);
            x->setText(QString::number(vec4.x, 'f', 2));
            x->setMaximumWidth(40);
            x->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(x);

            QLineEdit* y = new QLineEdit(parent);
            y->setText(QString::number(vec4.y, 'f', 2));
            y->setMaximumWidth(40);
            y->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(y);

            QLineEdit* z = new QLineEdit(parent);
            z->setText(QString::number(vec4.z, 'f', 2));
            z->setMaximumWidth(40);
            z->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(z);

            QLineEdit* w = new QLineEdit(parent);
            w->setText(QString::number(vec4.w, 'f', 2));
            w->setMaximumWidth(40);
            w->setValidator(new QDoubleValidator(layout));
            hbox->addWidget(w);

            layout->addRow(QString::fromStdString(field_name),hbox);
            added = true;
        }

        if(it->second == Variant::VAR_QUATERNION) {
            QHBoxLayout* hbox = new QHBoxLayout();
            quaternion quat = record->Get(it->first)->AsQuaternion();

            vector3 angles = glm::degrees(glm::eulerAngles(quat));

            QLineEdit* x = new QLineEdit(parent);
            x->setText(QString::number(angles.x, 'f', 2));
            x->setMaximumWidth(40);
            x->setValidator(new QDoubleValidator(layout));
            x->setObjectName(qFieldName + ".x");
            hbox->addWidget(x);

            x->setProperty("fieldRecord", QVariant::fromValue(record));

            QLineEdit* y = new QLineEdit(parent);
            y->setText(QString::number(angles.y, 'f', 2));
            y->setMaximumWidth(40);
            y->setValidator(new QDoubleValidator(layout));
            y->setObjectName(qFieldName + ".y");
            hbox->addWidget(y);

            y->setProperty("fieldRecord", QVariant::fromValue(record));

            QLineEdit* z = new QLineEdit(parent);
            z->setText(QString::number(angles.z, 'f', 2));
            z->setMaximumWidth(40);
            z->setValidator(new QDoubleValidator(layout));
            z->setObjectName(qFieldName + ".z");
            hbox->addWidget(z);

            z->setProperty("fieldRecord", QVariant::fromValue(record));

            connect(x, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));
            connect(y, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));
            connect(z, SIGNAL(editingFinished()), this, SLOT(textEditFinished()));

            layout->addRow(QString::fromStdString(field_name),hbox);
            added = true;
        }

        if(it->second == Variant::VAR_STRING) {
            QLineEdit* edit = new QLineEdit(parent);
            edit->setText(QString::fromStdString(record->GetString(it->first)));
            widget = edit;
        }

        if(it->second == Variant::VAR_OBJECT) {
            GigaObject* obj = record->Get(it->first)->AsObject();
            QVBoxLayout* vbox = new QVBoxLayout;

            QFormLayout* form = (QFormLayout*)GetFormLayout(obj->GetGigaName(), obj, widget);
            if(form->rowCount() > 1) {
                QLabel* label = new QLabel(QString::fromStdString(obj->GetGigaName()));
                label->setStyleSheet("font-weight: bold;");
                vbox->addWidget(label);
            }
            vbox->addLayout(form);

            layout->addRow(vbox);
            added = true;
        }

        if(added == false) {
            widget->setProperty("fieldName", QString::fromStdString(it->first));
            widget->setProperty("fieldRecord", QVariant::fromValue(record));
            layout->insertRow(0, QString::fromStdString(field_name), widget);
        }
    }

    return(layout);
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

void MainWindow::cbStateChange(int value) {
    QCheckBox* cb = (QCheckBox*)QObject::sender();
    QString fieldName = cb->property("fieldName").toString();
    DataRecord* record = qvariant_cast<DataRecord*>(cb->property("fieldRecord"));

    GigaObject* obj = record->GetObject();
    std::string className = obj->GetGigaName();
    DataRecordType* type = DataRecordType::GetType(className);

    record->Set(fieldName.toStdString(), value == Qt::Checked ? "true" : "false");
    obj->Deserialize(record);
}

void MainWindow::textEditFinished() {
    QLineEdit* edit = (QLineEdit*)QObject::sender();
    QWidget* parent = edit->parentWidget();
    QString fieldName = edit->objectName();
    if(fieldName.indexOf(".") != -1) {
        fieldName = fieldName.mid(0, fieldName.indexOf("."));
    }

    DataRecord* record = qvariant_cast<DataRecord*>(edit->property("fieldRecord"));

    GigaObject* obj = record->GetObject();
    std::string className = obj->GetGigaName();
    DataRecordType* type = record->GetType();
    int fieldType = type->GetKeyType(fieldName.toStdString());

    if(fieldType == Variant::VAR_VECTOR3) {
        float x, y, z;

        QLineEdit* tx = parent->findChild<QLineEdit*>(fieldName + ".x");
        QLineEdit* ty = parent->findChild<QLineEdit*>(fieldName + ".y");
        QLineEdit* tz = parent->findChild<QLineEdit*>(fieldName + ".z");

        x = tx->text().toFloat();
        y = ty->text().toFloat();
        z = tz->text().toFloat();

        vector3 vec3 = vector3(x, y, z);
        record->Set(fieldName.toStdString(), new Variant(vec3));
    }

    if(fieldType == Variant::VAR_QUATERNION) {
        float x, y, z;

        QLineEdit* tx = parent->findChild<QLineEdit*>(fieldName + ".x");
        QLineEdit* ty = parent->findChild<QLineEdit*>(fieldName + ".y");
        QLineEdit* tz = parent->findChild<QLineEdit*>(fieldName + ".z");

        x = tx->text().toFloat();
        y = ty->text().toFloat();
        z = tz->text().toFloat();

        quaternion quat = glm::quat(glm::radians(vector3(x, y, z)));
        record->Set(fieldName.toStdString(), new Variant(quat));
    }

    obj->Deserialize(record);
}
