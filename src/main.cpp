#include "leep.h"

#define LEEP_SINGLE_THREAD

void leep::GameInit()
{
	THE_ResourceMap *rm = GM.resource_map();
	const String tp = "../assets/tex/";
	THE_ResourceMapAddMesh(rm, "MatBall", "../assets/obj/matball-n.obj");
	THE_ResourceMapAddTexture(rm, "Skybox", 1024, 1024, THE_TEX_ENVIRONMENT);
	THE_ResourceMapAddTexture(rm, "Irradian", 1024, 1024, THE_TEX_ENVIRONMENT);
	THE_ResourceMapAddTexture(rm, "Prefilte", 128, 128, THE_TEX_PREFILTER_ENVIRONMENT);
	THE_ResourceMapAddTexture(rm, "LutMap", 512, 512, THE_TEX_LUT);

	THE_ResourceMapAddTexture(rm, "Gold_A" ,(tp + "celtic-gold/celtic-gold_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Gold_N" ,(tp + "celtic-gold/celtic-gold_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Gold_M" ,(tp + "celtic-gold/celtic-gold_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Gold_R" ,(tp + "celtic-gold/celtic-gold_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Peel_A", (tp + "peeling/peeling_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Peel_N", (tp + "peeling/peeling_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Peel_M", (tp + "peeling/peeling_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Peel_R", (tp + "peeling/peeling_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Rust_A", (tp + "rusted/rusted_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Rust_N", (tp + "rusted/rusted_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Rust_M", (tp + "rusted/rusted_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Rust_R", (tp + "rusted/rusted_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Tiles_A", (tp + "tiles/tiles_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Tiles_N", (tp + "tiles/tiles_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Tiles_M", (tp + "tiles/tiles_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Tiles_R", (tp + "tiles/tiles_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Future_A", (tp + "ship-panels/ship-panels_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Future_N", (tp + "ship-panels/ship-panels_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Future_M", (tp + "ship-panels/ship-panels_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Future_R", (tp + "ship-panels/ship-panels_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Shore_A", (tp + "shore/shore_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Shore_N", (tp + "shore/shore_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Shore_M", (tp + "shore/shore_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Shore_R", (tp + "shore/shore_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Cliff_A", (tp + "cliff/cliff_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Cliff_N", (tp + "cliff/cliff_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Cliff_M", (tp + "cliff/cliff_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Cliff_R", (tp + "cliff/cliff_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Granit_A", (tp + "granite/granite_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Granit_N", (tp + "granite/granite_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Granit_M", (tp + "granite/granite_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Granit_R", (tp + "granite/granite_R.png").c_str(), THE_TEX_R);

	THE_ResourceMapAddTexture(rm, "Foam_A", (tp + "foam/foam_A.png").c_str(), THE_TEX_SRGB);
	THE_ResourceMapAddTexture(rm, "Foam_N", (tp + "foam/foam_N.png").c_str(), THE_TEX_RGB);
	THE_ResourceMapAddTexture(rm, "Foam_M", (tp + "foam/foam_M.png").c_str(), THE_TEX_R);
	THE_ResourceMapAddTexture(rm, "Foam_R", (tp + "foam/foam_R.png").c_str(), THE_TEX_R);

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
		//dw.mesh = rm->meshes.at("MatBall");
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Gold_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Gold_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Gold_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Gold_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Shore_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Shore_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Shore_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Shore_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Peel_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Peel_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Peel_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Peel_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Rust_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Rust_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Rust_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Rust_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Tiles_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Tiles_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Tiles_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Tiles_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Future_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Future_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Future_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Future_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Cliff_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Cliff_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Cliff_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Cliff_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Granit_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Granit_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Granit_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Granit_N");
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
		dw.mesh = THE_ResourceMapGetMesh(rm, "MatBall");
		dw.mat.type = THE_MT_PBR;
		THE_MaterialSetData(&dw.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
		THE_Texture t[4];
		t[0] = THE_ResourceMapGetTexture(rm, "Foam_A");
		t[1] = THE_ResourceMapGetTexture(rm, "Foam_M");
		t[2] = THE_ResourceMapGetTexture(rm, "Foam_R");
		t[3] = THE_ResourceMapGetTexture(rm, "Foam_N");
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
	sky->data.eqr_cube.out_cube = THE_ResourceMapGetTexture(rm, "Skybox"); //GM.resource_map()->textures.at("Skybox");
	sky->data.eqr_cube.out_prefilt = THE_ResourceMapGetTexture(rm, "Prefilte");  //GM.resource_map()->textures.at("PrefilterSpec");
	sky->data.eqr_cube.out_lut = THE_ResourceMapGetTexture(rm, "LutMap");  //GM.resource_map()->textures.at("LutMap");
	sky->execute = THE_EquirectToCubeExecute;
	rendops->next = sky;

	THE_RenderCommand *irradiance = THE_AllocateCommand();
	strcpy(irradiance->data.eqr_cube.in_path, "../assets/tex/env/helipad-dif.hdr");
	irradiance->data.eqr_cube.out_cube = THE_ResourceMapGetTexture(rm, "Irradian");  //GM.resource_map()->textures.at("IrradianceEnv");
	irradiance->data.eqr_cube.out_prefilt = THE_UNINIT;
	irradiance->data.eqr_cube.out_lut = THE_UNINIT;
	irradiance->execute = THE_EquirectToCubeExecute;
	sky->next = irradiance;
	irradiance->next = NULL;

	THE_AddCommands(rendops);
}

void leep::GameLogic()
{
	THE_ScriptingExecute("../assets/scripts/update.lua");
	THE_InputUpdate();
	CameraMovement(1.0f, 1.0f).executeSystem();
	UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
	UpdateSceneGraph().executeSystem();

	// Render commands
	THE_PbrSceneData pbr_sd;
	pbr_sd.view_projection = smat4_multiply(camera.proj_mat, camera.view_mat);
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
	scene_tex[0] = THE_ResourceMapGetTexture(GM.resource_map(), "LutMap");  //GM.resource_map()->textures.at("LutMap");
	scene_tex[1] = THE_ResourceMapGetTexture(GM.resource_map(), "Irradian"); //GM.resource_map()->textures.at("IrradianceEnv");
	scene_tex[2] = THE_ResourceMapGetTexture(GM.resource_map(), "Prefilte"); //GM.resource_map()->textures.at("PrefilterSpec");

	THE_RenderCommand *usemat = THE_AllocateCommand();
	usemat->data.usemat.mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
	THE_InitNewMaterial(usemat->data.usemat.mat);
	usemat->data.usemat.mat->type = THE_MT_PBR;
	THE_MaterialSetFrameData(usemat->data.usemat.mat, (float*)&pbr_sd, sizeof(THE_PbrSceneData) / sizeof (float));
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
	sky->data.skybox.cubemap = THE_ResourceMapGetTexture(GM.resource_map(), "Skybox"); // GM.resource_map()->textures.at("Skybox");
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
    THE_Config cnfg;
    leep::Init(cnfg);
    leep::LogicThread lt(leep::Logic);
    leep::GM.startFrameTimer();
    while (!THE_WindowShouldClose())
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
