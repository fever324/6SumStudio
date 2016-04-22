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
        int ssize = reader.startArray();
        
        for(int i=0; i < ssize; i++) {
            int level = std::stoi(reader.getKey());
            
            if(reader.startObject()) {
                _levels[level] = new LevelInfo(level, reader.getNumber("overallStar"), reader.getNumber("time"), reader.getBool("locked"));
            }
            
            reader.endObject();
            reader.advance();
        }
        
        reader.endArray();
    }
    
    reader.endObject();
    reader.endJSON();
}

void ProgressModel::writeData(int level, float completeTime, int star){
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
    
    rapidjson::Value* currLevel = &levels.FindMember(levelString)->value;
    currLevel->FindMember("time")->value.SetDouble(completeTime);
    currLevel->FindMember("overallStar")->value.SetInt(star);
    currLevel->FindMember("locked")->value.SetBool(true);
    
    fclose(input);
    
    FILE* output = fopen((documentPath+"/progress.json").c_str(), "w");
    char* writeBuffer = new char[65536]();
    FileWriteStream os(output, writeBuffer, sizeof(writeBuffer));
    
    Writer<FileWriteStream> writer(os);
    document.Accept(writer);
    
    fclose(output);
    
    _levels[level]->setStar(star);
    _levels[level]->setTime(completeTime);
    _levels[level]->unlock();
}

int ProgressModel::getTotalLevels() {
    return _totalLevels;
}

int ProgressModel::getStars(int level) {
    return _levels[level]->getStar();
}

double ProgressModel::getCompleteTime(int level) {
    return _levels[level]->getTime();
}

bool ProgressModel::isLocked(int level) {
    return _levels[level]->isLocked();
}