#include "core/config.h"

namespace leep
{
    void GameInit();
    void GameLogic();
    void GameClose();

    void Init(const THE_Config &cnfg);
    void Logic();
    void Close();
    void RenderFrame();
    void ShowFrame();
}