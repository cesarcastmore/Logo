#ifndef INPUT_H
#define INPUT_H

#include <QDialog>
#include "logo.h"

namespace Ui {
class Input;
}

class Input : public QDialog
{
    Q_OBJECT

public:
    typedef void (Logo::*_functType)(int, double);
    _functType m_func;
    Logo &parent_window;
    explicit Input(Logo &parent, int dir, _functType func);
    ~Input();
    int direccion;

private slots:
    void on_inputButton_accepted();

private:
    Ui::Input *ui;
};

#endif // INPUT_H
