#pragma once
#include "mp_omp.h"

namespace ifmo_mtp_omp {
#define NUM_IMPLS 32

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(static))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp2)
MP_DECL_COLLAPSE(mp3)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(static))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(static) reduction(+:cell))
MP_DECL_COLLAPSE(mp4)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(static, 1))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp5)
MP_DECL_COLLAPSE(mp6)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(static, 1))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(static, 1) reduction(+:cell))
MP_DECL_COLLAPSE(mp7)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(static, 10))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp8)
MP_DECL_COLLAPSE(mp9)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(static, 10))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(static, 10) reduction(+:cell))
MP_DECL_COLLAPSE(mp10)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(static, 100))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp11)
MP_DECL_COLLAPSE(mp12)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(static, 100))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(static, 100) reduction(+:cell))
MP_DECL_COLLAPSE(mp13)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(dynamic, 1))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp14)
MP_DECL_COLLAPSE(mp15)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(dynamic, 1))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(dynamic, 1) reduction(+:cell))
MP_DECL_COLLAPSE(mp16)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(dynamic, 10))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp17)
MP_DECL_COLLAPSE(mp18)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(dynamic, 10))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(dynamic, 10) reduction(+:cell))
MP_DECL_COLLAPSE(mp19)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(dynamic, 100))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp20)
MP_DECL_COLLAPSE(mp21)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(dynamic, 100))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(dynamic, 100) reduction(+:cell))
MP_DECL_COLLAPSE(mp22)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(guided, 1))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp23)
MP_DECL_COLLAPSE(mp24)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(guided, 1))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(guided, 1) reduction(+:cell))
MP_DECL_COLLAPSE(mp25)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(guided, 10))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp26)
MP_DECL_COLLAPSE(mp27)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(guided, 10))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(guided, 10) reduction(+:cell))
MP_DECL_COLLAPSE(mp28)

#include "undef.h"
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL
#define __OMP_FOR OMP_FOR(schedule(guided, 100))
#define _____OMP_FOR
#define ________OMP_FOR
MP_DECL(mp29)
MP_DECL_COLLAPSE(mp30)

#include "undef.h"
#undef OMP_NESTED
#define OMP_NESTED 1
#define __OMP_PARALLEL OMP_PARALLEL()
#define _____OMP_PARALLEL
#define ________OMP_PARALLEL OMP_PARALLEL()
#define __OMP_FOR OMP_FOR(schedule(guided, 100))
#define _____OMP_FOR
#define ________OMP_FOR OMP_FOR(schedule(guided, 100) reduction(+:cell))
MP_DECL_COLLAPSE(mp31)
}
