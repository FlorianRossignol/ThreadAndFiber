#include "game_window.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "game_global.h"
#include "entity.h"
#include "job_system.h"
#include "Windows.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

namespace CityBuilderGame
{

	
	int CityBuilderGame::window_game::Create_Window(std::string name)
	 {
		//create base windows
		sf::RenderWindow _GameWindow(sf::VideoMode(_height,_width),name);
		_GameWindow.setVerticalSyncEnabled(true);
		//imgui
		ImGui::SFML::Init(_GameWindow);
		//bool for checkbox
		bool checkUnit = { true };
		bool checkThread = { false };
		bool checkTracy = { false };

		sf::Texture _bgTexture;
		if (!_bgTexture.loadFromFile("../game/Data/bgCity.png"))
		{
			return EXIT_FAILURE;
		}
		//add texture to sprite 
		sf::Sprite _bgSprite(_bgTexture);
		//building construct
		Building _building(moneyGlob);
		//entity construct
		Entity _entity;
		//init entity
		_entity.Init();
		//start game loop
		sf::Clock _deltaClock;
		
		while (_GameWindow.isOpen())
		{
#ifdef TRACY_ENABLE
			ZoneScopedN("Game Loop");
#endif
			sf::Event event;
			while (_GameWindow.pollEvent(event))
			{
				//imgui event
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
				{
					_GameWindow.close();
				}
				else
				{
					_GameWindow.clear();
				}

			}
			moneyGlob = 100000;
			
			_entity.Update(_building.GetSprite(),_GameWindow);

			//imgui sfml update
			ImGui::SFML::Update(_GameWindow, _deltaClock.restart());

			//big demoshow
			ImGui::ShowDemoWindow();

			ImGui::Begin("ToolBuilding");

			
			//imgui cursor
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			

			ImGui::Text("FPS = %f", ImGui::GetIO().Framerate);
			//imguisfmlcreate
			ImGui::Text("Money = %f", moneyGlob);

			ImGui::Text("House cost = 1000");
			
			if (ImGui::Button("House"))
			{
				if (moneyGlob >=1000.0f)
				{
					_building.Init();
					//set money to 0
					moneyGlob -= 1000.0f;
				}
			}

			if (ImGui::Button("Close"))
			{
				_GameWindow.close();
			}

			//todolist
			ImGui::Text("TODO LIST");
			
			//imgui checkbox
			ImGui::Checkbox("Add 100+ unit", &checkUnit);

			ImGui::Checkbox("Thread and fiber", &checkThread);

			ImGui::Checkbox("Profile With Tracy", &checkTracy);

			ImGui::End();

			ImGui::EndFrame();
			//draw imgui render
			ImGui::SFML::Render(_GameWindow);
			
			//draw sprite
			_GameWindow.draw(_bgSprite);
			//drawhouse
			_building.DrawHouse(_GameWindow);
			
			if (!deletedSprite)
			{
				ZoneScopedN("testdrawfibercoroutine");
				TRACY_FIBERS;
				auto coroutine = std::make_shared<JobSystem::FiberCoroutine>();
				TracyFiberEnter("coroutine");
				coroutine->Setup([&](JobSystem::Coroutine::Yield yield)
				{
					_entity.MultipleDraw(_GameWindow);
					yield();
				});
				
				while (coroutine->Step())
				{
					_entity.MultipleDraw(_GameWindow);
				}
				TracyFiberLeave;
			}
			
			//display image
			_GameWindow.display();

		}
		ImGui::SFML::Shutdown();
		return EXIT_SUCCESS;
	}
}
