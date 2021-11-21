#include "../include/Customer.h"
#include "Workout.cpp"

using namespace std;


//rule of 5:
//d-tor in case Customer refrence is deleted
Customer::~Customer() {
    this->stole();
}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
Customer& Customer::operator=(const Customer &rhs) {return *this;}
//move ass.op. to overload any calls via other classes
Customer& Customer::operator=(Customer &&rhs) {return *this;}
//copy c-tor:
Customer::Customer(const Customer& rhs):name(rhs.name),id(rhs.id){}
//move c-tor
Customer::Customer(Customer &&rhs):name(rhs.name),id(rhs.id){
    if (this!=&rhs)
        rhs.stole();
}

void Customer::stole() {delete this;}



// Requested functions
Customer::Customer(string c_name, int c_id):name(c_name),id(c_id){};
string Customer:: getName() const {return name;};
int Customer::getId() const {return id;}




// Customer Types:


// swt: orders all cardio from beginning to end
//......................................class: SweatyCustomer
SweatyCustomer::SweatyCustomer(string name, int id):Customer(name,id){}

//rule of 5:
//d-tor in case Customer refrence is deleted
SweatyCustomer::~SweatyCustomer() {delete this;}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
SweatyCustomer& SweatyCustomer::operator=(const SweatyCustomer &rhs) {return *this;}
//move ass.op. to overload any calls via other classes
SweatyCustomer& SweatyCustomer::operator=(SweatyCustomer &&rhs) {return *this;}
//copy c-tor:
SweatyCustomer::SweatyCustomer(const SweatyCustomer &other): Customer(other){}
//move c-tor
SweatyCustomer::SweatyCustomer(SweatyCustomer &&rhs): Customer(rhs){
    if (this!=&rhs)
        rhs.stole();
}


// get swt order list, needs at least one Cardio in workout_options
vector<int> SweatyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> workouts_list;
    // iterate over all workout options
    for(int i=0; i<workout_options.size();i++){
        // add only cardio workouts
        if(workout_options.at(i).getType()==2) {workouts_list.push_back(i);}
    }
    // if no cardio, workouts_list will be empty
    return workouts_list;
}

//returns customer order strategy
string SweatyCustomer::toString() const {return "swt";}




//chp - cheapest workout orders only once
//......................................class: CheapCustomer
//rule of 5:
//d-tor in case Workout refrence is deleted
CheapCustomer::~CheapCustomer() {delete this;}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
CheapCustomer& CheapCustomer::operator=(const CheapCustomer& rhs){return *this;}
//move ass.op. to overload any calls via other classes
CheapCustomer& CheapCustomer::operator=(CheapCustomer &&rhs) {return *this;}
//copy c-tor:
CheapCustomer::CheapCustomer(const CheapCustomer &rhs): Customer(rhs) {}
//move c-tor
CheapCustomer::CheapCustomer(CheapCustomer &&rhs): Customer(rhs){
    if (this!=&rhs)
        rhs.stole();
}

//c-tor
CheapCustomer::CheapCustomer(string name, int id):Customer(name,id){}


// get chp order list, needs at least one order in workout_options
std::vector<int> CheapCustomer::order(const std::vector <Workout> &workout_options) {
    vector<int> orders;
    // save only id of cheapest workout
    int cheap_workout_id = 0;
    // iterate over all workout options
    for(int i=0; i<workout_options.size();i++){
        // add only cardio workouts
        if(workout_options.at(i).getPrice()<workout_options.at(cheap_workout_id).getPrice()){
            cheap_workout_id = i;
        }
    }
    // makes sure workout_options isn't empty
    if(workout_options.size()>0) {orders.push_back(cheap_workout_id);}
    // will return empty vector if workout_options is empty
    return orders;
}

string CheapCustomer::toString() const {return"chp";};




// mcl - only anaerobic from most expensive to cheapest
//......................................class: HeavyMuscleCustomer
//rule of 5:
//d-tor
HeavyMuscleCustomer::~HeavyMuscleCustomer() {delete this;}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
HeavyMuscleCustomer& HeavyMuscleCustomer::operator=(const HeavyMuscleCustomer& rhs){return *this;}
//move ass.op. to overload any calls via other classes
HeavyMuscleCustomer& HeavyMuscleCustomer::operator=(HeavyMuscleCustomer &&rhs) {return *this;}
//copy c-tor:
HeavyMuscleCustomer::HeavyMuscleCustomer(const HeavyMuscleCustomer &rhs):Customer(rhs){}
//move c-tor
HeavyMuscleCustomer::HeavyMuscleCustomer(HeavyMuscleCustomer &&rhs): Customer(rhs){
    if (this!=&rhs)
        rhs.stole();
}

HeavyMuscleCustomer::HeavyMuscleCustomer(string name, int id):Customer(name,id){}

// compares Workouts by price in descending order
// get mcl order list, needs at least one Anaerobic in workout_options
vector<int> HeavyMuscleCustomer::order(const vector<Workout> &workout_options) {
    vector<int> orders;
    // iterate over all workout options
    for(int i=0; i<workout_options.size();i++){
        // add only anaerobic workouts
        if(workout_options.at(i).getType()==0) {orders.push_back(i);}
    }
    /*
    under the assumption the workout options is sorted by price dsc,
     order list can be returned normally
    */
    return orders;
}

string HeavyMuscleCustomer::toString() const {return "mcl";}





// fbd - cheapest cardio,  expensive mix, cheapest anaerobic
//......................................class: FullBodyCustomer
//rule of 5:
//d-tor
FullBodyCustomer::~FullBodyCustomer() {delete this;}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass.op. sole purpose is to overload any calls to the ass.op via other classes
FullBodyCustomer& FullBodyCustomer::operator=(const FullBodyCustomer& rhs){return *this;}
//move ass.op. to overload any calls via other classes.
FullBodyCustomer& FullBodyCustomer::operator=(FullBodyCustomer &&rhs) {return *this;}
//copy c-tor:
FullBodyCustomer::FullBodyCustomer(const FullBodyCustomer &rhs): Customer(rhs){}
//move c-tor
FullBodyCustomer::FullBodyCustomer(FullBodyCustomer &&rhs): Customer(rhs){
    if (this!=&rhs)
        rhs.stole();
}


FullBodyCustomer::FullBodyCustomer(string name, int id):Customer(name,id){}

// get fbd order list, needs one workout of each kind in workout_options
vector<int> FullBodyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> orders;
    // default index values until a workout is found
    int cheap_cardio_index = -1;
    int exp_mix_index = -1;
    int cheap_ana_index = -1;
    // finds optimal workout for each workout type
    for (int i = 0; i < workout_options.size(); i++) {
        // anaerobic
        if (workout_options.at(i).getType() == 0) {
            // change default value
            if (cheap_ana_index == -1) { cheap_ana_index = i; }

            if (workout_options.at(i).getPrice() <
                workout_options.at(cheap_ana_index).getPrice()) {
                // current workout is cheaper
                cheap_ana_index = i;
            }
        }
        // mix
        else if (workout_options.at(i).getType() == 1) {
            // change default value
            if (exp_mix_index == -1) { exp_mix_index = i; }
            if (workout_options.at(i).getPrice() > workout_options.at(exp_mix_index).getPrice()) {
                // current workout is more expensive
                exp_mix_index = i;
            }
        }
            // cardio
        else {
            // change default value
            if (cheap_cardio_index == -1) { cheap_cardio_index = i; }
            if (workout_options.at(i).getPrice() < workout_options.at(cheap_cardio_index).getPrice()) {
                // current workout is cheaper
                cheap_cardio_index = i;
            }
        }
    }
    // all workout options exist and customer order is possible
    if (cheap_ana_index > -1 and exp_mix_index > -1 and cheap_cardio_index > -1) {
        // exact order as requested
        orders.push_back(cheap_cardio_index);
        orders.push_back(exp_mix_index);
        orders.push_back(cheap_ana_index);
    }
    return orders;
     }

string FullBodyCustomer::toString() const {return "fbd";};