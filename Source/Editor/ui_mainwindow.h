/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "eternityopenglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Project;
    QAction *actionOpen_Project;
    QAction *actionClose_Project;
    QAction *actionNew_Scene;
    QAction *actionOpen_Scene;
    QAction *actionSave_Scene;
    QAction *actionClose_Scene;
    QAction *actionExit;
    QAction *actionDelete_Scene;
    QAction *actionNew_Entity;
    QAction *actionDelete_Entity;
    QAction *actionAdd_Component;
    QAction *actionPhysics_Debug;
    QAction *actionLight_Models;
    QAction *actionPublish;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *mainLayout;
    QVBoxLayout *assetViewLaout;
    QVBoxLayout *sceneViewLayout;
    QHBoxLayout *sceneMenuLayout;
    QHBoxLayout *leftButtons;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QHBoxLayout *centerButtons;
    QHBoxLayout *rightButtons;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton_2;
    EternityOpenGLWidget *openGLWidget;
    QVBoxLayout *assetLayout;
    QTabWidget *contentBrowser;
    QWidget *contentTab;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_7;
    QComboBox *assetLocationDropdown;
    QTreeView *currentFileSystemView;
    QListView *currentFoldeListrView;
    QWidget *consoleTab;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *consoleTextEdit;
    QVBoxLayout *inspectorPanelLayout;
    QVBoxLayout *sceneInspectorLayout;
    QGroupBox *sceneLayout;
    QVBoxLayout *verticalLayout_5;
    QTreeView *sceneTreeView;
    QVBoxLayout *propertiesInspectorLayout;
    QGroupBox *propertiesLayout;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *propertiesTableView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuScene;
    QMenu *menuEntity;
    QMenu *menuView;
    QMenu *menuDebug;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1195, 676);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionNew_Project = new QAction(MainWindow);
        actionNew_Project->setObjectName(QStringLiteral("actionNew_Project"));
        actionOpen_Project = new QAction(MainWindow);
        actionOpen_Project->setObjectName(QStringLiteral("actionOpen_Project"));
        actionClose_Project = new QAction(MainWindow);
        actionClose_Project->setObjectName(QStringLiteral("actionClose_Project"));
        actionNew_Scene = new QAction(MainWindow);
        actionNew_Scene->setObjectName(QStringLiteral("actionNew_Scene"));
        actionOpen_Scene = new QAction(MainWindow);
        actionOpen_Scene->setObjectName(QStringLiteral("actionOpen_Scene"));
        actionSave_Scene = new QAction(MainWindow);
        actionSave_Scene->setObjectName(QStringLiteral("actionSave_Scene"));
        actionClose_Scene = new QAction(MainWindow);
        actionClose_Scene->setObjectName(QStringLiteral("actionClose_Scene"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDelete_Scene = new QAction(MainWindow);
        actionDelete_Scene->setObjectName(QStringLiteral("actionDelete_Scene"));
        actionNew_Entity = new QAction(MainWindow);
        actionNew_Entity->setObjectName(QStringLiteral("actionNew_Entity"));
        actionDelete_Entity = new QAction(MainWindow);
        actionDelete_Entity->setObjectName(QStringLiteral("actionDelete_Entity"));
        actionAdd_Component = new QAction(MainWindow);
        actionAdd_Component->setObjectName(QStringLiteral("actionAdd_Component"));
        actionPhysics_Debug = new QAction(MainWindow);
        actionPhysics_Debug->setObjectName(QStringLiteral("actionPhysics_Debug"));
        actionPhysics_Debug->setCheckable(true);
        actionLight_Models = new QAction(MainWindow);
        actionLight_Models->setObjectName(QStringLiteral("actionLight_Models"));
        actionLight_Models->setCheckable(true);
        actionPublish = new QAction(MainWindow);
        actionPublish->setObjectName(QStringLiteral("actionPublish"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mainLayout = new QHBoxLayout();
        mainLayout->setSpacing(10);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        assetViewLaout = new QVBoxLayout();
        assetViewLaout->setSpacing(6);
        assetViewLaout->setObjectName(QStringLiteral("assetViewLaout"));
        sceneViewLayout = new QVBoxLayout();
        sceneViewLayout->setSpacing(6);
        sceneViewLayout->setObjectName(QStringLiteral("sceneViewLayout"));
        sceneMenuLayout = new QHBoxLayout();
        sceneMenuLayout->setSpacing(6);
        sceneMenuLayout->setObjectName(QStringLiteral("sceneMenuLayout"));
        leftButtons = new QHBoxLayout();
        leftButtons->setSpacing(0);
        leftButtons->setObjectName(QStringLiteral("leftButtons"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMaximumSize(QSize(200, 16777215));
        horizontalLayout_8 = new QHBoxLayout(widget);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_8->addWidget(pushButton);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_8->addWidget(pushButton_3);


        leftButtons->addWidget(widget, 0, Qt::AlignLeft);


        sceneMenuLayout->addLayout(leftButtons);

        centerButtons = new QHBoxLayout();
        centerButtons->setSpacing(6);
        centerButtons->setObjectName(QStringLiteral("centerButtons"));

        sceneMenuLayout->addLayout(centerButtons);

        rightButtons = new QHBoxLayout();
        rightButtons->setSpacing(6);
        rightButtons->setObjectName(QStringLiteral("rightButtons"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        widget_2->setMaximumSize(QSize(200, 16777215));
        horizontalLayout_9 = new QHBoxLayout(widget_2);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setFlat(false);

        horizontalLayout_9->addWidget(pushButton_2);


        rightButtons->addWidget(widget_2, 0, Qt::AlignRight);


        sceneMenuLayout->addLayout(rightButtons);


        sceneViewLayout->addLayout(sceneMenuLayout);

        openGLWidget = new EternityOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        sceneViewLayout->addWidget(openGLWidget);


        assetViewLaout->addLayout(sceneViewLayout);

        assetLayout = new QVBoxLayout();
        assetLayout->setSpacing(0);
        assetLayout->setObjectName(QStringLiteral("assetLayout"));
        assetLayout->setContentsMargins(-1, -1, -1, 10);
        contentBrowser = new QTabWidget(centralWidget);
        contentBrowser->setObjectName(QStringLiteral("contentBrowser"));
        sizePolicy.setHeightForWidth(contentBrowser->sizePolicy().hasHeightForWidth());
        contentBrowser->setSizePolicy(sizePolicy);
        contentBrowser->setMaximumSize(QSize(16777215, 250));
        contentTab = new QWidget();
        contentTab->setObjectName(QStringLiteral("contentTab"));
        horizontalLayout_2 = new QHBoxLayout(contentTab);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 5, 5, 5);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        assetLocationDropdown = new QComboBox(contentTab);
        assetLocationDropdown->setObjectName(QStringLiteral("assetLocationDropdown"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(assetLocationDropdown->sizePolicy().hasHeightForWidth());
        assetLocationDropdown->setSizePolicy(sizePolicy2);
        assetLocationDropdown->setMaximumSize(QSize(200, 16777215));

        verticalLayout_7->addWidget(assetLocationDropdown);

        currentFileSystemView = new QTreeView(contentTab);
        currentFileSystemView->setObjectName(QStringLiteral("currentFileSystemView"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(currentFileSystemView->sizePolicy().hasHeightForWidth());
        currentFileSystemView->setSizePolicy(sizePolicy3);
        currentFileSystemView->setMaximumSize(QSize(200, 16777215));

        verticalLayout_7->addWidget(currentFileSystemView);


        horizontalLayout_3->addLayout(verticalLayout_7);

        currentFoldeListrView = new QListView(contentTab);
        currentFoldeListrView->setObjectName(QStringLiteral("currentFoldeListrView"));
        currentFoldeListrView->setMaximumSize(QSize(16777215, 16777215));
        currentFoldeListrView->setFrameShape(QFrame::NoFrame);
        currentFoldeListrView->setFrameShadow(QFrame::Plain);
        currentFoldeListrView->setLineWidth(0);

        horizontalLayout_3->addWidget(currentFoldeListrView);


        horizontalLayout_2->addLayout(horizontalLayout_3);

        contentBrowser->addTab(contentTab, QString());
        consoleTab = new QWidget();
        consoleTab->setObjectName(QStringLiteral("consoleTab"));
        verticalLayout_2 = new QVBoxLayout(consoleTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        consoleTextEdit = new QPlainTextEdit(consoleTab);
        consoleTextEdit->setObjectName(QStringLiteral("consoleTextEdit"));
        consoleTextEdit->setFrameShape(QFrame::NoFrame);
        consoleTextEdit->setReadOnly(true);

        verticalLayout->addWidget(consoleTextEdit);


        verticalLayout_2->addLayout(verticalLayout);

        contentBrowser->addTab(consoleTab, QString());

        assetLayout->addWidget(contentBrowser);


        assetViewLaout->addLayout(assetLayout);


        mainLayout->addLayout(assetViewLaout);

        inspectorPanelLayout = new QVBoxLayout();
        inspectorPanelLayout->setSpacing(6);
        inspectorPanelLayout->setObjectName(QStringLiteral("inspectorPanelLayout"));
        sceneInspectorLayout = new QVBoxLayout();
        sceneInspectorLayout->setSpacing(0);
        sceneInspectorLayout->setObjectName(QStringLiteral("sceneInspectorLayout"));
        sceneLayout = new QGroupBox(centralWidget);
        sceneLayout->setObjectName(QStringLiteral("sceneLayout"));
        sceneLayout->setMaximumSize(QSize(250, 16777215));
        sceneLayout->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(sceneLayout);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, -1, 0, -1);
        sceneTreeView = new QTreeView(sceneLayout);
        sceneTreeView->setObjectName(QStringLiteral("sceneTreeView"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sceneTreeView->sizePolicy().hasHeightForWidth());
        sceneTreeView->setSizePolicy(sizePolicy4);
        sceneTreeView->setMinimumSize(QSize(0, 250));
        sceneTreeView->setMaximumSize(QSize(16777215, 400));
        sceneTreeView->setBaseSize(QSize(0, 0));

        verticalLayout_5->addWidget(sceneTreeView);


        sceneInspectorLayout->addWidget(sceneLayout);


        inspectorPanelLayout->addLayout(sceneInspectorLayout);

        propertiesInspectorLayout = new QVBoxLayout();
        propertiesInspectorLayout->setSpacing(0);
        propertiesInspectorLayout->setObjectName(QStringLiteral("propertiesInspectorLayout"));
        propertiesLayout = new QGroupBox(centralWidget);
        propertiesLayout->setObjectName(QStringLiteral("propertiesLayout"));
        propertiesLayout->setMaximumSize(QSize(250, 16777215));
        propertiesLayout->setFlat(true);
        verticalLayout_6 = new QVBoxLayout(propertiesLayout);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, -1, 0, -1);
        propertiesTableView = new QTableWidget(propertiesLayout);
        propertiesTableView->setObjectName(QStringLiteral("propertiesTableView"));

        verticalLayout_6->addWidget(propertiesTableView);


        propertiesInspectorLayout->addWidget(propertiesLayout);


        inspectorPanelLayout->addLayout(propertiesInspectorLayout);


        mainLayout->addLayout(inspectorPanelLayout);


        horizontalLayout->addLayout(mainLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1195, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuScene = new QMenu(menuBar);
        menuScene->setObjectName(QStringLiteral("menuScene"));
        menuEntity = new QMenu(menuBar);
        menuEntity->setObjectName(QStringLiteral("menuEntity"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuDebug = new QMenu(menuView);
        menuDebug->setObjectName(QStringLiteral("menuDebug"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuScene->menuAction());
        menuBar->addAction(menuEntity->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionOpen_Project);
        menuFile->addAction(actionClose_Project);
        menuFile->addSeparator();
        menuFile->addAction(actionNew_Scene);
        menuFile->addAction(actionOpen_Scene);
        menuFile->addAction(actionSave_Scene);
        menuFile->addAction(actionClose_Scene);
        menuFile->addSeparator();
        menuFile->addAction(actionPublish);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuScene->addAction(actionDelete_Scene);
        menuEntity->addAction(actionNew_Entity);
        menuEntity->addAction(actionDelete_Entity);
        menuEntity->addSeparator();
        menuEntity->addAction(actionAdd_Component);
        menuView->addAction(menuDebug->menuAction());
        menuDebug->addAction(actionLight_Models);
        menuDebug->addAction(actionPhysics_Debug);

        retranslateUi(MainWindow);

        contentBrowser->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Eternity Editor", Q_NULLPTR));
        actionNew_Project->setText(QApplication::translate("MainWindow", "New Project...", Q_NULLPTR));
        actionOpen_Project->setText(QApplication::translate("MainWindow", "Open Project...", Q_NULLPTR));
        actionClose_Project->setText(QApplication::translate("MainWindow", "Close Project", Q_NULLPTR));
        actionNew_Scene->setText(QApplication::translate("MainWindow", "New Scene...", Q_NULLPTR));
        actionOpen_Scene->setText(QApplication::translate("MainWindow", "Open Scene...", Q_NULLPTR));
        actionSave_Scene->setText(QApplication::translate("MainWindow", "Save Scene", Q_NULLPTR));
        actionClose_Scene->setText(QApplication::translate("MainWindow", "Close Scene", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        actionDelete_Scene->setText(QApplication::translate("MainWindow", "Delete Scene...", Q_NULLPTR));
        actionNew_Entity->setText(QApplication::translate("MainWindow", "New Entity...", Q_NULLPTR));
        actionDelete_Entity->setText(QApplication::translate("MainWindow", "Delete Entity", Q_NULLPTR));
        actionAdd_Component->setText(QApplication::translate("MainWindow", "Add Component...", Q_NULLPTR));
        actionPhysics_Debug->setText(QApplication::translate("MainWindow", "Physics", Q_NULLPTR));
        actionLight_Models->setText(QApplication::translate("MainWindow", "Lights", Q_NULLPTR));
        actionPublish->setText(QApplication::translate("MainWindow", "Publish...", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        contentBrowser->setTabText(contentBrowser->indexOf(contentTab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        contentBrowser->setTabText(contentBrowser->indexOf(consoleTab), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
        sceneLayout->setTitle(QApplication::translate("MainWindow", "Scene", Q_NULLPTR));
        propertiesLayout->setTitle(QApplication::translate("MainWindow", "Properties", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuScene->setTitle(QApplication::translate("MainWindow", "Scene", Q_NULLPTR));
        menuEntity->setTitle(QApplication::translate("MainWindow", "Entity", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "View", Q_NULLPTR));
        menuDebug->setTitle(QApplication::translate("MainWindow", "Debug", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
