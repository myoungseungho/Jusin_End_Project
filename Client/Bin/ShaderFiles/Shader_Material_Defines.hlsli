struct TexMoveDesc
{
    float2 vDirection;              //�ؽ���� ������ ����
    float fSpeed;                   //���ǵ�
    float fAccTime;                 //�̰� ���������� �޾ƾ����� �������� ������ ���߿�
};

struct SpriteAnimDesc // ���̴��� ���� (������ ó���� �� ������Ʈ����)
{
    float2 vSpriteSize;             //��������Ʈ ������
    float vSpriteCurPos;            //��������Ʈ ���� ��ġ
};

struct MaterialDesc
{
    int iBaseIndex;                 // g_Textures ����� Base�� �ΰ� �ִ��� 
    int iDiffuseInputIndex;         // ��ǻ���� ������ �� Index (������ -1�� �ؾ��ҰŰ��⵵)
    int iAlphaInputIndex;           // ������ ������ �� Index
    int iOutputIndex;               // �� �ڽ��� ������ �� Index
	
	/* �Լ������� ���� ������ ������ ���� ������� �ʾƵ� �� */
    bool isTexMove;                 // �ؽ����긦 �Ұ��� üũ ( �Լ� ��忡 ������ �Ǿ� �־����� )
    bool isSpriteAnimation;         // ��������Ʈ �ִϸ��̼� �Ұ��� üũ

    TexMoveDesc TexMove;            // TexMove����ü ����
    SpriteAnimDesc SpriteAnim;      // SpriteAnim����ü ����
};

float2 SpriteAnimation(float2 vTexcoord, SpriteAnimDesc tDesc)
{
    return vTexcoord * tDesc.vSpriteSize + tDesc.vSpriteCurPos * tDesc.vSpriteSize;
}

float2 TexcoordMove(float2 vTexcoord, TexMoveDesc tDesc)
{
    return vTexcoord + normalize(tDesc.vDirection) * tDesc.fSpeed * tDesc.fAccTime;
}