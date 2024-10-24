#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-9
#define MAX_CONSTRAINT 1e5

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

typedef struct
{
    double start, end;
} Interval;

typedef struct
{
    double x, y;
    double s;
    double vx, vy;
} Square;

typedef struct
{
    double x, y;
} Point;

Interval get_overlap(const Square *squareA, const Square *squareB, char axis)
{
    Interval result = {0, MAX_CONSTRAINT};
    double x1, v1, x2, v2;

    switch (axis)
    {
    case 'x':
        x1 = squareA->x;
        v1 = squareA->vx;
        x2 = squareB->x;
        v2 = squareB->vx;
        break;
    case 'y':
        x1 = squareA->y;
        v1 = squareA->vy;
        x2 = squareB->y;
        v2 = squareB->vy;
        break;
    default:
        printf("skibidi\n");
        fflush(stdout);
        *(int *)2077 = 1984;
        break;
    }

    double s1 = squareA->s;
    double s2 = squareB->s;
    double rel_v = v1 - v2;

    if (fabs(rel_v) < EPS)
    {
        if (x1 <= x2 + s2 && x2 <= x1 + s1)
            return result;
        result.start = MAX_CONSTRAINT;
        result.end = 0;
        return result;
    }

    double t1 = (x2 - x1 - s1) / rel_v;
    double t2 = (x2 + s2 - x1) / rel_v;

    if (t1 > t2)
    {
        double temp = t1;
        t1 = t2;
        t2 = temp;
    }

    result.start = MAX(result.start, t1);
    result.end = MIN(result.end, t2);
    return result;
}

Interval intersect_intervals(const Interval *a, const Interval *b)
{
    return (Interval){
        .start = MAX(a->start, b->start),
        .end = MIN(a->end, b->end)};
}

double overlap_length(double x1, double s1, double x2, double s2)
{
    return MAX(0, MIN(x1 + s1, x2 + s2) - MAX(x1, x2));
}

Point get_position_at_time(const Square *obj, double t)
{
    return (Point){
        .x = obj->x + obj->vx * t,
        .y = obj->y + obj->vy * t};
}

double get_area_at_time(const Square *red, const Square *blue, double t)
{
    Point red_pos = get_position_at_time(red, t);
    Point blue_pos = get_position_at_time(blue, t);

    double x_overlap = overlap_length(red_pos.x, red->s, blue_pos.x, blue->s);
    double y_overlap = overlap_length(red_pos.y, red->s, blue_pos.y, blue->s);

    return x_overlap * y_overlap;
}

double process(const Square *red, const Square *blue)
{
    Interval x_overlap = get_overlap(red, blue, 'x');
    Interval y_overlap = get_overlap(red, blue, 'y');
    Interval overlap = intersect_intervals(&x_overlap, &y_overlap);

    if (overlap.start > overlap.end || overlap.end < 0)
        return 0.0;
    if (overlap.start < 0)
        overlap.start = 0;

    double left = overlap.start;
    double right = overlap.end;

    while (right - left > EPS)
    {
        double m1 = left + (right - left) / 3;
        double m2 = right - (right - left) / 3;
        double a1 = get_area_at_time(red, blue, m1);
        double a2 = get_area_at_time(red, blue, m2);
        if (a1 < a2)
            left = m1;
        else
            right = m2;
    }

    return (left + right) / 2;
}

int main()
{
    int T;
    scanf("%d", &T);

    while (T--)
    {
        Square red, blue;
        scanf("%lf %lf %lf %lf %lf",
              &red.x, &red.y, &red.s, &red.vx, &red.vy);
        scanf("%lf %lf %lf %lf %lf",
              &blue.x, &blue.y, &blue.s, &blue.vx, &blue.vy);
        double result = process(&red, &blue);
        printf("%.3f\n", result);
    }

    return 0;
}
