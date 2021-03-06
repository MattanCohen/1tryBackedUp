#include "../include/Studio.h"

using namespace std;
//
//void Studio::createBackup() {
//    cout<<"hey"<<endl;
//    backed=true;
//    cout<<"line 8    Studio.cpp"<<endl;
//    delete backup;
//    cout<<"line 10    Studio.cpp"<<endl;
//    backup= new Studio(*this);
//    cout<<"line 12    Studio.cpp"<<endl;
//}
//bool Studio::isBacked() {
//    return backed;
//}
//void Studio::restoreBackup() {
//    cout<<"line 18    Studio.cpp"<<endl;
//    Studio* backupCopy=new Studio(*backup);
//    cout<<"line 20    Studio.cpp"<<endl;
//    stole();
//    cout<<"line 22    Studio.cpp"<<endl;
//    *this=*backupCopy;
//    cout<<"line 24    Studio.cpp"<<endl;
//    backup= new Studio(*backupCopy);
//    cout<<"line 26    Studio.cpp"<<endl;
//}


//rule of 5:
//d-tor in case Customer reference is deleted
Studio::~Studio() {
    this->stole();
}
//ass. op.
Studio& Studio::operator=(const Studio &rhs) {
    cout<<"line 37    Studio.cpp   &*this!=&rhs  ? "<<(&*this!=&rhs)<<endl;
    if (&*this!=&rhs){
        delete this;
        *this=new Studio();
        cout<<"line 41 studio.cpp"<<endl;
        cout<<rhs.open<<endl;
        cout<<open<<endl;
        open=rhs.open;
        cout<<"line 45 studio.cpp"<<endl;
        workout_options=rhs.workout_options;
        cout<<"line 47 studio.cpp"<<endl;
        sorted_workout_options=rhs.sorted_workout_options;
        cout<<"line 49 studio.cpp"<<endl;
        copyTrainers(rhs);
        cout<<"line 51 studio.cpp"<<endl;
        copyActionLogs(rhs);
        cout<<"line 53 studio.cpp"<<endl;
        workout_number=rhs.workout_number;
//        Studio::backup=new Studio(*rhs.backup);
    }
    return *this;
}
//move ass.op.
Studio& Studio::operator=(Studio &&rhs) {
    cout<<"line 61    Studio.cpp"<<endl;
    if (this!=&rhs){
        delete this;
        *this=new Studio();
        open=rhs.open;
        workout_options=rhs.workout_options;
        sorted_workout_options=rhs.sorted_workout_options;
        workout_number=rhs.workout_number;
        copyTrainers(rhs);
        copyActionLogs(rhs);
//        Studio::backup=new Studio(*rhs.backup);
        rhs.stole();
    }
    return *this;
}

//move c-tor
Studio::Studio(Studio &&rhs):open(rhs.open),trainers(rhs.trainers),workout_options(rhs.workout_options),sorted_workout_options(rhs.sorted_workout_options),actionsLog(rhs.actionsLog),workout_number(rhs.workout_number) {
    cout<<"line 79    Studio.cpp"<<endl;
    if(this!=&rhs){
        copyTrainers(rhs);
        copyActionLogs(rhs);
        rhs.stole();
    }
}
//copy c-tor:
Studio::Studio(const Studio& rhs):open(rhs.open),trainers(rhs.trainers),workout_options(rhs.workout_options),sorted_workout_options(rhs.sorted_workout_options),actionsLog(rhs.actionsLog),workout_number(rhs.workout_number){
    copyTrainers(rhs);
    copyActionLogs(rhs);
}

// pointer copy c-tor
Studio::Studio(const Studio *rhs):open(rhs->open),trainers(),workout_options(rhs->workout_options),sorted_workout_options(rhs->sorted_workout_options),actionsLog(),workout_number(rhs->workout_number){
    cout<<"line 94    Studio.cpp"<<endl;
    copyTrainers(rhs);
    cout<<"line 94    Studio.cpp"<<endl;
    copyActionLogs(rhs);
    cout<<"line 95    Studio.cpp"<<endl;
}

// ass pointer operator
Studio &Studio::operator=(const Studio *rhs) {
    cout<<"line 100    Studio.cpp"<<endl;
    if (this!=rhs){
        cout<<"line 100  1  Studio.cpp"<<endl;
        this->stole();
        cout<<"line 100  2  Studio.cpp"<<endl;
//        *this=new Studio();
        cout<<"line 100  3  Studio.cpp"<<endl;
        open=rhs->open;
        cout<<"line 100  4  Studio.cpp"<<endl;
        workout_options=rhs->workout_options;
        cout<<"line 100  5  Studio.cpp"<<endl;
        sorted_workout_options=rhs->sorted_workout_options;
        cout<<"line 100  6  Studio.cpp"<<endl;
        workout_number=rhs->workout_number;
        cout<<"line 100  7  Studio.cpp"<<endl;
        copyTrainers(rhs);
        cout<<"line 100  8  Studio.cpp"<<endl;
        copyActionLogs(rhs);
        cout<<"line 100  9  Studio.cpp"<<endl;
//        Studio::backup=new Studio(*rhs.backup);
    }
    return *this;
}



void Studio::stole() {
    // remove elements+pointers in trainers
    size_t i=1;
    while (i<trainers.size()){
        Trainer* trainer=trainers.at(i);
        delete trainer;
        i++;
    }
    if (trainers.size()>0)
        trainers.clear();
    i=1;
    while (actionsLog.size()>i){
        cout<<"143 stole cpp"<<endl;
        cout<<" 142 stole deleted action is:"<<actionsLog.at(i)->toString()<<endl;
        delete actionsLog.at(i);
        cout<<"143 stole cpp"<<endl;
        i++;
        cout<<"145 loop finish"<<endl;
    }
    cout<<"146 stole cpp"<<endl;
    actionsLog.clear();
    cout<<"stole cpp 150"<<endl;
    if (workout_options.size()>0)
        workout_options.clear();
    cout<<"stole cpp 153"<<endl;
    if (sorted_workout_options.size()>0)
        sorted_workout_options.clear();
    cout<<"stole cpp 156"<<endl;

    //    while (workout_options.size()>0){
//        workout_options.pop_back();
//    }
//    while (sorted_workout_options.size()>0){
//        sorted_workout_options.pop_back();
//    }
//    backup->stole();
}

//copy trainers for pointer
void Studio::copyTrainers(const Studio *rhs) {
    int i=0;
    while (trainers.size()<rhs->trainers.size()) {
        // create a copy for each trainer and push copy
        //Trainer *copyTrainer = new Trainer(const rhs.trainers.at(i));
        //Trainer(const Trainer& rhs);
        Trainer *trainerP = rhs->trainers.at(i);
        Trainer *copyTrainer = new Trainer(*trainerP);
        trainers.push_back(copyTrainer);
        //trainers.push_back(copyTrainer);
        i++;

        // Trainer copyTrainer = new Trainer(&rhs.trainers.at(i));
        // Trainers.push_back(*copyTrainer);
    }
}

//copy trainers for refrence
void Studio::copyTrainers(const Studio &rhs) {
    int i=0;
    while (trainers.size()<rhs.trainers.size()) {
        // create a copy for each trainer and push copy
        //Trainer *copyTrainer = new Trainer(const rhs.trainers.at(i));
        //Trainer(const Trainer& rhs);
        Trainer *trainerP = rhs.trainers.at(i);
        Trainer *copyTrainer = new Trainer(*trainerP);
        trainers.push_back(copyTrainer);
        //trainers.push_back(copyTrainer);
        i++;

        // Trainer copyTrainer = new Trainer(&rhs.trainers.at(i));
        // Trainers.push_back(*copyTrainer);
     }
}

//copy actionsloag for pointer
void Studio::copyActionLogs(const Studio *rhs) {
    // for each action log create a baby and puuush
    std::size_t i=0;
    while (actionsLog.size()<rhs->actionsLog.size()) {
        // extracts first word from string that contains action name
        string actionName = identifyAction(rhs->actionsLog.at(i)->toString());
        // based on action log type create a copy and push to list
        if (actionName == "order") {
            Order *temp=(Order*)rhs->actionsLog.at(i);
            Order *copy = new Order(*temp);
            actionsLog.push_back(copy);
        }
        else if (actionName == "move") {
            MoveCustomer *temp=(MoveCustomer*)rhs->actionsLog.at(i);
            MoveCustomer *copy = new MoveCustomer(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "close") {
            Close *temp=(Close*)rhs->actionsLog.at(i);
            Close *copy = new Close(*temp);
            actionsLog.push_back(copy);
        }
        else if (actionName == "closeall") {
            CloseAll *temp=(CloseAll*)rhs->actionsLog.at(i);
            CloseAll *copy = new CloseAll(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "workout_options") {
            PrintWorkoutOptions *temp=(PrintWorkoutOptions*)rhs->actionsLog.at(i);
            PrintWorkoutOptions *copy = new PrintWorkoutOptions(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "status") {
            PrintTrainerStatus *temp=(PrintTrainerStatus*)rhs->actionsLog.at(i);
            PrintTrainerStatus *copy = new PrintTrainerStatus(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "log") {
            PrintActionsLog *temp=(PrintActionsLog*)rhs->actionsLog.at(i);
            PrintActionsLog *copy = new PrintActionsLog(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "backup") {
            BackupStudio *temp=(BackupStudio*)rhs->actionsLog.at(i);
            BackupStudio *copy = new BackupStudio(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "restore") {
            RestoreStudio *temp=(RestoreStudio*)rhs->actionsLog.at(i);
            cout<<"pointer one "<<temp->toString()<<endl;
            RestoreStudio *copy = new RestoreStudio(*temp);
            cout<<copy->toString()<<endl;
            actionsLog.push_back(copy);
            cout<<"pushed to actions log"<<endl;
        }
        i++;
    }
}

//copy actionslog for refrence
void Studio::copyActionLogs(const Studio &rhs) {
    // for each action log create a baby and puuush
    std::size_t i=0;
    while (actionsLog.size()<rhs.actionsLog.size()) {
        // extracts first word from string that contains action name
        string actionName = identifyAction(rhs.actionsLog.at(i)->toString());
        // based on action log type create a copy and push to list
        if (actionName == "order") {
            Order *temp=(Order*)rhs.actionsLog.at(i);
            Order *copy = new Order(*temp);
            actionsLog.push_back(copy);
        }
        else if (actionName == "move") {
            MoveCustomer *temp=(MoveCustomer*)rhs.actionsLog.at(i);
            MoveCustomer *copy = new MoveCustomer(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "close") {
            Close *temp=(Close*)rhs.actionsLog.at(i);
            Close *copy = new Close(*temp);
            actionsLog.push_back(copy);
        }
        else if (actionName == "closeall") {
            CloseAll *temp=(CloseAll*)rhs.actionsLog.at(i);
            CloseAll *copy = new CloseAll(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "workout_options") {
            PrintWorkoutOptions *temp=(PrintWorkoutOptions*)rhs.actionsLog.at(i);
            PrintWorkoutOptions *copy = new PrintWorkoutOptions(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "status") {
            PrintTrainerStatus *temp=(PrintTrainerStatus*)rhs.actionsLog.at(i);
            PrintTrainerStatus *copy = new PrintTrainerStatus(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "log") {
            PrintActionsLog *temp=(PrintActionsLog*)rhs.actionsLog.at(i);
            PrintActionsLog *copy = new PrintActionsLog(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "backup") {
            BackupStudio *temp=(BackupStudio*)rhs.actionsLog.at(i);
            BackupStudio *copy = new BackupStudio(*temp);
            actionsLog.push_back(copy);
        }
        else if  (actionName == "restore") {
            cout<<"rhs action log size"<<rhs.actionsLog.size()<<endl;
            RestoreStudio *temp=(RestoreStudio*)rhs.actionsLog.at(i);
            cout<<"rhs one: "<<rhs.actionsLog.at(i)->toString()<<endl;
            cout<<"reference one "<<temp->toString()<<endl;
            RestoreStudio *copy = new RestoreStudio(*temp);
            cout<<copy->toString()<<endl;
            actionsLog.push_back(copy);
        }
        i++;
    }
}



//creating an empty Studio
Studio::Studio():open(false),trainers(),workout_options(),sorted_workout_options(),actionsLog(),workout_number(0){}

// given a list of trainer capacities adds to "trainers"
void Studio::AddTrainers(std::string trainersRow) {
    //_________________________________adding trainers:___________________
    //line contains capacities seperated by commas. ignore the commas.
    size_t j=0;
    int id=0;
    //while we didn't reach the new line
    while (j<trainersRow.size() && trainersRow.at(j)!='\n') {
        string trainerCapacityString="";
        // create string that contains capacity
        while(j<trainersRow.size() and trainersRow.at(j)!='\n' and trainersRow.at(j)!=','){
            trainerCapacityString = trainerCapacityString+trainersRow.at(j);
            j++;
        }
        // create fitting trainer object
        Trainer* trainer = new Trainer(stoi(trainerCapacityString),id);
        trainers.push_back(trainer);
        // skip comma/endline
        j++;
        id++;
    }
}
// given workout row adds it to "workout_options"
void Studio::AddWorkoutOption(std::string workoutRow, int workoutId) {
    size_t i=0;
    // extract workout name from the start to the first comma
    string workoutName="";
    while(workoutRow.at(i)!=','){workoutName.push_back(workoutRow.at(i)); i++;}
    // skip comma and spaces
    while(workoutRow.at(i)==',' or workoutRow.at(i)==' ') i++;
    // extract workout type from after the first comma to the second comma
    string workoutType="";
    while(workoutRow.at(i)!=','){workoutType.push_back(workoutRow.at(i)); i++;}


    //skip comma and spaces
    while(workoutRow.at(i)==',' or workoutRow.at(i)==' ') i++;
    // extract workout price from after the second comma to the first endLine
    string workoutPriceStr = "";
    while (i<workoutRow.size() && workoutRow.at(i)!='\n') {workoutPriceStr.push_back(workoutRow.at(i)); i++;}
    Workout work(workoutId,workoutName,stoi(workoutPriceStr),getType(workoutType));;
    workout_options.push_back(work);
}


// checks if a row is an empty line and needs to be ignored
bool Studio::isEmptyLine(std::string configRow) {
    // sanity check
    bool temp;
    if(configRow.size()==0) {temp= true;}
    else {
        // empty line or starts with a comment
        if(configRow.at(0)=='\n' or configRow.at(0)=='\r' or configRow.at(0)==' '  or configRow.at(0)=='#') {temp= true;}
            // line is not empty and contains relevant information
        else {temp= false;}
    }
    return temp;
}

//creating a Studio by a received config file. assumes config file is legal

Studio::Studio(const std::string &configFilePath):open(false),trainers(),workout_options(vector<Workout>()),sorted_workout_options(),actionsLog(),workout_number(0){
    // read config file row by row
    ifstream ReadConfigFile(configFilePath);
    string configRow;
    vector<string> configRows;
    // add only non empty rows to a vector of strings
    while(getline(ReadConfigFile,configRow)) {

        // ignore empty lines and comments
        if(!isEmptyLine(configRow)){
            configRows.push_back(configRow);
        }
    }
    // close file read session
    ReadConfigFile.close();
    // index to follow current configRow processed
    size_t rowIndex = 0;
    int ignored_char=configRows.at(0).find('\n');
    //from i=0 to the end of the line ('\n') is the number of trainers string representation
    int num_of_trainers= stoi(configRows.at(0).substr(0,ignored_char));
    rowIndex++;

    if(num_of_trainers>0){
        // get next row for trainers capacity and add to Studio
        AddTrainers(configRows.at(rowIndex));
        rowIndex++;
    }
    // workoutIndex used to create unique identifier for each workout
    size_t workoutIndex = 0;
    while(rowIndex<configRows.size()) {
        AddWorkoutOption(configRows.at(rowIndex),workoutIndex);
        workoutIndex++;
        rowIndex++;
    }
    // set identifier used to create unique Customer Id's
    workout_number=0;

    //all config file rows were read and implemented
    //after everything was set, we want to create a sorted workout options for heavy muscle customers
    //we'll create a sorted workout options vector from the workout options

    size_t iMax=0;
    vector<Workout> temp;
    for (size_t i=0; i<workout_options.size(); i++)
        temp.push_back(workout_options.at(i));
    while (temp.size()>0){
        for (size_t j=0; j<temp.size(); j++)
            if (temp.at(j).getPrice()>temp.at(iMax).getPrice())
                iMax=j;
        sorted_workout_options.push_back(temp.at(iMax));
        vector<Workout> newtemp;
        for (size_t i=0; i<temp.size(); i++)
            if (i!=iMax)
                newtemp.push_back(temp.at(i));
        while (temp.size()>0)
            temp.pop_back();
        for (size_t i=0; i<newtemp.size(); i++)
            temp.push_back(newtemp.at(i));
        iMax=0;
    }
}

WorkoutType Studio::getType(string rhs) {
    if (rhs=="Anaerobic")
        return WorkoutType(0);
    if (rhs=="Mixed")
        return WorkoutType(1);
    return WorkoutType(2);
}

//rule of 5:
//no need for d-tor, move c-tor, move ass.op. since all values are local


// return the actionType
std::string Studio::identifyAction(std::string userAction) {
    string commandPrefix = "";
    size_t i=0;
    while (i<userAction.size() && userAction.at(i)!=' '){
        commandPrefix.push_back(userAction.at(i));
        i++;
    }
    return commandPrefix;
}

// creates action based on type and user action
void Studio::startAction(std::string actionType, std::string userAction) {
    //if action is to open new trainer

    if (actionType=="open"){
        //information gathering to extract trainer id
        int firstSpaceIndex = userAction.find(" ");
        // +1 so that space won't be included
        string userActionAfterSpace = userAction.substr(firstSpaceIndex+1);

        int secondSpaceIndex = userActionAfterSpace.find(" ");
        // extract trainerId

        int trainerId = stoi(userActionAfterSpace.substr(0,secondSpaceIndex));
        // +1 so that space won't be included
        string customersDetails = userActionAfterSpace.substr(secondSpaceIndex+1);
        vector<Customer *> customersList;
        // true as long as customerDetails is empty
        bool noCustDetails = true;
        // look for a character different from space
        size_t j=1;
        while(j<customersDetails.size() and noCustDetails){
            if(customersDetails.at(j)!=' ') {noCustDetails=false;}
            j++;
        }
        // char different from space == customer details exist and need to be added
        if(!noCustDetails) {
            // add customers to list based on string details
            for (size_t i = 0; i < customersDetails.size(); i++) {
                string custName;
                // extract customer name
                while (customersDetails.at(i) != ',' and i < customersDetails.size()) {
                    custName = custName + (customersDetails.at(i));
                    i++;
                }
                string custStrategy = customersDetails.substr(i + 1, 3);
                i += 3;
                size_t t_cap = getTrainer(trainerId)->getCapacity();
                if (t_cap >= customersList.size() + 1) {
                    // create customer based on customer strategy
                    //SweatyCustomer *swt = new SweatyCustomer(custName, workout_number)
                    if (custStrategy == "swt"){
                        customersList.push_back(new SweatyCustomer(custName, workout_number));
                        workout_number++;
                    //CheapCustomer *chp = new CheapCustomer(custName, workout_number);
                     }else if (custStrategy == "chp") {
                        customersList.push_back(new CheapCustomer(custName, workout_number));
                        workout_number++;
                    } else if (custStrategy == "mcl") {
                        // trying to delete pointer of mcl
                        customersList.push_back(new HeavyMuscleCustomer(custName, workout_number));
                        workout_number++;
                    } else if (custStrategy == "fbd"){
                        customersList.push_back(new FullBodyCustomer(custName, workout_number));
                        workout_number++;
                    }
                    // promote unique customer identifier
                }
            }
        }
        // trainer & customer validation will be made in act segment
        OpenTrainer *openTrainer =new OpenTrainer(trainerId,customersList);
        openTrainer->act(*this);
        actionsLog.push_back(openTrainer);
    }
    //if action is to activate trainer's orders
    else if (actionType=="order") {
        // extract string after first space
        int trainerId = stoi(userAction.substr(userAction.find(" ")+1));
        // trainerId validity will be checked in act
        Order *order =new Order(trainerId);
        order->act(*this);
        actionsLog.push_back(order);
    }
    //if action is to move customer from 1 trainer to another
    else if (actionType=="move") {
        //make a new string by removing the string "move " from the input string
        string action=userAction.substr(5);
        //the remaning string is: x_y_z such as x=source_trainer_id, y=destination_trainer_id and z=customer_id
        //so x's indexes are 0 to first appearance of space, y's index is 2 to second appearance of sapce
        //and z's 4 to the end of the string (because each id can contain more than one digit)
        int src=stoi(action.substr(0,action.find(" ")));
        //remove the string "x " from the new string
        action=action.substr(action.find(" ")+1);
        int dst=stoi(action.substr(0,action.find(" ")));
        //remove the string "y " from the new string
        action=action.substr(action.find(" ")+1);
        int customer_id=stoi(action);
        MoveCustomer *move =new MoveCustomer(src,dst,customer_id);
        //*****returning a pointer of studio do we need move constructor****//
        move->act(*this);
        actionsLog.push_back(move);
    }
    //if action is to close a given trainer session
   else if(actionType=="close"){
       // extract string after first space
       int trainerId = stoi(userAction.substr(userAction.find(" ")+1));
       // trainerId validity will be checked in act
       Close *close=new Close(trainerId);
       //*****returning a pointer of studio do we need move constructor****//
       close->act(*this);
       actionsLog.push_back(close);
   }
    //if action is to close all trainers' sessions
    else if(actionType=="closeall" ) {
        CloseAll().act(*this);
    }
    //if action is to print workout options for studio
    else if (actionType=="workout_options") {
        PrintWorkoutOptions *pWO=new PrintWorkoutOptions();
        pWO->act(*this);
        actionsLog.push_back(pWO);
        }
    //if action is to print trainer status
    else if (actionType=="status") {
        int space=userAction.find(' ');
        while (userAction.at(space)==' '){
            space++;}
        int trainerId = stoi(userAction.substr(space));
        PrintTrainerStatus *pTS=new PrintTrainerStatus(trainerId);
        pTS->act(*this);
        actionsLog.push_back(pTS);
        }
    //backup the current studio state
    else if (actionType=="backup") {
        BackupStudio *backupS=new BackupStudio();
        cout<<"line 452    Studio.cpp"<<endl;
        backupS->act(*this);
        cout<<"line 452    Studio.cpp"<<endl;
        actionsLog.push_back(backupS);
        }
    //restore backed-up studio state
    else if (actionType=="restore") {
        RestoreStudio *restore=new RestoreStudio();
        restore->act(*this);
        actionsLog.push_back(restore);
        }
    // print log is last action
    else if (actionType=="log") {
        PrintActionsLog *printALog=new PrintActionsLog();
        printALog->act(*this);
        actionsLog.push_back(printALog);
    }
}

//open the Studio
void Studio::start(){
    open=true;
    cout<<"Studio is now open!"<<endl;
    string userAction;
    //loop for actions
    while(userAction!="closeall") {
        // wait command from terminal
        getline(cin,userAction);
        // no need to perform input checks
        string actionType = identifyAction(userAction);
        // create action based on type, act and document action
        startAction(actionType, userAction);
    }
    stole();
}


void Studio::bulkStart(const std::string &commandPath) {
    open=true;
    cout<<"Studio is now open!"<<endl;
    // extract command list
    // read command file row by row
    ifstream ReadCommandFile(commandPath);
    string commandRow;
    vector<string> commandRows;
    // add only non empty rows to a vector of strings
    while(getline(ReadCommandFile,commandRow)) {
        // ignore empty lines and comments
        if(!isEmptyLine(commandRow)){
            commandRows.push_back(commandRow);
        }
    }
    // close file read session
    ReadCommandFile.close();

    //loop for actions
    for(size_t i=0;i<commandRows.size();i++) {
        string userAction = commandRows.at(i);
        // no need to perform input checks
        if (i!=commandRows.size()-1){
        userAction = userAction.substr(0,userAction.size()-1);
        }
        string actionType = identifyAction(userAction);
        cout<<"doing: "<<userAction<<endl;
        cout<<"--------------------------------"<<endl;
        // create action based on type, act and document action
        startAction(actionType, userAction);
        cout<<"--------------------------------"<<endl;
    }
    cout<<"Bulk action list completed"<<endl;
    stole();
}

//returns the number of trainers
int Studio::getNumOfTrainers() const{
    return trainers.size();
}

//receives a trainer id and returns that trainer if exists assume the trainer exists
Trainer* Studio::getTrainer(int tid){
    size_t trainId=tid;
    if (tid>=0 and trainId<trainers.size())
        return trainers.at(trainId);
    return nullptr;
}

//returns the studio's action log
const vector<BaseAction *> &Studio::getActionsLog() const {return actionsLog;}

//returns the studio's workout options
vector<Workout>& Studio::getWorkoutOptions() {return workout_options;}

vector <Workout> &Studio::getSortedWorkoutOptions() {return sorted_workout_options;}

