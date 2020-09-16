#include "tomogram.h"
#include "ui_tomogram.h"

tomogram::tomogram(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tomogram)
{
    ui->setupUi(this);
}

tomogram::~tomogram()
{
    delete ui;
}

