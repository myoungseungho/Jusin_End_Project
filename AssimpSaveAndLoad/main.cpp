#include "ModelConverter.h"

#define DATAFILEPATH "../Client/Bin/Resources/Map/Volcano/"

int main() {
	ModelConverter converter;
	
	vector<pair<string, ModelType>> modelPaths = {
		{"Models/vo_cliff_back.fbx", TYPE_NONANIM},
		{"Models/vo_cliff_far.fbx", TYPE_NONANIM},
		{"Models/vo_cliff01.fbx", TYPE_NONANIM},
		{"Models/vo_cliff02.fbx", TYPE_NONANIM},
		{"Models/vo_cliff03.fbx", TYPE_NONANIM},
		{"Models/vo_cliff04.fbx", TYPE_NONANIM},
		{"Models/vo_ground00.fbx", TYPE_NONANIM},
		{"Models/vo_ground01.fbx", TYPE_NONANIM},
		{"Models/vo_island01.fbx", TYPE_NONANIM},
		{"Models/vo_island02.fbx", TYPE_NONANIM},
		{"Models/vo_lava_fall.fbx", TYPE_NONANIM},
		{"Models/vo_lava_ground.fbx", TYPE_NONANIM},
		{"Models/vo_lava_pool.fbx", TYPE_NONANIM},
		{"Models/vo_mountain.fbx", TYPE_NONANIM},
		{"Models/vo_skycloud.fbx", TYPE_NONANIM},
		{"Models/vo_smoke03.fbx", TYPE_NONANIM}
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
