#pragma once

#include "Base.h"

/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


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