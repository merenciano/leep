#include "leep.h"

#define LEEP_SINGLE_THREAD

void leep::GameInit()
{
	ResourceMap &rm = GM.resource_map();
	const String tp = "../assets/tex/";
	rm.addGeometry("MatBall", CUBE_MESH);
	rm.addTexture("Skybox", 1024, 1024, THE_TEX_ENVIRONMENT);
	rm.addTexture("IrradianceEnv", 1024, 1024, THE_TEX_ENVIRONMENT);
	rm.addTexture("PrefilterSpec", 128, 128, THE_TEX_PREFILTER_ENVIRONMENT);
	rm.addTexture("LutMap", 512, 512, THE_TEX_LUT);

	rm.addTexture("CelticGold_A" ,tp + "celtic-gold/celtic-gold_A.png", THE_TEX_SRGB);
	rm.addTexture("CelticGold_N" ,tp + "celtic-gold/celtic-gold_N.png", THE_TEX_RGB);
	rm.addTexture("CelticGold_M" ,tp + "celtic-gold/celtic-gold_M.png", THE_TEX_R);
	rm.addTexture("CelticGold_R" ,tp + "celtic-gold/celtic-gold_R.png", THE_TEX_R);

	rm.addTexture("Peeling_A" ,tp + "peeling/peeling_A.png", THE_TEX_SRGB);
	rm.addTexture("Peeling_N" ,tp + "peeling/peeling_N.png", THE_TEX_RGB);
	rm.addTexture("Peeling_M" ,tp + "peeling/peeling_M.png", THE_TEX_R);
	rm.addTexture("Peeling_R" ,tp + "peeling/peeling_R.png", THE_TEX_R);

	rm.addTexture("Rusted_A" ,tp + "rusted/rusted_A.png", THE_TEX_SRGB);
	rm.addTexture("Rusted_N" ,tp + "rusted/rusted_N.png", THE_TEX_RGB);
	rm.addTexture("Rusted_M" ,tp + "rusted/rusted_M.png", THE_TEX_R);
	rm.addTexture("Rusted_R" ,tp + "rusted/rusted_R.png", THE_TEX_R);

	rm.addTexture("Tiles_A" ,tp + "tiles/tiles_A.png", THE_TEX_SRGB);
	rm.addTexture("Tiles_N" ,tp + "tiles/tiles_N.png", THE_TEX_RGB);
	rm.addTexture("Tiles_M" ,tp + "tiles/tiles_M.png", THE_TEX_R);
	rm.addTexture("Tiles_R" ,tp + "tiles/tiles_R.png", THE_TEX_R);

	rm.addTexture("ShipPanels_A" ,tp + "ship-panels/ship-panels_A.png", THE_TEX_SRGB);
	rm.addTexture("ShipPanels_N" ,tp + "ship-panels/ship-panels_N.png", THE_TEX_RGB);
	rm.addTexture("ShipPanels_M" ,tp + "ship-panels/ship-panels_M.png", THE_TEX_R);
	rm.addTexture("ShipPanels_R" ,tp + "ship-panels/ship-panels_R.png", THE_TEX_R);

	rm.addTexture("Shore_A" ,tp + "shore/shore_A.png", THE_TEX_SRGB);
	rm.addTexture("Shore_N" ,tp + "shore/shore_N.png", THE_TEX_RGB);
	rm.addTexture("Shore_M" ,tp + "shore/shore_M.png", THE_TEX_R);
	rm.addTexture("Shore_R" ,tp + "shore/shore_R.png", THE_TEX_R);

	rm.addTexture("Cliff_A" ,tp + "cliff/cliff_A.png", THE_TEX_SRGB);
	rm.addTexture("Cliff_N" ,tp + "cliff/cliff_N.png", THE_TEX_RGB);
	rm.addTexture("Cliff_M" ,tp + "cliff/cliff_M.png", THE_TEX_R);
	rm.addTexture("Cliff_R" ,tp + "cliff/cliff_R.png", THE_TEX_R);

	rm.addTexture("Granite_A" ,tp + "granite/granite_A.png", THE_TEX_SRGB);
	rm.addTexture("Granite_N" ,tp + "granite/granite_N.png", THE_TEX_RGB);
	rm.addTexture("Granite_M" ,tp + "granite/granite_M.png", THE_TEX_R);
	rm.addTexture("Granite_R" ,tp + "granite/granite_R.png", THE_TEX_R);

	rm.addTexture("Foam_A" ,tp + "foam/foam_A.png", THE_TEX_SRGB);
	rm.addTexture("Foam_N" ,tp + "foam/foam_N.png", THE_TEX_RGB);
	rm.addTexture("Foam_M" ,tp + "foam/foam_M.png", THE_TEX_R);
	rm.addTexture("Foam_R" ,tp + "foam/foam_R.png", THE_TEX_R);

	THE_PbrData pbr;
	pbr.color = svec3(1.0f, 1.0f, 1.0f);
	pbr.tiling_x = 4.0f;
	pbr.tiling_y = 4.0f;
	pbr.use_albedo_map = 1.0f;
	pbr.use_pbr_maps = 1.0f;
	pbr.metallic = 0.5f;
	pbr.roughness = 0.5f;
	pbr.normal_map_intensity = 1.0f;

	sun_dir_intensity = svec4(0.0f, -1.0f, -0.1f, 1.0f);
	THE_InitMaterial(THE_MT_PBR, "pbr");

	GM.scene().createContainer(EntityType::RENDERABLE);

	// CelticGold
	{
		Entity e = Entity::CreateEntity("CelticGold", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 0.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("CelticGold_A");
		t[1] = GM.resource_map().textures.at("CelticGold_M");
		t[2] = GM.resource_map().textures.at("CelticGold_R");
		t[3] = GM.resource_map().textures.at("CelticGold_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Shore
	{
		pbr.tiling_x = 2.0f;
		pbr.tiling_y = 2.0f;
		pbr.normal_map_intensity = 0.5f;
		Entity e = Entity::CreateEntity("Shore", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(4.0f, 0.0f, 0.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Shore_A");
		t[1] = GM.resource_map().textures.at("Shore_M");
		t[2] = GM.resource_map().textures.at("Shore_R");
		t[3] = GM.resource_map().textures.at("Shore_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Peeling
	{
		pbr.tiling_x = 1.0f;
		pbr.tiling_y = 1.0f;
		pbr.normal_map_intensity = 0.7f;
		Entity e = Entity::CreateEntity("Peeling", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 0.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Peeling_A");
		t[1] = GM.resource_map().textures.at("Peeling_M");
		t[2] = GM.resource_map().textures.at("Peeling_R");
		t[3] = GM.resource_map().textures.at("Peeling_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Rusted
	{
		pbr.tiling_x = 1.0f;
		pbr.tiling_y = 1.0f;
		pbr.normal_map_intensity = 0.2f;
		Entity e = Entity::CreateEntity("Rusted", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 2.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Rusted_A");
		t[1] = GM.resource_map().textures.at("Rusted_M");
		t[2] = GM.resource_map().textures.at("Rusted_R");
		t[3] = GM.resource_map().textures.at("Rusted_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Tiles
	{
		pbr.tiling_x = 4.0f;
		pbr.tiling_y = 4.0f;
		pbr.normal_map_intensity = 1.0f;
		Entity e = Entity::CreateEntity("Tiles", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(4.0f, 0.0f, 2.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Tiles_A");
		t[1] = GM.resource_map().textures.at("Tiles_M");
		t[2] = GM.resource_map().textures.at("Tiles_R");
		t[3] = GM.resource_map().textures.at("Tiles_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Ship Panels
	{
		pbr.tiling_x = 1.0f;
		pbr.tiling_y = 1.0f;
		pbr.normal_map_intensity = 1.0f;
		Entity e = Entity::CreateEntity("ShipPanels", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 2.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("ShipPanels_A");
		t[1] = GM.resource_map().textures.at("ShipPanels_M");
		t[2] = GM.resource_map().textures.at("ShipPanels_R");
		t[3] = GM.resource_map().textures.at("ShipPanels_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Cliff
	{
		pbr.tiling_x = 8.0f;
		pbr.tiling_y = 8.0f;
		pbr.normal_map_intensity = 1.0f;
		Entity e = Entity::CreateEntity("Cliff", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 4.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Cliff_A");
		t[1] = GM.resource_map().textures.at("Cliff_M");
		t[2] = GM.resource_map().textures.at("Cliff_R");
		t[3] = GM.resource_map().textures.at("Cliff_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Granite
	{
		pbr.tiling_x = 2.0f;
		pbr.tiling_y = 2.0f;
		pbr.normal_map_intensity = 1.0f;
		Entity e = Entity::CreateEntity("Granite", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.translate(4.0f, 0.0f, 4.0f);
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Granite_A");
		t[1] = GM.resource_map().textures.at("Granite_M");
		t[2] = GM.resource_map().textures.at("Granite_R");
		t[3] = GM.resource_map().textures.at("Granite_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	// Foam
	{
		pbr.tiling_x = 2.0f;
		pbr.tiling_y = 2.0f;
		pbr.normal_map_intensity = 0.5f;
		Entity e = Entity::CreateEntity("Foam", EntityType::RENDERABLE);
		LTransform &tr = e.getComponent<LTransform>();
		tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 4.0f));
		Drawable &dw = e.getComponent<Drawable>();
		dw.mesh = GM.resource_map().meshes.at("MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(PbrData) / 4);
		THE_Texture t[4];
		t[0] = GM.resource_map().textures.at("Foam_A");
		t[1] = GM.resource_map().textures.at("Foam_M");
		t[2] = GM.resource_map().textures.at("Foam_R");
		t[3] = GM.resource_map().textures.at("Foam_N");
		THE_MaterialSetTexture(&dw.mat, t, 4);
	}

	THE_RenderCommand *rendops = THE_AllocateCommand();
	rendops->data.renderops.depth_test = 1;
	rendops->data.renderops.write_depth = 1;
	rendops->data.renderops.cull_face = THE_CULLFACE_BACK;
	rendops->data.renderops.blend = 1;
	rendops->data.renderops.sfactor = THE_BLENDFUNC_ONE;
	rendops->data.renderops.dfactor = THE_BLENDFUNC_ZERO;
	rendops->data.renderops.changed_mask = 0xFF; // Changed all
	rendops->execute = THE_RenderOptionsExecute;

	THE_RenderCommand *sky = THE_AllocateCommand();
	strcpy(sky->data.eqr_cube.in_path, "../assets/tex/env/helipad-env.hdr");
	sky->data.eqr_cube.out_cube = GM.resource_map().textures.at("Skybox");
	sky->data.eqr_cube.out_prefilt = GM.resource_map().textures.at("PrefilterSpec");
	sky->data.eqr_cube.out_lut = GM.resource_map().textures.at("LutMap");
	sky->execute = THE_EquirectToCubeExecute;
	rendops->next = sky;

	THE_RenderCommand *irradiance = THE_AllocateCommand();
	strcpy(irradiance->data.eqr_cube.in_path, "../assets/tex/env/helipad-dif.hdr");
	irradiance->data.eqr_cube.out_cube = GM.resource_map().textures.at("IrradianceEnv");
	irradiance->data.eqr_cube.out_prefilt = THE_UNINIT;
	irradiance->data.eqr_cube.out_lut = THE_UNINIT;
	irradiance->execute = THE_EquirectToCubeExecute;
	sky->next = irradiance;
	irradiance->next = NULL;

	THE_AddCommands(rendops);
}

void leep::GameLogic()
{
	LuaScripting::ExecuteScript("../assets/scripts/update.lua");
	GM.input().updateInput();
	CameraMovement(1.0f, 1.0f).executeSystem();
	UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
	UpdateSceneGraph().executeSystem();

	// Render commands
	THE_PbrSceneData pbr_sd;
	pbr_sd.view_projection = smat4_multiply(camera.view_mat, camera.proj_mat);
	pbr_sd.camera_position = THE_CameraPosition(&camera);
	pbr_sd.light_direction_intensity = sun_dir_intensity;

	THE_Material *full_screen_img = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
	THE_InitNewMaterial(full_screen_img);
	full_screen_img->type = THE_MT_FULL_SCREEN_IMAGE;
	THE_Texture fbtex = THE_CameraOutputColorTexture(&camera); // TODO: Revisar si deberia hacerse en aqui algo tan low level del renderer (algo como link camera to tex?)
	THE_MaterialSetFrameTexture(full_screen_img, &fbtex, 1);

	//THE_RenderCommand *fbuff = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	THE_RenderCommand* fbuff = THE_AllocateCommand();
	fbuff->data.usefb.fb = camera.fb;
	fbuff->data.usefb.def = 0;
	fbuff->execute = THE_UseFramebufferExecute;

	THE_RenderCommand *rops = THE_AllocateCommand();
	rops->data.renderops.blend = 1;
	rops->data.renderops.sfactor = THE_BLENDFUNC_ONE;
	rops->data.renderops.dfactor = THE_BLENDFUNC_ZERO;
	rops->data.renderops.depth_test = 1;
	rops->data.renderops.write_depth = 1;
	rops->data.renderops.cull_face = THE_CULLFACE_BACK;
	rops->data.renderops.changed_mask = 0xFF; // All changed
	rops->execute = THE_RenderOptionsExecute;
	fbuff->next = rops;

	THE_RenderCommand *clear = THE_AllocateCommand();
	clear->data.clear.bcolor = 1;
	clear->data.clear.bdepth = 1;
	clear->data.clear.bstencil = 0;
	clear->data.clear.color[0] = 0.2f;
	clear->data.clear.color[1] = 0.2f;
	clear->data.clear.color[2] = 0.2f;
	clear->data.clear.color[3] = 1.0f;
	clear->execute = THE_ClearExecute;
	rops->next = clear;

	THE_Texture scene_tex[3];
	scene_tex[0] = GM.resource_map().textures.at("LutMap");
	scene_tex[1] = GM.resource_map().textures.at("IrradianceEnv");
	scene_tex[2] = GM.resource_map().textures.at("PrefilterSpec");

	THE_RenderCommand *usemat = THE_AllocateCommand();
	usemat->data.usemat.mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
	THE_InitNewMaterial(usemat->data.usemat.mat);
	usemat->data.usemat.mat->type = THE_MT_PBR;
	THE_MaterialSetFrameData(usemat->data.usemat.mat, (float*)&pbr_sd, sizeof(PbrSceneData) / sizeof (float));
	THE_MaterialSetFrameTexture(usemat->data.usemat.mat, scene_tex, 3, 1);
	usemat->execute = THE_UseMaterialExecute;
	clear->next = usemat;
	usemat->next = NULL;

	THE_AddCommands(fbuff);

	Render(GM.scene().container(EntityType::RENDERABLE)).executeSystem();

	rops = THE_AllocateCommand();
	rops->data.renderops.cull_face = THE_CULLFACE_DISABLED;
	rops->data.renderops.changed_mask = THE_CULL_FACE_BIT;
	rops->execute = THE_RenderOptionsExecute;

	THE_RenderCommand *sky = THE_AllocateCommand();
	sky->data.skybox.cubemap = GM.resource_map().textures.at("Skybox");
	sky->execute = THE_SkyboxExecute;
	rops->next = sky;

	fbuff = THE_AllocateCommand();
	fbuff->data.usefb.def = 1;
	fbuff->execute = THE_UseFramebufferExecute;
	sky->next = fbuff;

	THE_RenderCommand *rops2 = THE_AllocateCommand();
	rops2->data.renderops.depth_test = 0;
	rops2->data.renderops.changed_mask = THE_DEPTH_TEST_BIT;
	rops2->execute = THE_RenderOptionsExecute;
	fbuff->next = rops2;

	clear = THE_AllocateCommand();
	clear->data.clear.bcolor = 1;
	clear->data.clear.bdepth = 0;
	clear->data.clear.bstencil = 0;
	clear->data.clear.color[0] = 1.0f;
	clear->data.clear.color[1] = 0.0f;
	clear->data.clear.color[2] = 0.0f;
	clear->data.clear.color[3] = 1.0f;
	clear->execute = THE_ClearExecute;
	rops2->next = clear;

	THE_RenderCommand *draw = THE_AllocateCommand();
	draw->data.draw.mesh = QUAD_MESH;
	draw->data.draw.mat = full_screen_img;
	draw->data.draw.inst_count = 1;
	draw->execute = THE_DrawExecute;
	clear->next = draw;
	draw->next = NULL;

	THE_AddCommands(rops);
}

void leep::GameClose()
{

}

int main(int argc, char **argv)
{
    leep::LeepConfig cnfg;
    leep::Init(cnfg);
    leep::LogicThread lt(leep::Logic);
    leep::GM.startFrameTimer();
    while (!leep::GM.window().windowShouldClose())
    {
#ifndef LEEP_SINGLE_THREAD
        lt.run();
        leep::RenderFrame();
        while (lt.running())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
#else
        leep::Logic();
        leep::RenderFrame();
#endif
        leep::ShowFrame();
    }
    leep::Close();

    return 0;
}
