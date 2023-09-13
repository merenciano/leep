#ifndef THE_TOOLS_LUA_SCRIPTING_H
#define THE_TOOLS_LUA_SCRIPTING_H

void THE_ScriptingInit(void);
void THE_ScriptingSetGlobal(const char *name, float value);
void THE_ScriptingExecute(const char *path);
void THE_ScriptingCommand(const char *command);

#endif

