#ifndef OPENSCENEDIALOG_H
#define OPENSCENEDIALOG_H

#include <QDialog>

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
};

#endif // OPENSCENEDIALOG_H
