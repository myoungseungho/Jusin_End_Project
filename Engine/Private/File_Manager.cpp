#include "..\Public\File_Manager.h"
#include <sstream>
#include <stdexcept>
#include <fstream>

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

HRESULT CFile_Manager::Save_Effects(wstring& FilePath, void* pArg)
{
	wofstream file(FilePath);
	if (!file.is_open()) {
		return E_FAIL;
	}

	vector<EFFECT_LAYER_DATA>* EffectDataList = reinterpret_cast<vector<EFFECT_LAYER_DATA>*>(pArg);

	// 레이어 데이터를 파일에 저장합니다.
	for (const auto& layerData : *EffectDataList) {
		file << L"[Layer]" << L"\n";
		file << L"LayerName: " << layerData.layerName << L"\n";
		file << L"Duration: " << layerData.duration << L"\n";
		file << L"TickPerSecond: " << layerData.tickPerSecond << L"\n";
		file << L"KeyFramesCount: " << layerData.keyFramesCount << L"\n\n";

		// 이펙트 데이터를 파일에 저장합니다.
		for (const auto& effectData : layerData.effects) {
			file << L"[Effect]" << L"\n";
			file << L"EffectName: " << effectData.effectName << L"\n";
			file << L"ModelName: " << effectData.modelName << L"\n";
			file << L"MaskTextureName: " << effectData.maskTextureName << L"\n";
			file << L"DiffuseTextureName: " << effectData.diffuseTextureName << L"\n";
			file << L"RenderIndex: " << effectData.renderIndex << L"\n";
			file << L"PassIndex: " << effectData.passIndex << L"\n";
			file << L"UniqueIndex: " << effectData.uniqueIndex << L"\n";
			file << L"Position: " << effectData.position.x << L" " << effectData.position.y << L" " << effectData.position.z << L"\n";
			file << L"Scale: " << effectData.scale.x << L" " << effectData.scale.y << L" " << effectData.scale.z << L"\n";
			file << L"Rotation: " << effectData.rotation.x << L" " << effectData.rotation.y << L" " << effectData.rotation.z << L"\n";
			file << L"IsNotPlaying: " << (effectData.isNotPlaying ? L"true" : L"false") << L"\n";
			file << L"IsLoop: " << (effectData.isLoop ? L"true" : L"false") << L"\n\n";

			// 키프레임 데이터를 파일에 저장합니다.
			for (const auto& keyFrameData : effectData.keyframes) {
				file << L"[KeyFrame]" << L"\n";
				file << L"KeyFrameNumber: " << keyFrameData.keyFrameNumber << L"\n";
				file << L"Position: " << keyFrameData.position.x << L" " << keyFrameData.position.y << L" " << keyFrameData.position.z << L"\n";
				file << L"Scale: " << keyFrameData.scale.x << L" " << keyFrameData.scale.y << L" " << keyFrameData.scale.z << L"\n";
				file << L"Rotation: " << keyFrameData.rotation.x << L" " << keyFrameData.rotation.y << L" " << keyFrameData.rotation.z << L"\n";
				file << L"CurTime: " << keyFrameData.curTime << L"\n";
				file << L"Duration: " << keyFrameData.duration << L"\n\n";
			}
		}
	}

	file.close();
	return S_OK;
}

void* CFile_Manager::Load_Effects(wstring& FilePath)
{
	vector<EFFECT_LAYER_DATA> effectDataList;

	wifstream file(FilePath);
	if (!file.is_open()) {
		throw runtime_error("Cannot open file for reading.");
	}

	wstring line;
	EFFECT_LAYER_DATA layerData;
	EFFECT_DATA effectData;
	EFFECT_KEYFRAME_DATA keyFrameData;

	while (getline(file, line)) {
		if (!line.empty()) {
			Read_Effects(line, layerData, effectData, keyFrameData);
		}
	}

	file.close();

	return &effectDataList;
}

HRESULT CFile_Manager::Read_Effects(wstring& Line, EFFECT_LAYER_DATA& LayerData, EFFECT_DATA& EffectData, EFFECT_KEYFRAME_DATA& KeyFrameData)
{
	wistringstream iss(Line);
	wstring key;
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		value.erase(0, value.find_first_not_of(L' '));
		value.erase(value.find_last_not_of(L' ') + 1);

		if (key == L"LayerName") LayerData.layerName = value;
		else if (key == L"Duration") LayerData.duration = stof(value);
		else if (key == L"TickPerSecond") LayerData.tickPerSecond = stof(value);
		else if (key == L"KeyFramesCount") LayerData.keyFramesCount = stoi(value);
		else if (key == L"EffectName") EffectData.effectName = value;
		else if (key == L"ModelName") EffectData.modelName = value;
		// Handle other effect attributes similarly...
		else if (key == L"KeyFrameNumber") KeyFrameData.keyFrameNumber = stoi(value);
		// Handle other keyframe attributes similarly...

		// Populate data structures when relevant sections end
		if (key == L"IsLoop") LayerData.effects.push_back(EffectData);
		if (key == L"KeyFrameNumber") EffectData.keyframes.push_back(KeyFrameData);

		return S_OK;
	}
	return E_FAIL;
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
