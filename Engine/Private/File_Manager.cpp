#include "..\Public\File_Manager.h"
#include <sstream>
#include <stdexcept>
#include <fstream>
#include "Camera.h"
CFile_Manager::CFile_Manager()
{
}

HRESULT CFile_Manager::Initialize()
{
	return S_OK;
}

HRESULT CFile_Manager::SaveObjects(const wstring& filename, void* pArg)
{
	// ������ ���� ���� ���ų� ������ ���� ����
	wofstream file(filename);
	if (!file.is_open()) {
		// ���� ������ ������ ��� ������ ���� �����Ͽ� �ٽ� �õ�
		file.open(filename, ios::out | ios::trunc);
		if (!file.is_open()) {
			// ���� ������ ������ ��� ���� ��ȯ
			return E_FAIL;
		}
	}

	vector<FILEDATA>* pvecFileData = reinterpret_cast<vector<FILEDATA>*>(pArg);

	for (const auto& data : *pvecFileData) {
		file << L"LayerName: " << data.layerName << L"\n";
		file << L"PrototypeTag: " << data.prototypeTag << L"\n";
		file << L"LevelIndex: " << data.levelIndex << L"\n";
		file << L"Position: " << data.position.x << L" " << data.position.y << L" " << data.position.z << L"\n";
		file << L"RightRotation: " << data.rightRotation.x << L" " << data.rightRotation.y << L" " << data.rightRotation.z << L"\n";
		file << L"UpRotation: " << data.upRotation.x << L" " << data.upRotation.y << L" " << data.upRotation.z << L"\n";
		file << L"LookRotation: " << data.lookRotation.x << L" " << data.lookRotation.y << L" " << data.lookRotation.z << L"\n";
		file << L"Scale: " << data.scale.x << L" " << data.scale.y << L" " << data.scale.z << L"\n";
		file << L"IsParsing: " << data.isParsing << L"\n\n";
	}

	file.close();

	return S_OK;
}

void* CFile_Manager::LoadObjects(const wstring& filename)
{
	m_vecpFileData.clear();

	wifstream file(filename);
	if (!file.is_open()) {
		throw runtime_error("Cannot open file for reading.");
		return nullptr;
	}

	wstring line;
	FILEDATA data;

	// ������ ���������� �д´�.
	while (getline(file, line)) {
		// �� ������ �ƴ϶��
		if (!line.empty()) {
			// �� ���� �Ľ��ϰ� �Ľ� �����ϸ� ���� ����
			if (ParseLine(line, data) == E_FAIL) {
				file.close();
				return nullptr;
			}
		}
	}

	file.close();
	return &m_vecpFileData;
}

HRESULT CFile_Manager::ParseLine(const wstring& line, FILEDATA& obj) {
	wistringstream iss(line);
	wstring key;
	// ':' �ݷ��� �������� Ű�� ������ ���ڿ� �и�
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		// �¿� ���� ���� (C++11 �̻�)
		value.erase(0, value.find_first_not_of(L' ')); // ���� ���� ����
		value.erase(value.find_last_not_of(L' ') + 1); // ������ ���� ����

		if (key == L"IsParsing") {
			obj.isParsing = (value == L"true" || value == L"1");
			// ��� ���� ���� �� ��ü�� ����
			m_vecpFileData.push_back(obj);
			obj = FILEDATA(); // ��ü �ʱ�ȭ
		}
		else if (key == L"LayerName") {
			obj.layerName = value;
		}
		else if (key == L"PrototypeTag") {
			obj.prototypeTag = value;
		}
		else if (key == L"LevelIndex") {
			obj.levelIndex = stoi(value);
		}
		else if (key == L"Position") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.position.x = a;
			obj.position.y = b;
			obj.position.z = c;
		}
		else if (key == L"RightRotation") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.rightRotation.x = a;
			obj.rightRotation.y = b;
			obj.rightRotation.z = c;
		}
		else if (key == L"UpRotation") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.upRotation.x = a;
			obj.upRotation.y = b;
			obj.upRotation.z = c;
		}
		else if (key == L"LookRotation") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.lookRotation.x = a;
			obj.lookRotation.y = b;
			obj.lookRotation.z = c;
		}
		else if (key == L"Scale") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.scale.x = a;
			obj.scale.y = b;
			obj.scale.z = c;
		}

		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CFile_Manager::Save_All_CameraPoints(const wstring& filename, void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	// pArg�� CameraSaveData�� ĳ����
	CCamera::CameraSaveData* pSaveData = static_cast<CCamera::CameraSaveData*>(pArg);
	if (nullptr == pSaveData)
		return E_FAIL;

	const auto& vecVirtualCamera = pSaveData->vecVirtualCamera;
	const auto& cameraIndexMap = pSaveData->cameraIndexMap;

	// ���� ��Ʈ�� ����
	std::wofstream file(filename);
	if (!file.is_open())
	{
		// ���� ���� ���� ó��
		return E_FAIL;
	}

	// ��� ���� ī�޶� ��ȸ
	for (size_t i = 0; i < vecVirtualCamera.size(); ++i)
	{
		CCamera* pVirtualCamera = vecVirtualCamera[i];

		// �� ID�� ��ų ID�� ��� ���� ���ε� ���� ã��
		_int modelID = -1;
		_int skillID = -1;

		for (const auto& pair : cameraIndexMap)
		{
			if (pair.second == i)
			{
				modelID = pair.first.first;
				skillID = pair.first.second;
				break;
			}
		}

		// ���� ī�޶� ���� ����
		file << L"[VirtualCamera]\n";
		file << L"ModelID: " << static_cast<int>(modelID) << L"\n";
		file << L"SkillID: " << static_cast<int>(skillID) << L"\n";

		const auto& vecPoints = pVirtualCamera->m_vecPoints;
		file << L"PointCount: " << vecPoints.size() << L"\n\n";

		// �� CameraPoint ����
		for (const auto& point : vecPoints)
		{
			file << L"[CameraPoint]\n";
			file << L"Position: " << point.position.x << L" " << point.position.y << L" " << point.position.z << L"\n";
			file << L"Rotation: " << point.rotation.x << L" " << point.rotation.y << L" " << point.rotation.z << L" " << point.rotation.w << L"\n";
			file << L"Duration: " << point.duration << L"\n";
			file << L"InterpolationType: " << static_cast<int>(point.interpolationType) << L"\n";
			file << L"Damping: " << point.damping << L"\n\n";
		}
	}

	file.close();

	return S_OK;
}

HRESULT CFile_Manager::Load_All_CameraPoints(const wstring& filename)
{
	//// ���� ��Ʈ�� ����
	//std::wifstream file(filename);
	//if (!file.is_open()) {
	//	// ���� ���� ���� ó��
	//	return E_FAIL;
	//}

	//std::wstring line;
	//CCamera* pCurrentCamera = nullptr;
	//_int modelID = -1;
	//_int skillID = -1;

	//while (std::getline(file, line)) {
	//	// ���� ���� ����
	//	if (line.empty())
	//		continue;

	//	if (line == L"[VirtualCamera]") {
	//		// ModelID, SkillID, PointCount �ʱ�ȭ
	//		modelID = -1;
	//		skillID = -1;
	//		size_t pointCount = 0;

	//		// ���� 3���� ���� �б�
	//		std::getline(file, line); // ModelID
	//		modelID = std::stoi(line.substr(line.find(L":") + 1));

	//		std::getline(file, line); // SkillID
	//		skillID = std::stoi(line.substr(line.find(L":") + 1));

	//		std::getline(file, line); // PointCount
	//		pointCount = std::stoul(line.substr(line.find(L":") + 1));

	//		// ���� ī�޶� ã��
	//		int cameraIndex = GetCameraIndex(modelID, skillID);
	//		if (cameraIndex >= 0 && cameraIndex < m_pMainCamera->m_vecVirtualCamera.size()) {
	//			pCurrentCamera = m_pMainCamera->m_vecVirtualCamera[cameraIndex];
	//			// ���� ����Ʈ �ʱ�ȭ
	//			pCurrentCamera->m_vecPoints.clear();
	//		}
	//		else {
	//			// ���� ī�޶� ã�� ���� ��� ó�� (�ʿ信 ���� ����)
	//			pCurrentCamera = nullptr;
	//		}

	//		// ���� ���� (�� ��) �ǳʶٱ�
	//		std::getline(file, line);
	//	}
	//	else if (line == L"[CameraPoint]") {
	//		if (pCurrentCamera == nullptr)
	//			continue; // ���� ī�޶� ���� ��� �ǳʶ�

	//		CCamera::CameraPoint point;

	//		// Position �б�
	//		std::getline(file, line);
	//		{
	//			std::wistringstream iss(line.substr(line.find(L":") + 1));
	//			iss >> point.position.x >> point.position.y >> point.position.z;
	//		}

	//		// Rotation �б�
	//		std::getline(file, line);
	//		{
	//			std::wistringstream iss(line.substr(line.find(L":") + 1));
	//			iss >> point.rotation.x >> point.rotation.y >> point.rotation.z >> point.rotation.w;
	//		}

	//		// Duration �б�
	//		std::getline(file, line);
	//		point.duration = std::stof(line.substr(line.find(L":") + 1));

	//		// InterpolationType �б�
	//		std::getline(file, line);
	//		point.interpolationType = static_cast<CCamera::InterpolationType>(std::stoi(line.substr(line.find(L":") + 1)));

	//		// Damping �б�
	//		std::getline(file, line);
	//		point.damping = std::stof(line.substr(line.find(L":") + 1));

	//		// ����Ʈ �߰�
	//		pCurrentCamera->m_vecPoints.push_back(point);

	//		// ���� ���� (�� ��) �ǳʶٱ�
	//		std::getline(file, line);
	//	}
	//}

	//file.close();
	return S_OK;
}


CFile_Manager* CFile_Manager::Create()
{
	CFile_Manager* pInstance = new CFile_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFileManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFile_Manager::Free()
{
	__super::Free();
}
