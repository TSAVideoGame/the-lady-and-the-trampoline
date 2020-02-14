#include "trampoline_manager.h"
#include "game_states.h"
#include "game.h"
#include "constants.h"
#include "trampoline.h"
#include <cstdlib>
#include <ctime>

TrampolineManager::TrampolineManager(Renderer* ren, Player* p) : ItemManager(ren)
{
  player = p;
  std::srand(std::time(nullptr));
}

TrampolineManager::~TrampolineManager()
{

}

void TrampolineManager::update()
{
  if (GameStates::getFirstTick())
  {
    removeObjects();
    switch (GameStates::getState())
    {
      case GameState::LEVEL:
      {
        maxReached = false;
        Game::levelInfo.difficulty = 1;
        objects.push_back(new Trampoline(renderer, player, 0, WINDOW_HEIGHT - 48, WINDOW_WIDTH));
        objects.push_back(new Trampoline(renderer, player, std::rand() % WINDOW_WIDTH - (WINDOW_WIDTH / 8), WINDOW_HEIGHT / 2, std::rand() % 512 + 512));
      }
      break;
    }
  }

  if (GameStates::getState() == GameState::LEVEL)
  {

    if (-Game::levelInfo.maxHeight / 2000 > Game::levelInfo.difficulty)
      Game::levelInfo.difficulty = -Game::levelInfo.maxHeight / 2000;

    if (Game::levelInfo.difficulty < Game::levelInfo.maxDifficulty)
    {
      if (objects.size() > 10)
        objects.pop_front();
      if (lastTrampY - Game::levelInfo.maxHeight > 256 * Game::levelInfo.difficulty)
      {
        objects.push_back(new Trampoline(renderer, player, std::rand() % WINDOW_WIDTH - (WINDOW_WIDTH / (8 * Game::levelInfo.difficulty)), Game::camera.y, std::rand() % 512 + 128));
        lastTrampY = Game::camera.y;
      }
    }
    else
    {
      if (!maxReached)
      {
        objects.push_back(new Trampoline(renderer, player, 0, Game::camera.y, WINDOW_WIDTH));
        maxReached = true;
      }
    }
  }

  for (auto tramp : objects)
    tramp->update();
}

void TrampolineManager::draw()
{
  for (auto tramp : objects)
    tramp->draw();
}

void TrampolineManager::removeObjects()
{
  for (auto obj : objects)
    delete obj;
  std::deque<Trampoline*>().swap(objects);
}
