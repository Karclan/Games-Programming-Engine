#include "custom\levelController.h"

void LevelController::initialize()
{
	addEventListener(EventType::UPDATE);
	_player = nullptr;

	SPtr_GameObject player = findGameObject("Robot");
	SPtr_Custom playerCustom = std::static_pointer_cast<Custom>(player->getComponent(ComponentType::CUSTOM));
	_player = static_cast<PlayerController*>(playerCustom->getBehaviour().get());
	_gui = std::static_pointer_cast<GUI>(getComponent(ComponentType::GUI));
}


void LevelController::onActive()
{
	//Create message that will be displayed to screen
	_score=0;
	_gui->setMessage("Life: ",  5, 750, 0.8, glm::vec3(1,0,0), true);								//0
	_gui->setMessage("Score: ",  5, 700, 0.8, glm::vec3(1,1,0), true);								//1
	_gui->setMessage("Game Over - you failed at DMU",  200, 400, 0.6, glm::vec3(1,0,1), false);		//2
	_gui->setMessage("Congratulations, have a first!", 200, 400, 0.6, glm::vec3(1,1,1), false);		//3
}


void LevelController::update(float t)
{
	//std::cout << _player->test;
	_gui->updateMessage(0, "Life: " + std::to_string(_player->getLife()));
	_gui->updateMessage(1, "Score: " + std::to_string(_score));

	if(_player->getLife() <= 0) {_score-=100; loadScene("gameFailed.XML");}
	if(_score >= 60) {loadScene("gameComplete.XML");}
	
}



