#ifndef PEAK_H
#define PEAK_H


class Peak
{
public:
    double value {0.0};
    int index {-1};
    int margin_left {0};
    int margin_right {0};
public:
    Peak();
};

#endif // PEAK_H
