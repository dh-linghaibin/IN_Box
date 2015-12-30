

#ifndef PROMPT_H
#define PROMPT_H

#include "Type.h"

void PromptInit(void);
void PromptTimeServive(void);
void PromptSetMusic(u8 data);
void PromptSetMoto(u8 data);
u8 PromptPocket(void);
void PromptPocketReflect(void);
void PromptPocketSetFlag(u8 cmd);
void PromptMucicReflect(void);
void PromptMucicSetFlag(u8 cmd);
void PromptMusicShowdown(void);

#endif
