# noypixels
Кроссплатформенный редактор пиксельных спрайтов и анимаций

![alt text](https://github.com/Danchess17/noypixels/blob/main/image_2023-07-28_13-19-15.png)

Отличия checkpoint3 от checkpoint2:
added windows support
added animations edit

Отличия текущей стадии (checkpoint2) от предыдущей (checkpoint1):
Добавлен графический интерфейс.
В noypixels/include/noypixel/graphics.h был создан родительский класс Window, от которого наследуются следующие классы:

MainWindow - главное окно, содержит все окна и рендерит их
MenuWindow - окно меню
ColorWindow - окно для выбора цвета кисти
SpritesWindow - окно, содержащее в себе список всех спрайтов
NewSpriteWindow - окно для создания нового спрайта
CanvasWindow - окно для рисования спрайта
Также была добавлена обновленная версия UML диаграммы с png-интерпретацией.

Обновлённые зависимости и инструкции:
dependencies:
premake5 -- built in project
stb_image -- built in project
imgui -- built in project
yaml-cpp -- run "apt install libyaml-cpp-dev" for ubuntu
sdl2 -- run "apt install libsdl2-dev" for ubuntu
opengl3 -- run "apt install libgl-dev" for ubuntu

build:
dep/premake5 gmake && make config=release noypixels

run:
bin/Release/noypixels

***dependencies***:  
    premake5            -- built in project  
    stb_image           -- built in project  
    imgui               -- built in project  
    yaml-cpp            -- run "apt install libyaml-cpp-dev" for ubuntu  
    sdl2                -- run "apt install libsdl2-dev" for ubuntu  
    opengl3             -- run "apt install libgl-dev" for ubuntu  
  

 ### Отличия текущей стадии (checkpoint2) от предыдущей (checkpoint1):  
 Добавлен графический интерфейс.  
 В noypixels/include/noypixel/graphics.h был создан родительский класс Window, от которого наследуются следующие классы:   
 1. MainWindow - главное окно, содержит все окна и рендерит их
 2. MenuWindow - окно меню
 3. ColorWindow - окно для выбора цвета кисти
 4. SpritesWindow - окно, содержащее в себе список всех спрайтов
 5. NewSpriteWindow - окно для создания нового спрайта
 6. CanvasWindow - окно для рисования спрайта

Также была добавлена обновленная версия UML диаграммы с png-интерпретацией.  
#### Обновлённые зависимости и инструкции:  

