#pragma once

class Manager {
public:
    // singleton access
    static Manager& get() { return sInstance; }

    // empty constructor/destructor
    Manager();
    ~Manager();

    // prevent copies
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    // explicit startup/shutdown
    void startUp();
    void shutDown();

    // functionality
    void stopAllProcesses();

private:
    static Manager sInstance;

    bool isInitialized;
};
