#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <string>
#include <vector>

enum class GameMode {Online, Offline};
enum class GameVersion {Demo, Batch};

class GameConfig {
private:
    GameMode mode = GameMode::Online;
    GameVersion version = GameVersion::Demo;
    std::vector<int> survivalRules = {2, 3};
    std::vector<int> birthRules = {3};
    std::vector<std::pair<int, int>> aliveCells;
    std::string nameUniverse = "Random Universe";
    std::string outputFile = "output";
    int width = 35, height = 35;
    int offlineIterations = 1;
public:
    void setIterations(int iters);
    int getIterations() const;

    void setOutputFile(const std::string&);
    std::string getOutputFile() const;
    void setSize(int userWidth, int userHeight);
    void setGameMode(GameMode userMode);
    void setGameVersion(GameVersion userVersion);
    void setSurvivalRules(const std::vector<int>& userSurvivalRules);
    void setBirthRules(const std::vector<int>& userBirthRules);
    void setName(const std::string& userName);
    void setAliveCells(const std::vector<std::pair<int, int>>& userAliveCells);


    int getWidth() const;
    int getHeight() const;

    std::string getNameUniverse() const;

    GameMode getMode() const;
    GameVersion getVersion() const;
    std::vector<std::pair<int, int>> getAliveCells() const;

    std::vector<int> getBirthRules() const;
    std::vector<int> getSurvivalRules() const;
};

#endif