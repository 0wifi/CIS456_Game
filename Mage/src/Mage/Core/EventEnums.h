#pragma once
#include "Core.h"
namespace Mage {

    enum class MAGE_API MouseButton
    {
        Left = 0x0001,
        Middle = 0x0002,
        Right = 0x0004,
        Extra1 = 0x0008,
        Extra2 = 0x0010
    } ;

    enum class MAGE_API ControllerSensor
    {
        Unknown,
        Accelerometer,
        Gyroscope,
        LeftAccelerometer,
        RightAccelerometer,
        LeftGyroscope,
        RightGyroscope
    } ;

    enum class MAGE_API KeyModifier
    {
        LeftShift = 0x0001,
        RightShift = 0x0002,
        LeftCtrl = 0x0004,
        RightCtrl = 0x0008,
        LeftAlt = 0x0010,
        RightAlt = 0x0020,
        LeftGui = 0x0040,
        RightGui = 0x0080,
        NumLock = 0x0100,
        CapsLock = 0x0200,
        AltGr = 0x0400,
        ScrollLock = 0x0800
    } ;

    enum class MAGE_API Key
    {
        UnknownKey,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero,
        Return, Escape, Backspace, Tab, Space, Minus, Equals, LeftBracket,
        RightBracket,
        BackSlash, Semicolon, Apostrophe, Grave, Comma, Period, ForwardSlash,
        CapsLock,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrintScreen, ScrollLock,
        Pause, Insert, Home, PageUp, Delete, End, PageDown, Right, Left, Down, Up,
        NumLock,
        KeypadDivide, KeypadMultiply, KeypadMinus, KeypadPlus, KeypadEnter,
        KeypadPeriod,
        Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8,
        Keypad9, Keypad0,
        LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt, LeftGui,
        RightGui, AltGr
    } ;
}
