/********************************************************************************
** Form generated from reading UI file 'openscenedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENSCENEDIALOG_H
#define UI_OPENSCENEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OpenSceneDialog
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *sceneDropdown;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OpenSceneDialog)
    {
        if (OpenSceneDialog->objectName().isEmpty())
            OpenSceneDialog->setObjectName(QStringLiteral("OpenSceneDialog"));
        OpenSceneDialog->resize(400, 122);
        verticalLayout = new QVBoxLayout(OpenSceneDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sceneDropdown = new QComboBox(OpenSceneDialog);
        sceneDropdown->setObjectName(QStringLiteral("sceneDropdown"));

        verticalLayout->addWidget(sceneDropdown);

        buttonBox = new QDialogButtonBox(OpenSceneDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(OpenSceneDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), OpenSceneDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), OpenSceneDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(OpenSceneDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenSceneDialog)
    {
        OpenSceneDialog->setWindowTitle(QApplication::translate("OpenSceneDialog", "Open Scene", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OpenSceneDialog: public Ui_OpenSceneDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENSCENEDIALOG_H
