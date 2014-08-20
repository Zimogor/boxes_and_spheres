#include "Constants.h"

// приложение
bool Const::winMaximized = true; // окно на весь экран
UINT Const::animationSpeed = 24; // скорость анимации (кадров в секунду)
float Const::clipBlendSpeed = 3.0f; // скорость смешения анимации
bool Const::writeToDebugFile = false; // запись в файл отладки
bool Const::breakDT = true; // включение искусственных тормозов
UINT Const::sleepTime = 0; // искусственное снижение производительности
bool Const::showConsole = true; // отображать консоль
#if defined (DEBUG) || defined (_DEBUG)
bool Const::loadConstants = true; // загрузить константы из файла
#else 
bool Const::loadConstants = true; // загрузить константы из файла
#endif

// видео
bool Const::detalization = true; // высокая детализация
bool Const::backCulling = true; // режим backCulling
bool Const::wireFrame = false; // режим показа сетки
bool Const::alpha = true; // альфа-канал
UINT Const::vSync = 0; // вертикальныя синхронизация
bool Const::multisampling = true; // мультисэмплирование
bool Const::anisotrophicFilter = false; // анизотропная фильтрация
bool Const::showBoundingBox = false; // показывать boundingbox

// территория
int Const::regionCellSize = 4; // размер региона территории (в квадратах)
float Const::updateDistance = 1.0f; // растояние на котором обновляется поле регионов
UINT Const::regionVertexAmount = Const::regionCellSize * Const::regionCellSize * 4; // количество вершин в регионе
UINT Const::regionIndexAmount = Const::regionCellSize * Const::regionCellSize * 6; // количество индексов в регионе
float Const::regionHalfCellSize = Const::regionCellSize * 0.5f; // половина размера региона в ячейках
int Const::regionFieldSize = UINT(Const::showRadius * 2 / Const::regionCellSize + 1); // размер поля регионов
UINT Const::tilesInSquadForest = 2; // количество тайлов в одной текстуре леса
UINT Const::tilesInSquadMeadow = 2; // количество тайлов в одной текстуре луга
UINT Const::tilesInSquadSwamp = 2; // количество тайлов в одной текстуре болота
float Const::widthScatter = 0.1f; // разброс по ширине
float Const::heightScatter = 0.25f; // разброс по высоте
float Const::positionScatter = 0.3f; // разброс по координатам
bool Const::showTiles = false; // показывать клетки
wchar_t* Const::testFileName = NULL; // имя тестируемого объекта
int Const::location = 0; // номер стартовой локации
UINT Const::patchFieldSize = 128; // размер поля патчей ??? без кофигурации
int Const::patchCellSize = 256; // размер лоскута в ячейках ??? без конфигурации ??? было 256

// свет и тени
bool Const::shadowsOn = true; // включить тени
NumXY<UINT> Const::shadowMapSize = NumXY<UINT>(1800, 1600); // разрешение карты теней
myXMFLOAT3 Const::lightPosition = spawnPoint + myXMFLOAT3(6.0f, 3.0f, 12.0f); // координаты источника света
XMFLOAT4 Const::viewRect = { -5.f, 5.0f, 5.0f, -5.0f }; // размер проекции света
NumXY<float> Const::nearFarLightZ(10.0f, 20.0f); // отрезание света (x - near, y - far)

// камера
bool Const::releaseCamera = true; // камера для релиза с ограничениями
float Const::scrollSpeed = 0.0006f; // скорость скроллинга
float Const::showRadius = 23.0f; // радиус отображения регионов
float Const::maxRadius = Const::releaseCamera ? 11.0f : 100.0f; // максимальное отдаление
float Const::minRadius = Const::releaseCamera ? 4.0f : 0.3f; // минимальное приближение
NumXY<float> Const::pitchLimit = NumXY<float>( -1.0f, 1.0f ); // ограничения тангажа

// герой
myXMFLOAT3 Const::spawnPoint = myXMFLOAT3(5.0f, 0.0f, 20.0f); // место зарождения героя
float Const::heroSpeed = 5.0f; // скорость героя
bool Const::fullInventory = false; // полный инвентарь
float Const::distanceToPick = 0.6f; // расстояние подбирания с территории

// глубины спрайтов
float Const::mainMenuBackgroundDepth = 15.0f; // глубина фона основного меню
float Const::mainMenuButtonsDepth = 9.0f; // глубина кнопок основного меню
float Const::mainMenuButtonsFontDepth = 0.3f; // глубина текста кнопок основного меню
float Const::ingameMenuBackgroundDepth = 10.0f; // глубина фона игрового меню
float Const::ingameMenuButtonsDepth = 9.0f; // глубина кнопок игрового меню
float Const::ingameMenuIconsDepth = 0.45f; // глубина иконок игрового меню
float Const::ingameMenuIconsFontDepth = 0.4f; // глубина шрифта иконок игрового меню
float Const::ingameMenuIconsUpDepth = 0.1f; // наращивание глубины выделеных иконок
float Const::consoleDepth = 0.2f; // глубина консоли
float Const::helpFontDepth = 1.0f; // глубина вспомогательного шрифта
float Const::mapDepth = 9.0f; // глубина карты

// ---------------------------------функции----------------------------------

// файлы с константами
std::ifstream Const::config; // кофигурационный файл

// загрузить константы из файла
bool Const::LoadFromFile() {

	// открытие файла
	config.open("resources//config", std::ios_base::binary);
	if (!config.is_open()) {
		MyMessage(L"constant file doesn't exist");
		return false;
	}

	// загрузить параметры видео
	config.read((char*)&alpha, 1); // альфа-канал
	config.read((char*)&backCulling, 1); // backCulling
	config.read((char*)&wireFrame, 1); // режим отображения сетки
	config.read((char*)&detalization, 1); // детализация
	config.read((char*)&vSync, 4); // вертикальная синхронизация
	config.read((char*)&multisampling, 1); // мультисэмплирование
	config.read((char*)&anisotrophicFilter, 1); // анизотропная фильтрация
	config.read((char*)&showBoundingBox, 1); // показывать bounding box

	// загрузить параметры приложения
	config.read((char*)&winMaximized, 1); // на весь экран
	config.read((char*)&animationSpeed, 4); // скорость анимации
	config.read((char*)&writeToDebugFile, 1); // запись в файл отладки
	config.read((char*)&breakDT, 1); // снижать скорость при тормозах
	config.read((char*)&sleepTime, 4); // искуственные тормоза
	config.read((char*)&showConsole, 1); // показывать консоль
	config.read((char*)&clipBlendSpeed, 4); // скорость смешения анимации

	// загрузить параметры территории
	config.read((char*)&regionCellSize, 4); // размер региона в ячейках
	config.read((char*)&updateDistance, 4); // расстояние обновления регионов
	config.read((char*)&tilesInSquadForest, 4); // количество тайлов на текстуру леса
	config.read((char*)&tilesInSquadMeadow, 4); // количество тайлов на текстуру болота
	config.read((char*)&tilesInSquadSwamp, 4); // количество тайлов на текстуру болота
	config.read((char*)&widthScatter, 4); // разброс объектов по ширине
	config.read((char*)&heightScatter, 4); // разброс объектов по высоте
	config.read((char*)&positionScatter, 4); // разброс объектов по координатам
	config.read((char*)&showTiles, 1); // показывать клетки	
	// режим тестирования объекта
	bool testObject; // нужно тестирование
	int wordLength; // количество симоволов в названии
	config.read((char*)&testObject, 1);
	config.read((char*)&wordLength, 4);
	testFileName = new wchar_t[wordLength + 1];
	config.read((char*)testFileName, wordLength * sizeof(wchar_t) + sizeof(wchar_t));
	if (!testObject) ReleaseNULLS(testFileName); // тестирование не нужно
	config.read((char*)&location, 4); // номер стартовой локации

	// вычислить зависимые константы
	regionVertexAmount = regionCellSize * regionCellSize * 4; // количество вершин в регионе
	regionIndexAmount = regionCellSize * regionCellSize * 6; // количество индексов в регионе
	regionHalfCellSize = regionCellSize * 0.5f; // половина размера региона в ячейках
	regionFieldSize = UINT(showRadius * 2 / regionCellSize + 1); // размер поля регионов

	// загрузить параметры освещения и теней
	float halfShadowBoxSide;
	config.read((char*)&shadowsOn, 1);
	config.read((char*)&shadowMapSize.x, 4);
	config.read((char*)&shadowMapSize.y, 4);
	config.read((char*)&lightPosition.x, 4);
	config.read((char*)&lightPosition.y, 4);
	config.read((char*)&lightPosition.z, 4);
	config.read((char*)&halfShadowBoxSide, 4);
	config.read((char*)&nearFarLightZ.x, 4);
	config.read((char*)&nearFarLightZ.y, 4);
	// высислить зависимые константы
	viewRect.y = viewRect.z = -halfShadowBoxSide;
	viewRect.w = viewRect.x = halfShadowBoxSide;

	// загрузить параметры для камеры
	float maxRelaseRadius, maxFreeRadius, minReleaseRadius, minFreeRadius;
	float maxPitch, minPitch;
	config.read((char*)&releaseCamera, 1); // фиксированная релизная камера
	config.read((char*)&scrollSpeed, 4); // скорость скроллинга
	config.read((char*)&showRadius, 4); // радиус отображения регионов
	config.read((char*)&maxRelaseRadius, 4); // максимальный радиус в релизной камере
	config.read((char*)&minReleaseRadius, 4); // минимальный радиус в релизной камере
	config.read((char*)&maxFreeRadius, 4); // максимальный радиус в свободной камере
	config.read((char*)&minFreeRadius, 4); // минимальный радиус в релизной камере
	config.read((char*)&maxPitch, 4); // максимальный угол тангажа
	config.read((char*)&minPitch, 4); // минимальный угол тангажа
	// вычислить зависимые константы
	maxRadius = releaseCamera ? maxRelaseRadius : maxFreeRadius; // максимальное отдаление
	minRadius = releaseCamera ? minReleaseRadius : minFreeRadius; // минимальное приближение
	pitchLimit = NumXY<float>(minPitch, maxPitch); // ограничения тангажа
	scrollSpeed /= 1000.0f; // скорость скроллинга

	// загрузить параметры героя
	float heroPosX, heroPosY;
	config.read((char*)&heroPosX, 4); // X-координата героя
	config.read((char*)&heroPosY, 4); // Y-координата героя
	config.read((char*)&heroSpeed, 4); // скорость перемещения героя
	config.read((char*)&fullInventory, 1); // полный инвентарь
	config.read((char*)&distanceToPick, 4); // растояние подбирания с территории
	// вычислить зависимые константы
	spawnPoint = myXMFLOAT3(heroPosX, 0.0f, heroPosY); // место зарождения героя

	// загрузить глубины спрайтов
	config.read((char*)&mainMenuBackgroundDepth, 4); // главное меню
	config.read((char*)&mainMenuButtonsDepth, 4); // кнопки главного меню
	config.read((char*)&mainMenuButtonsFontDepth, 4); // шрифт кнопок главного меню
	config.read((char*)&ingameMenuBackgroundDepth, 4); // внутриигровое меню
	config.read((char*)&ingameMenuButtonsDepth, 4); // кнопки внутриигрового меню
	config.read((char*)&ingameMenuIconsDepth, 4); // иконки внутриигрового меню
	config.read((char*)&ingameMenuIconsFontDepth, 4); // шрифт иконок внутриигрового меню
	config.read((char*)&ingameMenuIconsUpDepth, 4); // наращивание глубины выделенных иконок
	config.read((char*)&consoleDepth, 4); // консоль
	config.read((char*)&helpFontDepth, 4); // вспомогательный шрифт
	config.read((char*)&mapDepth, 4); // карта

	// закрыть файлы
	config.close();

	maxRadius = releaseCamera ? 11.0f : 100.0f; // максимальное отдаление

	return true;

}

// выгрузить константы
void Const::UnloadConstants() {

	ReleaseNULLS(testFileName);

}