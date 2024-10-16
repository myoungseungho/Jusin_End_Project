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

	HRESULT Save_Effects(wstring& FilePath, void* pArg);
	void*		Load_Effects(wstring& FilePath);
	HRESULT Read_Effects(wstring& Line, EFFECT_LAYER_DATA& LayerData, EFFECT_DATA& EffectData, EFFECT_KEYFRAME_DATA& KeyFrameData);

private:
	vector<FILEDATA> m_vecpFileData;
	vector<EFFECT_LAYER_DATA>* m_pLoadedEffectData = { nullptr };

public:
	static CFile_Manager* Create();
	virtual void Free() override;
};

END