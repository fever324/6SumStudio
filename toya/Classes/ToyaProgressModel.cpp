#include <cstdio>
#include "ToyaProgressModel.h"
#include <cornell.h>

#include <json/rapidjson.h>
#include <json/filereadstream.h>
#include <json/document.h>
#include <json/filewritestream.h>
#include <json/writer.h>

#define PROGRESS_DATA "jsons/progress.json"

using namespace std;
using namespace cocos2d;
using namespace rapidjson;

static ProgressModel* _progress = nullptr;

ProgressModel* ProgressModel::getInstance() {
    if(_progress == nullptr) {
        _progress = new (std::nothrow) ProgressModel();
        _progress->readData();
    }
    
    return _progress;
}

void ProgressModel::readData() {
    JSONReader reader;
    
    reader.initWithFile(PROGRESS_DATA);
    if (!reader.startJSON()) {
        CCASSERT(false, "Failed to load asset directory");
        return;
    }
    
    _totalLevels = (int) reader.getNumber("totalLevels");
    _levelsCompleted = (int) reader.getNumber("levelCompleted");
    
    if(reader.startObject("levels")) {
        for(int i=0; i < reader.getSize(); i++)
            _scores.push_back(reader.getNumber(std::to_string(i)));
    }
    
    reader.endObject();
    reader.endJSON();
}

void ProgressModel::writeData(int level, int score) {
    FILE* input = fopen(PROGRESS_DATA, "r");
    char* readBuffer = new char[65536]();
    FileReadStream is(input, readBuffer, sizeof(readBuffer));
    
    Document document;
    document.ParseStream(is);
    
    rapidjson::Value& levels = document["levels"];
    const char* levelString = std::to_string(level).c_str();
    rapidjson::Value levelValue(levelString, document.GetAllocator());
    rapidjson::Value scoreValue(score);
    
    if(levels.HasMember(levelString)) {
        levels[levelString] = scoreValue;
    } else {
        levels.AddMember(levelValue, scoreValue, document.GetAllocator());
        document["levelCompleted"].SetInt(level+1);
    }
    
    FILE* output = fopen(PROGRESS_DATA, "w");
    char* writeBuffer = new char[65536]();
    FileWriteStream os(output, writeBuffer, sizeof(writeBuffer));
    
    Writer<FileWriteStream> writer(os);
    document.Accept(writer);
    
    fclose(input);
    fclose(output);
    
    if(level == _levelsCompleted) {
        _scores.push_back(score);
        _levelsCompleted++;
    } else {
        _scores[level] = score;
    }
}

int ProgressModel::getScore(int level) {
    return _scores[level-1];
}