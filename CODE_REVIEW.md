# Sorcery Madness — сорс ревю

**Дата:** 2026-11-25 (последна ревизия: същия ден, след поправките — виж т. 8б)
**Обхват:** целият собствен код (`common`, `networking`, `engine`, `client`, `server`, premake скриптове, шейдъри) — ≈4 000 реда. `vendor/` не е ревюиран.
**Състояние на репото:** `main` + некомитнати промени. Ревюто е върху **работното копие**.
**Метод:** само четене на кода, без компилация. Оправените находки са махнати; номерацията (C/H/M/L/B) е запазена, за да не се разминават препратките.

---

## Съдържание

1. [Обобщение](#1-обобщение)
2. [Критични — чупят билда или водят до срив/зависване](#2-критични)
3. [Високи — грешно поведение](#3-високи)
4. [Средни — крехкост, течове, скрити капани](#4-средни)
5. [Ниски — дребни бъгове и почистване](#5-ниски)
6. [Стил, именуване, правопис](#6-стил-именуване-правопис)
7. [Билд система (premake)](#7-билд-система-premake)
8. [Оценка на архитектурата](#8-оценка-на-архитектурата)
9. [Препоръчан ред за оправяне](#9-препоръчан-ред-за-оправяне)

---

## 1. Обобщение

| Тежест | Отворени | Оправени |
|---|---|---|
| Критично | 1 (C6 частично) | 7 |
| Високо | 7 (H11 ново) | 4 |
| Средно | 12 (M17 ново) | 5 |
| Ниско | 17 | 1 |
| Билд | 9 | 1 |

Накратко:
- Всички критични бъгове от първото ревю (билд, безкраен цикъл, overflow, UB) са оправени; от C6 остава повторният `Connect`.
- Протоколът за „надеждни“ пакети реално не работи: няма ACK, request ID-тата на двете страни се смесват, сървърът никога не ретрайва и не чисти чакащите пакети (H1–H3).
- Рендерът вече е свързан (submit → draw call), но **нищо не се вижда** заради целочислен `glm::ortho` (H11).
- Архитектурата като идея е добра (слоеве, event bus, разделение на модули), но изпълнението е полуготово и има няколко структурни проблема — виж т. 8.

---

## 2. Критични

### C6. Повторен `Connect` от вече свързан клиент остава без отговор
**Файлове:** [ServerPacketProcessor.cpp:36-41](server/source/ServerPacketProcessor/ServerPacketProcessor.cpp:36), [SessionManager.cpp:24-30](server/source/Sessions/SessionManager.cpp:24)

Оправено: `ProcessPackets` вече чисти опашката, а дедупликацията е по `(адрес && ID)`. Остава:

Сценарий: `ConnectionEstablished` се губи → клиентът ретрайва `Connect` (политиката му е `Unlimited`) → `CreateSession` вижда известен адрес и връща `nullopt` → сървърът не отговаря. Клиентът остава „Connecting“ завинаги и ретрайва с растящ интервал (виж M9).

**Поправка:** повторен `Connect` от известен адрес да върне отново `ConnectionEstablished` със същата сесия (идемпотентност), без да създава нов играч. Клиентът да игнорира `ConnectionEstablished`, ако вече е свързан.

---

## 3. Високи

### H1. „Надеждната“ доставка няма ACK — request ID-тата на двете страни се смесват
**Файлове:** [NetworkEndpoint.h:53-54, 143-146](networking/source/Networking/NetworkEndpoint.h:53), [ServerPacketProcessor.cpp:47](server/source/ServerPacketProcessor/ServerPacketProcessor.cpp:47)

- Всеки endpoint брои своите request ID-та от 1.
- При получаване на **какъвто и да е** пакет, ако ID-то му съвпада с ID на наш чакащ пакет, нашият се маха като потвърден.
- Сървърът копира ID-то на `Connect` в `ConnectionEstablished` (за да служи като ACK), но после пуска `UpdateWorldState` със своя брояч, който **също започва от 1**.

Резултат: случаен пакет от другата страна „потвърждава“ несвързан наш пакет. Няма отделно поле за ACK.

**Поправка (минимална):** добави в хедъра `nnAckId` (ID-то, което потвърждаваш) отделно от `nnRequestId`. Надеждните пакети се потвърждават само по `nnAckId` + адрес на подателя. По-добре — sequence + ack bitfield (стандартният подход за UDP игри).

### H2. Сървърът никога не ретрайва и никога не чисти pending опашката
**Файл:** [Server.cpp:73-93](server/source/Server.cpp:73)

`Server::Run` не вика `ProcessPendingPackets()`. Всички `Reliable` пакети на сървъра (`ConnectionEstablished`) влизат в `PendingPacketsQueueManager` и **остават там завинаги** (клиентът не праща ACK) → неограничен растеж на паметта, и никакъв ретрай при загуба.

### H3. Ретрайнатите пакети отиват на грешен адрес
**Файлове:** [NetworkEndpoint.h:178](networking/source/Networking/NetworkEndpoint.h:178), [PendingPacket.h](networking/source/Networking/Packets/PendingPacket.h)

`PendingPacket` не пази адреса на получателя; `ProcessPendingPackets` праща без адрес → `BaseSocket::Send` използва `m_oSocketAddress`. За клиента това случайно е сървърът (работи). За сървъра това е **собственият му bind адрес** — ретраят отива при самия него. Щом оправиш H2, ще удариш този бъг.

**Поправка:** `SocketAddress oDestination;` в `PendingPacket`, попълва се в `SendPacket`.

### H6. `Timer` връща грешни стойности
**Файл:** [Timer.cpp](common/source/Common/Timer.cpp)

Оправено: началният момент вече се взима при създаване (`= now()` в хедъра). Остава:
1. `Elapsed()` прави `duration_cast<seconds>` → отрязва до цели секунди (всичко под 1 s е 0).
2. `ElapsedMilliseconds()` **дели** на 1000 вместо да умножава → практически винаги 0.
3. Деструкторът вика `Restart()` — безсмислено.

Логовете „Shader creation took X ms“ и „Server start took X ms“ показват 0.

**Поправка:**
```cpp
float Timer::Elapsed()
{ return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_oStartInterval).count(); }
float Timer::ElapsedMilliseconds()
{ return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_oStartInterval).count(); }
```

### H9. `Transform`: ротацията се игнорира
**Файл:** [Transform.cpp:13-20](engine/source/Engine/GameObjects/Transform.cpp:13)

Оправено: скалата по подразбиране е 1. Остава: `oRotation` съществува, но `GetModelMatrix` не я прилага. За 2D: `glm::rotate(modelMatrix, glm::radians(oRotation.z), {0, 0, 1})` между `translate` и `scale`.

### H10. `Shader`: изтичане на GL обекти и `glDeleteProgram` с невалиден ID
**Файл:** [Shader.cpp:57-121](engine/source/Engine/Graphics/OpenGL/Shader.cpp:57)

- При грешка в компилацията на фрагментния шейдър вертексният не се трие; при грешка в линка не се трият нито шейдърите, нито програмата.
- При неуспех `m_nObjectID` остава `INVALID_OBJECT_ID` (= `-1` → `0xFFFFFFFF`) и деструкторът вика `glDeleteProgram(0xFFFFFFFF)` → `GL_INVALID_VALUE`. В OpenGL „няма обект“ е **0**, не -1 (виж и M10).
- Проверките `if (!pVertexShaderSource)` са мъртви — `c_str()` никога не е `nullptr`. Реалният случай (празен/липсващ файл) не се хваща.
- `BOOL` (Windows тип) вместо `GLint`.

### H11. `glm::ortho(0, 1600, 0, 900)` с цели числа → нищо не се вижда
**Файл:** [Renderer2D.cpp:25](engine/source/Engine/Graphics/Rendrer2D/Renderer2D.cpp:25)

`glm::ortho` е шаблон — с `int` аргументи `T = int` и делението е **целочислено**: `2 / 1600 = 0`, `2 / 900 = 0`. Мащабът по X и Y става 0 и всеки връх отива в (-1, -1) — квадратът колапсира в точка в ъгъла. Компилира се без предупреждение, защото `glm::imat4` се конвертира неявно към `glm::mat4`.

**Поправка:**
```cpp
m_oProjectionMatrix = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f);
```

Две неща, които ще излязат веднага след това:
- **Мащабът вече е в пиксели.** Квадратът е 1×1 ([OpenGLQuad.cpp](engine/source/Engine/Graphics/OpenGL/OpenGLQuad.cpp)), а `ExampleBaseGameLayer` слага `oScale = 0.5` → играчът ще е половин пиксел. Сложи напр. `oScale = {50, 50, 1}` и позиция, различна от (0, 0) — иначе е в долния ляв ъгъл.
- `1600×900` е твърдо зададено и не следва размера на прозореца. Матрицата се смята всеки кадър — може да се смята веднъж (и при resize). Редът `m_oProjectionMatrix = glm::mat4(1);` преди нея е излишен.

---

## 4. Средни

**M3. `Application.h` няма `#pragma once`.** [Application.h:1](engine/source/Engine/Core/Application/Application.h:1) — първото двойно включване в една единица → redefinition на `engine::Application`.

**M4. `BaseSocket::Open` — частично оправен.** [BaseSocket.cpp:17-55](networking/source/Networking/Socket/Base/BaseSocket.cpp:17) — проверката за `INVALID_SOCKET` е добавена. Остава:
- новото `return false` е **преди** `freeaddrinfo` → изтичане на `pAddressResult`. Премести `freeaddrinfo` преди проверката.
- ако `ioctlsocket` се провали → `continue` без `closesocket` → изтичане на сокет; ако е последният кандидат, `m_oSocket` остава валиден **блокиращ** сокет, адресът не е попълнен, а `Open` връща `true`. Затвори сокета и върни `m_oSocket = INVALID_SOCKET` преди `continue`.
- `getaddrinfo` грешката не се логва.
- `m_oSocket == SOCKET_ERROR` е излишно — за `SOCKET` това е същата стойност като `INVALID_SOCKET`.

**M6. Получените пакети не се валидират по размер спрямо типа.** [NetworkEndpoint.h:192-205](networking/source/Networking/NetworkEndpoint.h:192), [Networking.h:9-17](networking/source/Networking/Networking.h:9) — проверява се само `>= sizeof(PacketHeader)`. `DeserializePacket<T>` копира `sizeof(T)` независимо от `nPacketSize`. Не е memory-unsafe (буферът е 1024 и нулиран), но 16-байтов пакет с тип `UpdateWorldState` минава като валиден с нули. Добави `if (oPacket.nPacketSize < sizeof(T)) return nullptr;` и `static_assert(sizeof(T) <= n_MAX_PACKET_SIZE)` в `SendPacket`.

**M7. На Windows UDP `recvfrom` връща `WSAECONNRESET`, когато клиент затвори порта.** [NetworkEndpoint.h:95-103](networking/source/Networking/NetworkEndpoint.h:95) — текущият код прекъсва приемането за този тик без лог. Стандартно: `WSAIoctl(sock, SIO_UDP_CONNRESET, FALSE...)` на сървърния сокет, плюс лог на останалите грешки.

**M9. `Unlimited` политиката никога не спира, а таймаутът расте без горна граница** (`+= 2` всеки път). Сложи таван.

**M10. `INVALID_OBJECT_ID -1` за `unsigned int`.** [OpenGLObject.h:6](engine/source/Engine/Graphics/OpenGL/OpenGLObject.h:6) — OpenGL ползва 0 за „няма обект“. `VertexBuffer::Destroy` проверява `!= 0`, `Shader`/`VertexArray` — нищо. Унифицирай на `0` и проверявай навсякъде.

**M11. `VertexBufferLayout` е наследник на `OpenGLObject`, без да е GL обект.** [VertexBuffer.h:13](engine/source/Engine/Graphics/OpenGL/VertexBuffer.h:13) — `Bind/UnBind/Destroy` са празни, `m_nObjectID` не се ползва. `Generate()` в конструктора вика `glVertexAttribPointer` → layout-ът зависи от това кой VAO/VBO е bound **в момента на конструиране**. Направи го прост `struct` с описание, а `VertexArray` да прилага layout-а при `AddVertexBuffer`.

**M12. `LayerStack` притежава слоевете чрез сурови указатели.** [LayerStack.cpp](engine/source/Engine/Layers/LayerStack.cpp), [GameApplication.cpp:20](client/source/GameApplication.cpp:20) — `PushLayer(new ...)`; ако `OnAttach` върне `false`, слоят изтича. `nullptr` не се проверява преди `OnAttach`. Ползвай `std::unique_ptr<ILayer>`.

**M14. `NonCopyableObject` блокира и move.** [NonCopyableObject.h](common/source/Common/NonCopyableObject.h) — виртуалният деструктор е махнат; полиморфните наследници (`ILayer`, `GameObject`, `Event`, `IEventListener`, `OpenGLObject`) си имат свой `virtual` деструктор, така че нищо не се чупи. Остава: `delete`-натият copy блокира и **move**. Ако потрябва местене — `NonCopyableObject(NonCopyableObject&&) = default;` и `operator=(NonCopyableObject&&) = default;`. По желание деструкторът да е `protected`.

**M15. `Window` продължава при грешка.** [Window.cpp:18-53](engine/source/Engine/Core/Application/Window.cpp:18) — ако `glfwInit` или `glfwCreateWindow` се провалят, в Release кодът продължава с `nullptr` (там `ASSERT` е no-op). Резултатът от `InitializeGlad` се проверява само с `ASSERT`. `Application::Initialize` винаги връща `true`. Няма `glfwSetErrorCallback`, няма `glfwSwapInterval` (без vsync → 100% GPU/CPU).

**M16. `Server::Run` е busy-loop на 100% CPU и няма начин да спре.** [Server.cpp:73-93](server/source/Server.cpp:73) — `SERVER_TICK_RATE`, `nextSimulationTick`, `previousTime` са дефинирани, но не се ползват. Няма `sleep_until`. `m_bIsRunning` никога не става `false` (няма Ctrl+C handler). Ако сървърът изостане, `nextWorldStateUpdate += ...` ще прати серия от ъпдейти наведнъж — по-добре `nextWorldStateUpdate = currentTime + WORLD_STATE_RATE` или cap.

**M17. `LayerStack::Draw` рисува слоевете в обратен ред.** [LayerStack.cpp:40-49](engine/source/Engine/Layers/LayerStack.cpp:40) — обратният ред е правилен за **събития** (горният слой ги получава пръв и може да ги „изяде“), но не и за **рисуване**: горният слой (UI, debug) трябва да се рисува **последен**, за да е отгоре. Сега горният се рисува пръв, а играта — върху него. С един слой не личи. `Draw` да е отдолу нагоре; `Update` — по избор; `Clear` в обратен ред е правилно.

---

## 5. Ниски

| # | Къде | Какво |
|---|---|---|
| L1 | [ServerPacketProcessor.cpp:52-53](server/source/ServerPacketProcessor/ServerPacketProcessor.cpp:52) | `positionX` се присвоява два пъти, `positionY` никога. `1270` вместо `1280`. `rand()` без seed — ползвай `<random>`. |
| L2 | [ServerPacketProcessor.cpp:24](server/source/ServerPacketProcessor/ServerPacketProcessor.cpp:24) | `LOG_ERROR("Unsupported packet type: ", x)` — липсва `{0}`, стойността не се печата (в клиента е оправено). |
| L4 | [Assert.h](common/source/Common/Assert.h) | Макросът е `if (...) {...}` без `do { } while (0)` → капан с `if/else`. Използва `LOG_ERROR`, без да включва `Logger.h`. |
| L5 | [Common.h:17](common/source/Common/Common.h:17) | `#define ELEMENT_NOT_FOUND -1` вече не се ползва никъде (заменен с `std::optional`) — махни го. |
| L7 | [Logger.cpp:6](common/source/Common/Logger/Logger.cpp:6) | `DEFAULT_LOGGER_PATTERN` е дефиниран, но не се ползва. `LOG_*` преди `Logger::Init` → null deref. `stdout_color_mt` хвърля при второ `Init` със същото име. |
| L8 | [EventTypes.h](engine/source/Engine/EventSystem/EventTypes.h), [PacketTypes.h](networking/source/Networking/Packets/PacketTypes.h) | Ползват `std::string`/`uint16_t` без `<string>`/`<cstdint>`. |
| L9 | [EventDispatcher.h:26-34](engine/source/Engine/EventSystem/EventDispatcher.h:26), [EventBus.cpp:40-49](engine/source/Engine/EventSystem/EventBus.cpp:40) | `bIsHandled` вече се попълва, но **никой не го чете** — `EventBus::Sumbit` праща събитието на всички слушатели независимо от него. Добави `if (oEvent.bIsHandled) break;` в цикъла. `Dispatch` все още винаги връща `true`. |
| L10 | [EventDispatcher.h:43-45](engine/source/Engine/EventSystem/EventDispatcher.h:43) | Макросите `SUBMIT_EVENT` и т.н. завършват на `;` и разчитат, че извикващият ще напише `events::` пред тях (`events::SUBMIT_EVENT(x)` → `events::EventBus::...`). Замени с inline функции. |
| L11 | [Window.cpp:43](engine/source/Engine/Core/Application/Window.cpp:43) | `oWindow` в close callback-а не се ползва. |
| L12 | [FileSystem.cpp:14](engine/source/Engine/FileSystem/FileSystem.cpp:14) | `tellg()` може да върне `-1` → `size_t` огромно → `bad_alloc`. Пътищата са относителни спрямо CWD — работи от VS (`debugdir`), не и при директно стартиране на exe-то. |
| L13 | [GameTime.cpp:14-20](engine/source/Engine/Core/Application/GameTime.cpp:14) | Първият кадър има delta = цялото време от `glfwInit`. Инициализирай `m_dLastFrame` при старт. |
| L14 | [VertexShader.glsl:10](assets/shaders/VertexShader.glsl:10) | `vec4(oPosition, 1.0, 1.0)` — z=1 с 2D `ortho` дава z=-1, точно на границата на видимия обем. Работи, но е на ръба — сложи `0.0`. |
| L15 | [SocketAddress.cpp:25-29](networking/source/Networking/SocketAddress.cpp:25) | `Set` игнорира реалната дължина от `recvfrom`; `sockaddr` (16 байта) поддържа само IPv4. За бъдеще: `sockaddr_storage`. |
| L16 | [PendingPacketsQueueManager.h](networking/source/Networking/PendingPacketsQueueManager/PendingPacketsQueueManager.h), [SessionManager.h](server/source/Sessions/SessionManager.h) | Ползват `std::optional` и `std::unordered_map` без `<optional>`/`<unordered_map>` — компилира се само заради транзитивни include-и (spdlog). |
| L17 | [EventDispatcher.h:9](engine/source/Engine/EventSystem/EventDispatcher.h:9) | `typename = typename std::enable_if<...>` не ограничава нищо — без `::type` (или `enable_if_t`) това е винаги валиден тип. На C++23: `template<std::derived_from<Event> TEvent>`. |
| L18 | [PlayerGameObject.cpp:13-14](client/source/Client/PlayerGameObject.cpp:13) | Шейдърът се създава мързеливо в `Draw` без проверка `IsShaderValid()` — при грешка всеки кадър се bind-ва невалидна програма. Всеки играч ще има собствен шейдър. По-добре: шейдърът да се зарежда в `LoadContent` (там има как да се върне `false`) и да се споделя. |
| L19 | [Window.h:40](engine/source/Engine/Core/Application/Window.h:40) | `GetNativeWindow` връща `const GLFWwindow*`, а GLFW функциите искат `GLFWwindow*` — всеки извикващ ще трябва `const_cast`. Върни `GLFWwindow*`. |

### Мъртъв код (да се махне)
- **`client/source/Game.*`** — целият стар клас (глобални променливи, `key_callback`, ръчен VAO), `main` го е закоментирал.
- **`RecievedPacketsQueueManager.*`** — не се ползва; `Submit` е деклариран, но не е имплементиран.
- **`networking/source/Networking/Networking.cpp`** — празен.
- `Client::m_oRecivedPackets`, `Server::m_oRecivedPackets` — не се ползват (опашката е в базовия клас).
- `class ClientContext;` в [Client.h:8](client/source/Client/Client.h:8), празна `private:` секция.
- `const auto ePacketType` в [NetworkEndpoint.h:156](networking/source/Networking/NetworkEndpoint.h:156).
- Цикълът в `ProcessPackets` преди `m_oRecivedPacketsQueue.clear()` — `erase` и `++` вече нямат ефект; достатъчно е `for (const auto& p : queue) ProcessPacket(p); queue.clear();`.
- `if (!pPacketHeader)` / `if (!pBasePacket)` / `if (!pPacketData)` — указател от `&` или `reinterpret_cast` на масив никога не е `nullptr`.
- `struct networking::ReceivedPacket;` в [Server.h:8](server/source/Server.h:8) — излишна forward-декларация (хедърът вече е включен).
- В `GameApplication` клиентът изобщо не използва `Client` — мрежата е изключена от новия енджин път.

---

## 6. Стил, именуване, правопис

**Правописни грешки в публични имена** (по-добре сега, отколкото след като се разпространят):

| Сега | Трябва |
|---|---|
| `Recieve`, `RecievePackets`, `oRecievedPacket`, `nBytesRecived`, `m_oRecivedPacketsQueue` | `Receive`, `Received` |
| `RecievedPacketsQueueManager/` (папка и файлове) | `ReceivedPacketsQueueManager` |
| `EventBus::Sumbit` | `Submit` |
| `Rendrer2D/` (папка) | `Renderer2D` |
| `m_pRendered2D` | `m_pRenderer2D` |
| `GetDeltaTtime` | `GetDeltaTime` |
| `PacketRetryPolicty.h` | `PacketRetryPolicy.h` |
| `GetSesion` | `GetSession` |
| `m_oRevivedPacketsMap` | `m_oReceivedPacketsMap` |
| `pCurrentStideOffset`, `nStrideOffeset` | `Stride`, `Offset` |
| `pVertrexBuffer` | `pVertexBuffer` |
| `oFucntionReference` | `oFunctionReference` |
| `initilize`, `occured`, `initalize`, `diffrent` | `initialize`, `occurred`, `different` |

**Други:**
- `IsPacketAlreadyQueued` връща `true`, когато пакетът **НЕ е** в опашката — името казва обратното. Извикващият код компенсира с `if (!IsPacketAlreadyQueued)` → лог „Duplicate“. Обърни логиката и името (`IsDuplicate`).
- `OpenGLContext::CheckIfClientSupportsOpenGLVersion4` проверява за 4.3, а се иска 4.4 контекст; шейдърите са `#version 330`. Избери една версия.
- Унгарска нотация (`m_o`, `nn`, `str`) е последователна — ОК, но `oVertices` за `std::vector<float>` и `oPlayerStateDataElementSize` за `size_t` са подвеждащи.
- Празни конструктори/деструктори навсякъде — `= default` или просто ги махни.
- Методи, които не променят състояние, но не са `const` (`Timer::Elapsed`, `Shader::SetMatrix` може да остане non-const).
- `SendPacket` приема `TPacket&` (не-`const`) и **мутира** пакета на извикващия (слага request ID). В `Server::Update` същият обект се праща до всички клиенти → всички получават едно и също ID. Това е изненадващ страничен ефект.

---

## 7. Билд система (premake)

| # | Проблем | Препоръка |
|---|---|---|
| B1 | `DEBUG` е дублиран в `client/premake5.lua` | Вече идва от workspace-а — махни го от Client. Регенерирай проектите: сегашните `.vcxproj` са от преди промяната. |
| B4 | `filter "system:linux"` в Networking, но проектът е само за Windows | Махни филтъра. |
| B5 | GLM include dir е `vendor/glm/glm` → include-ите са `<mat4x4.hpp>`, `<gtc/...>`, `<matrix.hpp>` | Стандартното е `vendor/glm` + `<glm/mat4x4.hpp>`. Сегашното е податливо на колизии с други `matrix.hpp`. |
| B6 | `server/premake5.lua` хардкодва пътища вместо `IncludeDirectories[...]` | Използвай таблицата. |
| B7 | Няма `warnings "Extra"` / `fatalwarnings` | Повечето бъгове тук (неизползвани променливи, signed/unsigned, `LOG_ERROR` без placeholder, init order) щяха да излязат като предупреждения. |
| B8 | Release няма `defines { "NDEBUG" }`, няма `runtime "Release"` / `staticruntime` | Изрично задай. |
| B9 | `glad.c` се компилира в Engine, но Client ползва glad директно (`Game.cpp`) | Glad като отделен проект в групата `Dependencies`, както GLFW. |
| B10 | `.vscode/settings.json` е комитнат; `premake5.exe` в корена (не е комитнат) | Решете дали `.vscode` е за всички; добави скрипт `GenerateProjects.bat`. |
| B11 | Submodule `vendor/GLFW` е `-dirty` | Некомитнати промени в submodule-а — или ги комитни във форка, или ги отмени. |

---

## 8. Оценка на архитектурата

### Какво е направено добре

- **Модулното разделение е правилно.** `common` → `networking` / `engine` → `client` / `server` е чиста зависимостна посока без цикли между проекти. Сървърът не зависи от engine-а — точно така трябва.
- **Слоеве (`LayerStack` / `ILayer`)** — правилният модел за игров енджин (game layer, UI layer, debug overlay).
- **Event bus с типизиран dispatcher** — разумен старт, разделя GLFW от логиката.
- **Тънки RAII обвивки около GL обекти** (`VertexArray`, `VertexBuffer`, `Shader`) — правилната посока.
- **`NetworkEndpoint<TSocket>`** — общата логика за send/receive/retry е изнесена от клиента и сървъра, а те имплементират само `ProcessPacket`. Правилно разделение.
- **Packet processor класове** отделно от endpoint-а (`ClientPacketProcessor`, `ServerPacketProcessor`) — добро разделение на транспорт от логика.
- **Неблокиращ UDP** — правилният избор за real-time игра.
- **Фиксиран tick rate** на сървъра (заложен, макар и недовършен) — правилна идея.
- Последователни конвенции за именуване и структура на файлове.

### Структурни проблеми

**1. Протоколът е „memcpy на struct“.**
Пакетите се пращат като суров образ на C++ структура (`reinterpret_cast<const char*>(&oPacket)`). Това значи:
- зависи от padding, подравняване, размер на `enum`, endianness и компилатор;
- изпращат се padding байтове;
- няма магическо число и версия на протокола в хедъра (`ProtocolVersion` съществува, но не се праща);
- променлива дължина е невъзможна (затова е фиксиран масив от 5 играча).

**Препоръка:** малък `BitWriter`/`BitReader` (или `ByteStream`) с явен `Serialize(stream)` за всеки пакет. Хедър: `magic (u32) | protocol version (u16) | type (u16) | sequence (u16) | ack (u16) | ack bits (u32)`.

**2. Надеждността е смесена с транспорта и е грешна по дизайн.**
Request ID = ACK ID = sequence — едно поле за три роли (H1). Pending опашката вече е член на endpoint-а, но е без адрес на получателя (H3) и с ключ, който колизира между клиенти. Сървърът дори не я обработва (H2).

**Препоръка:** обект `Connection` за всеки отдалечен адрес (на сървъра — по един за сесия), който притежава своите sequence/ack броячи и pending опашка. `NetworkEndpoint` само рутира пакетите към правилната `Connection`. Махни singleton-ите.

**3. Сесии и състояние на играча са разделени и могат да се разминат.**
`SessionManager` (singleton) пази сесиите, `ServerContext` пази отделен списък от `PlayerStateData`. Няма disconnect, таймаут, heartbeat — сесиите никога не се трият. Сесията се идентифицира само по IP:порт.

**Препоръка:** `Session` да притежава `Connection` + състоянието на играча; `SessionManager` да е член на `Server` (не singleton); heartbeat + таймаут.

**4. Прекалено много singleton-и.**
`EventBus`, `SessionManager`, `Logger` (статичен). (`PendingPacketsQueueManager` вече е член на endpoint-а ✅.) Това прави кода невъзможен за unit тестове, скрива зависимостите и в мрежовия случай е директен бъг (споделена pending опашка). Изключение може да се направи за логера.

**5. Engine-ът няма реален рендер път.**
Пътят submit → draw call вече работи (`GameObject::Draw(Renderer2D&)`) ✅. Остава: няма камера (projection-ът е твърдо зададен в рендерера, H11); няма batching; шейдърът се сетва по име при всяка команда (`glGetUniformLocation` всеки кадър); `glClear` е в рендерера с `TODO REMOVE`.

**Препоръка:** `Renderer2D::BeginScene(camera)` → `DrawQuad(transform, color/texture)` → `EndScene()` с batching; `ILayer::Draw(Renderer2D&)` (вече е заложено) и `GameObject::Draw(Renderer2D&)`; кеширане на uniform locations в `Shader`.

**6. Event системата е непълна.**
- Редът вече е детерминиран (`std::map`), `bIsHandled` се попълва, но `EventBus` не спира при него (L9).
- Слоевете се регистрират директно в глобалния bus, вместо `Application` да ги пропагира през `LayerStack`.
- Отписване по време на dispatch инвалидира итератора.
- Само едно събитие (`WindowClose`) — няма input, resize, key.

**7. `Application` смесва жизнен цикъл и инициализация на подсистеми.**
`Logger::Init("Application")` е хардкоднат вътре в engine-а; `Window` вика `glfwInit` през временен `OpenGLContext`; `Window` дестракторът вика `glfwTerminate` (два прозореца → проблем). Грешките не се пропагират (`Initialize` винаги `true`).

**8. Клиентът е разцепен на два свята.**
Мрежата живее в стария `Game` (мъртъв), а новият `GameApplication` няма мрежа. Следващата стъпка е мрежата да стане слой (`NetworkLayer`) или подсистема на `Application`, която пише в `GameContext`, а game layer-ът чете от него.

### Оценка

| Аспект | Оценка | Коментар |
|---|---|---|
| Разделение на модули | **8/10** | Правилна посока на зависимостите, сървърът е независим от графиката. |
| Engine дизайн (идея) | **7/10** | Layers + events + renderer queue — стандартният модел, правилно заложен. |
| Engine изпълнение | **4/10** | Рендерерът не рисува, ownership със сурови указатели. |
| Мрежова архитектура (идея) | **6/10** | Общ endpoint + processor-и + неблокиращ UDP — добре. |
| Мрежово изпълнение | **3/10** | Грешна ACK семантика, singleton опашка, сървърът не ретрайва. |
| Сигурност (вход от мрежата) | **3/10** | Без валидация на размер/тип на пакетите. |
| Управление на ресурси/RAII | **5/10** | Добре за GL обвивки и сокети, лошо за слоеве и шейдъри при грешка. |
| Билд система | **6/10** | Работи на Windows; `DEBUG` дефиниран грешно, регистър на пътища. |
| Четимост/стил | **6/10** | Последователно, но много правописни грешки и мъртъв код. |
| **Общо** | **~5/10** | Солидна скелетна архитектура, но кодът е в преходно, недовършено състояние. |

**С една дума:** архитектурната *идея* е зряла — виждаш как се прави енджин и как се разделят клиент/сървър. Проблемите са в *изпълнението* и в мрежовия протокол, който има нужда от редизайн (connection обект, отделен ACK, сериализация вместо memcpy), преди да се надгражда отгоре. Ако оправиш Високите и направиш точки 1–4 от архитектурните препоръки, проектът ще стои на стабилна основа.

---

## 8а. Преоценка — в контекста на прототип в активна разработка

Горната оценка е като за завършен код. За прототип, който се развива и тества в момента, голяма част от находките са **нормални за етапа** и не са причина за притеснение: memcpy сериализация, singleton-и, липса на batching, мъртъв код (`Game.*`), правопис, хардкоднати адреси.

| Аспект | Оценка за прототип |
|---|---|
| Насока и идея | **8/10** — правилните абстракции са избрани рано (модули, слоеве, events, render queue, общ endpoint + processor-и, неблокиращ UDP, tick rate). Няма нищо, което трябва да се изхвърли. |
| Изпълнение спрямо етапа | **6/10** — скелетът е на място; бъговете са типични за код „по средата на рефакторинг“. Единственото, което трябва да се реши **преди** да се трупа още код отгоре, е семантиката на ACK/надеждността (H1–H3), защото всичко мрежово ще стъпи на нея. |

## 8б. Ревизия след поправките (2026-11-25)

Оправени и махнати от ревюто: **C1–C5, C7** (критичните), **M1** (`Windows.h`), **M2** (кръговия include), **M8** (`erase` в `GetPacketsToRetry`), **B2** (навсякъде C++23). C6 е частично оправен — остатъкът е описан в т. 2.

Проектът е само за Windows/MSVC — махнати са находките за преносимост (`__int64`, `__super`, `__debugbreak`, регистър на пътищата B3, платформен слой).

Оставено по избор: **L6** (`IN/OUT/OPTIONAL/PURE/BASE`) — конвенция на проекта, остава. Работи и при двата реда на include-ите, защото навсякъде се пише `PURE;`.

Втора ревизия: оправени **M5** (`WinsockContext`), **L3** (празният лог, сега е `//TODO LOG`), L2 в клиента. Частично: **H6** (началният момент), **M4** (проверка за невалиден сокет), **M14** (виртуалният деструктор).

Ново от втората ревизия: **C8** (`InitializeGlad` в `ASSERT`), **H7 пренаписан** — `DEBUG` в главния файл попада само в GLFW, а Server го загуби; **L16** (липсващи include-и).

**Не е оправено, въпреки че изглеждаше така:** H1 (ACK). В работното копие няма поле за ACK — `FindPendingPacket` още търси по `nnRequestId`, а `ConnectionEstablished` копира ID-то на `Connect`.

Трета ревизия (комити `Design`, `Hot fixes`): оправени **C8**, **H4** (draw call + `Submit` през `GameObject::Draw(Renderer2D&)`), **H5** (слоят вече не рисува сам), **H7/B1** (`DEBUG` на ниво workspace — проектите трябва да се регенерират), **H8**, **M13**; `PendingPacketsQueueManager` вече не е singleton. Частично: **H9** (скала 1, ротацията остава), **L9**, **L14**.

Ново от третата ревизия: **H11** (целочислен `glm::ortho`), **M17** (обратен ред на рисуване на слоевете), **L17–L19**.

## 9. Препоръчан ред за оправяне

1. **Да се вижда нещо:** H11 (`ortho` с `float` + мащаб в пиксели), M17 (ред на рисуване), H9 (ротация).
2. **Билд:** регенерирай проектите, махни `DEBUG` от Client (B1), M3 (`#pragma once`), B7 (warnings), L16.
3. **Мрежа — потвърждение:** по [ACK_DESIGN.md](ACK_DESIGN.md) — H1, H2, H3, C6.
4. **Енджин:** H10, M10.
5. **Common и мрежа:** H6 (`Timer`), M4 (`freeaddrinfo`, `ioctlsocket`).
6. **Мрежов редизайн:** `Connection` per peer, после сериализация вместо memcpy.
7. **Почистване:** мъртъв код, правопис, L1, L2.
