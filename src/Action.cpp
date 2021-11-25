#include "../include/Action.h"

using namespace std;

//..................................................class:BaseAction
//c-tor
BaseAction::BaseAction():errorMsg(),status(){}

//rule of 5:
//d-tor
BaseAction::~BaseAction(){}
//ass.op
BaseAction& BaseAction::operator=(const BaseAction& rhs){
    if (&*this!=&rhs){
        errorMsg=rhs.errorMsg;
        status=rhs.status;
    }
    return *this;
}
//copy c-tor
BaseAction::BaseAction(const BaseAction &rhs):errorMsg(rhs.errorMsg),status(rhs.status){}
//no need for move ctor move ass. op. or dtor FOR ALL ACTION CLASSES inherited from
//BaseAction because no variables are stored on the heap

//returns the status of the action
ActionStatus BaseAction::getStatus() const {return status;}

//changes status to completed.
void BaseAction::complete() {
    status=ActionStatus(0);
}

//changes status to error and prints error message
void BaseAction::error(string errorMsg) {
    status=ActionStatus(1);
    this->errorMsg=errorMsg;
    cout<<errorMsg<< endl;
}
//returns the error message
string BaseAction::getErrorMsg() const {return errorMsg;};



//..................................................class:OpenTrainer

// Trainer *trainer = studio.getTrainer(trainerId);er
// c-tor
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(),trainerId(id),customers(customersList){}

//rule of 5:
//d-tor
OpenTrainer::~OpenTrainer(){
    size_t i=1;
    while (i<customers.size()) {
        Customer *customer = customers.at(i);
        delete customer;
        i++;
    }
    if (customers.size()>0)
        customers.clear();
}
//we can not implement ass.op and move ass. op. because trainerId is constant
//ass. op. to overload in case of uses in different functions
OpenTrainer& OpenTrainer::operator=(const OpenTrainer &rhs) {return *this;}
//move ass. op. to overload
OpenTrainer& OpenTrainer::operator=(OpenTrainer &&rhs) {return *this;}
// copy c-tor
OpenTrainer::OpenTrainer(const OpenTrainer &rhs):BaseAction(rhs),trainerId(rhs.trainerId),customers(){copyCustomers(rhs);}
//move c-tor
OpenTrainer::OpenTrainer(OpenTrainer &&rhs):BaseAction(rhs),trainerId(rhs.trainerId),customers(rhs.customers){
    if (this!=&rhs){
        copyCustomers(rhs);
    }
}

void OpenTrainer::copyCustomers(const OpenTrainer &rhs) {
    int i=0;
    while (customers.size()<rhs.customers.size()){
        //if a Sweaty Customer
        if (rhs.customers.at(i)->toString()=="swt"){
            SweatyCustomer *bruteCopy=new SweatyCustomer(rhs.customers.at(i)->getName(),rhs.customers.at(i)->getId());
            customers.push_back(bruteCopy);
        }
        else if (rhs.customers.at(i)->toString()=="chp"){
            CheapCustomer *bruteCopy=new CheapCustomer(rhs.customers.at(i)->getName(),rhs.customers.at(i)->getId());
            customers.push_back(bruteCopy);
        }
        else if (rhs.customers.at(i)->toString()=="mcl"){
            HeavyMuscleCustomer *bruteCopy=new HeavyMuscleCustomer(rhs.customers.at(i)->getName(),rhs.customers.at(i)->getId());
            customers.push_back(bruteCopy);
        }
        else if (rhs.customers.at(i)->toString()=="fbd"){
            FullBodyCustomer *bruteCopy=new FullBodyCustomer(rhs.customers.at(i)->getName(),rhs.customers.at(i)->getId());
            customers.push_back(bruteCopy);
        }
        i++;
    }
}



//prints status for log
std::string OpenTrainer::toString() const {
    //set "open trainerId "
    string prefix="open "+std::to_string(trainerId)+" ";
    //for every customer append to "open trainerId "-"cust1,cust1strategy "
    for (size_t i=0; i<customers.size(); i++)
        prefix+=customers.at(i)->getName()+","+customers.at(i)->toString()+" ";
    if (getStatus()==0)
        return prefix+"Completed";
    return prefix+"Error: "+getErrorMsg();
}

void OpenTrainer::act(Studio &studio) {
    bool exists=true;
    //check bad trainer id
    if (studio.getTrainer(trainerId)== nullptr)
        return;
    // access pointer value
    Trainer* opening=studio.getTrainer(trainerId);
//check if the trainer exists but is already open
    if (exists)
        if (opening->isOpen())
            exists=false;

    opening->openTrainer();
    //save trainers capacity
    size_t cap=opening->getCapacity();
    if (!exists){
        error("Workout session does not exist or is already open"); // or trainer's capacity too low
        return;
    }
//if trainer doesn't have capacity
    if (opening->getCustomers().size()+1>cap)
        return;
    //else, all good! add the customers and their orders to (the) trainer (we're) opening
    for (size_t i=0; i<customers.size();i++) {
        Customer *customer_i = customers.at(i);
        //if customer can order then add them and their orders
        size_t customers_size=opening->getCustomers().size();
        // can we add another customer
        if (customer_i->order(studio.getWorkoutOptions()).size() > 0 and customers_size+1<=cap)
            //if they are heavy muscle use sorted workout_options
            opening->addCustomer(customer_i);

    }
    complete();
}


//..................................................class:Order
//c-tor
Order::Order(int id):BaseAction(),trainerId(id) {}

//rule of 5:
//d-tor
Order::~Order(){}
//we can not implement ass.op and move ass. op. because trainerId is constant
//ass. op. to overload in case of uses in different functions
Order& Order::operator=(const Order& rhs) {return *this;}
//copy c-tor
Order::Order(const Order &rhs):BaseAction(rhs),trainerId(rhs.trainerId) {}


//prints status for log
string Order::toString() const {
    string prefix="order "+to_string(trainerId);
    if (getStatus()==0)
        return prefix+" Completed";
    return prefix+" Error: "+getErrorMsg();
}

//orders trainerId's orders if possible
void Order::act(Studio &studio) {
    //if trainer exist
    if (trainerId<studio.getNumOfTrainers() && trainerId>=0){
        //and if trainer is open
        Trainer* opening=studio.getTrainer(trainerId);
        if (studio.getTrainer(trainerId)->isOpen()) {
            vector < Customer * > &customersList = studio.getTrainer(trainerId)->getCustomers();
            //if there's at least 1 customers ordering:
            if (customersList.size()>0){
                //create the orders
                for (size_t i=0; i<customersList.size(); i++){
                    Customer* customer_i=customersList.at(i);
                    //if customer is muscler use sorted workout options
                    if (customer_i->toString()=="mcl") {
                        opening->order(customer_i->getId(), customer_i->order(studio.getSortedWorkoutOptions()),
                                       studio.getWorkoutOptions());
                    }
                        //else use default workout_options
                    else{
                        opening->order(customer_i->getId(), customer_i->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
                    }
                }
                //print the orders
                vector <OrderPair> &orderList = studio.getTrainer(trainerId)->getOrders();
                //print every order done by every customer of the trainer
                for (size_t i = 0; i < orderList.size(); i++) {
                    for (size_t j=0; j<customersList.size(); j++)
                        //for every order find the customer who ordered it
                        if (orderList.at(i).first==customersList.at(j)->getId()){
                            string customer_name =customersList.at(j)->getName();
                            cout<<customer_name<<" Is Doing "<<orderList.at(i).second.getName()<<endl;
                        }

//                    if (orderList.at(i).first==customer_id){
//                        string customer_name;
//                        for (size_t j = 0; j < customersList.size(); j++)
//                            if (customersList.at(j)->getId() == orderList.at(i).first) {
//                                customer_name =
//                                customer_id=j;
//                                j = customersList.size();
//                            }
//                        string workout_name = orderList.at(i).second.getName();
//                        cout << customer_name + " Is Doing " + workout_name << endl;
//                    }

                }
            }
        }
        complete();

        return;
    }
    //if trainer doesn't exist or is close
    error("Trainer does not exist or is not open");
}




//..................................................class:MoveCustomer
//c-tor
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId) {}

//rule of 5:
//d-tor
MoveCustomer::~MoveCustomer(){}
//we can not implement ass.op and move ass. op. because all variable are constant
//ass. op. to overload in case of uses in different functions
MoveCustomer& MoveCustomer::operator=(const MoveCustomer &rhs) {return *this;}
//copy c-tor
MoveCustomer::MoveCustomer(const MoveCustomer &rhs):BaseAction(rhs),srcTrainer(rhs.srcTrainer),dstTrainer(rhs.dstTrainer),id(rhs.id) {}


//returns status for log
std::string MoveCustomer::toString() const {
    string prefix="move "+to_string(srcTrainer)+" "+to_string(dstTrainer)+" "+to_string(id);
    if (getStatus()==0)
        return prefix+" Completed";
    return prefix+" Error: "+getErrorMsg();
}

//moves the customer
void MoveCustomer::act(Studio &studio) {
    bool dst=true;
    bool src=true;
    //first check and reject dummy customers
    if (id==-1)
        dst=false;
    //if the customer isn't dummy customer:
    //check if destination trainer doesn't exist
    if (dst)
        if (dstTrainer<0 || dstTrainer>=studio.getNumOfTrainers())
            dst= false;
    //if destination trainer exists but isn't open
    if (dst)
        if (!studio.getTrainer(dstTrainer)->isOpen())
            dst=false;
    //if destination trainer exists and open but doesn't have available spots
    if (dst){
        int curr_numof_cust=studio.getTrainer(dstTrainer)->getCustomers().size();
        int dst_capacity=studio.getTrainer(dstTrainer)->getCapacity();
        if (curr_numof_cust+1>dst_capacity)
            dst=false;
    }
    //check if source trainer is the destination trainer
    if (srcTrainer==dstTrainer)
        dst=false;
    //if customer is already at destination trainer
    if (dst)
        if (studio.getTrainer(dstTrainer)->getCustomer(id)!= nullptr)
            dst=false;
    //if destination trainer isn't available for whatever reason, change status to error, print error and stop the program
    if (!dst){
        error("Cannot move Customer");
        return;
    }

    //if source trainer doesn't exist
    if (srcTrainer<0 || srcTrainer>=studio.getNumOfTrainers())
        src= false;
    //if source trainer exists but isn't open
    if (src){
        if (!studio.getTrainer(srcTrainer)->isOpen())
            src=false;
    }
    //if source trainer exist and open but isn't responsible for the customer
    if (src){
        vector<Customer*>& src_customerList=studio.getTrainer(srcTrainer)->getCustomers();
        bool owned=false;
        size_t i=0;
        while(i<src_customerList.size() and !owned){
            owned=(src_customerList.at(i)->getId()==id);
            i++;
        }
        if (!owned)
            src=false;
    }

    //if source trainer isn't available for whatever reason, change status to error, print error and stop the program
    if (!src){
        error("Cannot move Customer");
        return;
    }
    Trainer* source=studio.getTrainer(srcTrainer);
    //add customer id from source to dstTrainer
    string name=source->getCustomer(id)->getName();
    string type=source->getCustomer(id)->toString();
    if (type=="swt"){
        studio.getTrainer(dstTrainer)->addCustomer(new SweatyCustomer(name,id));}
    if (type=="chp"){
        studio.getTrainer(dstTrainer)->addCustomer(new CheapCustomer(name,id));}
    if (type=="mcl"){
        studio.getTrainer(dstTrainer)->addCustomer(new HeavyMuscleCustomer(name,id));}
    if (type=="fbd"){
        studio.getTrainer(dstTrainer)->addCustomer(new FullBodyCustomer(name,id));}
    studio.getTrainer(dstTrainer)->order(id,source->getCustomer(id)->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());
    //remove customer id from source
    source->removeCustomer(id);
//close trainer source if customer id was his last customer
    if (source->getCustomers().size()==0){
        source->closeTrainer();
    }

    complete();

}


//..................................................class:Close
// c-tor
Close::Close(int id):BaseAction(),trainerId(id){}


//rule of 5:
//d-tor
Close::~Close(){}
//we can not implement ass.op and move ass. op. because trainerId is constant
//ass. op. to overload in case of uses in different functions
Close& Close::operator=(const Close &rhs) {return *this;}
// copy c-tor
Close::Close(const Close &rhs):BaseAction(rhs),trainerId(rhs.trainerId) {}


//prints status from log
string Close::toString() const {
    string prefix = "close "+to_string(trainerId);
   if(this->getStatus()==0) {
       return prefix+" Completed";
   }
   else{
       return prefix+" Error: "+this->getErrorMsg();
   }
}


//close trainerId
void Close::act(Studio &studio) {
    //boolean if given trainerId can be closed
    bool isError = false;
    //make sure trainerId is legal
    if (trainerId < studio.getNumOfTrainers() and trainerId >= 0) {
        //check if trainer is legal but their session is already closed
        if (!studio.getTrainer(trainerId)->isOpen())
            isError = true;
    }
    //if trainer isn't legal
    else
        isError = true;
    //check if there's an error and if so print error
    if (isError) {
        error("Trainer does not exist or is not open");
    }
    else {
        Trainer *trainer = studio.getTrainer(trainerId);
        // close action, clears customer/order lists
        trainer->closeTrainer();
        // salary for closed trainer
        int salary = trainer->getSalary();
        complete();
        // action log is printed after salary has been updated
        cout<<"Trainer "+to_string(trainerId)+" closed. Salary "+ to_string(salary)+"NIS"<< endl;
    }
}


//..................................................class:CloseAll
// c-tor
CloseAll::CloseAll():BaseAction(){};


//rule of 5:
//d-tor
CloseAll::~CloseAll(){}
// ass op.
CloseAll &CloseAll::operator=(const CloseAll &rhs) {
    BaseAction::operator=(rhs);
    return *this;
}

// copy c-tor
CloseAll::CloseAll(const CloseAll &other):BaseAction(other) {}


//prints status for log
std::string CloseAll::toString() const {
    return "closeall Completed";
}

void CloseAll::act(Studio &studio) {
    int numOfTrainers = studio.getNumOfTrainers();
    // go over all trainers
    for(int i=0;i<numOfTrainers;i++) {
        // if session is open close trainer
        if(studio.getTrainer(i)->isOpen()) {
            Close close_trainer = Close(i);
            close_trainer.act(studio);
        }
        //else, trainer is closed but is part of studio's trainers list. Make sure to delete it
        else {
            Trainer *trainer = studio.getTrainer(i);
            // close action, clears customer/order lists
            trainer->closeTrainer();
        }

    }
    complete();
}

// no need to create error function because action is always successful


//..................................................class:PrintWorkoutOptions
// c-tor
PrintWorkoutOptions::PrintWorkoutOptions():BaseAction() {}


//rule of 5:
//d-tor
PrintWorkoutOptions::~PrintWorkoutOptions(){}
// ass op.
PrintWorkoutOptions& PrintWorkoutOptions::operator=(const PrintWorkoutOptions &rhs) {
    BaseAction::operator=(rhs);
    return *this;
}

// copy c-tor
PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions& other):BaseAction(other){}



//prints status for log
string PrintWorkoutOptions::toString() const {
    return "workout_options Completed";
}

// convert Enum to String representation
std::string PrintWorkoutOptions::getTypeAsString(WorkoutType type) const{
    if(type == 0)
        return "ANAEROBIC";
    if (type == 1)
        return "MIXED";
    return "CARDIO";
}

void PrintWorkoutOptions::act(Studio &studio) {
    vector<Workout> workout_options = studio.getWorkoutOptions();
    for(size_t i=0; i<workout_options.size();i++) {
        Workout w = workout_options.at(i);
        // <workout_name>, <workout_type>, <workout_price>
        string workoutS = w.getName()+", "+getTypeAsString(w.getType())+", "+to_string(w.getPrice());
        // print workout
        cout<<workoutS<< endl;
    }
    complete();
}


//..................................................class:PrintTrainerStatus
//ctor
PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(), trainerId(id){}

//rule of 5:
//d-tor
PrintTrainerStatus::~PrintTrainerStatus(){}
//we can not implement ass.op and move ass. op. because trainerId is constant
//ass. op. to overload in case of uses in different functions
PrintTrainerStatus& PrintTrainerStatus::operator=(const PrintTrainerStatus &rhs) {return *this;}
//copy c-tor
PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus &rhs):BaseAction(rhs),trainerId(rhs.trainerId) {}


//prints status for log
string PrintTrainerStatus::toString() const {
    return "status "+ to_string(trainerId)+" Completed";
}

//prints trainer's status, customers, their orders and salary
void PrintTrainerStatus::act(Studio &studio) {
    bool open=studio.getTrainer(trainerId)->isOpen();
    if (!open){
        cout<<"Trainer "<<to_string(trainerId)<<" status: closed"<< endl;
        return;
    }
    else
        cout<<"Trainer "<<to_string(trainerId)<<" status: open"<< endl;
//print customers
    cout<<"Customers:"<<endl;
    vector<Customer*> customers=studio.getTrainer(trainerId)->getCustomers();
    for (size_t i=0; i<customers.size(); i++) {
        //for every none-dummy customer
        if (customers.at(i)->getId() != -1) {
            string customer_i_id = to_string(customers.at(i)->getId());
            string customer_i_name = customers.at(i)->getName();
            cout << customer_i_id + " " + customer_i_name << endl;
        }
    }
    //print orders
    cout<<"Orders:"<<endl;
    vector<OrderPair> orders=studio.getTrainer(trainerId)->getOrders();
    for (size_t i=0; i<orders.size(); i++){
        string workout_name=orders.at(i).second.getName();
        string workout_price=to_string(orders.at(i).second.getPrice());
        string customer_id=to_string(orders.at(i).first);
        //for every none-dummy customer
        if (customer_id!="-1")
            cout<<workout_name+" "+workout_price+" "+customer_id<< endl;
    }
    cout<<"Current Trainer's Salary: "+to_string(studio.getTrainer(trainerId)->getSalary())+"NIS"<< endl;
    complete();
}

//..................................................class:PrintActionsLog

// c-tor
PrintActionsLog::PrintActionsLog():BaseAction(){}

//d-tor
PrintActionsLog::~PrintActionsLog(){}
//ass op.
PrintActionsLog& PrintActionsLog::operator=(const PrintActionsLog &rhs){
    BaseAction::operator=(rhs);
    return *this;
}

// copy c-tor
PrintActionsLog::PrintActionsLog(const PrintActionsLog& other):BaseAction(other){}


//print status for log
string PrintActionsLog::toString() const {
    return "log Completed";
}

void PrintActionsLog::act(Studio &studio) {
    vector<BaseAction*> actionsLog = studio.getActionsLog();
   for (size_t i=0; i<actionsLog.size(); i++)
       cout<<actionsLog.at(i)->toString()<<endl;
    complete();
}

// no need to create error function because action is always successful
//extern bool ever_backed;
//extern Studio* backup;

//..................................................class:BackupStudio
// c-tor
BackupStudio::BackupStudio():BaseAction(){}

//d-tor
BackupStudio::~BackupStudio(){}
//ass op.
BackupStudio &BackupStudio::operator=(const BackupStudio &rhs){
    BaseAction::operator=(rhs);
    return *this;
}
//move ass. op.
BackupStudio &BackupStudio::operator=(BackupStudio &&rhs) {
    BaseAction::operator=(rhs);
    return *this;
}
// copy c-tor
BackupStudio::BackupStudio(const BackupStudio& rhs):BaseAction(rhs){}
//move c-tor
BackupStudio::BackupStudio(BackupStudio &&rhs):BaseAction(rhs) {}


void BackupStudio::act(Studio &studio) {
    if(ever_backed)
        backup->stole();
    backup=new Studio(studio);
    cout<<backup->getActionsLog().at(0)->toString()<<"                        Action cpp line 603"<<endl;
    ever_backed=true;
    complete();
}

//print status for log
string BackupStudio::toString() const {
    return "backup Completed";
}

//..................................................class:RestoreStudio
// c-tor
RestoreStudio::RestoreStudio():BaseAction(){}

RestoreStudio::~RestoreStudio(){}
//ass op.
RestoreStudio &RestoreStudio::operator=(const RestoreStudio &rhs){
    BaseAction::operator=(rhs);
    return *this;
}
//move ass.op.
RestoreStudio &RestoreStudio::operator=(RestoreStudio &&rhs) {
    if (this!=&rhs){
        BaseAction::operator=(rhs);
    }
    return *this;
}
// copy c-tor
RestoreStudio::RestoreStudio(const RestoreStudio& rhs):BaseAction(rhs){}
//move c-tor
RestoreStudio::RestoreStudio(RestoreStudio &&rhs):BaseAction(rhs){}



//print status for log
string RestoreStudio::toString() const {
    if (ever_backed)
        return "restore Completed";
    return "restore Error: No backup available";
}


void RestoreStudio::act(Studio &studio) {
    if (ever_backed){
        cout<<"action cpp   line 647"<<endl;
        Studio* tempBackup = new Studio(backup);
        cout<<"action cpp   line 649"<<endl;
        studio.stole();
        cout<<"action cpp   line 651"<<endl;
        studio.operator=(tempBackup);
        cout<<"action cpp   line 653"<<endl;
        backup->stole();
        cout<<"action cpp   line 655"<<endl;
        backup->operator=(tempBackup);
        return;
    }
    error("No backup available");
}

