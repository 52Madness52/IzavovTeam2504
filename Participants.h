#pragma once

#include "HashTypes.h"

// Алгоритмы участников

#include "algorithms/participant01.h"
#include "algorithms/participant02.h"
#include "algorithms/participant03.h"
#include "algorithms/participant04.h"
#include "algorithms/participant05.h"

// Общий тип функции хеширования
using Participant = Digest(*)(const Bytes&);
