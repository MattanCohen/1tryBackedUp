#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

//extern int currId; // currId defualt initialization is 0

class Trainer{
public:
    Trainer(int t_capacity);
    Trainer(int t_capacity,int id);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    int getId ();

    int getOriginalCapacity() const;
    virtual ~Trainer();
    Trainer& operator=(const Trainer& rhs);
    Trainer& operator=(Trainer&& rhs);
    Trainer(const Trainer& rhs);
    Trainer(const Trainer* rhs);
    Trainer(Trainer&& rhs);
    void stole();
    void copyCustomersList(const Trainer& rhs);
    // doesn't seem to exist in cpp best to remove
    void copyCustomersList(const Trainer* rhs);

private:
    int capacity;
    int originalCapacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int id;
    //accumulated salary resets to 0 only upon restarting the studio
    int accumulatedSalary;
};


#endif
