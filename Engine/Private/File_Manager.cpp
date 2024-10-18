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
	// 파일을 쓰기 모드로 열거나 없으면 새로 생성
	wofstream file(filename);
	if (!file.is_open()) {
		// 파일 생성에 실패한 경우 파일을 새로 생성하여 다시 시도
		file.open(filename, ios::out | ios::trunc);
		if (!file.is_open()) {
			// 파일 생성에 실패한 경우 오류 반환
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

	// 라인을 순차적으로 읽는다.
	while (getline(file, line)) {
		// 빈 라인이 아니라면
		if (!line.empty()) {
			// 각 라인 파싱하고 파싱 실패하면 실패 리턴
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
	// ':' 콜론을 기준으로 키와 값으로 문자열 분리
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		// 좌우 공백 제거 (C++11 이상)
		value.erase(0, value.find_first_not_of(L' ')); // 왼쪽 공백 제거
		value.erase(value.find_last_not_of(L' ') + 1); // 오른쪽 공백 제거

		if (key == L"IsParsing") {
			obj.isParsing = (value == L"true" || value == L"1");
			// 모든 값을 읽은 후 객체를 저장
			m_vecpFileData.push_back(obj);
			obj = FILEDATA(); // 객체 초기화
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
	//if (nullptr == pArg)
	//	return E_FAIL;

	//// pArg를 CameraSaveData로 캐스팅
	//CCamera::CameraSaveData* pSaveData = static_cast<CCamera::CameraSaveData*>(pArg);
	//if (nullptr == pSaveData)
	//	return E_FAIL;

	//const auto& vecVirtualCamera = pSaveData->vecVirtualCamera;
	//const auto& cameraIndexMap = pSaveData->cameraIndexMap;

	//// 파일 스트림 열기
	//std::wofstream file(filename);
	//if (!file.is_open())
	//{
	//	// 파일 생성 실패 처리
	//	return E_FAIL;
	//}

	//// 모든 가상 카메라 순회
	//for (size_t i = 0; i < vecVirtualCamera.size(); ++i)
	//{
	//	CCamera* pVirtualCamera = vecVirtualCamera[i];

	//	// 모델 ID와 스킬 ID를 얻기 위해 매핑된 값을 찾음
	//	_int modelID = -1;
	//	_int skillID = -1;

	//	for (const auto& pair : cameraIndexMap)
	//	{
	//		if (pair.second == i)
	//		{
	//			modelID = pair.first.first;
	//			skillID = pair.first.second;
	//			break;
	//		}
	//	}

	//	// 가상 카메라 섹션 시작
	//	file << L"[VirtualCamera]\n";
	//	file << L"ModelID: " << modelIDToString[modelID] << L"\n";
	//	file << L"SkillID: " << skillIDToString[skillID] << L"\n";

	//	const auto& vecPoints = pVirtualCamera->m_vecPoints;
	//	file << L"PointCount: " << vecPoints.size() << L"\n\n";

	//	// 각 CameraPoint 저장
	//	for (const auto& point : vecPoints)
	//	{
	//		file << L"[CameraPoint]\n";
	//		file << L"Position: " << point.position.x << L" " << point.position.y << L" " << point.position.z << L"\n";
	//		file << L"Rotation: " << point.rotation.x << L" " << point.rotation.y << L" " << point.rotation.z << L" " << point.rotation.w << L"\n";
	//		file << L"Duration: " << point.duration << L"\n";
	//		file << L"InterpolationType: " << point.interpolationType << L"\n";
	//		file << L"Damping: " << point.damping << L"\n";
	//		file << L"HasWorldFloat4x4: " << (point.hasWorldFloat4x4 ? 1 : 0) << L"\n\n";
	//	}
	//}

	//file.close();

	return S_OK;
}

vector<CameraData> CFile_Manager::Load_All_CameraPoints(const wstring& filename)
{
	vector<CameraData> cameraDataList;

	//// 파일 스트림 열기
	//std::wifstream file(filename);
	//if (!file.is_open()) {
	//	// 파일 열기 실패 처리
	//	return cameraDataList; // 빈 리스트 반환
	//}

	//std::wstring line;
	//CameraData currentCameraData;
	//bool insideVirtualCamera = false;

	//while (std::getline(file, line)) {
	//	// 공백 라인 무시
	//	if (line.empty())
	//		continue;

	//	if (line == L"[VirtualCamera]") {
	//		// 이전 VirtualCamera 데이터 저장
	//		if (insideVirtualCamera) {
	//			cameraDataList.push_back(currentCameraData);
	//		}

	//		// 새로운 CameraData 초기화
	//		currentCameraData = CameraData();
	//		currentCameraData.points.clear();

	//		// ModelID 읽기
	//		std::getline(file, line);
	//		size_t colonPos = line.find(L":");
	//		if (colonPos != std::wstring::npos) {
	//			size_t valueStart = colonPos + 1;
	//			valueStart = line.find_first_not_of(L" \t", valueStart);
	//			std::wstring modelIDStr = line.substr(valueStart);

	//			if (stringToModelID.find(modelIDStr) != stringToModelID.end()) {
	//				currentCameraData.modelID = stringToModelID[modelIDStr];
	//			}
	//			else {
	//				currentCameraData.modelID = -1; // 기본값 설정
	//			}
	//		}
	//		else {
	//			currentCameraData.modelID = -1; // 기본값 설정
	//		}

	//		// SkillID 읽기
	//		std::getline(file, line);
	//		colonPos = line.find(L":");
	//		if (colonPos != std::wstring::npos) {
	//			size_t valueStart = colonPos + 1;
	//			valueStart = line.find_first_not_of(L" \t", valueStart);
	//			std::wstring skillIDStr = line.substr(valueStart);

	//			if (stringToSkillID.find(skillIDStr) != stringToSkillID.end()) {
	//				currentCameraData.skillID = stringToSkillID[skillIDStr];
	//			}
	//			else {
	//				currentCameraData.skillID = -1; // 기본값 설정
	//			}
	//		}
	//		else {
	//			currentCameraData.skillID = -1; // 기본값 설정
	//		}

	//		// PointCount 읽기
	//		std::getline(file, line);
	//		colonPos = line.find(L":");
	//		if (colonPos != std::wstring::npos) {
	//			size_t valueStart = colonPos + 1;
	//			valueStart = line.find_first_not_of(L" \t", valueStart);
	//			size_t pointCount = std::stoul(line.substr(valueStart));
	//		}
	//		else {
	//			// 기본값 또는 에러 처리
	//		}

	//		// 다음 라인 (빈 줄) 건너뛰기
	//		std::getline(file, line);

	//		insideVirtualCamera = true;
	//	}
	//	else if (line == L"[CameraPoint]") {
	//		CameraPoint point;

	//		// Position 읽기
	//		std::getline(file, line);
	//		{
	//			std::wistringstream iss(line.substr(line.find(L":") + 1));
	//			iss >> point.position.x >> point.position.y >> point.position.z;
	//		}

	//		// Rotation 읽기
	//		std::getline(file, line);
	//		{
	//			std::wistringstream iss(line.substr(line.find(L":") + 1));
	//			iss >> point.rotation.x >> point.rotation.y >> point.rotation.z >> point.rotation.w;
	//		}

	//		// Duration 읽기
	//		std::getline(file, line);
	//		point.duration = std::stof(line.substr(line.find(L":") + 1));

	//		// InterpolationType 읽기
	//		std::getline(file, line);
	//		point.interpolationType = std::stoi(line.substr(line.find(L":") + 1));

	//		// Damping 읽기
	//		std::getline(file, line);
	//		point.damping = std::stof(line.substr(line.find(L":") + 1));

	//		// HasWorldFloat4x4 읽기
	//		std::getline(file, line);
	//		point.hasWorldFloat4x4 = stoi(line.substr(line.find(L":") + 1));

	//		// 포인트 추가
	//		currentCameraData.points.push_back(point);

	//		// 다음 라인 (빈 줄) 건너뛰기
	//		std::getline(file, line);
	//	}
	//	else {
	//		// 다른 경우는 무시
	//	}
	//}

	//// 파일의 끝에 도달했을 때 마지막 VirtualCamera 데이터 저장
	//if (insideVirtualCamera) {
	//	cameraDataList.push_back(currentCameraData);
	//}

	//file.close();
	//return cameraDataList;

	return cameraDataList;
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
