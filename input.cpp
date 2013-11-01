#include "input.h"
#include "ui_input.h"

Input::Input(Logo &parent, int dir, _functType func) :

    ui(new Ui::Input),
  m_func(func),
    parent_window(parent)
{
    ui->setupUi(this);
    direccion=dir;
    ui->inputNumber->setMaximum(1000);
    ui->inputNumber->setMinimum(-1000);
}

Input::~Input()
{

}

void Input::on_inputButton_accepted()
{
    (parent_window.*m_func)(direccion, ui->inputNumber->value());
}
