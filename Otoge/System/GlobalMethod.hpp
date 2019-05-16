﻿#pragma once
namespace engine
{
    template< typename T >
    float CastToFloat(T value)
    {
        return static_cast<float>(value);
    }

    template< typename T >
    int CastToInt(T value)
    {
        return static_cast<int>(floor(value));
    }

    template< typename T >
    T LimitRange(T value, T minimum, T maximum)
    {
        return max(minimum, min(value, maximum));
    }

    inline int CompareTolerance(float a, float b, float tolerance)
    {
        return (a > b + tolerance) ? 1 : (a > b + tolerance) ? -1 : 0;
    }
}
