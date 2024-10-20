
struct TexMoveDesc
{
    float2 vDirection;              //텍스쿠드 움직일 방향
    float fSpeed;                   //스피드
};

struct SpriteAnimDesc // 쉐이더는 간단 (나머지 처리는 다 업데이트에서)
{
    float2 vSpriteSize;             //스프라이트 사이즈
    float vSpriteCurPos;            //스프라이트 현재 위치
};

struct MaterialDesc
{
    int iBaseIndex;                 // g_Textures 몇번을 Base로 두고 있는지 
    int iDiffuseInputIndex;    // 디퓨즈의 영향을 준 Index (없으면 -1로 해야할거같기도)
    int iAlphaInputIndex;      // 알파의 영향을 준 Index
	
	// 함수노드들은 같은 종류가 여러개 들어갈걸 고려하지 않아도 됨
    bool isTexMove;                 // 텍스무브를 할건지 체크 ( 함수 노드에 연결이 되어 있었는지 )
    bool isSpriteAnimation;         // 스프라이트 애니메이션 할건지 체크

    TexMoveDesc TexMove;            // TexMove구조체 정보
    SpriteAnimDesc SpriteAnim;      // SpriteAnim구조체 정보
};

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

MaterialDesc    g_MaterialDesc[100];    // All_In_One
texture2D       g_Textures[100];        // 구조체안에 넣으면 쉐이더에 넣기 애매해짐
int             g_iNumTextures;         // 루프 돌아야됨

