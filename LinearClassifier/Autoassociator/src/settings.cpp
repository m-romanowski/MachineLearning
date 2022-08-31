#include "settings.h"
#include <QDebug>

Settings::Settings(QWidget* _parent)
    : QDialog (_parent)
{
    this->setModal(true);
    this->setWindowTitle("Settings");

    readSettingsFile(); // Read settings file if exists.
    initGUI(); // Initialize GUI.

    // If window will be close then save settings data to file.
    connect(this, &QDialog::finished, this, &Settings::saveSettings);
}

Settings::~Settings()
{
    delete blockResizingLabel;
    delete blockResizingCheckBox;
    delete trainingDataDirLabel;
    delete trainDirPath;
    delete trainDirChoosePath;
    delete noiseProbabilityLabel;
    delete noiseProbabilityBox;
    delete mainOptGroupLayout;
    delete mainOptGroup;
    delete penWeightLabel;
    delete penWeightBox;
    delete penColorLabel;
    delete penColorPreview;
    delete editorGroupLayout;
    delete editorGroup;
    delete mainLayout;
}

void Settings::initGUI()
{
    // Main
    mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    // Main group
    mainOptGroup = new QGroupBox;
    mainOptGroup->setTitle(tr("Main"));
    mainLayout->addWidget(mainOptGroup);
    mainOptGroupLayout = new QGridLayout;
    mainOptGroupLayout->setSpacing(1);
    mainOptGroup->setLayout(mainOptGroupLayout);

    blockResizingLabel = new QLabel(tr("Block resizing (view):"));
    blockResizingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainOptGroupLayout->addWidget(blockResizingLabel, 0, 0);

    blockResizingCheckBox = new QCheckBox;
    blockResizingCheckBox->setChecked(data.blockResizing);
    blockResizingCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(blockResizingCheckBox, &QCheckBox::stateChanged, this, &Settings::blockResizingStateChanged);
    mainOptGroupLayout->addWidget(blockResizingCheckBox, 0, 1);

    trainingDataDirLabel = new QLabel(tr("Training data dir:"));
    trainingDataDirLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainOptGroupLayout->addWidget(trainingDataDirLabel, 1, 0);

    trainDirPath = new QLineEdit;
    trainDirPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    trainDirPath->setMaximumHeight(35);
    trainDirPath->setText(data.trainingDataDir);
    mainOptGroupLayout->addWidget(trainDirPath, 1, 1);

    trainDirChoosePath = new QPushButton(tr("..."));
    trainDirChoosePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    trainDirChoosePath->setMaximumHeight(35);
    connect(trainDirChoosePath, &QPushButton::clicked, this, &Settings::openDirPath);
    mainOptGroupLayout->addWidget(trainDirChoosePath, 1, 2);

    noiseProbabilityLabel = new QLabel(tr("Noise probability (%):"));
    noiseProbabilityLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainOptGroupLayout->addWidget(noiseProbabilityLabel, 2, 0);

    noiseProbabilityBox = new QSpinBox;
    noiseProbabilityBox->setMinimum(1);
    noiseProbabilityBox->setMaximum(100);
    noiseProbabilityBox->setValue(data.noiseProbability);
    noiseProbabilityBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(noiseProbabilityBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &Settings::noiseProbabilityChanged);
    mainOptGroupLayout->addWidget(noiseProbabilityBox, 2, 1);

    // Editor group
    editorGroup = new QGroupBox;
    editorGroup->setTitle(tr("Editor"));
    mainLayout->addWidget(editorGroup);
    editorGroupLayout = new QGridLayout;
    editorGroupLayout->setSpacing(1);
    editorGroup->setLayout(editorGroupLayout);

    penWeightLabel = new QLabel(tr("Pen weight:"));
    penWeightLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editorGroupLayout->addWidget(penWeightLabel, 0, 0);

    penWeightBox = new QSpinBox;
    penWeightBox->setMinimum(1);
    penWeightBox->setMaximum(100);
    penWeightBox->setValue(data.penWeight);
    penWeightBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(penWeightBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &Settings::penWeightChanged);
    editorGroupLayout->addWidget(penWeightBox, 0, 1);

    penColorLabel = new QLabel(tr("Pen color:"));
    penColorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editorGroupLayout->addWidget(penColorLabel, 1, 0);

    penColorPreview = new ClickableLabel;

    QPixmap pixColor(100, 35);
    pixColor.fill(data.penColor);

    penColorPreview->setPixmap(pixColor);
    penColorPreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editorGroupLayout->addWidget(penColorPreview, 1, 1);
    connect(penColorPreview, &ClickableLabel::clicked, this, &Settings::colorPicker);
}

void Settings::readSettingsFile()
{
    QString path = QDir::currentPath() + "/" + DEFAULT_SETTINGS_FILE_NAME;
    QFile file(path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(" ");

            if(fields[1].isEmpty())
                continue;

            if(fields[0].contains("PEN_WEIGHT"))
            {
                data.penWeight = std::stoi(fields[1].toStdString());
            }
            else if(fields[0].contains("PEN_COLOR"))
            {
                QStringList rgb = fields[1].split(",");

                if(!rgb.isEmpty())
                {
                    int r = std::stoi(rgb[0].toStdString());
                    int g = std::stoi(rgb[1].toStdString());
                    int b = std::stoi(rgb[2].toStdString());

                    if((r > 255 || r < 0) || (g > 255 || g < 0) || (b > 255 || b < 0))
                        continue;

                    data.penColor = QColor(r, g, b);
                }
            }
            else if(fields[0].contains("BLOCK_RESIZING"))
            {
                data.blockResizing = fields[1] == "1";
            }
            else if(fields[0].contains("TRAIN_DATA_DIR"))
            {
                data.trainingDataDir = fields[1];
            }
            else if(fields[0].contains("NOISE_PROBABILITY"))
            {
                data.noiseProbability = std::stoi(fields[1].toStdString());
            }
        }

        file.close();

        prevDataState = data;
        emit newSettings(data);
    }
}

void Settings::saveSettings()
{
    QString path = QDir::currentPath() + "/" + DEFAULT_SETTINGS_FILE_NAME;
    QFile file(path);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text) && data != prevDataState)
    {
        QTextStream out(&file);

        out << "PEN_WEIGHT " + QString::number(data.penWeight) + "\n";
        out << "PEN_COLOR " + QString::number(data.penColor.red()) + "," +
            QString::number(data.penColor.green()) + "," + QString::number(data.penColor.blue()) + "\n";
        out << "BLOCK_RESIZING " + QString(data.blockResizing ? "1" : "0") + "\n";
        out << "TRAIN_DATA_DIR " + data.trainingDataDir + "\n";
        out << "NOISE_PROBABILITY " + QString::number(data.noiseProbability);

        file.close();
    }
}

void Settings::penWeightChanged(int _value)
{
    data.penWeight = _value;
    emit newSettings(data);
}

void Settings::blockResizingStateChanged(bool _state)
{
    data.blockResizing = _state;
    emit newSettings(data);
}

void Settings::openDirPath()
{
    QString oldPath = data.trainingDataDir;

    QString path = QFileDialog::getExistingDirectory(this, tr("Open directory"),
        oldPath != "" ? oldPath : QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(path == "")
    {
        trainDirPath->setText(oldPath);
    }
    else
    {
        trainDirPath->setText(path);
        data.trainingDataDir = path;
    }

    emit newSettings(data);
}

void Settings::colorPicker()
{
    QColor newColor = QColorDialog::getColor(data.penColor);

    if(newColor != data.penColor)
    {
        QPixmap pixColor(100, 35);
        pixColor.fill(newColor);
        penColorPreview->setPixmap(pixColor);

        data.penColor = newColor;
        emit newSettings(data);
    }
}

void Settings::noiseProbabilityChanged(int _value)
{
    data.noiseProbability = _value;
    emit newSettings(data);
}
