#ifndef DEFINITION_H
#define DEFINITION_H

enum class Color { black, darkGray, lightGray, white, unknown, ghostAdd, ghostMove };
enum class Direction { horizontal0, vertical1, horizontal2, vertical3, unknown };

#define NOT_CONNECTED 0
#define CONNECTING    1
#define CONNECTED     2

#define WAKE_UP_NONE    0
#define WAKE_UP_DTR     1
#define WAKE_UP_RTS     2
#define WAKE_UP_DTR_INV 3
#define WAKE_UP_RTS_INV 4

#define NONE_PORT "<None>"

#define TIMEOUT_WRITE     1000
#define TIMEOUT_READ      3000
#define NB_TRY_WRITE_MAX  3

#endif // DEFINITION_H
