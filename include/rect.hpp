#pragma once

struct Rect {
    enum class Type {
        Pixels, Percent
    };

    Type type = Type::Pixels;
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;

    static Rect pixels(float x, float y, float width, float height) noexcept;
    static Rect percent(float x, float y, float width, float height) noexcept;

    Rect toPixels(float width, float height) const noexcept;
    Rect toPercent(float width, float height) const noexcept;
};