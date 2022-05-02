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
