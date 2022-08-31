#include "settings.h"

Settings::Settings(QWidget* _parent)
    : QDialog (_parent)
{
    this->setModal(true);
    this->setWindowTitle(tr("Settings"));

    init();
    readSettingsFile();

    connect(this, &QDialog::finished, this, &Settings::saveSettings);
}

Settings::~Settings()
{
    delete trainDirPath;
    delete trainDataLabel;
    delete trainDirChoosePath;
    delete secondLayout;
    delete layout;
}

void Settings::init()
{
    // Layouts
    layout = new QVBoxLayout(this);

    secondLayout = new QGridLayout;
    secondLayout->setSpacing(1);

    layout->addLayout(secondLayout);

    // Widgets

    // Training
    trainDataLabel = new QLabel(tr("Training data dir:"));

    trainDirPath = new QLineEdit;
    trainDirPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    trainDirPath->setMaximumHeight(35);

    trainDirChoosePath = new QPushButton(tr("..."));
    trainDirChoosePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    trainDirChoosePath->setMaximumHeight(35);
    connect(trainDirChoosePath, &QPushButton::clicked, this, &Settings::openDirPath);

    // Noise
    noiseGroupLayout = new QGridLayout;
    noiseGroupLayout->setSpacing(1);

    noiseGroup = new QGroupBox(tr("Random noise"));
    noiseGroup->setCheckable(true);
    noiseGroup->setChecked(false);
    connect(noiseGroup, &QGroupBox::clicked, this, &Settings::toggleRandomNoise);

    noiseItemSizeLabel = new QLabel(tr("Additional items:"));
    noiseItemSizeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    noiseItemSizeBox = new QSpinBox;
    noiseItemSizeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    noiseItemSizeBox->setMinimum(1);
    connect(noiseItemSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &Settings::noiseItemSizeChanged);

    noiseProbabilityLabel = new QLabel(tr("Probability (% to change pixel color):"));
    noiseItemSizeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    noiseProbabilityBox = new QSpinBox;
    noiseProbabilityBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    noiseProbabilityBox->setMinimum(1);
    noiseProbabilityBox->setMaximum(100);
    connect(noiseProbabilityBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &Settings::noiseProbabilityChanged);

    noiseGroupLayout->addWidget(noiseItemSizeLabel, 0, 0);
    noiseGroupLayout->addWidget(noiseItemSizeBox, 0, 1);
    noiseGroupLayout->addWidget(noiseProbabilityLabel, 1, 0);
    noiseGroupLayout->addWidget(noiseProbabilityBox, 1, 1);

    noiseGroup->setLayout(noiseGroupLayout);

    secondLayout->addWidget(trainDataLabel, 0, 0);
    secondLayout->addWidget(trainDirPath, 0, 1);
    secondLayout->addWidget(trainDirChoosePath, 0, 2);
    layout->addWidget(noiseGroup);

    this->setLayout(layout);
}

void Settings::readSettingsFile()
{
    QFile file(QDir::currentPath() + "/" + DEFAULT_SETTINGS_FILE_NAME);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(" ");

            if(fields[0].contains("TRAIN_DATA_DIR"))
            {
                trainDirPath->setText(fields[1]);
                data.trainingDataDir = fields[1];
            }
            else if(fields[0].contains("ENABLE_RANDOM_NOISE"))
            {
                bool isChecked = fields[1] == "1";
                noiseGroup->setChecked(isChecked);
                data.noiseItemCount = isChecked;
            }
            else if(fields[0].contains("RANDOM_NOISE_ITEMS_COUNT"))
            {
                noiseItemSizeBox->setValue(std::stoi(fields[1].toStdString()));
            }
            else if(fields[0].contains("RANDOM_NOISE_PROBABILITY"))
            {
                noiseProbabilityBox->setValue(std::stoi(fields[1].toStdString()));
            }
        }

        file.close();
    }
    else
    {
        saveSettings();
    }

    emit newSettings(data);
}

void Settings::openDirPath()
{
    QString oldPath = data.trainingDataDir;

    QString path = QFileDialog::getExistingDirectory(this, tr("Open directory"), oldPath != "" ? oldPath : QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

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

void Settings::saveSettings()
{
    QFile file(QDir::currentPath() + "/" + DEFAULT_SETTINGS_FILE_NAME);

    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(&file);

        // Save training data path to the settings file.
        out << "TRAIN_DATA_DIR " + data.trainingDataDir + "\n";

        QString enableNoiseValue = data.enableRandom ? "1" : "0";
        out << "ENABLE_RANDOM_NOISE " + enableNoiseValue + "\n";

        out << "RANDOM_NOISE_ITEMS_COUNT " + QString::number(data.noiseItemCount) + "\n";
        out << "RANDOM_NOISE_PROBABILITY " + QString::number(data.noiseProbability) + "\n";

        // Create training directory in default path (execute app path) if not exists.
        if(!QDir(data.trainingDataDir).exists())
            QDir().mkdir(data.trainingDataDir);

        trainDirPath->setText(data.trainingDataDir);

        file.close();
    }
}

void Settings::toggleRandomNoise(bool _checked)
{
    data.enableRandom = _checked;
    emit newSettings(data);
}

void Settings::noiseItemSizeChanged(int _value)
{
    data.noiseItemCount = _value;
    emit newSettings(data);
}

void Settings::noiseProbabilityChanged(int _value)
{
    data.noiseProbability = _value;
    emit newSettings(data);
}
