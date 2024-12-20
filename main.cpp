/**
 * @file main.cpp
 * @brief Главный файл программы, запускающий приложение.
 */

#include <QApplication>
#include "MainWindow.h"

/**
 * @brief Главная функция приложения.
 *
 * Эта функция инициализирует объект QApplication, создаёт главное окно приложения
 * и запускает основной цикл обработки событий.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @return Код завершения приложения (0 при успешном завершении).
 */

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); /**< Объект для управления жизненным циклом приложения. */

    MainWindow window; /**< Главное окно приложения. */
    window.show(); /**< Отображение главного окна. */

    return app.exec(); /**< Запуск основного цикла обработки событий. */
}
