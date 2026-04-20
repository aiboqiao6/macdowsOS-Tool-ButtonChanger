#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
#include <mutex>
#include <unordered_set>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include <SFML/System.hpp>
//
#include"LogSystem.h"
using namespace std;

void buttonchange(HWND hWnd_Window) {
    if (hWnd_Window == NULL) {
        exit(0);
    }
    //抗锯齿
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 10;
    // 仅改这一行声明，后续所有 window. 操作全局替换为 window.
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 200, 70 }), L"macdowsOS Tool test", sf::Style::None, sf::State::Windowed, settings);

    //垂直同步
    window.setVerticalSyncEnabled(true);
    // 2. 窗口永久置顶
    HWND hWnd = window.getNativeHandle();
    if (hWnd == NULL) {
        return;
    }
    // 3. 配置窗口透明与鼠标穿透
    LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle
        | WS_EX_LAYERED    // 分层窗口，支持透明度
        | WS_EX_TRANSPARENT // 鼠标事件穿透到下层
        | WS_EX_TOOLWINDOW  // 隐藏任务栏图标
    );
    // 色键透明：纯黑背景完全透明，可按需调整透明色
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    //
    //window.setActive(true);
    //红绿灯
    //红
    sf::CircleShape op_button_red(15);
    op_button_red.setFillColor(sf::Color(245, 0, 0));
    op_button_red.setOutlineThickness(0.2);
    op_button_red.setOutlineColor(sf::Color(0, 0, 0));
    op_button_red.setPointCount(10000);
    //黄
    sf::CircleShape op_button_yellow(15);
    op_button_yellow.setFillColor(sf::Color(255, 215, 0));
    op_button_yellow.setOutlineThickness(0.2);
    op_button_yellow.setOutlineColor(sf::Color(0, 0, 0));
    op_button_yellow.setPointCount(10000);
    //绿
    sf::CircleShape op_button_green(15);
    op_button_green.setFillColor(sf::Color(0, 245, 0));
    op_button_green.setOutlineThickness(0.2);
    op_button_green.setOutlineColor(sf::Color(0, 0, 0));
    op_button_green.setPointCount(10000);

    op_button_green.setPosition({ 15, 15 });
    op_button_yellow.setPosition({ 84, 15 });
    op_button_red.setPosition({ 154, 15 });

    //back
    sf::CircleShape button1(30);
    button1.setPointCount(100);
    button1.setPosition({ 1,1 });
    button1.setFillColor(sf::Color(64, 64, 64));
    button1.setPointCount(10000);

    sf::RectangleShape button2({ 139.5,60 });
    button2.setPosition({ 30,1 });
    button2.setFillColor(sf::Color(64, 64, 64));

    sf::CircleShape button3(30);
    button3.setPointCount(100);
    button3.setPosition({ 140,1 });
    button3.setFillColor(sf::Color(64, 64, 64));
    button3.setPointCount(10000);


    window.clear(sf::Color::Black);
    // 绘制所有内容，和普通SFML程序无差异
    window.draw(button1);
    window.draw(button2);
    window.draw(button3);

    window.draw(op_button_red);
    window.draw(op_button_yellow);
    window.draw(op_button_green);
    // 提交帧
    window.display();


    RECT windowRect;

    int top = 0, right = 0;
    /* LPCTSTR changeapp = L"QQ";
     //获取hwnd
     hWnd_Window = FindWindowW(NULL, changeapp);
     while (hWnd_Window == NULL) {
         hWnd_Window = FindWindowW(NULL, changeapp);
     }*/
     //是否被激活
    bool iftopon = (GetForegroundWindow() == hWnd_Window);
    while (window.isOpen()) {
        //wcout << L"wqddwq" << endl;
        //获取窗口信息
        GetWindowRect(hWnd_Window, &windowRect);

        top = windowRect.top;
        right = windowRect.right;
        //位置
        SetWindowPos(hWnd, hWnd_Window, right - 210, top + 10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
        SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        wcout << top << " " << right << " " << windowRect.top << " " << windowRect.right << endl;
        while ((top == windowRect.top && right == windowRect.right)) {

            //当窗口激活状态更改时刷新
            if (iftopon != (GetForegroundWindow() == hWnd_Window)) {
                //获取窗口信息
                GetWindowRect(hWnd_Window, &windowRect);

                top = windowRect.top;
                right = windowRect.right;
                SetWindowPos(hWnd, hWnd_Window, right - 210, top + 10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
                SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
                //
                iftopon = (GetForegroundWindow() == hWnd_Window);
            }

            Sleep(1);
            //刷新窗口信息
            GetWindowRect(hWnd_Window, &windowRect);
            //防无响应
            while (const optional event = window.pollEvent()) {}
        }

    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    freopen("log.txt", "w", stdout);

    // 1. 读取文件第一行十六进制字符串
    const wchar_t* SAVE_FILE = L"temp.tmp";
    std::wifstream file(SAVE_FILE);
    if (!file.is_open()) return 0;

    std::wstring hexStr;
    if (!std::getline(file, hexStr)) { file.close(); return 0; }
    file.close();

    // 2. 十六进制转 HWND
    HWND hwnd = nullptr;
    try {
        hwnd = reinterpret_cast<HWND>(std::stoull(hexStr, nullptr, 16));
    }
    catch (...) { return 0; }

    
    MESSAGE_(L"hwnd ", hwnd);
    buttonchange(hwnd);
    return 0;
}