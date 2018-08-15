#include "City.h"


// Construtores
City::City(){
    this->x = 0;
    this->y = 0;
}

City::City(const double x, const double y):x(x),y(y){}

// Getters and Setters

double City::getX() const{return(x);}

double City::getY() const{return(y);}

void City::setX(const double x){this->x = x;}

void City::setY(const double y){this->y = y;}

// Overload de operadores

bool City::operator==(const City &c) const{  // Overload para comparação de city==city
    if((c.getX() == this->x) && (c.getY() == this->y) ){
        return(true);
    }else{
        return(false);
    }
}

ostream& operator<<(ostream &output,const City &c){  // Overload para impressão da população
    output<<setfill(' ')<<"x: "<<setw(6)<<c.getX()<<" y: "<<setw(6)<<c.getY();
    return(output);
}

const City& City::operator=(const City &c){  // Overload de atribuição city=city
    this->x = c.getX();
    this->y = c.getY();
}
