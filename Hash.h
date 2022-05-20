#include <vector>
#include <functional>
#include <set>
#include <unordered_set>
//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstracti (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables
template <class T, size_t SIZE>
class Hash {
    public:
        virtual ~Hash() {}
        virtual void insert(T new_data) = 0;
        virtual void remove(T old_data) = 0;
        virtual bool search(T test_data) const = 0;
        virtual void change(T old_data, T new_data) = 0;
};

// YOU: Write three hash tables, one for linear probing, one for chaining, one using any other method you want 
// (Quadratic, Double Hashing, or using the STL unordered_set class are all ok)

// LINEAR PROBING
template <class T, size_t SIZE>
class LinearProbingHash : public Hash<T,SIZE> {
    //This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
    //OPEN means the bucket has never held a value in it
    //FILLED means it currently holds a valid value in it
    //DELETED means the value in it has been deleted and not replaced yet
    enum class STATUS : char {OPEN, FILLED, DELETED};
    std::vector<T> data;
    std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
    public:
    LinearProbingHash() {
        data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
        status.resize(SIZE,STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
    }
    void insert(T new_data) override {
        if (search(new_data)) return;
        //Step 1 - Compute the initial bucket to try
        size_t address = std::hash<T>{}(new_data) % SIZE;
        size_t counter = 0;
        //Step 2 - We will start at the bucket at "address" and then move to the right,
        //  looping around to the start if necessary, to find first available bucket
        //  Either an OPEN bucket or a DELETED bucket will do.
        //TODO: Make this not infinite loop on a full hash table
        while (status.at(address) == STATUS::FILLED) {
            if (counter >= SIZE) return;
            counter++;
            address = (address+1)%SIZE;//Move one bucket to the right, looping around

        }
        //Ok, we've found an open spot, write the data in and mark it as filled
        data.at(address) = new_data;
        status.at(address) = STATUS::FILLED;
    }
    //YOU:
    void remove(T old_data) override {
        size_t address = std::hash<T>{}(old_data) % SIZE;
        size_t counter = 0;
        while (true) {
            //if (status.at(address) == STATUS::OPEN or status.at(address) == STATUS::DELETED) return;
            if (data.at(address) == old_data and status.at(address) == STATUS::FILLED) {
                status.at(address) = STATUS::DELETED;
            }
            counter++;
            if (counter >= SIZE) return;
            address = (address+1)%SIZE;
        }
    }
    //YOU:
    bool search(T test_data) const override {
        size_t address = std::hash<T>{}(test_data) % SIZE;
        if (data.at(address) == test_data and status.at(address) == STATUS::FILLED) return true;
        size_t counter = 0;
        while (true) {
            if (status.at(address) == STATUS::FILLED) {
                if (data.at(address) == test_data) return true;
            }
            counter++;
            if (counter >= SIZE) return false;
            address = (address+1)%SIZE;
        }
        return false;
    }
    //YOU:
    void change(T old_data, T new_data) override {
        //Remember, only change it if it actually is in the hash table
        //size_t address = std::hash<T>{}(old_data) % SIZE;
        if (search(old_data)) {
            remove(old_data);
            insert(new_data);
        }
    }
};
// CHAINING
//YOU: Do the same as the above, with chaining instead of linear probing
//You can do chaining either with LLs or with BSTs
template <class T, size_t SIZE>
class ChainingHash : public Hash<T,SIZE> {
    //YOU:
    enum class STATUS : char {OPEN, FILLED, DELETED};
    std::vector<std::set<T>> data;
    std::vector<STATUS> status;
    public:
    //YOU:
    ChainingHash() {
        data.resize(SIZE);
        status.resize(SIZE,STATUS::OPEN);
    }
    //YOU:
    void insert(T new_data) override { //Override means it is overriding a virtual function
        //Step 1 - Compute the initial bucket to try
        if (search(new_data)) return;
        size_t address = std::hash<T>{}(new_data) % SIZE;
        data.at(address).insert(new_data);
    }
    //YOU:
    void remove(T old_data) override {
        size_t address = std::hash<T>{}(old_data) % SIZE;
        data.at(address).erase(old_data);
    }
    //YOU:
    bool search(T test_data) const override {
        size_t address = std::hash<T>{}(test_data) % SIZE;
        return data.at(address).contains(test_data);
    }
    //YOU:
    void change(T old_data, T new_data) override {
        //size_t address = std::hash<T>{}(old_data) % SIZE;
        if (search(old_data)) {
            remove(old_data);
            insert(new_data);
        }
    }
};

// STL UNORDERED_SET IMPLIMENTATION
//YOU : Write a third hashing class here, following the pattern above. You can do it using double hashing, quadratic hashing, or the unordered_set STL class, whatever you like.
template <class T, size_t SIZE>
class SetHash : public Hash<T,SIZE> {
    enum class STATUS : char {OPEN, FILLED, DELETED};
    std::vector<std::unordered_set<T>> data;
    std::vector<STATUS> status;
    public:
    SetHash() {
        data.resize(SIZE);
        status.resize(SIZE,STATUS::OPEN);
    }
    void insert(T new_data) override {
        if (search(new_data)) return;
        size_t address = std::hash<T>{}(new_data) % SIZE;
        data.at(address).insert(new_data);
    }
    void remove(T old_data) override {
        size_t address = std::hash<T>{}(old_data) % SIZE;
        data.at(address).erase(old_data);
    }
    bool search(T test_data) const override {
        size_t address = std::hash<T>{}(test_data) % SIZE;
        return data.at(address).contains(test_data);
    }
    void change(T old_data, T new_data) override {
        //size_t address = std::hash<T>{}(old_data) % SIZE;
        if (search(old_data)) {
            remove(old_data);
            insert(new_data);
        }
    }
};
