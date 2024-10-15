#pragma once

#include "Base.h"

/* 생성한 객체들을 보관한다. */
/* Prototype 방식으로 객체를 추가한다. */


BEGIN(Engine)

#pragma once
class CFile_Manager final : public CBase
{
private:
	CFile_Manager();
	virtual ~CFile_Manager() = default;
public:
	HRESULT Initialize();
public:
	HRESULT SaveObjects(const wstring& filename, void* pArg);
	void* LoadObjects(const wstring& filename);
	HRESULT ParseLine(const wstring& line, FILEDATA& obj);

private:
	vector<FILEDATA> m_vecpFileData;

public:
	static CFile_Manager* Create();
	virtual void Free() override;
};

END