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
	if (nullptr == pArg)
		return E_FAIL;

	wofstream file(filename);
	if (!file.is_open())
	{
		// 파일 생성 실패 처리
		return E_FAIL;
	}

	CameraSaveData* saveData = static_cast<CameraSaveData*>(pArg);

	// 각 모델 순회
	for (const auto& model : saveData->models)
	{
		file << L"[VirtualCamera]\n";
		auto modelIt = modelIDToString.find(model.modelID);
		if (modelIt != modelIDToString.end())
		{
			file << L"ModelID: " << modelIt->second << L"\n\n";
		}
		else
		{
			file << L"ModelID: " << L"Unknown" << L"\n\n";
		}

		// 각 스킬 순회
		for (const auto& skill : model.skills)
		{
			file << L"[Skill]\n";
			file << L"SkillName: " << skill.skillName.c_str() << L"\n\n";

			// 각 애니메이션 순회
			for (const auto& anim : skill.animations)
			{
				file << L"[Animation]\n";
				file << L"AnimationName: " << anim.animationName.c_str() << L"\n";
				file << L"PointCount: " << anim.points.size() << L"\n\n";

				// 각 포인트 순회
				for (const auto& point : anim.points)
				{
					file << L"[CameraPoint]\n";
					file << L"Position: " << point.position.x << L" " << point.position.y << L" " << point.position.z << L"\n";
					file << L"Rotation: " << point.rotation.x << L" " << point.rotation.y << L" " << point.rotation.z << L" " << point.rotation.w << L"\n";
					file << L"Duration: " << point.duration << L"\n";
					file << L"InterpolationType: " << point.interpolationType << L"\n";
					file << L"Damping: " << point.damping << L"\n";
					file << L"HasWorldFloat4x4: " << (point.hasWorldFloat4x4 ? 1 : 0) << L"\n\n";
				}
			}
		}
	}

	file.close();
	return S_OK;
}


HRESULT CFile_Manager::Load_All_CameraPoints(const std::wstring& filename, CameraSaveData* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    std::wifstream file(filename);
    if (!file.is_open())
    {
        // 파일 열기 실패 처리
        return E_FAIL;
    }

    // Unicode 파일 읽기를 위한 설정 (필요한 경우)
    // file.imbue(std::locale("kor")); // 로케일 설정 (시스템에 따라 다를 수 있음)

    std::wstring line;
    CameraSaveData::ModelData currentModel;
    CameraSaveData::ModelData::SkillData currentSkill;
    CameraSaveData::ModelData::SkillData::AnimationData currentAnim;

    // 변수 초기화
    currentModel.modelID = -1;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line == L"[VirtualCamera]")
        {
            // 이전 모델 데이터 저장
            if (currentModel.modelID != -1)
            {
                pArg->models.push_back(currentModel);
                currentModel = CameraSaveData::ModelData();
                currentModel.modelID = -1; // 모델 ID 초기화
            }

            // ModelID 읽기
            std::getline(file, line);
            if (line.find(L"ModelID: ") != std::wstring::npos)
            {
                std::wstring modelStr = line.substr(9);
                // 선행 공백 제거
                size_t first = modelStr.find_first_not_of(L" \t");
                if (first != std::wstring::npos)
                    modelStr = modelStr.substr(first);

                // WStringToString 함수 호출 제거
                auto it = stringToModelID.find(modelStr);
                if (it != stringToModelID.end())
                {
                    currentModel.modelID = it->second;
                }
                else
                {
                    currentModel.modelID = -1; // 알 수 없는 모델
                }
            }

            // 다음 라인 (빈 줄) 건너뛰기
            std::getline(file, line);
        }

        else if (line == L"[Skill]")
        {
            // 이전 스킬 데이터 저장
            if (!currentSkill.skillName.empty())
            {
                currentModel.skills.push_back(currentSkill);
                currentSkill = CameraSaveData::ModelData::SkillData();
            }

            // SkillName 읽기
            std::getline(file, line);
            if (line.find(L"SkillName: ") != std::wstring::npos)
            {
                std::wstring skillNameStr = line.substr(11);
                // 선행 공백 제거
                size_t first = skillNameStr.find_first_not_of(L" \t");
                if (first != std::wstring::npos)
                    skillNameStr = skillNameStr.substr(first);

                currentSkill.skillName = WStringToString(skillNameStr);
            }

            // 다음 라인 (빈 줄) 건너뛰기
            std::getline(file, line);
        }
        else if (line == L"[Animation]")
        {
            // 이전 애니메이션 데이터 저장
            if (!currentAnim.animationName.empty())
            {
                currentSkill.animations.push_back(currentAnim);
                currentAnim = CameraSaveData::ModelData::SkillData::AnimationData();
            }

            // AnimationName 읽기
            std::getline(file, line);
            if (line.find(L"AnimationName: ") != std::wstring::npos)
            {
                std::wstring animNameStr = line.substr(16);
                // 선행 공백 제거
                size_t first = animNameStr.find_first_not_of(L" \t");
                if (first != std::wstring::npos)
                    animNameStr = animNameStr.substr(first);

                currentAnim.animationName = WStringToString(animNameStr);
            }

            // PointCount 읽기 (필요시 사용)
            std::getline(file, line);
            if (line.find(L"PointCount: ") != std::wstring::npos)
            {
                // size_t pointCount = std::stoul(line.substr(12));
                // 현재는 PointCount를 사용하지 않으므로 주석 처리
            }

            // 다음 라인 (빈 줄) 건너뛰기
            std::getline(file, line);
        }
        else if (line == L"[CameraPoint]")
        {
            CameraPoint point;

            // Position 읽기
            std::getline(file, line);
            if (line.find(L"Position: ") != std::wstring::npos)
            {
                std::wstring posStr = line.substr(10);
                std::wistringstream iss(posStr);
                iss >> point.position.x >> point.position.y >> point.position.z;
            }

            // Rotation 읽기
            std::getline(file, line);
            if (line.find(L"Rotation: ") != std::wstring::npos)
            {
                std::wstring rotStr = line.substr(10);
                std::wistringstream iss(rotStr);
                iss >> point.rotation.x >> point.rotation.y >> point.rotation.z >> point.rotation.w;
            }

            // Duration 읽기
            std::getline(file, line);
            if (line.find(L"Duration: ") != std::wstring::npos)
            {
                std::wstring durStr = line.substr(10);
                point.duration = std::stof(durStr);
            }

            // InterpolationType 읽기
            std::getline(file, line);
            if (line.find(L"InterpolationType: ") != std::wstring::npos)
            {
                std::wstring interpStr = line.substr(19);
                point.interpolationType = std::stoi(interpStr);
            }

            // Damping 읽기
            std::getline(file, line);
            if (line.find(L"Damping: ") != std::wstring::npos)
            {
                std::wstring dampingStr = line.substr(9);
                point.damping = std::stof(dampingStr);
            }

            std::getline(file, line);
            if (line.find(L"HasWorldFloat4x4: ") != std::wstring::npos)
            {
                std::wstring hasWStr = line.substr(17);
                hasWStr = Trim(hasWStr); // Trim whitespace

                try
                {
                    point.hasWorldFloat4x4 = (std::stoi(hasWStr) != 0);
                }
                catch (const std::exception& e)
                {
                    // Handle error (e.g., log it and set a default value)
                    point.hasWorldFloat4x4 = false;
                    // You might want to log e.what() for debugging
                }
            }

            // 다음 라인 (빈 줄) 건너뛰기
            std::getline(file, line);

            // 포인트 추가
            currentAnim.points.push_back(point);
        }
        else
        {
            // 기타 섹션 또는 무시할 내용
            // 필요시 처리 추가
        }
    }

    // 파일의 끝에 도달했을 때 마지막 데이터 추가
    if (!currentAnim.animationName.empty())
    {
        currentSkill.animations.push_back(currentAnim);
    }
    if (!currentSkill.skillName.empty())
    {
        currentModel.skills.push_back(currentSkill);
    }
    if (currentModel.modelID != -1)
    {
        pArg->models.push_back(currentModel);
    }

    file.close();
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
