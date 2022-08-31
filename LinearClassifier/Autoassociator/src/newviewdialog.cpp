#include "newviewdialog.h"

NewViewDialog::NewViewDialog(QWidget* _parent)
    : QDialog (_parent)
{
    this->setWindowTitle("New view");

    initGUI();
}

NewViewDialog::~NewViewDialog()
{
    delete widthBoxLabel;
    delete heightBoxLabel;
    delete widthBox;
    delete heightBox;
    delete buttonBox;
    delete mainLayout;
}

void NewViewDialog::initGUI()
{
    // Main layout
    mainLayout = new QGridLayout;

    // Layouts widgets.
    widthBoxLabel = new QLabel(tr("Width:"));
    mainLayout->addWidget(widthBoxLabel, 0, 0);

    heightBoxLabel = new QLabel(tr("Height:"));
    mainLayout->addWidget(heightBoxLabel, 1, 0);

    widthBox = new QSpinBox;
    widthBox->setMinimum(1);
    widthBox->setMaximum(9999);
    mainLayout->addWidget(widthBox, 0, 1);

    heightBox = new QSpinBox;
    heightBox->setMinimum(1);
    heightBox->setMaximum(9999);
    mainLayout->addWidget(heightBox, 1, 1);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewViewDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NewViewDialog::reject);
    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);
}
