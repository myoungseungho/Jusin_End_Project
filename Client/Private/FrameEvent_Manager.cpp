#include "stdafx.h"
#include "..\Public\FrameEvent_Manager.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   
#include <sstream>

#include "Animation.h"
IMPLEMENT_SINGLETON(CFrameEvent_Manager)

CFrameEvent_Manager::CFrameEvent_Manager()
{
}


/*
void CFrameEvent_Manager::Add_Event(_wstring strText)
{
	


    // 분리된 문자열을 저장할 배열
    string splitText[10];
    int index = 0;

    // wstringstream을 사용하여 wstring을 처리
   wstringstream wss(strText);
   wstring temp;

    // 쉼표를 기준으로 문자열을 분리하여 splitText 배열에 저장
    while (std::getline(wss, temp, L',') && index < 10) 
    {
        // wstring을 string으로 변환 후 저장
        splitText[index] = std::string(temp.begin(), temp.end());
        index++;
    }



    splitText;
    _bool DebugPo = true;

}
*/

/*
void CFrameEvent_Manager::Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, _wstring strText)
{
    // 분리된 문자열을 저장할 배열
    string splitText[10];
    int index = 0;

    // wstringstream을 사용하여 wstring을 처리
    wstringstream wss(strText);
    wstring temp;

    // 쉼표를 기준으로 문자열을 분리하여 splitText 배열에 저장
    while (std::getline(wss, temp, L',') && index < 10)
    {
        // wstring을 string으로 변환 후 저장
        splitText[index] = std::string(temp.begin(), temp.end());
        index++;
    }



    splitText;
    _bool DebugPo = true;

    FrameEvent[iCharacterIndex][iAnimationIndex][fPosition].push_back("TEST");

}
*/

void CFrameEvent_Manager::Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, string strText)
{
    string splitText[10];
    int index = 0;

    // stringstream을 사용하여 string을 처리
   stringstream ss(strText);
   string temp;

    // 쉼표를 기준으로 문자열을 분리하여 splitText 배열에 저장
    while (std::getline(ss, temp, ',') && index < 10) {
        splitText[index] = temp;
        index++;
    }


    _float fValue[9]{};

    //split[1] 은 fValue[0] , split[2]는 fValue[1]...
    for (int i = 1; i < 9; i++)
    {
        if (splitText[i] == "")
            break;

        fValue[i - 1] = Convert_strtoFloat(splitText[i]);

    }


    _bool bDebugPoint = false;


    //if (splitText[0] == "FrameMoveTest")
    //{
    //
    //}

   
    FrameEvent[iCharacterIndex][iAnimationIndex][fPosition].push_back(strText);

}


/*
void CFrameEvent_Manager::LoadFile(const _char* TextFilePath)
{


    ifstream file(TextFilePath);

    // 파일을 제대로 열었는지 확인
    if (!file.is_open()) 
    {

        MSG_BOX(TEXT("FrameEvevnt_Manager:: LoadFile 에서 txt 파일 로드 실패\nPath:")) ;
        return ;
    }

    std::string line;
    // 파일의 각 줄을 처리
    while (std::getline(file, line)) 
    {


        //모든 쉼표를 분리하는 코드.  폐기예정

       // 분리된 문자열을 저장할 배열
       std::string splitText[10];
       int index = 0;
       
       // stringstream을 사용하여 줄을 쉼표 기준으로 분리
       std::stringstream ss(line);
       std::string temp;
       
       while (std::getline(ss, temp, ',') && index < 10) 
       {
           splitText[index] = temp;
           index++;
       }
       
       splitText;
       _bool bDebugTest = true;
       
       Add_Event(Convert_strtoCharacterIndex(splitText[0]),  //캐릭터 이름 변환  
                 Convert_strtoFloat(splitText[1]),           //애니메이션 이름 변환  (현재는 float)
                 Convert_strtoFloat(splitText[2]),           //글자를 float 애니메이션 position으로 변환
                 splitText[3]);
    

        


    }

    // 파일 닫기
    file.close();

    //return LOAD_SUCCES;
    return ;

}
*/

void CFrameEvent_Manager::LoadFile2(const _char* TextFilePath)
{

    ifstream file(TextFilePath);

    // 파일을 제대로 열었는지 확인
    if (!file.is_open())
    {

        MSG_BOX(TEXT("FrameEvevnt_Manager:: LoadFile 에서 txt 파일 로드 실패\nPath:"));
        return;
    }

    std::string line;
    // 파일의 각 줄을 처리
    while (std::getline(file, line))
    {


       // 분리된 문자열을 저장할 배열
        std::string splitText[4]; // 최대 4개로 나누기 (3번 쉼표 + 나머지)
        int index = 0;

        // stringstream을 사용하여 줄을 쉼표 기준으로 분리
        std::stringstream ss(line);
        std::string temp;

        // 최대 세 번까지 쉼표로 나누고 나머지는 그대로 저장
        while (index < 3)
        {
            std::getline(ss, temp, ',');
            splitText[index] = temp;
            index++;
        }

        // 스트림의 남은 부분을 한 번에 저장 (남은 모든 텍스트 가져오기)
        std::getline(ss, temp); // 남은 부분 읽기
        splitText[3] = temp;

        // 디버깅용 출력
        _bool bDebugTest = true;

        
        Add_Event(Convert_strtoCharacterIndex(splitText[0]),  //캐릭터 이름 변환  
            Convert_strtoFloat(splitText[1]),           //애니메이션 이름 변환  (현재는 float)
            Convert_strtoFloat(splitText[2]),           //글자를 float 애니메이션 position으로 변환
            splitText[3]);


       

    }

    // 파일 닫기
    file.close();

    //return LOAD_SUCCES;
    return;

}

void CFrameEvent_Manager::UseEvent(string strEventText, CGameObject* pGameobject)
{
    string splitText[10];
    int index = 0;

    // stringstream을 사용하여 string을 처리
    stringstream ss(strEventText);
    string temp;

    // 쉼표를 기준으로 문자열을 분리하여 splitText 배열에 저장
    while (std::getline(ss, temp, ',') && index < 10) {
        splitText[index] = temp;
        index++;
    }


    _float fValue[9]{};

    //split[1] 은 fValue[0] , split[2]는 fValue[1]...
    for (int i = 1; i < 9; i++)
    {
        if (splitText[i] == "")
            break;

        fValue[i - 1] = Convert_strtoFloat(splitText[i]);

    }


    _bool bDebugPoint = false;


    if (splitText[0] == "ObjectMove")
    {

        CTransform* pTransform = static_cast<CTransform*>(pGameobject->Get_Component(TEXT("Com_Transform")));

        if (nullptr == pTransform)
            return;
        _vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);
        vPos += _vector{ fValue[0],fValue[1],fValue[2],fValue[3] };
        pTransform->Set_State(CTransform::STATE_POSITION, vPos);

        //static_cast<CTransform*>(pGameobject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _vector{ fValue[0],fValue[1],fValue[2],fValue[3]});

        _bool bDebug = false;
    }

    if (splitText[0] == "TickPerSecondChange")
    {

        CModel* pModel = static_cast<CModel*>(pGameobject->Get_Component(TEXT("Com_Model")));
        pModel->Get_pCurrentAnimation()->m_fTickPerSecond = fValue[0];

    }


}

CHARACTER_INDEX CFrameEvent_Manager::Convert_strtoCharacterIndex(string strText)
{
    /*
   const static enum CHARACTER_INDEX {
		SELECT_GOKU, SELECT_BOU, SELECT_21, SELECT_HIT,
		PLAY_GOKU, PLAY_BOU, PLAY_21, PLAY_HIT, CHARACTER_INDEX_END
	};
   */

    if (strText == "SELECT_GOKU")
        return SELECT_GOKU;

    if (strText == "SELECT_BOU")
        return SELECT_BOU;

    if (strText == "SELECT_21")
        return SELECT_21;

    if (strText == "SELECT_HIT")
        return SELECT_HIT;



    if (strText == "PLAY_GOKU")
        return PLAY_GOKU;

    if (strText == "PLAY_BOU")
        return PLAY_BOU;

    if (strText == "PLAY_21")
        return PLAY_21;

    if (strText == "PLAY_HIT")
        return PLAY_HIT;



    if (strText == "SELECT_GOKU")
        return SELECT_GOKU;


    return CHARACTER_INDEX_END;
}

_float CFrameEvent_Manager::Convert_strtoFloat(string strText)
{
    _float floatValue;

    stringstream ss(strText);
    ss >> floatValue;

    return floatValue;
}

void CFrameEvent_Manager::Free()
{
	__super::Free();


}
