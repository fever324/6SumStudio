#ifndef Toya_PROGRESS_MODEL_h
#define Toya_PROGRESS_MODEL_h

#include <vector>

using namespace std;

class ProgressModel {
private:
    int _totalLevels;
    int _levelsCompleted;
    
    vector<int> _scores;
    
    void readData();
    
public:
    static ProgressModel* getInstance();
    void writeData(int level, int score);
    int getScore(int level);
};

#endif /* ToyaProgressModel_h */
