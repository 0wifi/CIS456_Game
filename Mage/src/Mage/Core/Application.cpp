#include "Application.h"
#include "../Renderer/Camera.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/SpriteRenderer.h"
#include "../Scene/EntityManager.h"
#include "../Scene/ComponentManager.h"
#include "../Scene/SystemManager.h"
#include "../Scene/SystemList.h"

namespace Mage {

	struct Application::Impl final
	{
		std::unique_ptr<Window> window;
		std::unique_ptr<Camera> camera;
		std::unique_ptr<EventManager> event_manager;
		std::unique_ptr<EntityManager> entity_manager;
		std::unique_ptr<ComponentManager> component_manager;
		std::unique_ptr<SystemManager> system_manager;
		std::unique_ptr<TextRenderer> text_renderer;
		std::unique_ptr<SpriteRenderer> sprite_renderer;

		bool closing = false;

		void construct(const char* title, bool full_screen = true,
			uint_fast32_t w = 0, uint_fast32_t h = 0, int_fast8_t swap_interval = 0)
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) > 0)
				throw Exception( (std::string("Failed initializing SDL2: ") + SDL_GetError()).c_str() );

			window = std::unique_ptr<Window>(
				new Window( title, full_screen, w, h, swap_interval));
			camera = std::unique_ptr<Camera>(new Camera());
			event_manager = std::unique_ptr<EventManager>(new EventManager(*window));
			entity_manager = std::unique_ptr<EntityManager>(new EntityManager());
			component_manager = std::unique_ptr<ComponentManager>(new ComponentManager());
			system_manager = std::unique_ptr<SystemManager>(new SystemManager());
			text_renderer = std::unique_ptr<TextRenderer>(new TextRenderer(*window));
			sprite_renderer = std::unique_ptr<SpriteRenderer>(new SpriteRenderer(*camera));

			entity_manager->set_component_manager(*component_manager);
			entity_manager->set_system_manager(*system_manager);
			component_manager->set_system_manager(*system_manager);
			system_manager->set_component_manager(*component_manager);

			camera->left = 0.0f;
			camera->bottom = 0.0f;
			camera->top = static_cast<float>(window->get_height());
			camera->right = static_cast<float>(window->get_width());
		}
	};

	struct ElapsedTime
	{
		std::chrono::time_point<std::chrono::system_clock> start
			= std::chrono::system_clock::now();
		std::chrono::time_point<std::chrono::system_clock> end
			= std::chrono::system_clock::now();
		float elapsed = 0.0f;

		void update()
		{
			end = std::chrono::system_clock::now();
			elapsed = static_cast<std::chrono::duration<float>>(end - start).count();
			start = end;
		}
	};

	Application::Application(const char* title, bool full_screen,
		uint_fast32_t w, uint_fast32_t h, int_fast8_t swap_interval)
	{
		LOG_E_INFO("Application starting.");
		_impl = new Impl();
		_impl->construct(title, full_screen, w, h, swap_interval);
	}

	Application::~Application()
	{
		delete _impl;
		LOG_E_INFO("Application stopping.");
	}

	Window* Application::get_window() const
	{
		return _impl->window.get();
	}
	Camera* Application::get_camera() const
	{
		return _impl->camera.get();
	}
	EventManager* Application::get_event_manager() const
	{
		return _impl->event_manager.get();
	}
	EntityManager* Application::get_entity_manager() const
	{
		return _impl->entity_manager.get();
	}
	ComponentManager* Application::get_component_manager() const
	{
		return _impl->component_manager.get();
	}
	SystemManager* Application::get_system_manager() const
	{
		return _impl->system_manager.get();
	}
	TextRenderer* Application::get_text_renderer() const
	{
		return _impl->text_renderer.get();
	}
	SpriteRenderer* Application::get_sprite_renderer() const
	{
		return _impl->sprite_renderer.get();
	}
	//follow this pattern for otehr forward declared classes

	void Application::close()
	{
		_impl->closing = true;
		LOG_E_INFO("Application::close() called.");
	}

	void Application::run()
	{
		LOG_E_INFO("Application running.");

		ElapsedTime et;

		while (!_impl->closing)
		{
			_impl->entity_manager->update();

			_impl->event_manager->poll_events();

			_impl->window->clear_window();

			//for each system
			//    call update on that system (supplying et.elapsed)
			for (auto s : _impl->system_manager->get_all_systems())
				s->update(*get_component_manager(), et.elapsed);

			//present window
			_impl->window->present();

			et.update();
		}
	}
}
