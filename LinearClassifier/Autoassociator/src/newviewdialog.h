#ifndef NEWVIEWDIALOG_H
#define NEWVIEWDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLabel>

class NewViewDialog : public QDialog
{
public:
    explicit NewViewDialog(QWidget* _parent = nullptr);
    ~NewViewDialog();

    // Getters
    [[nodiscard]] int getWidth() const { return widthBox->value(); }
    [[nodiscard]] int getHeight() const { return heightBox->value(); }
    [[nodiscard]] QSize getSize() const { return {widthBox->value(), heightBox->value()}; }

private:
    void initGUI();

    QGridLayout* mainLayout;
    QLabel* widthBoxLabel;
    QLabel* heightBoxLabel;
    QSpinBox* widthBox;
    QSpinBox* heightBox;
    QDialogButtonBox* buttonBox;
};

#endif // NEWVIEWDIALOG_H
