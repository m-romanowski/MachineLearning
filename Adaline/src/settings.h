#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTextStream>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>

#include <string>

#define DEFAULT_SETTINGS_FILE_NAME "settings.ini"
#define DEFAULT_TRAINING_DATA_DIR "training_data"

struct SettingsData
{
    // Training
    QString trainingDataDir = QDir::currentPath() + "/" + DEFAULT_TRAINING_DATA_DIR;

    // Random noises
    bool enableRandom = false; // Enable random noise for each TrainingItem
    int noiseItemCount = 1; // One additional item per TrainingItem
    int noiseProbability = 1; // 1%
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget* _parent = nullptr);
    ~Settings();

    [[nodiscard]] SettingsData getSettingsData() const { return data; }

signals:
    void newSettings(SettingsData _data);

private slots:
    void openDirPath();
    void toggleRandomNoise(bool _checked);
    void noiseItemSizeChanged(int _value);
    void noiseProbabilityChanged(int _value);

private:
    void init();
    void readSettingsFile();
    void saveSettings();

    // Layouts
    QVBoxLayout* layout;
    QGridLayout* secondLayout;

    // Widgets
    // Train
    QLabel* trainDataLabel;
    QLineEdit* trainDirPath;
    QPushButton* trainDirChoosePath;

    // Random noise
    QGroupBox* noiseGroup;
    QGridLayout* noiseGroupLayout;
    QLabel* noiseItemSizeLabel;
    QLabel* noiseProbabilityLabel;
    QSpinBox* noiseItemSizeBox;
    QSpinBox* noiseProbabilityBox;

    SettingsData data;
};

#endif // SETTINGS_H
