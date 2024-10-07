#include "ModelConverter.h"

#define DATAFILEPATH "../Client/Bin/ModelData/Eff/Model/"

int main() {
	ModelConverter converter;

	vector<pair<string, ModelType>> modelPaths = {
		{"Models/bun_cookie.fbx", TYPE_NONANIM},
		{"Models/bun201_eff00.fbx", TYPE_NONANIM},
		{"Models/bun203_eff00.fbx", TYPE_NONANIM},
		{"Models/bun204_eff00.fbx", TYPE_NONANIM},
		{"Models/bun231_eff00.fbx", TYPE_NONANIM},

		{"Models/bun232_eff00.fbx", TYPE_NONANIM},
		{"Models/bun261_eff00.fbx", TYPE_NONANIM},
		{"Models/bun301_eff00.fbx", TYPE_NONANIM},
		{"Models/bun403_BindLoop.fbx", TYPE_NONANIM},
		{"Models/bun403_BindStart.fbx", TYPE_NONANIM}, //10


		{"Models/bun403_jizoku.fbx", TYPE_NONANIM},
		{"Models/cmn_AuraBall.fbx", TYPE_NONANIM},
		{"Models/cmn_BeamCore.fbx", TYPE_NONANIM},
		{"Models/cmn_BeamCorePattern.fbx", TYPE_NONANIM},
		{"Models/cmn_bodyaura00.fbx", TYPE_NONANIM},

		{"Models/cmn_crossplane00.fbx", TYPE_NONANIM},
		{"Models/cmn_crossplane01.fbx", TYPE_NONANIM},
		{"Models/cmn_crossplane02.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinder00.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinder01.fbx", TYPE_NONANIM}, //20


		{"Models/cmn_cylinder01_ura.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinder02.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinder03.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinder04.fbx", TYPE_NONANIM},
		{"Models/cmn_cylinderBeam00.fbx", TYPE_NONANIM},

		{"Models/cmn_cylinderBeam01.fbx", TYPE_NONANIM},
		{"Models/cmn_futtobiBGpoint.fbx", TYPE_NONANIM},
		{"Models/cmn_futtobiBGtemprate.fbx", TYPE_NONANIM},
		{"Models/cmn_GrassModel.fbx", TYPE_NONANIM},
		{"Models/cmn_halfsphere00.fbx", TYPE_NONANIM}, //30


		{"Models/cmn_halfsphere01.fbx", TYPE_NONANIM},
		{"Models/cmn_hibana00.fbx", TYPE_NONANIM},
		{"Models/cmn_kakusanaura.fbx", TYPE_NONANIM},
		{"Models/cmn_kidan00.fbx", TYPE_NONANIM},
		{"Models/cmn_KidanTrail.fbx", TYPE_NONANIM},

		{"Models/cmn_pivot_plane01.fbx", TYPE_NONANIM},
		{"Models/cmn_povot_plane00.fbx", TYPE_NONANIM},
		{"Models/cmn_puffycylinder.fbx", TYPE_NONANIM},
		{"Models/cmn_puffyplane.fbx", TYPE_NONANIM},
		{"Models/cmn_puffyplane_harf.fbx", TYPE_NONANIM},//40


		{"Models/cmn_rock04.fbx", TYPE_NONANIM},
		{"Models/cmn_rock05.fbx", TYPE_NONANIM},
		{"Models/cmn_rubble01.fbx", TYPE_NONANIM},
		{"Models/cmn_saiyanAura.fbx", TYPE_NONANIM},
		{"Models/cmn_screw.fbx", TYPE_NONANIM},

		{"Models/cmn_screw2.fbx", TYPE_NONANIM},
		{"Models/cmn_screw3.fbx", TYPE_NONANIM},
		{"Models/cmn_shenlongAura00.fbx", TYPE_NONANIM},
		{"Models/cmn_shenlongAura01.fbx", TYPE_NONANIM},
		{"Models/cmn_shenlongAura02.fbx", TYPE_NONANIM},//50


		{"Models/cmn_SmokeModels00.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels01.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels02.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels03.fbx", TYPE_NONANIM},
		{"Models/cmn_sphere00.fbx", TYPE_NONANIM},

		{"Models/cmn_sphere00_inv.fbx", TYPE_NONANIM},
		{"Models/cmn_sphere00_inv2.fbx", TYPE_NONANIM},
		{"Models/cmn_stonemodel00.fbx", TYPE_NONANIM},
		{"Models/cmn_strike00.fbx", TYPE_NONANIM},
		{"Models/cmn_TestStrike.fbx", TYPE_NONANIM},//60


		{"Models/cmn_WindModel00.fbx", TYPE_NONANIM},
		{"Models/gks_cookie.fbx", TYPE_NONANIM},
		{"Models/gks262_eff00.fbx", TYPE_NONANIM},
		{"Models/gks262_eff01.fbx", TYPE_NONANIM},
		{"Models/gks262_eff10.fbx", TYPE_NONANIM},

		{"Models/gks301_eff00.fbx", TYPE_NONANIM},
		{"Models/gks404_eff00.fbx", TYPE_NONANIM},
		{"Models/gks404_eff00_sub.fbx", TYPE_NONANIM},
		{"Models/gks404_eff00_sub2.fbx", TYPE_NONANIM},
		{"Models/gks406_eff00.fbx", TYPE_NONANIM},//70


		{"Models/gks406_eff01.fbx", TYPE_NONANIM},
		{"Models/gks406_eff02.fbx", TYPE_NONANIM},
		{"Models/gks406_eff03.fbx", TYPE_NONANIM},
		{"Models/gks430_tameaura_00.fbx", TYPE_NONANIM},
		{"Models/gks431_kaijo.fbx", TYPE_NONANIM},

		{"Models/gks432_coremodel.fbx", TYPE_NONANIM},
		{"Models/gks432_coremodel2.fbx", TYPE_NONANIM},
		{"Models/gks806_sphere00.fbx", TYPE_NONANIM},
		{ "Models/gks808_Sphere.fbx", TYPE_NONANIM },
		{ "Models/gks808_Sphere_inv.fbx", TYPE_NONANIM },//80


		{ "Models/gks814_fistaura.fbx", TYPE_NONANIM },
		{ "Models/htn_cookie.fbx", TYPE_NONANIM },
		{ "Models/htn_halfsphere01.fbx", TYPE_NONANIM },
		{ "Models/htn_plane01.fbx", TYPE_NONANIM },
		{ "Models/htn_plane03.fbx", TYPE_NONANIM },

		{ "Models/htn301_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn403_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn403_eff01_sub.fbx", TYPE_NONANIM },
		{ "Models/htn405_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn405_eff01_sub.fbx", TYPE_NONANIM },//90


		{ "Models/htn406_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn406_eff01_sub.fbx", TYPE_NONANIM },
		{ "Models/htn407_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn407_eff02.fbx", TYPE_NONANIM },
		{ "Models/htn408_eff01.fbx", TYPE_NONANIM },

		{ "Models/htn408_eff02.fbx", TYPE_NONANIM },
		{ "Models/htn432_eff01.fbx", TYPE_NONANIM },
		{ "Models/htn432_eff01_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_a01.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_a02.fbx", TYPE_NONANIM },//100



		{ "Models/htn600cs_eff_hibi_a03.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_a04.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_b01.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_b02.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_b03.fbx", TYPE_NONANIM },

		{ "Models/htn600cs_eff_hibi_b03_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_b04.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_b04_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_c01.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_c01_sub.fbx", TYPE_NONANIM },//110


		{ "Models/htn600cs_eff_hibi_c02.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_c02_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_c03.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_c04.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_d01.fbx", TYPE_NONANIM },

		{ "Models/htn600cs_eff_hibi_d01_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_d02.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_d02_sub.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_d03.fbx", TYPE_NONANIM },
		{ "Models/htn600cs_eff_hibi_d04.fbx", TYPE_NONANIM },//120


		{ "Models/ton_cookie.fbx", TYPE_NONANIM },
		{ "Models/ton_kidan01.fbx", TYPE_NONANIM },
		{ "Models/ton205_eff1.fbx", TYPE_NONANIM },
		{ "Models/ton432_eff00.fbx", TYPE_NONANIM },
		{ "Models/ton433_sphere01.fbx", TYPE_NONANIM },

		{ "Models/cmn_rock04.fbx", TYPE_NONANIM },
		{ "Models/cmn_puffyplane2.fbx", TYPE_NONANIM },
		{ "Models/cmn_rock00.fbx", TYPE_NONANIM },
		{ "Models/cmn_rock01.fbx", TYPE_NONANIM },
		{ "Models/cmn_rock02.fbx", TYPE_NONANIM },//130

	};

	bool printVertices = false; // Vertex 정보를 출력할지 여부를 결정하는 플래그

	for (const auto& modelInfo : modelPaths) {
		const auto& modelPath = modelInfo.first;
		const auto& modelType = modelInfo.second;

		std::string fileName = modelPath.substr(modelPath.find_last_of("/") + 1);
		std::string binFilePath = std::string(DATAFILEPATH) + fileName.substr(0, fileName.find_last_of(".")) + ".bin";

		if (!converter.LoadModel(modelPath, modelType)) {
			std::cerr << "Failed to load model: " << modelPath << std::endl;
			continue;
		}

		ModelHeader header;
		vector<MeshData> meshes;
		vector<MaterialData> materials;
		vector<AnimationData> animations;
		vector<BoneData> bones; // 뼈 데이터를 저장할 벡터
		converter.FillModelData(converter.GetScene(), header, bones, meshes, materials, animations, printVertices);

		if (!converter.SaveModelToBinary(binFilePath, header, meshes, materials, animations, bones, printVertices)) {
			std::cerr << "Failed to save model to binary: " << binFilePath << std::endl;
			continue;
		}

		ModelHeader loadedHeader;
		vector<MeshData> loadedMeshes;
		vector<MaterialData> loadedMaterials;
		vector<AnimationData> loadedAnimations;
		vector<BoneData> loadedBones; // 로드된 뼈 데이터를 저장할 벡터
		if (!converter.LoadModelFromBinary(binFilePath, loadedHeader, loadedMeshes, loadedMaterials, loadedAnimations, loadedBones)) {
			std::cerr << "Failed to load model from binary: " << binFilePath << std::endl;
			continue;
		}

		std::cout << "Model processed successfully: " << modelPath << std::endl;
	}

	return 0;
}
