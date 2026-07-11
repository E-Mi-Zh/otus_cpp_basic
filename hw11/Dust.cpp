#include "Dust.hpp"
#include <cmath>

/**
 * Задает скорость объекта
 * @param velocity новое значение скорости
 */
void Dust::setVelocity(const Velocity& velocity) {
    // TODO: место для доработки
    this->velocity = velocity;
}

/**
 * @return скорость объекта
 */
Velocity Dust::getVelocity() const {
    // TODO: место для доработки
    return {velocity};
}

/**
 * @brief Выполняет отрисовку объекта
 * @details объект Dust абстрагирован от конкретного
 * способа отображения пикселей на экране. Он "знаком"
 * лишь с интерфейсом, который предоставляет Painter
 * Рисование выполняется путем вызова painter.draw(...)
 * @param painter контекст отрисовки
 */
void Dust::draw(Painter& painter) const {
    // TODO: место для доработки
    painter.draw(this->center, this->radius, this->color);
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Dust::setCenter(const Point& center) {
    // TODO: место для доработки
    this->center = center;
}

/**
 * @return центр объекта
 */
Point Dust::getCenter() const {
    // TODO: место для доработки
    return {center};
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется
 */
double Dust::getRadius() const {
    // TODO: место для доработки
    return {radius};
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все шары
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Dust::getMass() const {
    // TODO: место для доработки
    return {M_PI * radius * radius * radius * 4. / 3.};
}

/**
 * Задает цвет объекта
 * @param color новый цвет объекта
 */
void Dust::setColor(const Color& color) {
    // TODO: место для доработки
    this->color = color;
}

/**
 * Уменьшает время жизни частицы
  */
bool Dust::decay() {
    if (this->lifetime > 0) {
        this->lifetime--;
        return false;
    } else {
        return true;
    }
}


Dust::Dust(Point point, Velocity velocity, Color color, double radius) {
    this->radius = radius;
    this->lifetime = 3000;
    setCenter(point);
    setVelocity(velocity);
    setColor(color);
}