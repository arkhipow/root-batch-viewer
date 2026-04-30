#include "rect.hpp"

Rect Rect::pixels(float x, float y, float width, float height) noexcept {
    return { Type::Pixels, x, y, width, height };
}

Rect Rect::percent(float x, float y, float width, float height) noexcept {
    return { Type::Percent, x, y, width, height };
}

Rect Rect::toPixels(float width, float height) const noexcept {
    if (type == Type::Pixels) {
        return *this;
    }

    return Rect::pixels(
        x * width / 100,
        y * height / 100,
        this->width * width / 100,
        this->height * height / 100
    );
}

Rect Rect::toPercent(float width, float height) const noexcept {
    if (type == Type::Percent) {
        return *this;
    }

    return Rect::percent(
        x * 100 / width,
        y * 100 / height,
        this->width * 100 / width,
        this->height * 100 / height
    );
}