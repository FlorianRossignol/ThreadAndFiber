#include <iostream>
#include "tool_game.h"
#include "game_window.h"
#include "job_system.h"
#include <tracy/Tracy.hpp>


	int main(int argc,char** argv)
	{
		ZoneScopedN("test");
		JobSystem::Initialize();
		CityBuilderGame::window_game window;
		JobSystem::Execute([&] {window.Create_Window("CItyBuilderGame"); });
		JobSystem::Wait();
		/*window.Create_Window("CityBuilderGame");*/
		return EXIT_SUCCESS;
	}


//#include <iostream>
//#include <chrono>
//#include <string>
//
//#include "job_system.h"
//
//using namespace std;
//
//void Spin(float milliseconds)
//{
//    milliseconds /= 1000.0f;
//    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
//    double ms = 0;
//    while (ms < milliseconds)
//    {
//        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
//        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
//        ms = time_span.count();
//    }
//}
//
//struct timer
//{
//    string name;
//    chrono::high_resolution_clock::time_point start;
//
//    timer(const string& name) : name(name), start(chrono::high_resolution_clock::now()) {}
//    ~timer()
//    {
//        auto end = chrono::high_resolution_clock::now();
//        cout << name << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " milliseconds" << endl;
//    }
//};
//
//int main()
//{
//    JobSystem::Initialize();
//
//    // Serial test
//    {
//        auto t = timer("Serial test: ");
//        Spin(100);
//        Spin(100);
//        Spin(100);
//        Spin(100);
//    }
//
//    // Execute test
//    {
//        auto t = timer("Execute() test: ");
//        JobSystem::Execute([&] { Spin(100); });
//        JobSystem::Execute([&] { Spin(100); });
//        JobSystem::Execute([&] { Spin(100); });
//        JobSystem::Execute([&] { Spin(100); });
//        JobSystem::Wait();
//    }
//
//    return 0;
//}
