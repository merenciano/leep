#include "render/commands/init-material.h"

#include "core/manager.h"

namespace motoret
{
    InitMaterial::InitMaterial()
    {

    }

    InitMaterial::~InitMaterial()
    {

    }

    InitMaterial& InitMaterial::set_material(MaterialType material)
    {
        material_ = material;
        return *this;
    }

    void InitMaterial::executeCommand() const
    {
        Renderer &r = Manager::instance().renderer();
        r.materials_[material_]->init();
    }
}