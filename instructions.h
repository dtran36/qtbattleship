#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QDialog>

namespace Ui {
class Instructions;
}

class Instructions : public QDialog
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = nullptr);
    ~Instructions();

private slots:
    /**
     * @brief on_pushButton_clicked Hides instructions when "Close" button is pressed.
     */
    void on_pushButton_clicked();

private:
    Ui::Instructions *ui;
};

#endif // INSTRUCTIONS_H
