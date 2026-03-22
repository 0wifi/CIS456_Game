#pragma once

#include "Core.h"

namespace Mage {

    class Window ;

    class MAGE_API OnAppClosingEventListener : public EventListenerBase
    {
    public: 
        virtual void on_app_closing() = 0;
    };
  
    class MAGE_API OnWindowMinimizedEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_minimized() = 0;
    };

    class MAGE_API OnWindowMaximizedEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_maximized() = 0;
    };

    class MAGE_API OnWindowRestoredEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_restored() = 0;
    };

    class MAGE_API OnWindowMouseEnteredEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_mouse_entered() = 0;
    };

    class MAGE_API OnWindowMouseLeftEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_mouse_left() = 0;
    };

    class MAGE_API OnWindowFocusGainedEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_focus_gained() = 0;
    };

    class MAGE_API OnWindowFocusLostEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_focus_lost() = 0;
    };

    class MAGE_API OnWindowResizedEventListener : public EventListenerBase
    {
    public:
        virtual void on_window_resized(uint_fast32_t width, uint_fast32_t height) = 0;
    };
    


    class MAGE_API OnMouseMotionEventListener : public EventListenerBase
    {
    public:
        virtual void on_mouse_motion(float x, float y,
            float x_delta, float y_delta, uint_fast32_t button_states) = 0;
    };

    class MAGE_API OnMouseWheelEventListener : public EventListenerBase
    {
    public:
        virtual void on_mouse_wheel(float x, float y,
            float mouse_x, float mouse_y, bool direction_flipped) = 0;
    };

    class MAGE_API OnMouseButtonDownEventListener : public EventListenerBase
    {
    public:
        virtual void on_mouse_button_down(MouseButton button,
            float x, float y, uint8_t click_count) = 0;
    };
    class MAGE_API OnMouseButtonUpEventListener : public EventListenerBase
    {
    public:
        virtual void on_mouse_button_up(MouseButton button,
            float x, float y, uint8_t click_count) = 0;
    };

    class MAGE_API OnKeyDownEventListener : public EventListenerBase
    {
    public:
        virtual void on_key_down(Key key, uint_fast16_t key_modifiers, uint_fast8_t repeat_count) = 0;
    };
    class MAGE_API OnKeyUpEventListener : public EventListenerBase
    {
    public:
        virtual void on_key_up(Key key, uint_fast16_t key_modifiers) = 0;
    };

    class MAGE_API OnControllerAxisMotionEventListener : public EventListenerBase
    {
    public:
        virtual void on_controller_axis_motion(int_fast32_t controller_id, uint_fast32_t axis_id, float axis_value) = 0;
    };
    class MAGE_API OnControllerButtonDownEventListener : public EventListenerBase
    {
    public:
        virtual void on_controller_button_down(int_fast32_t controller_id, uint_fast8_t button_id) = 0;
    };
    class MAGE_API OnControllerButtonUpEventListener : public EventListenerBase
    {
    public:
        virtual void on_controller_button_up(int_fast32_t controller_id, uint_fast8_t button_id) = 0;
    };
    class MAGE_API OnControllerSensorUpdateEventListener : public EventListenerBase
    {
    public:
        virtual void on_controller_sensor_update(int_fast32_t controller_id, ControllerSensor sensor_id,
            float sensor_value_x, float sensor_value_y, float sensor_value_z, uint64_t timestamp) = 0;
    };
    //OnControllerAxisMotionEventListener -> event.caxis
    //             on_blah(uint_fast32_t controller_id, uint_fast8_t axis_id, float axis_value)
    //OnControllerButtonDownEventListener -> event.cbutton
    //              on_blah(uint_fast32_t controller_id, uint_fast8_t button_id)
    //OnControllerButtonUpEventListener -> event.cbutton
    //              on_blah(uint_fast32_t controller_id, uint_fast8_t button_id)
    //OnControllerSensorUpdateEventListener -> event.sensor
    //              on_blah(uint_fast32_t controller_id, uint_fast32_t sensor_id,
    //                          float sensor_value_x, float sensor_value_y, float sensor_value_z, uint64_t timestamp)

    class MAGE_API EventManager : ICopyDisable
    {
        friend class Application;
       
    public:
        ~EventManager();

        void poll_events();

        void motion_event_logging(bool enabled);
        void sensor_event_logging(bool enabled);
        void window_event_logging(bool enabled);
        void keyboard_event_logging(bool enabled);
        void mouse_event_logging(bool enabled);
        void controller_event_logging(bool enabled);

        void add_on_app_closing_event_listener(OnAppClosingEventListener* listener);
        void add_on_window_minimized_event_listener(OnWindowMinimizedEventListener* listener);
        void add_on_window_maximized_event_listener(OnWindowMaximizedEventListener* listener);
        void add_on_window_restored_event_listener(OnWindowRestoredEventListener* listener);
        void add_on_window_mouse_entered_event_listener(OnWindowMouseEnteredEventListener* listener);
        void add_on_window_mouse_left_event_listener(OnWindowMouseLeftEventListener* listener);
        void add_on_window_focus_gained_event_listener(OnWindowFocusGainedEventListener* listener);
        void add_on_window_focus_lost_event_listener(OnWindowFocusLostEventListener* listener);
        void add_on_window_resized_event_listener(OnWindowResizedEventListener* listener);

        void add_on_mouse_motion_event_listener(OnMouseMotionEventListener* listener);
        void add_on_mouse_wheel_event_listener(OnMouseWheelEventListener* listener);
        void add_on_mouse_button_down_event_listener(OnMouseButtonDownEventListener* listener);
        void add_on_mouse_button_up_event_listener(OnMouseButtonUpEventListener* listener);

        void add_on_key_down_event_listener(OnKeyDownEventListener* listener);
        void add_on_key_up_event_listener(OnKeyUpEventListener* listener);

        void add_on_controller_axis_motion_event_listener(OnControllerAxisMotionEventListener* listener);
        void add_on_controller_button_down_event_listener(OnControllerButtonDownEventListener* listener);
        void add_on_controller_button_up_event_listener(OnControllerButtonUpEventListener* listener);
        void add_on_controller_sensor_update_event_listener(OnControllerSensorUpdateEventListener* listener);


    private:
        explicit EventManager(Window& window);

        struct Impl;
        Impl* _impl;
    };
}
