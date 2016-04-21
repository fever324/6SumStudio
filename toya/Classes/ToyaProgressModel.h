#ifndef TOYA_PROGRESS_MODEL_h
#define TOYA_PROGRESS_MODEL_h

#include <vector>

using namespace std;

class ProgressModel {
private:
    int _totalLevels;
    int _levelsCompleted;
    
    vector<int> _scores;
    
    void readData();
    
public:
    static void init();
    static ProgressModel* getInstance();
    void writeData(int level, int score, float completeTime, int star);
    int getScore(int level);
    int getNextLevel();
};

#endif /* ToyaProgressModel_h */
