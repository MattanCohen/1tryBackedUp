#include "../include/Workout.h"
using namespace std;

//rule of 5:
//d-tor in case Workout reference is deleted
Workout::~Workout() {
    this->stole();
}
//we can not implement move c-tor ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
Workout& Workout::operator=(const Workout& rhs){return *this;}
//copy c-tor:
Workout::Workout(const Workout& rhs):id(rhs.id),name(rhs.name),price(rhs.price),type(rhs.type){}
// fixed function
void Workout::stole() {delete this;}

//C-tor
Workout::Workout(int w_id,string w_name, int w_price, WorkoutType w_type):id(w_id),name(w_name),price(w_price),type(w_type){}
//returns ID
int Workout::getId() const {return id;}

//returns name
string Workout::getName() const {return name;}

//returns price
int Workout::getPrice() const {return price;}

//returns type
WorkoutType Workout::getType() const {return type;}

//since all values are const
//copy c-tor, d-tor,
