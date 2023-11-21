#pragma once

#include "DifficultyMode.h"
#include <vector>

using namespace std;

struct ScoreHistory {
  float score;
  int run;
  std::string mode;
};

struct GameScore {
  float score;
  bool newScore;
  float topScore;
  int runs;
  std::vector<ScoreHistory> scores;

  void pushScore(ScoreHistory score) {
    scores.push_back(score);
    std::sort(scores.begin(), scores.end(), [](const ScoreHistory& a, const ScoreHistory& b) {
      return a.score > b.score;
    });
  }

  std::vector<ScoreHistory> highestScores(uint8_t limit = 10) {
    std::vector<ScoreHistory> topScores(scores.begin(), scores.begin() + std::min<size_t>(scores.size(), limit));
    return topScores;
  }
};

GameScore gScore;