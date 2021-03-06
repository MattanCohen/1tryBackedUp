#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Studio.h"
#include <fstream>

//Forward declaration
class Studio;
extern Studio* backup;

//**nir:what is this???*//
//Mattan: that was the backup studio for backup/restore functions
//        I moved it to the cpp file doopsi poopsi i made an oopsi..

enum ActionStatus{
    COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;

    virtual ~BaseAction();
    virtual BaseAction& operator=(const BaseAction& rhs);
    BaseAction(const BaseAction& rhs);

protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    void act(Studio &studio);
    std::string toString() const;

    ~OpenTrainer();
    OpenTrainer& operator=(const OpenTrainer& rhs);
    OpenTrainer& operator=(OpenTrainer&& rhs);
    OpenTrainer(const OpenTrainer& rhs);
    OpenTrainer(OpenTrainer&& rhs);
    void copyCustomers(const OpenTrainer& rhs);

private:
	const int trainerId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;

    ~Order();
    Order& operator=(const Order& rhs);
    Order(const Order &rhs);

private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;

    ~MoveCustomer();
    MoveCustomer& operator=(const MoveCustomer& rhs);
    MoveCustomer(const MoveCustomer& rhs);

private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;

    ~Close();
    Close& operator=(const Close& rhs);
    Close(const Close& rhs);

private:
    // trainerId requested and salary (if command is successful)
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;

    ~CloseAll();
    CloseAll& operator=(const CloseAll& rhs);
    CloseAll(const CloseAll& other);

private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;

    ~PrintWorkoutOptions();
    PrintWorkoutOptions& operator=(const PrintWorkoutOptions& rhs);
    PrintWorkoutOptions(const PrintWorkoutOptions& rhs);

private:
    std::string getTypeAsString(WorkoutType type) const;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;

    ~PrintTrainerStatus();
    PrintTrainerStatus& operator=(const PrintTrainerStatus& rhs);
    PrintTrainerStatus(const PrintTrainerStatus& rhs);

private:
    const int trainerId;
};

class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;

    ~PrintActionsLog();
    PrintActionsLog& operator=(const PrintActionsLog& rhs);
    PrintActionsLog(const PrintActionsLog& other);

private:
};

extern bool ever_backed;
//extern Studio* backup;

class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;

    ~BackupStudio();
    BackupStudio& operator=(const BackupStudio& rhs);
    BackupStudio& operator=(BackupStudio&& rhs);
    BackupStudio(const BackupStudio& rhs);
    BackupStudio(BackupStudio&& rhs);
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;

    ~RestoreStudio();
    RestoreStudio& operator=(const RestoreStudio& rhs);
    RestoreStudio& operator=(RestoreStudio&& rhs);
    RestoreStudio(const RestoreStudio& rhs);
    RestoreStudio(RestoreStudio&& rhs);

private:
};




#endif