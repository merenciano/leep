#include "core/config.h"

namespace leep
{
    void GameInit();
    void GameLogic();
    void GameClose();

    void Init(const LeepConfig &cnfg);
    void Logic();
    void Close();
    void RenderFrame();
    void ShowFrame();
}