#ifndef TOYA_PROGRESS_MODEL_h
#define TOYA_PROGRESS_MODEL_h

#include <vector>
#include <map>

using namespace std;

class ProgressModel {
private:
    struct LevelInfo {
        int _level;
        int _overallStar;
        double _time;
        bool _locked;
        
        LevelInfo(int level, int overallStar, double time, bool locked) {
            _level = level;
            _overallStar = overallStar;
            _time = time;
            _locked = locked;
        }
        
        void setStar(int star) { _overallStar = star; }
        void setTime(double time) { _time = time; }
        void unlock() { _locked = false; }
        
        int getStar() { return _overallStar; }
        double getTime() { return _time; }
        bool isLocked() { return _locked; }
    };
    
    int _totalLevels;
    int _levelsCompleted;
    
    map<int, LevelInfo*> _levels;
    
    void readData();
    
public:
    static void init();
    static ProgressModel* getInstance();
    
    int getTotalLevels();
    
    int getStars(int level);
    double getCompleteTime(int level);
    bool isLocked(int level);
    
    void writeData(int level, float completeTime, int star);
};

#endif /* ToyaProgressModel_h */
