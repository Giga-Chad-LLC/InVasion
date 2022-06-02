# InVasion — Client
## Установка Godobuf
Уже все должно быть настроено, вы просто стартуете игру и смотрите, что напечатано в консоли (должно вывести число Пи, сама функция для тестинга godobuf записана в файле `/client/godot/game/screens/game/scripts/world.gd`)
Туториал, как пользоваться godobuf - [здесь](https://github.com/oniksan/godobuf).
Пример проекта на godotbuf - [тут](https://habr.com/ru/post/540034/)

## Настраиваем GDNative (опционально)
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
