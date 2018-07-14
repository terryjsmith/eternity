#ifndef OPENSCENEDIALOG_H
#define OPENSCENEDIALOG_H

#include <QDialog>
#include <Core/DataRecord.h>

namespace Ui {
class OpenSceneDialog;
}

class OpenSceneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenSceneDialog(QWidget *parent = 0);
    ~OpenSceneDialog();

private slots:
    void accept();

private:
    Ui::OpenSceneDialog *ui;

    // Cached loaded scenes
    std::vector<DataRecord*> m_scenes;
};

#endif // OPENSCENEDIALOG_H
