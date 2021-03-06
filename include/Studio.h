#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <fstream>



class BaseAction;

class Studio{
//given procedures:
public:
    Studio();
    Studio(const std::string &configFilePath); //make news, delete what's not needed
    void start();//make news, delete whats not needed
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid); //make news, delete whats not needed
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

    std::vector<Workout>& getSortedWorkoutOptions();
    WorkoutType getType(std::string rhs);


    ~Studio();
    Studio& operator=(const Studio& rhs);
    Studio& operator=(Studio&& rhs);
    Studio(const Studio& rhs);
    Studio(Studio&& rhs);
    void stole();

    // special pointer "constructor"
    Studio& operator=(const Studio* rhs);
    Studio(const Studio* rhs);

//    void createBackup();
//    bool isBacked();
//    void restoreBackup();
    void bulkStart(const std::string &commandPath);
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<Workout> sorted_workout_options;
    std::vector<BaseAction*> actionsLog;
    //assign each customer the number assosiated to time of arrival to the studio
    int workout_number;
    std::string identifyAction(std::string userAction);
    void startAction(std::string actionType, std::string userAction);//make news, delete whats not needed
    void AddTrainers(std::string trainersRow);//make news, delete whats not needed
    void AddWorkoutOption(std::string workoutRow, int workoutId);
    bool isEmptyLine(std::string configRow);
    void copyTrainers(const Studio& rhs);
    void copyTrainers(const Studio* rhs);
    void copyActionLogs(const Studio& rhs);
    void copyActionLogs(const Studio* rhs);
//    Studio* backup;
//    bool backed;
};

#endif
