#include "Projectile.hpp"


// constructor, which takes the projectile type, its x and y position, its target's x and y position and speed as parameters
Projectile::Projectile(const std::string type, float xPos, float yPos, float targetX, float targetY, float speed)
    : type_(type), xPos_(xPos), yPos_(yPos), targetX_(targetX), targetY_(targetY), speed_(speed), angle_(0.0f) {}

// destructor
Projectile::~Projectile() {}

// returns the x coordinate of the projectile as float
float Projectile::getX() const {
    return xPos_;
}

// returns the y coordinate of the projectile as float
float Projectile::getY() const {
    return yPos_;
}

// calculates the correct angle for the projectile so that it goes nicely towards the target enemy
// the calculated angle is assigned to float angle_
// returns nothing
void Projectile::setAngle() {
    float dx = targetX_ - xPos_;
    float dy = targetY_ - yPos_;
    float pythagora = std::sqrt(dx*dx+dy*dy);
    float angle = 0.0f;
    float angleInRadians;
    if((std::abs(dx) < 0.00001f) && (dy > 0)) { //down
        angle = 90.0f;
    }
    else if((std::abs(dx) < 0.00001f) && (dy < 0)) { //up
        angle = -90.0f;
    }
    else if((dx < 0) && (std::abs(dy) < 0.00001f)) { //left
        angle = 180.0f;
    }
    else if((dx > 0) && (std::abs(dy) < 0.00001f)) { //right
        angle = 0.0f;
    }
    else if((dx > 0) && (dy > 0)) {
        angleInRadians = std::acos(dx/pythagora);
        angle = angleInRadians*(180.0f/static_cast<float>(M_PI));
    }
    else if((dx > 0) && (dy < 0)) {
        angleInRadians = std::acos(dx/pythagora);
        angle = -angleInRadians*(180.0f/static_cast<float>(M_PI));
    }
    else if((dx < 0) && (dy > 0)) {
        angleInRadians = std::acos(dy/pythagora);
        angle = 90.0f+angleInRadians*(180.0f/static_cast<float>(M_PI));
    }
    else if((dx < 0) && (dy < 0)) {
        angleInRadians = std::acos(std::abs(dx)/pythagora);
        angle = -(180.0f-angleInRadians*(180.0f/static_cast<float>(M_PI)));
    }
    
    angle_ = angle;
}

// returns the angle of the projectile as float
float Projectile::getAngle() {
    return angle_;
}

// returns the distance to the projectile's target as float
float Projectile::getDistance() {
    float distance = 0.0f;
    float dx = targetX_ - xPos_;
    float dy = targetY_ - yPos_;
    float pythagora = std::sqrt(dx*dx+dy*dy);
    distance = pythagora;
    return distance;
}

// moves the projectile towards its target, returns 0 if the projectile needs to be destroyed and 1 if not
int Projectile::updatePosition(float deltaTime) {
    float step = speed_*deltaTime;
    float distance = getDistance(); // distance to target
    if(distance < 0.3) {
        return 0;
    }
    else if(step < distance) {
        xPos_ += speed_ * std::cos(angle_*(static_cast<float>(M_PI)/180.0f)) * deltaTime;
        yPos_ += speed_ * std::sin(angle_*(static_cast<float>(M_PI)/180.0f)) * deltaTime;
        return 1;
    }
    else {
        return 0;
    }
}

