        // glDrawArrays(GL_TRIANGLE_FAN, 0, object->get_vertices_count());

// class Circle{
//     Point2D center;
//     float radius;

//     public:
//         inline Circle(): radius(1.0f){}
//         inline Circle(const Point2D& c, const float r): center(c), radius(r){}
//         bool contains(const Point2D& p);
//         bool contains(Line2D line);
//         bool contains(const Circle& other);
// };

// class Rectangle{
//     Point2D origin;
//     vec2 size;

//     public:
//         inline Rectangle(): size(1., 1.){}
//         inline Rectangle(const Point2D& origin, const vec2& size): origin(origin), size(size){}

//         Rectangle from_minmax(Point2D min, Point2D max);
//         vec2 get_min();
//         vec2 get_max();
//         bool contains(const Point2D& p);
//         bool contains(Line2D line);
// };

// class OrientedRectangle{
//     Point2D center;
//     vec2 half_extents;
//     float rotation;

//     public:
//         inline OrientedRectangle(): half_extents(1., 1.), rotation(0.0){}
//         inline OrientedRectangle(const Point2D& center, const vec2& half_extents)
//         : center(center), half_extents(half_extents), rotation(0.0){}
//         inline OrientedRectangle(const Point2D& center, const vec2& half_extents, float rotation)
//         : center(center), half_extents(half_extents), rotation(rotation){}
//         bool contains(Point2D p);
//         bool contains(Line2D line);
// };


// bool Circle::collide(Line2D line){
//     return false;
// }

// bool Circle::contains(Point2D point){
//     vec2 diff = this->center - point;
//     float mag = diff.mag();
//     float dist = mag * mag - this->radius * this->radius;
//     return dist <= 0.0;
// }


// Rectangle Rectangle::from_minmax(Point2D min, Point2D max) {
//     return Rectangle(min, max - min);
// }

// vec2 Rectangle::get_min() {
//     vec2 a = this->origin;
//     vec2 b = this->origin + this->size;
//     return vec2(
//         fminf(a.x, b.x),
//         fminf(a.y, b.y)
//     );
// }

// vec2 Rectangle::get_max() {
//     vec2 a = this->origin;
//     vec2 b = this->origin + this->size;
//     return vec2(
//         fmaxf(a.x, b.x),
//         fmaxf(a.y, b.y)
//     );
// }

// bool Rectangle::contains(const Point2D& p) {
//     vec2 min = this->get_min();
//     vec2 max = this->get_max();
//     return min.x <= p.x && min.y <= p.y && max.x >= p.x && max.y >= p.y;
// }

// bool Rectangle::contains(Line2D line) {
//     if (this->contains(line.start) || this->contains(line.end)) return true;

//     vec2 norm = (line.end - line.start).normalized();
//     norm.x = (norm.x != 0) ? 1.0f / norm.x : 0;
//     norm.y = (norm.y != 0) ? 1.0f / norm.y : 0;

//     vec2 min = (this->get_min() - line.start) * norm;
//     vec2 max = (this->get_max() - line.start) * norm;

//     float tmin = fmaxf(
//         fminf(min.x, max.x),
//         fminf(min.y, max.y)
//     );
//     float tmax = fminf(
//         fmaxf(min.x, max.x),
//         fmaxf(min.y, max.y)
//     );
//     if (tmax < 0 || tmin > tmax) return false;
//     float t = (tmin < 0.0f) ? tmax : tmin;
//     return t > 0.0f && t < line.length();
// }


// bool Circle::contains(const Point2D& p) {
//     vec2 diff = this->center - p;
//     float mag = diff.mag();
//     float dist = mag * mag - this->radius * this->radius;
//     return dist <= 0.0;
// }

// bool Circle::contains(Line2D line) {
//     vec2 ab = line.end - line.start;
//     float t = ab.dot(this->center - line.start) / ab.dot(ab);
//     if (t < 0.0f || t > 1.0f) {
//         return false;
//     }
//     Point2D closestPoint = line.start + ab * t;
//     Line2D circleToClosest(this->center, closestPoint);
//     return circleToClosest.length() < this->radius;
// }

// bool Circle::contains(const Circle& other){
//     Line2D line(this->center, other.center);
//     return line.length() < this->radius + other.radius;
// }

// bool OrientedRectangle::contains(Point2D p) {
//     vec2 rot = p - this->center;
//     float radians = -DEG2RAD(this->rotation);
//     float x1 = rot.x * cosf(radians) - rot.y * sinf(radians);
//     float y1 = rot.x * sinf(radians) + rot.y * cosf(radians);

//     Rectangle r(Point2D(), this->half_extents * 2.0);
//     vec2 local_p(x1 + this->half_extents.x, y1 + this->half_extents.y);
//     return r.contains(local_p);
// }

// bool OrientedRectangle::contains(Line2D line){
//     float theta = -DEG2RAD(this->rotation);
//     Line2D local_line;

//     vec2 rot = line.start - this->center;
//     rot = vec2(
//         rot.x * cosf(theta) - rot.y * sinf(theta),
//         rot.x * sinf(theta) + rot.y * cosf(theta)
//     );
//    local_line.start = rot + this->half_extents;

//     rot = line.end - this->center;
//     rot = vec2(
//         rot.x * cosf(theta) - rot.y * sinf(theta),
//         rot.x * sinf(theta) + rot.y * cosf(theta)
//     );
//     local_line.end = rot + this->half_extents;

//     Rectangle local_rect(Point2D(), this->half_extents * 2.0f);
//     return local_rect.contains(local_line);
// }
