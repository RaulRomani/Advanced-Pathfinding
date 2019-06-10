#ifndef H_LOCATION
#define H_LOCATION
#include <ostream>
#include <math.h>

using namespace std;

class Location {
private:
  double x;
  double y;

public:

  Location() {
    this->x = 0.0;
    this->y = 0.0;
  }

  Location(double x, double y) {
    this->x = x;
    this->y = y;
  }

  void copy(Location location) {
    x = location.x;
    y = location.y;
  }

  float dist(const Location &location) {
    return sqrt( pow(x - location.getX(),2) + pow(y - location.getY(),2));
  }

  Location middle(const Location &location) {
    Location loc( (x + location.getX())/2, (y + location.getY())/2);
    return  loc;
  }

  double length() const {
    return sqrt(x*x + y*y);
  }


  bool isEquals(Location location) {
    return (x == location.x && y == location.y);
  }

  bool isEmpty() {
    return (x == 0.0 && y == 0.0);
  }

  double getX() const {
    return x;
  }

  void setX(double x) {
    this->x = x;
  }

  const double getY() const {
    return y;
  }

  void setY(double y) {
    this->y = y;
  }

  inline bool operator!=(Location b) {
    return x != b.x && y != b.y;
  }

  friend ostream& operator<<(ostream& outputStream, const Location& location) {
    return outputStream << "(" << location.getX() << ", " << location.getY() << ")";
  };

//  friend ostream& operator<<(ostream& outputStream, const Location& location) {
//    return outputStream << location.getX() << " " << location.getY();
//  };

};

struct LocationCompare {

  bool operator()(const Location& l, const Location& r) const {
    return l.length() < r.length();
  }
};


#endif

