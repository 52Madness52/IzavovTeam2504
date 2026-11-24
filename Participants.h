#pragma once

#include "HashTypes.h"

// Подключение алгоритмов участников
// Добавляем файлы по мере готовности алгоритмов

// Общий тип функции хеширования
using Participant = Digest(*)(const Bytes&);
