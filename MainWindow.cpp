/**
 * @file MainWindow.cpp
 * @brief Реализация методов класса MainWindow.
 */

#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QSettings>
#include <QFile>
#include <QTextStream>

/**
 * @brief Конструктор класса MainWindow.
 *
 * Инициализирует пользовательский интерфейс и загружает настройки.
 * @param parent Родительский виджет (по умолчанию nullptr).
 */

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), systemOn(false) {
    setupUI();
    loadSettings();
    airDirectionSlider->setValue(airdirection);
    started = true;
}

/**
 * @brief Деструктор класса MainWindow.
 *
 * Сохраняет настройки перед завершением работы.
 */

MainWindow::~MainWindow() {
    saveSettings();
}

/**
 * @brief Настройка графического интерфейса пользователя.
 */

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Air Direction
    airDirectionLabel = new QLabel(QString("Air direction(degrees from normal): %1").arg(airdirection), this);

    airDirectionSlider = new QSlider(Qt::Horizontal, this);
    airDirectionSlider->setRange(-45, 45);
    connect(airDirectionSlider, &QSlider::valueChanged, this, &MainWindow::updateAirDirection);

    // Temperature
    temperatureLabel = new QLabel(QString("Temperature: %1").arg(temperature), this);

    temperatureUnit = new QComboBox(this);
    temperatureUnit->addItems({"°C", "°F", "K"});
    connect(temperatureUnit, &QComboBox::currentTextChanged, this, &MainWindow::updateTemperatureUnit);

    temperatureSlider = new QSlider(Qt::Horizontal, this);
    //temperatureSlider->setRange(15, 30);
    connect(temperatureSlider, &QSlider::valueChanged, this, &MainWindow::updateTemperature);


    // Humidity
    humidityLabel = new QLabel(QString("Humidity: %1 %").arg(humidity), this);

    // Pressure
    pressureLabel = new QLabel(QString("Pressure: %1").arg(pressure), this);
    pressureUnit = new QComboBox(this);
    pressureUnit->addItems({"Pascal (Pa)", "mmHg"});
    connect(pressureUnit, &QComboBox::currentTextChanged, this, &MainWindow::updatePressureUnit);

    // Power Button
    powerButton = new QPushButton("Turn System ON", this);
    connect(powerButton, &QPushButton::clicked, this, &MainWindow::toggleSystem);

    // Themes
    darkThemeButton = new QPushButton("Dark Theme", this);
    lightThemeButton = new QPushButton("Light Theme", this);
    connect(darkThemeButton, &QPushButton::clicked, [=]() { changeTheme(true); });
    connect(lightThemeButton, &QPushButton::clicked, [=]() { changeTheme(false); });

    // Graphics Scene
    scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    scenetext = scene->addText("System Status: OFF");

    // Layout Management

    mainLayout->addWidget(airDirectionLabel);
    mainLayout->addWidget(airDirectionSlider);

    mainLayout->addWidget(temperatureLabel);
    mainLayout->addWidget(temperatureSlider);
    mainLayout->addWidget(temperatureUnit);

    mainLayout->addWidget(humidityLabel);
    mainLayout->addWidget(pressureLabel);
    mainLayout->addWidget(pressureUnit);

    mainLayout->addWidget(powerButton);
    mainLayout->addWidget(darkThemeButton);
    mainLayout->addWidget(lightThemeButton);
    mainLayout->addWidget(view);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    changeTheme(true);
}

/**
 * @brief Включение или выключение системы.
 */

void MainWindow::toggleSystem() {
    systemOn = !systemOn;
    powerButton->setText(systemOn ? "Turn System OFF" : "Turn System ON");
    scene->clear();
    scene->addText(systemOn ? "System Status: ON" : "System Status: OFF");
}

/**
 * @brief Изменение темы приложения.
 *
 * @param dark Если true, применяется тёмная тема; иначе светлая.
 */

void MainWindow::changeTheme(bool dark) {
    QFile styleFile(dark ? "C://Users//artem//CLionProjects//radarmmstest//dark.qss" : "C://Users//artem//CLionProjects//radarmmstest//light.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        if (!dark) {
            scenetext->setDefaultTextColor(Qt::black);
        }else{
            scenetext->setDefaultTextColor(Qt::white);
        }
        QString style = QTextStream(&styleFile).readAll();
        qApp->setStyleSheet(style);
    }
}

/**
 * @brief Загрузка пользовательских настроек из XML-файла.
 */

void MainWindow::loadSettings() {
    int temp;
    QFile xmlfile("C://Users//artem//CLionProjects//radarmmstest//settings.xml");
    if (xmlfile.open(QFile::ReadOnly)) {
        QXmlStreamReader xmlReader(&xmlfile);
        xmlReader.setDevice(&xmlfile);
        xmlReader.readNext();
        while(!xmlReader.atEnd()){
            if(xmlReader.isStartElement()){
                if(xmlReader.name() == "temperature"){
                    temperature = xmlReader.readElementText().toFloat();
                }else if(xmlReader.name() == "humidity"){
                    humidity = xmlReader.readElementText().toFloat();
                }else if(xmlReader.name() == "pressure"){
                    pressure = xmlReader.readElementText().toFloat();
                }else if(xmlReader.name() == "airdirection"){
                    airdirection = xmlReader.readElementText().toFloat();
                }else if(xmlReader.name() == "temperatureUnit"){
                    temperatureUnit->setCurrentText(xmlReader.readElementText());
                }else if(xmlReader.name() == "pressureUnit"){
                    pressureUnit->setCurrentText(xmlReader.readElementText());
                }
            }
            xmlReader.readNext();
        }
        xmlfile.close();
        humidityLabel->setText(QString("Humidity: %1 %").arg(humidity));
        pressureLabel->setText(QString("Pressure: %1").arg(pressure));
        if (temperatureUnit->currentText() == "°C") {
            temp = temperature;
            temperatureSlider->setRange(15, 30);
            temperatureSlider->setValue(temp);
        }

        if (temperatureUnit->currentText() == "°F") {
            temp = temperature;
            temperatureSlider->setRange(59, 86);
            temperatureSlider->setValue(temp);
        }

        if (temperatureUnit->currentText() == "K") {
            temp = temperature;
            temperatureSlider->setRange(288, 303);
            temperatureSlider->setValue(temp);
        }
    }
}

/**
 * @brief Сохранение пользовательских настроек в XML-файл.
 */

void MainWindow::saveSettings() {

    QFile xmlfile("C://Users//artem//CLionProjects//radarmmstest//settings.xml");
    if (xmlfile.open(QFile::WriteOnly)) {
        QXmlStreamWriter xmlWriter(&xmlfile);
        xmlWriter.setAutoFormatting(true);  // Устанавливаем автоформатирование текста
        xmlWriter.writeStartDocument();     // Запускаем запись в документ
        xmlWriter.writeStartElement("settings");

        xmlWriter.writeStartElement("temperature");
        xmlWriter.writeCharacters(std::to_string(temperature));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("humidity");
        xmlWriter.writeCharacters(std::to_string(humidity));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("pressure");
        xmlWriter.writeCharacters(std::to_string(pressure));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("airdirection");
        xmlWriter.writeCharacters(std::to_string(airdirection));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("temperatureUnit");
        xmlWriter.writeCharacters(temperatureUnit->currentText());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("pressureUnit");
        xmlWriter.writeCharacters(pressureUnit->currentText());
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        xmlfile.close();
    }
}

/**
 * @brief Обновление отображаемой температуры.
 */

void MainWindow::updateTemperature() {
    temperature = temperatureSlider->value();
    temperatureLabel->setText(QString("Temperature: %1").arg(temperature));
}

/**
 * @brief Обновление отображаемого направления воздуха.
 */

void MainWindow::updateAirDirection() {
    airdirection = airDirectionSlider->value();
    airDirectionLabel->setText(QString("Air direction(degrees from normal): %1").arg(airdirection));
}

/**
 * @brief Обновление единиц измерения температуры.
 */

void MainWindow::updateTemperatureUnit() {
    int temp;
    if(started) {
        if (temperatureUnit->currentText() == "°C") {
            if (temperature >= 15 && temperature <= 30) {
                return;
            }

            if (temperature >= 59 && temperature <= 86) {
                temp = (temperature - 32) * 5 / 9;
                temperatureSlider->setRange(15, 30);
                temperatureSlider->setValue(temp);
            }

            if (temperature >= 288 && temperature <= 303) {
                temp = temperature - 273;
                temperatureSlider->setRange(15, 30);
                temperatureSlider->setValue(temp);
            }
        }

        if (temperatureUnit->currentText() == "°F") {
            if (temperature >= 15 && temperature <= 30) {
                temp = (temperature * 9 / 5) + 32;
                temperatureSlider->setRange(59, 86);
                temperatureSlider->setValue(temp);
            }

            if (temperature >= 59 && temperature <= 86) {
                return;
            }

            if (temperature >= 288 && temperature <= 303) {
                temp = ((temperature - 273) * 9 / 5) + 32;
                temperatureSlider->setRange(59, 86);
                temperatureSlider->setValue(temp);
            }
        }

        if (temperatureUnit->currentText() == "K") {
            if (temperature >= 15 && temperature <= 30) {
                temp = temperature + 273;
                temperatureSlider->setRange(288, 303);
                temperatureSlider->setValue(temp);
            }

            if (temperature >= 59 && temperature <= 86) {
                temp = (temperature - 32) * 5 / 9 + 273;
                temperatureSlider->setRange(288, 303);
                temperatureSlider->setValue(temp);
            }

            if (temperature >= 288 && temperature <= 303) {
                return;
            }
        }
    }
}

/**
 * @brief Обновление единиц измерения давления.
 */

void MainWindow::updatePressureUnit() {
    if (started) {
        if (pressureUnit->currentText() == "Pascal (Pa)") {
            pressure = (pressure * 101325) / 760;
            pressureLabel->setText(QString("Pressure: %1").arg(pressure));
        }

        if (pressureUnit->currentText() == "mmHg") {
            pressure = (pressure * 760) / 101325;
            pressureLabel->setText(QString("Pressure: %1").arg(pressure));
        }
    }
}