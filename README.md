# InVasion
## Установка Godobuf
Уже все должно быть настроено, вы просто стартуете игру и смотрите, что напечатано в консоли (должно вывести число Пи, сама функция для тестинга godobuf записана в файле `/client/godot/game/screens/game/scripts/world.gd`)
Туториал, как пользоваться godobuf - [здесь](https://github.com/oniksan/godobuf).
Пример проекта на godotbuf - [тут](https://habr.com/ru/post/540034/)

## Настраиваем GDNative
**Мотивация**: хотим работать с grpc на клиенте Godot, но в Godot нет поддержки grpc, поэтому будем писать клиентский код, ответственный за взаимодействие с сервером, на C++.

**Реквизиты**: `scons` (https://scons.org/), компилятор C++ (Для винды - VS 2019, для Linux, пока хз, но скорее всего любой).

### Windows
Официальный туториал от Godot - [клик](https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html).
1. **Создание байндинг (чтобы GDNative работал)**:
	
	Открываем Developer `x86_x64 Cross Tools Command Prompt for VS 2019` (или VS 2017) (важно! *x86_x64*), переходим в папку проекта `/client/godot-cpp`.

	В этой папке вводим:
	```sh
	scons platform=windows generate_bindings=yes -j4
	```
	Это сгенерирует С++ байдинги для Godot. Это нужно для того, чтобы пользоваться GDNative плагинами (мы хотим настроить так, чтобы можно было работать с grpc).
2. **Компиляция наших плагинов**: 
	
	Сперва заходим в папку `/client/godot/game` и создаем тут пустую папку `/bin`, туда будут складываться все наши скомпилированные плюсовые скрипты. Далее тут мы создадим еще парочку файлов, которые расскажут godot, где искать наши библиотеки.

	Далее, переходим в папку `/client/godot/`.
	
	Здесь в папке `/cpp` будут лежать все наши плюсовые файлы, сейчас там только лежит папка `/cpp/example`, в которой лежит код с примером из [туториала](https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html) от godot.

	В текущей папке лежит файл `SConstruct` - это питоновский скрипт, в котором прописаны инструкции сборки. Если промотать содежимое файла в самый низ, то можно увидеть место, для редактирования путей до наших плюсовый файлов. Сейчас там написано что-то типо такого:
	```python	
	...
	# tweak this if you want to use different folders, or more folders, to store your source code in.
	env.Append(CPPPATH=['cpp/'])
	sources = Glob('cpp/example/*.cpp')

	library = env.SharedLibrary(target=env['target_path'] + env['target_name'] , source=sources)
	...
	```
	Итак, запускаем `scons platform=windows bits=64`. В папке `/client/game/bin` появилась папка `win64`, в которой лежат наши dll'ки.
3.	**Загружаем все в редактор godot**: 

	Залазием в папку `/client/godot/game/bin` и создаем там два файла (на самом деле файлы уже созданы, можете открыть проект godot и увидеть на главном экране летающий логотип godot, а в терминале выводится `GD Native Init`. Я это уберу впоследствии, но папку `/client/godot/game/bin` из `.gitignore` я убрал специально).

	Первый имеет название `gdexample.gdnlib` и следующее содержимое (это только, чтобы протетить пример от godot, далее будет создавать свои такие же файлы, но с правильными названиями для наших библиотек), более подробно об этой секции [здесь](https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html#using-the-gdnative-module):
	```cpp
	[general]

	singleton=false
	load_once=true
	symbol_prefix="godot_"
	reloadable=false

	[entry]

	X11.64="res://bin/x11/libgdexample.so"
	Windows.64="res://bin/win64/libgdexample.dll"
	OSX.64="res://bin/osx/libgdexample.dylib"

	[dependencies]

	X11.64=[]
	Windows.64=[]
	OSX.64=[]
	```

	Второй файл будет называться `gdexample.gdns`, такие файлы нужно будет делать для каждого GDNative скрипта, которым мы будет пользоваться внутри Godot (более подробно по ссылке в начале пункта). Его содержимое:
	```python
	[gd_resource type="NativeScript" load_steps=2 format=2]

	[ext_resource path="res://bin/gdexample.gdnlib" type="GDNativeLibrary" id=1]

	[resource]

	resource_name = "gdexample"
	class_name = "GDExample"
	library = ExtResource(1)
	```


### Linux
Сам не собирал, но все должно быть так же, как для винды, только можно билдить из любого терминала. Также везде, где я писал `scons platform=windows` заменить на `scons platform=linux`. Остальные шаги должны остаться неизменными. Если будут какие-то ошибки, можно открыть файлы `/client/godot/SConstruct`, также `client/godot-cpp/SConsruct`, `client/godot-cpp/binding_generator.py`, `client/godot-cpp/CMakeLists.txt`.


## Настраиваем gRPC локально
**Если вы еще не установили grpc, то сперва прочитайте этот README.**

### Windows
Будет устанавливать через пакетный менеджер от Microsoft `vcpkg` (все это делао будет весить 10GB+ со всеми зависимостями, ужас просто, а они говорят про light-weight, bruh..). Можно паралельно следовать этому [туториалу](https://sanoj.in/2020/05/07/working-with-grpc-in-windows.html), но некоторые детали будут отличаться.

1. **Устанавливаем `vcpkg`**:
	
	Переходим на github майков и следуем туториалу - https://github.com/microsoft/vcpkg#getting-started. Можно поступить как я и сделать на диске `C` папочку `dev` и склонировать репо `vcpkg` туда: 
	```sh
	cd /c
	mkdir dev
	cd dev/
	git clone https://github.com/microsoft/vcpkg
	./vcpkg/bootstrap-vcpkg.bat
	```

	Добавляем `C:\dev\vcpkg\vcpkg.exe` файл в PATH. Добавляем в переменные среды путь `C:\dev\vcpkg`, чтобы обновления вступили в действие, переоткрываем терминал.
2. **Устанавливаем gPRC, Protobuf и их зависимости**:

	```sh
	vcpkg install grpc:x64-windows
	```
	```sh
	vcpkg install protobuf protobuf:x64-windows
	```
	```sh
	vcpkg integrate install
	```

	Теперь нужно добавить `protoc.exe` в PATH, чтобы мы могли вызывать команду из терминала. У меня он лежал в `C:\dev\vcpkg\packages\protobuf_x64-windows\tools\protobuf`, но может оказаться и в `C:\dev\vcpkg\packages\protobuf_x64-windows\bin`.

	Весь процесс установки на общажном вайфае у меня занял порядка 40 минут.
3. **Проверим, что у нас все работает**:
	
	Можно сделать тестовый минимпроект для проверки работоспособности установленных пакетов. Тут следуем этому [туториалу](https://sanoj.in/2020/05/07/working-with-grpc-in-windows.html).

	В моменте билда `grpc` (не протобуф, а сразу после него), вместо 
	```sh
	protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc="\c\dev\vcpkg\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe" helloworld.proto
	```
	Мне нужно было написать это (так как иначе ошибка была):
	```sh
	protoc -I. --grpc_out=. --plugin=protoc-gen-grpc="\c\dev\vcpkg\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe" helloworld.proto
	```
	PS. (Нужно было убрать знак `=` после `-I`) НО! Я мог тут неверно указать команду, которую я ввел, можно прогуглить, если что.

	Когда дошли до момента с компиляцией (не протофайлов, а всего проекта), то можно использовать команду для VS 2019: вместо
	```sh
	cmake -G "Visual Studio 14 2015 Win64" ../ -DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake
	```  
	Пишем
	```sh
	cmake -G "Visual Studio 16 2019" ../ -DCMAKE_TOOLCHAIN_FILE=/c/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
	```
	Дефолтная платформа для сборки будет, кажется, `x64` (можно менять флагом `-A`, но про это надо погуглить).

	Далее доделываем как в туториале, и все должно нормально работать.
	То есть пишем `cmake --build .`, и у нас генерируются файлы `server.exe` и `client.exe`.

	После всего этого нам предстоит подружить **GDNative** и **gRPC**. Готовимся к боли и молимся, что Бог нам поможет...
	
	PSS. Тут я не показал, как простестировать gRPC таким же образом, как для Линукса, так как было лень разбираться с настройками CMake. Займемся этим уже для связки c GDNative (интеграции с Godot).


### Linux
Проверьте папку `/servers/grpc`, вы должны найти там тестовый проект, основанный на этом [туториале](https://medium.com/@shradhasehgal/get-started-with-grpc-in-c-36f1f39367f4).

### Реквизиты
1. Нужно установить protobuf и grpc по этому [туториалу](https://grpc.io/docs/languages/cpp/quickstart/).
2. (**Для пользователей WSL/WSL2**) Для того, чтобы работать с кодом сервером достаточно поставить grpc и protobuf в WSl, но впоследствии скорее всего прийдется ставить и на Windows. Также замечу, что работа с виндовской файловой системой из ядра Linux - капец медленная, прям жесть. Так что лучше иметь оба репо и на винде и на WSl, либо установить protobuf и grpc на винду и запускать напрямую из нее (этим я еще не успел заняться, там больно). 
3. Поставить можно следующим образом:
	
	1. В дириктории `/home/${username}` (можно ее вывести, написав в терминале `echo $HOME`) создать папку `grpc-install`, и уже туда склонировать репозиторий из тутриала гугла (этот [пункт](https://grpc.io/docs/languages/cpp/quickstart/#clone-the-grpc-repo) того туториала)
	2. Далее следовать всем пунктам гугловского туториала.
	3. Проверьте, что проект HelloWorld от гугл работает.

4. Теперь давайте соберем проект в папке `/servers/grpc`:

	1. Делаем `cd /servers/grpc`
	2. Затем последовательно команды: `mkdir -p cmake/build`, `cd cmake/build`, `cmake ../..`
	3. Сразу после пишем `make`
	4. Теперь проверим, что наш проект работает: запускаем `./server` и из другого терминала в той же дириктории `./client`. На экран долно вывести что-то разумное (там будет переворот строки, если конкретно).


## Соединяем GDNative и gRPC

To be continued...
Точнее not to be continue... (анрил)