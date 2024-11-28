/**
 * @file MainWindow.h
 * @brief Заголовочный файл главного окна приложения.
 */

 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QString>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsTextItem>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>



/**
 * @class MainWindow
 * @brief Класс главного окна приложения для управления системой кондиционирования.
 *
 * Этот класс предоставляет графический интерфейс пользователя (GUI), позволяющий:
 * - отображать параметры температуры, влажности и давления,
 * - управлять состоянием системы кондиционирования,
 * - переключать режимы отображения (светлая/тёмная тема),
 * - сохранять и загружать настройки пользователя.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    /**
     * @brief Конструктор класса MainWindow.
     *
     * Инициализирует главное окно приложения и настраивает пользовательский интерфейс.
     *
     * @param parent Родительский виджет (по умолчанию nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса MainWindow.
     *
     * Уничтожает все динамически выделенные ресурсы.
     */
    ~MainWindow();

private:
    // Поля
    float temperature;          /**< Текущая температура. */
    int humidity;               /**< Текущая влажность. */
    float pressure;             /**< Текущее давление. */
    int airdirection;           /**< Направление воздуха. */
    bool started = false;       /**< Флаг состояния системы (включена/выключена). */

    // Методы
    /**
     * @brief Настройка пользовательского интерфейса.
     */
    void setupUI();

    /**
     * @brief Загрузка пользовательских настроек из XML-файла.
     */
    void loadSettings();

    /**
     * @brief Сохранение пользовательских настроек в XML-файл.
     */
    void saveSettings();

    /**
     * @brief Включение или выключение системы кондиционирования.
     */
    void toggleSystem();

    /**
     * @brief Изменение темы интерфейса.
     *
     * @param dark Если true, переключение на тёмную тему; если false, на светлую.
     */
    void changeTheme(bool dark);

    /**
     * @brief Обновление отображаемой температуры.
     */
    void updateTemperature();

    /**
     * @brief Обновление отображаемого направления потока воздуха.
     */
    void updateAirDirection();

    /**
     * @brief Обновление единиц измерения температуры.
     */
    void updateTemperatureUnit();

    /**
     * @brief Обновление единиц измерения давления.
     */
    void updatePressureUnit();

    // Компоненты пользовательского интерфейса
    QGraphicsScene *scene;               /**< Графическая сцена для отображения состояния системы. */
    QGraphicsTextItem *scenetext;        /**< Текстовый элемент графической сцены. */

    QLabel *temperatureLabel;           /**< Метка для отображения температуры. */
    QLabel *humidityLabel;              /**< Метка для отображения влажности. */
    QLabel *pressureLabel;              /**< Метка для отображения давления. */
    QLabel *airDirectionLabel;          /**< Метка для отображения направления воздуха. */

    QComboBox *temperatureUnit;         /**< Выпадающий список для выбора шкалы температуры. */
    QComboBox *pressureUnit;            /**< Выпадающий список для выбора единиц измерения давления. */

    QPushButton *powerButton;           /**< Кнопка включения/выключения системы. */
    QPushButton *darkThemeButton;       /**< Кнопка переключения на тёмную тему. */
    QPushButton *lightThemeButton;      /**< Кнопка переключения на светлую тему. */

    QSlider *temperatureSlider;         /**< Слайдер для настройки температуры. */
    QSlider *airDirectionSlider;        /**< Слайдер для настройки направления воздуха. */

    bool systemOn;                      /**< Флаг состояния системы (работает или нет). */
};

#endif // MAINWINDOW_H
