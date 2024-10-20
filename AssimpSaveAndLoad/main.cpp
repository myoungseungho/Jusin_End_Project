#include "ModelConverter.h"

#define DATAFILEPATH "../Client/Bin/ModelData/Eff/TestModel/"

int main() {
	ModelConverter converter;

	vector<pair<string, ModelType>> modelPaths = {
		{"Models/cmn_SmokeModels0101.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0102.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0103.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0104.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0105.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0106.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0107.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0108.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0109.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0110.fbx", TYPE_NONANIM},

		{"Models/cmn_SmokeModels0111.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0112.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0201.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0202.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0203.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0204.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0205.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0206.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0301.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0302.fbx", TYPE_NONANIM},

		{"Models/cmn_SmokeModels0303.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0304.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0305.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0306.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0307.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0308.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0309.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0310.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0311.fbx", TYPE_NONANIM},
		{"Models/cmn_SmokeModels0312.fbx", TYPE_NONANIM},
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
