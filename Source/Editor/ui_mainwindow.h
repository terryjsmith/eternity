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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
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
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *leftSideLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton_2;
    EternityOpenGLWidget *openGLWidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *contentBrowser;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_7;
    QComboBox *comboBox;
    QTreeView *treeView_2;
    QListView *listView;
    QWidget *tab_2;
    QVBoxLayout *inspectorPanelLayout;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *tableWidget;
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
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        leftSideLayout = new QVBoxLayout();
        leftSideLayout->setSpacing(6);
        leftSideLayout->setObjectName(QStringLiteral("leftSideLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
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


        horizontalLayout_6->addWidget(widget, 0, Qt::AlignLeft);


        horizontalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));

        horizontalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
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


        horizontalLayout_5->addWidget(widget_2, 0, Qt::AlignRight);


        horizontalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout->addLayout(horizontalLayout_4);

        openGLWidget = new EternityOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(openGLWidget);


        leftSideLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, -1, 10);
        contentBrowser = new QTabWidget(centralWidget);
        contentBrowser->setObjectName(QStringLiteral("contentBrowser"));
        sizePolicy.setHeightForWidth(contentBrowser->sizePolicy().hasHeightForWidth());
        contentBrowser->setSizePolicy(sizePolicy);
        contentBrowser->setMaximumSize(QSize(16777215, 250));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
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
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy2);
        comboBox->setMaximumSize(QSize(200, 16777215));

        verticalLayout_7->addWidget(comboBox);

        treeView_2 = new QTreeView(tab);
        treeView_2->setObjectName(QStringLiteral("treeView_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(treeView_2->sizePolicy().hasHeightForWidth());
        treeView_2->setSizePolicy(sizePolicy3);
        treeView_2->setMaximumSize(QSize(200, 16777215));

        verticalLayout_7->addWidget(treeView_2);


        horizontalLayout_3->addLayout(verticalLayout_7);

        listView = new QListView(tab);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setMaximumSize(QSize(16777215, 16777215));
        listView->setFrameShape(QFrame::NoFrame);
        listView->setFrameShadow(QFrame::Plain);
        listView->setLineWidth(0);

        horizontalLayout_3->addWidget(listView);


        horizontalLayout_2->addLayout(horizontalLayout_3);

        contentBrowser->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        contentBrowser->addTab(tab_2, QString());

        verticalLayout_2->addWidget(contentBrowser);


        leftSideLayout->addLayout(verticalLayout_2);


        horizontalLayout->addLayout(leftSideLayout);

        inspectorPanelLayout = new QVBoxLayout();
        inspectorPanelLayout->setSpacing(6);
        inspectorPanelLayout->setObjectName(QStringLiteral("inspectorPanelLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(250, 16777215));
        groupBox->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, -1, 0, -1);
        treeView = new QTreeView(groupBox);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout_5->addWidget(treeView);


        verticalLayout_3->addWidget(groupBox);


        inspectorPanelLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(250, 16777215));
        groupBox_2->setFlat(true);
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, -1, 0, -1);
        tableWidget = new QTableWidget(groupBox_2);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        verticalLayout_6->addWidget(tableWidget);


        verticalLayout_4->addWidget(groupBox_2);


        inspectorPanelLayout->addLayout(verticalLayout_4);


        horizontalLayout->addLayout(inspectorPanelLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1195, 22));
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
        contentBrowser->setTabText(contentBrowser->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        contentBrowser->setTabText(contentBrowser->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Scene", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Properties", Q_NULLPTR));
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
