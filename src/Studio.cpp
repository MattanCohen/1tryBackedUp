#include "../include/Studio.h"

using namespace std;


//for debugging only
//recieves trainers vector, workout options vector and action log to set to new Studio
Studio::Studio(const std::vector<Trainer *> _trainers, const std::vector <Workout> _workout_options):trainers(_trainers),
                workout_options(_workout_options),sorted_workout_options(_workout_options){
    // use reversed iterators in order to sort in dsc order
    sort(sorted_workout_options.rbegin(),sorted_workout_options.rend(),sort_by_price());
}
//for debugging only



//rule of 5:
//d-tor in case Customer refrence is deleted
Studio::~Studio() {
    this->stole();
}
//ass. op.
Studio& Studio::operator=(const Studio &rhs) {
    if (&*this!=&rhs){
        open=rhs.open;
        trainers=rhs.trainers;
        workout_options=rhs.workout_options;
        sorted_workout_options=rhs.sorted_workout_options;
        actionsLog=rhs.actionsLog;
        workout_number=rhs.workout_number;
    }
    return *this;
}
//move ass.op.
Studio& Studio::operator=(Studio &&rhs) {
    if (this!=&rhs){
        open=rhs.open;
        trainers=rhs.trainers;
        workout_options=rhs.workout_options;
        sorted_workout_options=rhs.sorted_workout_options;
        actionsLog=rhs.actionsLog;
        workout_number=rhs.workout_number;
        rhs.stole();
    }
    return *this;
}
//move c-tor
Studio::Studio(Studio &&rhs):open(rhs.open),trainers(rhs.trainers),workout_options(rhs.workout_options),sorted_workout_options(rhs.sorted_workout_options),actionsLog(rhs.actionsLog),workout_number(rhs.workout_number) {
    if(this!=&rhs)
        rhs.stole();
}
//copy c-tor:
Studio::Studio(const Studio& rhs):open(rhs.open),trainers(rhs.trainers),workout_options(rhs.workout_options),sorted_workout_options(rhs.sorted_workout_options),actionsLog(rhs.actionsLog),workout_number(rhs.workout_number) {}

void Studio::stole() {delete this;}

//creating an empty Studio
Studio::Studio():open(false){workout_number=0;}

// given a list of trainer capacities adds to "trainers"
void Studio::AddTrainers(std::string trainersRow) {

    //_________________________________adding trainers:___________________
    //line contains capacities seperated by commas. ignore the commas.
    int i=0;
    //while we didn't reach the new line
    while (i<trainersRow.size() && trainersRow.at(i)!='\n') {
        string trainerCapacityString="";
        // create string that contains capacity
        while(trainersRow.at(i)!='\n' or trainersRow.at(i)!=','){
            trainerCapacityString.push_back(trainersRow.at(i));
            i++;
        }
        // create fitting trainer object
        Trainer *trainer = new Trainer(stoi(trainerCapacityString));
        trainers.push_back(trainer);
        // skip comma/endline
        i++;
    }
}
// given workout row adds it to "workout_options"
void Studio::AddWorkoutOption(std::string workoutRow, int workoutId) {
    int i=0;
    // extract workout name from the start to the first comma
    string workoutName="";
    while(workoutRow.at(i)!=','){workoutName.push_back(workoutRow.at(i));}
    // skip comma
    i++;
    // extract workout type from after the first comma to the second comma
    string workoutType="";
    while(workoutRow.at(i)!=','){workoutType.push_back(workoutRow.at(i));}
    //skip comma
    i++;
    // extract workout price from after the second comma to the first endLine
    string workoutPriceStr = "";
    while (i<workoutRow.size() && workoutRow.at(i)!='\n') {workoutPriceStr.push_back(workoutRow.at(i));}
    Workout work(workoutId,workoutName,stoi(workoutPriceStr),getType(workoutType));;
    workout_options.push_back(work);
}


// checks if a row is an empty line and needs to be ignored
bool Studio::isEmptyLine(std::string configRow) {
    // sanity check
    if(configRow.size()==0) {return true;}
    else {
        // empty line or starts with a comment
        if(configRow.at(0)=='\n' or configRow.at(0)=='#') {return true;}
            // line is not empty and contains relevant information
        else {return false;}
    }
}

//creating a Studio by a received config file. assumes config file is legal
Studio::Studio(const std::string &configFilePath) {
    // read config file row by row
    ifstream ReadConfigFile(configFilePath);
    string configRow;
    vector<string> configRows;
    // add only non empty rows to a vector of strings
    while(getline(ReadConfigFile,configRow)) {
        // ignore empty lines and comments
        if(!isEmptyLine(configRow))
            configRows.push_back(configRow);
    }
    // close file read session
    ReadConfigFile.close();
    // index to follow current configRow processed
    int rowIndex = 0;
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
    int workoutIndex = 0;
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
    for (int i=0; i<workout_options.size(); i++)
        sorted_workout_options.push_back(workout_options.at(i));
    // use reversed iterators in order to sort in dsc order
    std::sort(sorted_workout_options.rbegin(),sorted_workout_options.rend(),sort_by_price());
}


/*
//creating a Studio by a received config file. assumes config file is legal
Studio::Studio(const string &configFilePath){
    // read config path and insert row by row to string
    ifstream ReadConfigFile(configFilePath);
    string configRow;
    string configFile="";
    while(getline(ReadConfigFile,configRow)) {
        configFile.append(configRow);
    }
    int i=0;
    int ignored_char=configFile.find('\n');
     //_________________________________adding trainers:___________________
    //from i=0 to the end of the line ('\n') is the number of trainers string representation
    int num_of_trainers= stoi(configFile.substr(0,ignored_char));
    //remove from the string the first line and the \n char
    configFile=configFile.substr(ignored_char+1);
    //next line contains capacities seperated by commas. ignore the commas.
    int ignore_comma=configFile.find(',');
    ignored_char=configFile.find('\n');
    i=0;
    //while we didn't reach the new line
    while (i<configFile.size() && configFile.at(i)!='\n'){
        //if there's workout options in the config file, ignore_comma > i = previous_ignore_comma+1
        //if there's no workout options, ignore_comma = 0 < i so we have 1 more trainer to add and that's it
        if (ignore_comma<i){
            Trainer trainer=Trainer(stoi(configFile.substr(i,configFile.size())));
            trainers.push_back(trainer);
            return;
        }
        else{

        }
        for (int j=i; j<ignore_comma-i; j++)
        //each trainer_i id is the int represented by indexes i to appearance of ',' so add to trainers
        Trainer trainer=Trainer(stoi(configFile.substr(i,ignore_comma-i)));
        //push the trainer to the trainers list
        trainers.push_back(trainer);
        //use i as index of next new argument
        i=ignore_comma+1;
        ignore_comma=configFile.substr(i).find(',');
        ignored_char=configFile.substr(i).find('\n');
        //meaning there's only trainers and no workout options
        //the function should add the last trainer and finish
        if (configFile.size()>ignore_comma+1){
            Trainer trainer_i(stoi(configFile.substr(0,configFile.size())));
            trainers.push_back(trainer_i);
        }
            configFile=configFile.substr(ignore_comma+1);
        //point at next comma
     }
    //configFile's size is 0 if no workout options were provided
    if (configFile.size()==0)
        return;
    //else it now starts with \n
    configFile=configFile.substr(ignore_comma+1);
    //_________________________________adding workouts:___________________
    //current i points at the first appearance of workout name in tuples given.
    //each tuple holds workout_name,workout_type,workout_price (devided by commas)
    //the tuples are designated by a new line '\n', and the config file ends
    //with the last tuple. each line holds at most 2 commas.
    int workout_id=0;
    string workout_name;
    string curr_type;
    int price;
    string price_in_string;
    WorkoutType workout_type;
    int num_of_commas=0;
    while (i<configFile.size()){
        //no commas yet means we are getting a new name of a new workout.
        if (num_of_commas==0){
            workout_name.erase();
            while (configFile.at(i)!=','){
                workout_name.push_back(configFile.at(i));
                i++;
            }
            num_of_commas++;
        }
        //once num_of_commas has been increased, we are getting the type of the new workout
        else if (num_of_commas==1){
            curr_type.erase();
            while (configFile.at(i)!=','){
                curr_type.push_back(configFile.at(i));
                i++;
            }
            workout_type=getType(curr_type);
            num_of_commas++;
        }
        //now we are getting the price of the new workout
        else if (num_of_commas==2){
            price_in_string.erase();
            //we need to watch it because the last price might be the last price thus i will get out of range
            while (i+1<configFile.size() && configFile.at(i)!='\n'){
                price_in_string.push_back(configFile.at(i));
                i++;
            }
            num_of_commas++;
            price= stoi(price_in_string);
        }
        //there's not really 3 commas ever but it signifies that we finished receiving the new workout
        else if(num_of_commas==3){
            Workout work(workout_id,workout_name,price,workout_type);;
            workout_options.push_back(work);
            //reset num_of_commas to signify a new workout is ready to come
            num_of_commas=0;
            //now i either points to the last char in config file or to '\n' so either way we append i
            i++;
        }
    }
    //now i is out of config file range meaning config file was read thoroughly
    //after everything was set, we want to create a sorted workout options for heavy muscle customers

    // set identifier used to create unique Customer Id's
    workout_number=0;
}
*/
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
    string commmandPrefix = "";
    int i=0;
    while (i<userAction.size() && userAction.at(i)!=' '){
        commmandPrefix.push_back(userAction.at(i));
        i++;
    }
    return commmandPrefix;
}

// creates action based on type and user action
void Studio::startAction(std::string actionType, std::string userAction) {
    //if action is to open new trainer
    if (actionType=="open"){
        // extract trainer id
        int firstSpaceIndex = userAction.find(" ");
        // +1 so that space won't be included
        string userActionAfterSpace = userAction.substr(firstSpaceIndex+1);
        int secondSpaceIndex = userActionAfterSpace.find(" ");
        // extract trainerId
        int trainerId = stoi(userActionAfterSpace.substr(0,secondSpaceIndex));
        // +1 so that space won't be included
        string customersDetails = userActionAfterSpace.substr(secondSpaceIndex+1);
        vector<Customer *> customersList;
        // add customers to list based on string details
        for(int i=0;i<customersDetails.size();i++) {
            string custName = "";
            // extract customer name
            while(customersDetails.at(i)!=',' and i<customersDetails.size())
                {
                    custName.push_back(customersDetails.at(i));
                    i++;
                }
            string custStrategy = customersDetails.substr(i,i+3);
            i+=3;
            // create customer based on customer strategy
            if(custStrategy=="swt")
            {
                SweatyCustomer swt = SweatyCustomer(custName,workout_number);
                customersList.push_back(&swt);
            }
            else if(custStrategy=="chp")
            {
                CheapCustomer chp(custName,workout_number);
                customersList.push_back(&chp);
            }
            else if(custStrategy=="mcl")
            {
                HeavyMuscleCustomer mcl(custName,workout_number);
                customersList.push_back(&mcl);
            }
            else {
                FullBodyCustomer fbc(custName,workout_number);
                customersList.push_back(&fbc);
            }
            // promote unique customer identifier
            workout_number++;
        }
        // trainer & customer validation will be made in act segment
        OpenTrainer openTrainer(trainerId,customersList);
        //*****returning a pointer of studio do we need move constructor****//
        openTrainer.act(*this);
        actionsLog.push_back(&openTrainer);

    }
    //if action is to activate trainer's orders
    if (actionType=="order") {
        // extract string after first space
        int trainerId = stoi(userAction.substr(userAction.find(" ")+1));
        // trainerId validity will be checked in act
        Order order(trainerId);
        //*****returning a pointer of studio do we need move constructor****//
        order.act(*this);
        actionsLog.push_back(&order);
    }
    //if action is to move customer from 1 trainer to another
    if (actionType=="move") {
        //make a new string by removing the string "move " from the input string
        string action=userAction.substr(5);
        //the remaning string is: x_y_z such as x=source_trainer_id, y=destination_trainer_id and z=customer_id
        //so x's indexes are 0 to first appearance of space, y's index is 2 to second appearance of sapce
        //and z's 4 to the end of the string (because each id can contain more than one digit)
        int dst=stoi(action.substr(0,action.find(" ")));
        //remove the string "x " from the new string
        action=action.substr(action.find(" ")+1);
        int src=stoi(action.substr(0,action.find(" ")));
        //remove the string "y " from the new string
        action=action.substr(action.find(" ")+1);
        int customer_id=stoi(action);
        MoveCustomer move(src,dst,customer_id);
        //*****returning a pointer of studio do we need move constructor****//
        move.act(*this);
        actionsLog.push_back(&move);
    }
    //if action is to close a given trainer session
   if(actionType=="close"){
       // extract string after first space
       int trainerId = stoi(userAction.substr(userAction.find(" ")+1));
       // trainerId validity will be checked in act
       Close close(trainerId);
       //*****returning a pointer of studio do we need move constructor****//
       close.act(*this);
       actionsLog.push_back(&close);
   }
    //if action is to close all trainers' sessions
    if(actionType=="closeall") {
        CloseAll cA;
        cA.act(*this);
        actionsLog.push_back(&cA);
    }
    //if action is to print workout options for studio
    if (actionType=="workout_options") {
        PrintWorkoutOptions pWO;
        pWO.act(*this);
        actionsLog.push_back(&pWO);
        }
    //if action is to print trainer status
    if (actionType=="status") {
        // everything after "status " string and not including endline
        int trainerId = stoi(userAction.substr(7,userAction.size()-1));
        PrintTrainerStatus pTS(trainerId);
        pTS.act(*this);
        actionsLog.push_back(&pTS);
        }
    //backup the current studio state
    if (actionType=="backup") {
        BackupStudio backupS;
        backupS.act(*this);
        actionsLog.push_back(&backupS);
        }
    //restore backed-up studio state
    if (actionType=="restore") {
        RestoreStudio restore;
        restore.act(*this);
        actionsLog.push_back(&restore);
        }
    // print log is last action
    else {
        PrintActionsLog printALog;
        printALog.act(*this);
        actionsLog.push_back(&printALog);
    }
}

//open the Studio
void Studio::start(){
    open=true;
    cout<<"Studio is now open!"<<endl;
    string userAction="";
    //loop for actions
    while(userAction!="closeall") {
        // wait command from terminal
        cin>>userAction;
        // no need to perform input checks
        string actionType = identifyAction(userAction);
        // create action based on type, act and document action
        startAction(actionType, userAction);
    }
}


//returns the number of trainers
int Studio::getNumOfTrainers() const{
    return trainers.size();
}

//receives a trainer id and returns that trainer if exists assume the trainer exists
Trainer* Studio::getTrainer(int tid){
    return trainers.at(tid);
}

//returns the studio's action log
const vector<BaseAction *> &Studio::getActionsLog() const {return actionsLog;}

//returns the studio's workout options
vector<Workout>& Studio::getWorkoutOptions() {return workout_options;}

vector <Workout> &Studio::getSortedWorkoutOptions() {return sorted_workout_options;}

