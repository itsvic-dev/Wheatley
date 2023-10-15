#ifndef _ASSERT_H
#define _ASSERT_H

void Panic(char *msg);

#define STATUS_PANIC(msg) if (status != EFI_SUCCESS) Panic(msg);
#define __STR(x) #x
#define STR(x) __STR(x)
#define ASSERT(cond) if (!(cond)) Panic("assert failed: " #cond " (on line " STR(__LINE__) ")")

#endif // _ASSERT_H
