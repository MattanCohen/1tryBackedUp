#include "../include/Trainer.h"
#include <ostream>
#include <iostream>

using namespace std;

//extern int currId; // currId defualt initialization is 0

//c-tor with given capacity. the lists will remain empty
Trainer::Trainer (int t_capacity):capacity(t_capacity),originalCapacity(t_capacity),open(false),customersList(),orderList(),id(),accumulatedSalary(0){}
Trainer::Trainer (int t_capacity,int id):capacity(t_capacity),originalCapacity(t_capacity),open(false),customersList(),orderList(),id(id),accumulatedSalary(0){}

//rule of 5:
//d-tor
Trainer::~Trainer() {
    stole();
}
//ass op.
Trainer& Trainer::operator=(const Trainer& rhs){
    if (&*this!=&rhs){
        capacity=rhs.capacity;
        originalCapacity = rhs.originalCapacity;
        open=rhs.open;
        orderList=rhs.orderList;
        id=rhs.id;
        accumulatedSalary=rhs.accumulatedSalary;
        copyCustomersList(rhs);
    }
    return *this;
}
//move ass op.
Trainer& Trainer::operator=(Trainer&& rhs){
    if (this!=&rhs){
        capacity=rhs.capacity;
        originalCapacity = rhs.originalCapacity;
        open=rhs.open;
        orderList=rhs.orderList;
        id=rhs.id;
        accumulatedSalary=rhs.accumulatedSalary;
        copyCustomersList(rhs);
        rhs.stole();
    }
    return *this;
}
//copy c-tor:
Trainer::Trainer(const Trainer& rhs):capacity(rhs.capacity),originalCapacity(rhs.originalCapacity),open(rhs.open),customersList(),orderList(rhs.orderList),id(rhs.id),accumulatedSalary(rhs.accumulatedSalary){
    copyCustomersList(rhs);
}
//copy c-tor for pointer:
//Trainer::Trainer(const Trainer* rhs):capacity(rhs->capacity),open(rhs->open),customersList(),orderList(rhs->orderList),id(rhs->id),accumulatedSalary(rhs->accumulatedSalary){
//    copyCustomersList(rhs);
//}
//move c-tor
Trainer::Trainer(Trainer&& rhs):capacity(rhs.capacity),originalCapacity(rhs.originalCapacity),open(rhs.open),customersList(),orderList(rhs.orderList),id(rhs.id),accumulatedSalary(rhs.accumulatedSalary){
    copyCustomersList(rhs);
    rhs.stole();
}



void Trainer::stole() {
    size_t i=1;
    while (i<customersList.size()){
        Customer* customer_to_delete= customersList.at(i);
        delete customer_to_delete;
        i++;
    }
    if (customersList.size()>0)
        customersList.clear();
    if (orderList.size()>0)
        orderList.clear();
    capacity=originalCapacity;
}


void Trainer::copyCustomersList(const Trainer& rhs) {
    int i=0;
    while (customersList.size()<rhs.customersList.size()) {
        // according to Customer type create new and push to customerList
        if(rhs.customersList.at(i)->toString()=="swt"){
            SweatyCustomer *bruteCopy=new SweatyCustomer(rhs.customersList.at(i)->getName(),rhs.customersList.at(i)->getId());
            customersList.push_back(bruteCopy);
        }
        else if (rhs.customersList.at(i)->toString()=="chp"){
            CheapCustomer *bruteCopy=new CheapCustomer(rhs.customersList.at(i)->getName(),rhs.customersList.at(i)->getId());
            customersList.push_back(bruteCopy);
        }
        else if (rhs.customersList.at(i)->toString()=="mcl"){
            HeavyMuscleCustomer *bruteCopy=new HeavyMuscleCustomer(rhs.customersList.at(i)->getName(),rhs.customersList.at(i)->getId());
            customersList.push_back(bruteCopy);
        }
        else { // only fbd type is left
            FullBodyCustomer *bruteCopy=new FullBodyCustomer(rhs.customersList.at(i)->getName(),rhs.customersList.at(i)->getId());
            customersList.push_back(bruteCopy);
        }
        i++;
    }
}




//returns trainer's capacity
int Trainer::getCapacity() const{return capacity;}

int Trainer::getOriginalCapacity() const {return originalCapacity;}

//adds customer. assumes trainer is available
void Trainer::addCustomer(Customer* customer){
    //if the customer isn't already in trainer's customerslist
   if (getCustomer(customer->getId())== nullptr)
        customersList.push_back(customer);
}

//remove Customer with the identifing number "id" from trainer's customers list
void Trainer::removeCustomer(int id) {
    //create a copy of orders vector to set without id's orders
    //make sure we're not removing dummy customer
    if (id==-1)
        return;
    for (size_t i=0; i<customersList.size(); i++){
        if (customersList.at(i)->getId()==id) {
            Customer* customer_to_delete= customersList.at(i);
            delete customer_to_delete;
            customersList.at(i) = new CheapCustomer("XXX",-1);
            //if a dummy customer was added, compensate by raising capacity by 1
            capacity++;
            i=customersList.size();
        }
    }
    // if we removed all customers close the trainer
    int size=customersList.size();
    if (capacity-size==originalCapacity)
        closeTrainer();
}

//get the Customer with id "id" or null if doesn't exists or dummy
Customer* Trainer::getCustomer(int id){
    Customer* ans= nullptr;
    for (size_t i = 0; i < customersList.size(); i++)
            if (customersList.at(i)->getId() == id) {
                return customersList.at(i);
            }
        return ans;
    }

//returns customersList
vector<Customer*>& Trainer:: getCustomers(){
        return customersList;
    }

//returns orderList
vector<OrderPair>& Trainer:: getOrders(){
        return orderList;
    }

//overloading make_pair for pair<int,Workout> types, to change second=work to second=Workout(work)
//to avoid using the copy ass. op. since Workout objects contain only const values
//OrderPair& Trainer::make_Pair(const int cus_id, const Workout& work){
//    OrderPair paired=make_pair(cus_id, Workout(work));
//    OrderPair &ans=paired;
//    return ans;
//}

//order a workout from this trainer for the customer customer_id
//assumes customer_id belongs to this trainer and workout_ids exists in workout_options.
void Trainer:: order(const int customer_id,const vector<int>workout_ids,const vector<Workout>& workout_options){
    //check if customer is dummy or real
    if (customer_id==-1)
        return;
    //if customer isn't dummy add it's orders to trainer's list
    for (size_t i=0; i<workout_ids.size(); i++){
        int work_id=workout_ids.at(i);
        Workout work(workout_options.at(work_id));
        orderList.push_back(make_pair(customer_id,work));
        accumulatedSalary+=work.getPrice();
    }
}

//changes trainer's status to open. since they were closed, their lists will be empty
void Trainer::openTrainer(){
        open=true;
    }

//changes trainer's status to close. since they were open, close and delete their lists.
//if a trainer gets closed, it's salary for ordered workouts is already accumalated in order.
void Trainer:: closeTrainer(){
    open=false;
    stole();
}

//computes and returns salary
int Trainer::getSalary(){return accumulatedSalary;}

//returns true iff trainer is open
bool Trainer::isOpen(){return open;}

//returns trainer's id
int Trainer::getId(){return id;};

