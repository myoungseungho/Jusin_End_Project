
struct TexMoveDesc
{
    float2 vDirection;              //�ؽ���� ������ ����
    float fSpeed;                   //���ǵ�
};

struct SpriteAnimDesc // ���̴��� ���� (������ ó���� �� ������Ʈ����)
{
    float2 vSpriteSize;             //��������Ʈ ������
    float vSpriteCurPos;            //��������Ʈ ���� ��ġ
};

struct MaterialDesc
{
    int iBaseIndex;                 // g_Textures ����� Base�� �ΰ� �ִ��� 
    int iDiffuseInputIndex;    // ��ǻ���� ������ �� Index (������ -1�� �ؾ��ҰŰ��⵵)
    int iAlphaInputIndex;      // ������ ������ �� Index
	
	// �Լ������� ���� ������ ������ ���� ������� �ʾƵ� ��
    bool isTexMove;                 // �ؽ����긦 �Ұ��� üũ ( �Լ� ��忡 ������ �Ǿ� �־����� )
    bool isSpriteAnimation;         // ��������Ʈ �ִϸ��̼� �Ұ��� üũ

    TexMoveDesc TexMove;            // TexMove����ü ����
    SpriteAnimDesc SpriteAnim;      // SpriteAnim����ü ����
};

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

MaterialDesc    g_MaterialDesc[100];    // All_In_One
texture2D       g_Textures[100];        // ����ü�ȿ� ������ ���̴��� �ֱ� �ָ�����
int             g_iNumTextures;         // ���� ���ƾߵ�

