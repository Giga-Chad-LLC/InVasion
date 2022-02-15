# InVasion
## Godot client
Работаю над добавлением protobuf в godot.

## gRPC server
**Если вы еще не установили grpc, то сперва прочитайте этот README**

Проверьте папку `/servers/grpc`, вы должны найти там тестовый проект, основанный на этом [туториале](https://medium.com/@shradhasehgal/get-started-with-grpc-in-c-36f1f39367f4).

### Реквизиты
1. Нужно установить protobuf и grpc по этому [туториалу](https://grpc.io/docs/languages/cpp/quickstart/).
2. (**Для пользователей WSL/WSL2**) Для того, чтобы работать с кодом сервером достаточно поставить grpc и protobuf в WSl, но впоследствии скорее всего прийдется ставить и на Windows. Также замечу, что работа с виндовской файловой системой из ядра Linux - капец медленное, прям жесть. Так что лучше иметь оба репо и на винде и на WSl, либо установить protobuf и grpc на винду и запускать напрямую из нее (этим я еще не успел заняться).
3. Поставить можно следующим образом:
	
	1. В дириктории `/home/username` (можно ее вывести, написав в терминале `echo $HOME`) создать папку `grpc-install`, и уже туда склонировать репозиторий из тутриала гугла (этот [пункт](https://grpc.io/docs/languages/cpp/quickstart/#clone-the-grpc-repo) того туториала)
	2. Далее следовать всем пунктам гугловского туториала.
	3. Проверьте, что проект HelloWorld от гугл работает.

4. Теперь давайте соберем проект в папке `/servers/grpc`:

	1. Делаем `cd /servers/grpc`
	2. Затем последовательно команды: `mkdir -p cmake/build`, `cd cmake/build`, `cmake ../..`
	3. Сразу после пишем `make`
	4. Теперь проверим, что наш проект работает: запускаем `./server` и из другого терминала в той же дириктории `./client`. На экран долно вывести что-то разумное (там будет переворот строки, если конкретно).