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

void ProgressModel::init() {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"progress.json"];
    
    if([fileManager fileExistsAtPath:filePath] == NO) {
        NSString *resourcePath = [[NSBundle mainBundle] pathForResource:@"jsons/progress" ofType:@".json"];
        [fileManager copyItemAtPath:resourcePath toPath:filePath error:&error];
    }
}

ProgressModel* ProgressModel::getInstance() {
    if(_progress == nullptr) {
        _progress = new (std::nothrow) ProgressModel();
        _progress->readData();
    }
    
    return _progress;
}

void ProgressModel::readData() {
    NSArray * paths(NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES));
    const char* const path([[paths objectAtIndex:0] fileSystemRepresentation]);
    const std::string documentPath(path);
    
    JSONReader reader;
    reader.initWithFile(documentPath + "/progress.json");
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

void ProgressModel::writeData(int level, int score, float completeTime, int star){
    NSArray * paths(NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES));
    const char* const path([[paths objectAtIndex:0] fileSystemRepresentation]);
    const std::string documentPath(path);
    
    FILE* input = fopen((documentPath+"/progress.json").c_str(), "r");
    char* readBuffer = new char[65536]();
    FileReadStream is(input, readBuffer, sizeof(readBuffer));
    
    Document document;
    document.ParseStream(is);
    
    rapidjson::Value& levels = document["levels"];
    const char* levelString = std::to_string(level).c_str();
    rapidjson::Value levelValue(levelString, document.GetAllocator());
    
    rapidjson::Value scoreValue(score);
    rapidjson::Value timeValue(completeTime);
    rapidjson::Value starValue(star);
    
    rapidjson::Value result(kObjectType);
    result.AddMember("score", scoreValue, document.GetAllocator());
    result.AddMember("completeTime", timeValue, document.GetAllocator());
    result.AddMember("star", starValue, document.GetAllocator());
    
    if(levels.HasMember(levelString)) {
        levels[levelString] = result;
    } else {
        levels.AddMember(levelValue, result, document.GetAllocator());
        document["levelCompleted"].SetInt(level+1);
    }
    
    fclose(input);
    
    FILE* output = fopen((documentPath+"/progress.json").c_str(), "w");
    char* writeBuffer = new char[65536]();
    FileWriteStream os(output, writeBuffer, sizeof(writeBuffer));
    
    Writer<FileWriteStream> writer(os);
    document.Accept(writer);
    
    fclose(output);
    
    if(level >= _levelsCompleted) {
        _scores.push_back(score);
        _levelsCompleted++;
    } else {
        _scores[level] = score;
    }
}

int ProgressModel::getScore(int level) {
    return _scores[level];
}

int ProgressModel::getNextLevel() {
    return _levelsCompleted;
}