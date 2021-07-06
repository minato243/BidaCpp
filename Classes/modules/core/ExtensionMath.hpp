#ifndef __EXT_MATH__
#define __EXT_MATH__
#include <vector>

#define POS_INFINITY DBL_MAX
#define NEG_INFINITY -DBL_MAX

namespace ps {
    namespace ExtMath {
        class vector;
        class plane;
        class quaternion;
        class segment;

        extern const double PI;
        extern const double TWO_PI;

        bool isZeroNumber(double num);

        bool isLessThanZeroNumber(double num);

        bool isGreatThanZeroNumber(double num);

        double acos(double value);

        double asin(double value);

        double cos(double value);

        double sin(double value);

        double minifyAngle(double value);

        class vector {
        public:
            vector();
            vector(double x, double y);
            vector(double x, double y, double z);
            vector(const vector & v);

            double x;
            double y;
            double z;

            vector & operator=(const vector & v);

            void reverse();
            void fixZeroVectorPrecision();
            void rotate2D(double angle);
            void applyQuaternion(const quaternion & q);
            void assignZero();

            // Immutable operators
            static bool equals(const vector & u, const vector & v);

            static bool equalsZero(const vector & u);

            static double dot(const vector & u, const vector & v);

            static vector cross(const vector & u, const vector & v);

            static double crossZ(const vector & u, const vector & v);

            static vector add(const vector & u, const vector & v);

            static vector add(const vector & u, const vector & v, const vector & w);

            static vector sub(const vector & u, const vector & v);

            static vector multiply(double n, const vector & u);

            static vector reverse(const vector & u);

            // Project vector u on vector v
            static vector project(const vector & u, const vector & v);

            static double length(const vector & v);

            static double lengthSquare(const vector & v);

            static vector unit(const vector & v);

            static vector fromSegment(const segment & seg);

            static vector reflect(const vector & dir, const vector & nor);

            static double distance(const vector & u, const vector & v);

            static double distanceSquare(const vector & u, const vector & v);

            static vector applyQuaternion(const vector & v, const quaternion & q);

            static vector mix(const vector & u, const vector & v, double range);
        };

        class plane {
        public:
            plane(const vector & p, const vector & n);

            vector p;
            vector n;

            // Immutable operators
            static vector projectPoint(const vector & q, const plane & pl);

            static vector projectVector(const vector & v, const plane & pl);
        };

        class quaternion {
        public:
            quaternion();
            quaternion(double w, double x, double y, double z);
            quaternion(const vector & v); // From Vector

            double w;
            double x;
            double y;
            double z;

            void reverse();

            // Immutable operators
            static quaternion multiply(const quaternion & p, const quaternion & q);

            static quaternion reverse(const quaternion & q);

            static bool equals(const quaternion & p, const quaternion & q);
        };

        class segment {
        public:
            segment(const vector & s, const vector & e);

            vector s;
            vector e;

            // Immutable operators
            static bool projectPoint(const vector & p, const segment & seg, vector & output);

            static vector projectPointOnLine(const vector & p, const segment & seg);

            static double distance(const vector & p, const segment & seg);

            static double distanceSquare(const vector & p, const segment & seg);

            static bool isCoPlanar(const segment & s, const segment & t);

            static bool differentSide(const segment & seg, const vector & p1, const vector & p2);

            static bool intersect(const segment & s, const segment & t);
        };

        class Shape {
        public:
            virtual bool checkPointInside(const vector & p) = 0;
        };

        class AABB : public Shape {
        public:
            double minX = 0;
            double minY = 0;
            double maxX = 0;
            double maxY = 0;

            bool checkPointInside(const vector & p) override;
        };

        class Polygon2D : public Shape {
        public:
            Polygon2D();
            bool checkPointInside(const vector & p) override;
            std::vector<vector> & polygon();

        private:
            static bool ccw(const vector & a, const vector & b, const vector & c);

            static bool intersection(const vector & a, const vector & b, const vector & c, const vector & d);
        private:
            std::vector<vector> points;
            static const vector OUTSIDE_ORIGIN;
        };
    };
}
#endif
