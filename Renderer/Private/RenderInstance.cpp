#include "..\Public\RenderInstance.h"

IMPLEMENT_SINGLETON(CRenderInstance)

CRenderInstance::CRenderInstance()	
{

}

HRESULT CRenderInstance::Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	
	return S_OK;
}

void CRenderInstance::Update_Engine(_float fTimeDelta)
{
	/* �������� �����ϴ� ��ü�� ��, �ݺ����� ������ �ʿ��� ��ü���� �ִٸ�. */
	/* ���⿡�� ������ �������ش�. */	
}

HRESULT CRenderInstance::Render_Engine()
{
	/* �������� �����ϴ� ��ü�� ��, �ݺ����� ������ �ʿ��� ��ü���� �ִٸ�. */
	/* ���⿡�� ������ �������ش�. */

	if (FAILED(m_pRenderer->Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);	
}


void CRenderInstance::Release_Engine()
{

	Safe_Release(m_pRenderer);

	CRenderInstance::Get_Instance()->Destroy_Instance();
}

void CRenderInstance::Free()
{
	__super::Free();

	
}
