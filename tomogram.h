#ifndef TOMOGRAM_H
#define TOMOGRAM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class tomogram; }
QT_END_NAMESPACE

class tomogram : public QWidget
{
    Q_OBJECT

public:
    tomogram(QWidget *parent = nullptr);
    ~tomogram();

private:
    Ui::tomogram *ui;
};
#endif // TOMOGRAM_H
