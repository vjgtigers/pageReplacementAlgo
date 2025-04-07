# pageReplacementAlgo


#include <iostream>
#include <vector>
using namespace std;


//basic memory class that algorithms can inherit from with basic funtions and some required templates
//modular design allows for easy implementation of other memory algorithms and easy to change variables to test
//different frame sizes
class memoryClass {
public:
    virtual ~memoryClass() = default;

    //basic variable definitions
    int frameSize;
    int pageFaults;
    vector<int> memArr = {};

    //templated functions that inhereted classes need to implement
    virtual void insertPage(int page) = 0;
    virtual void advanceElements() = 0;

    //constructor
    memoryClass(int frameSize) {
        this->frameSize = frameSize;
        this->pageFaults = 0;
        memArr.resize(frameSize, -1);
    }

    //checks if a page is in the memory array
    bool checkForPage(int page) {
        if(memArr.size() != frameSize) {cout << "Frame Size Error" << endl; exit(-1);}
        for(int& item : memArr) {
            if(item == page) {
                return true;
            }
        }
        return false;
    }

    //output the memory array
    void outputRam() {
        for(int i = 0; i < frameSize; ++i) {
            cout << memArr[i] << ", ";
        }
        cout << endl;
    }



};

//fifo implementation inheriting from memoryClass
class FIFO : public memoryClass {
public:
    FIFO(int size) : memoryClass(size){};//constructor

    //move all elements down one in the memory array
    void advanceElements() override {
        for(int i = memArr.size()-2; i >= 0; --i) {
            memArr[i+1] = memArr[i];
        }
    }
    //inserts a page into the first slot in memory if not found
    void insertPage(int page) override {
        if(checkForPage(page) == true) {
            return;
        }

        pageFaults+= 1;
        advanceElements();
        memArr[0] = page;

        return;

    }


};

//lru implementation inhereting from memory class
class LRU : public memoryClass {
public:
    explicit LRU(int size) : memoryClass(size){}; //constructor

    //move all elements down one in the memory array
    void advanceElements() override {
        for(int i = memArr.size()-2; i >= 0; --i) {
            memArr[i+1] = memArr[i];
        }
    }

    //insert page if not found and if found move it to the top
    void insertPage(int page) override {
        if(checkForPage(page) == true) {
            for(int i = 0; i < frameSize; ++i) {
                if(memArr[i] == page) {
                    if(i == 0) {
                        break;
                    }

                    for(int p = i-1; p >= 0; --p) {
                        memArr[p+1] = memArr[p];
                    }
                    memArr[0] = page;

                    break;
                }
            }
            return;
        }
        pageFaults += 1;
        advanceElements();
        memArr[0] = page;
        return;

    }



};

//ref pages
int pageref[100] = {1, 2, 7, 1, 2, 1, 3, 4, 2, 5, 2, 7, 8, 9, 1, 2, 7, 2, 1, 3, 1, 7, 2, 3, 2, 1, 2, 4, 5, 4, 5, 6, 7, 8, 9, 6, 3, 1, 2, 3,
1, 2, 3, 6, 7, 5, 6, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 7, 7, 7, 2, 2, 1, 7, 6, 7, 8, 7, 8, 9, 7, 8, 2, 7, 8, 3, 5, 3,
5, 7, 1, 3, 1, 3, 1, 3, 2, 3, 4, 5, 4, 6, 4, 7, 3, 8, 3, 9};



//enums for defining the test that is being ran
enum simType {FIFO_, LRU_};

//struct to record results of each test to find the best
struct simResults {
    simType type;
    int frameSize;
    int pageFaults;
};


//function to run tests on the different algorithms with provided info
//modular simulation function since the clases inheret from the parent
//so it makes it easy to choose which one to create the class from without multiple different functions
void runSimulation(simType type, int frameSize, vector<simResults>& data) {
    memoryClass* sim;
    simResults result{};
    if(type == FIFO_) {
        sim = new FIFO(frameSize);
        result.type = FIFO_;
    }
    if(type == LRU_) {
        sim = new LRU(frameSize);
        result.type = LRU_;
    }

    for(int i = 0; i < 100; ++i) {
        sim->insertPage(pageref[i]);
        //sim->outputRam();
    }
    cout << "Frame Number " << sim->frameSize << ": " << sim->pageFaults << " Page Faults" << endl;
    result.frameSize = frameSize;
    result.pageFaults = sim->pageFaults;
    data.push_back(result);
    delete sim;
}


int main()
{


    vector<simResults> results;

    cout << "Page Replacement Algorithm Evaluation Results" << endl << endl;
    cout << "FIFO Results:" << endl;

    //run different tests
    runSimulation(FIFO_, 3, results);
    runSimulation(FIFO_, 4, results);
    runSimulation(FIFO_, 5, results);
    runSimulation(FIFO_, 6, results);
    runSimulation(FIFO_, 7, results);

    cout << endl;
    cout << "LRU Results:" << endl;
    runSimulation(LRU_, 3, results);
    runSimulation(LRU_, 4, results);
    runSimulation(LRU_, 5, results);
    runSimulation(LRU_, 6, results);
    runSimulation(LRU_, 7, results);

    simResults best{};
    best = results[0];
    for(int i = 1; i < results.size(); ++i) {
        if(best.pageFaults > results[i].pageFaults) {
            best = results[i];
        }
    }
    //find best and output info
    cout << endl;
    cout << "Minimum Page Fault: Algorithm ";
    if(best.type == LRU_) {
        cout << "LRU ";
    } else {
        cout << "FIFO ";
    }
    cout << "with " << best.pageFaults << " and Frame Number " << best.frameSize << endl;

    return 0;
}
