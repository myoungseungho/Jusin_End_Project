#include "ModelConverter.h"

#define DATAFILEPATH "../Client/Bin/Resources/Map/Space/"

int main() {
	ModelConverter converter;

	vector<pair<string, ModelType>> modelPaths = {
		{"Models/BRRock_1.fbx", TYPE_NONANIM},
		{"Models/Meteo_1.fbx", TYPE_NONANIM},
		{"Models/Meteo_2.fbx", TYPE_NONANIM},
		{"Models/Meteo_3.fbx", TYPE_NONANIM},
		{"Models/Rock_1.fbx", TYPE_NONANIM},
		{"Models/Rock_2.fbx", TYPE_NONANIM},
		{"Models/Rock_3.fbx", TYPE_NONANIM}

	};

	bool printVertices = false; // Vertex ������ ������� ���θ� �����ϴ� �÷���

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
		vector<BoneData> bones; // �� �����͸� ������ ����
		converter.FillModelData(converter.GetScene(), header, bones, meshes, materials, animations, printVertices);

		if (!converter.SaveModelToBinary(binFilePath, header, meshes, materials, animations, bones, printVertices)) {
			std::cerr << "Failed to save model to binary: " << binFilePath << std::endl;
			continue;
		}

		ModelHeader loadedHeader;
		vector<MeshData> loadedMeshes;
		vector<MaterialData> loadedMaterials;
		vector<AnimationData> loadedAnimations;
		vector<BoneData> loadedBones; // �ε�� �� �����͸� ������ ����
		if (!converter.LoadModelFromBinary(binFilePath, loadedHeader, loadedMeshes, loadedMaterials, loadedAnimations, loadedBones)) {
			std::cerr << "Failed to load model from binary: " << binFilePath << std::endl;
			continue;
		}

		std::cout << "Model processed successfully: " << modelPath << std::endl;
	}

	return 0;
}
