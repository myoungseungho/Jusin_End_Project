#pragma once

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}


	template<typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_AddRef(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
			iRefCnt = pInstance->AddRef();

		return iRefCnt;
	}


	template<typename T>
	unsigned int Safe_Release(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();
			if (0 == iRefCnt)
				pInstance = nullptr;
		}

		return iRefCnt;
	}

	// 1. 벡터의 길이를 구하는 함수
	inline _float GetVectorLength(const _vector& v)
	{
		_vector length = XMVector3Length(v);
		return XMVectorGetX(length);
	}

	// 2. 내적
	inline _float DotProduct(const _vector& v1, const _vector& v2)
	{
		XMVECTOR dot = XMVector3Dot(v1, v2);
		return XMVectorGetX(dot);
	}

	// 3. Matrix를 Float4x4
	inline _float4x4 MatrixToFloat4x4(const _matrix& mat)
	{
		_float4x4 floatMat;
		XMStoreFloat4x4(&floatMat, mat);
		return floatMat;
	}

	//4. Float4x4를 Matrix를 바꿔줌
	inline _matrix Float4x4ToMatrix(const _float4x4& floatMat)
	{
		return XMLoadFloat4x4(&floatMat);
	}

	// 5. _float4 더하기
	inline _float4 AddFloat4(const _float4& f1, const _float4& f2)
	{
		_float4 result;
		result.x = f1.x + f2.x;
		result.y = f1.y + f2.y;
		result.z = f1.z + f2.z;
		result.w = f1.w + f2.w;
		return result;
	}

	// 6. _float4 빼기
	inline _float4 SubtractFloat4(const _float4& f1, const _float4& f2)
	{
		_float4 result;
		result.x = f1.x - f2.x;
		result.y = f1.y - f2.y;
		result.z = f1.z - f2.z;
		result.w = f1.w - f2.w;
		return result;
	}

	// 7. _float4 곱하기
	inline _float4 MultiplyFloat4(const _float4& f1, const _float4& f2)
	{
		_float4 result;
		result.x = f1.x * f2.x;
		result.y = f1.y * f2.y;
		result.z = f1.z * f2.z;
		result.w = f1.w * f2.w;
		return result;
	}

	// 8. _float4 나누기
	inline _float4 DivideFloat4(const _float4& f1, const _float4& f2)
	{
		_float4 result;
		result.x = f1.x / f2.x;
		result.y = f1.y / f2.y;
		result.z = f1.z / f2.z;
		result.w = f1.w / f2.w;
		return result;
	}

	//9. _float4x4와 _matrix를 곱하는 함수
	inline _matrix Multiply_Float4x4_Matrix(const _float4x4& f4x4, const _matrix& mat)
	{
		// _float4x4를 XMMATRIX로 변환
		XMMATRIX xmMat1 = XMLoadFloat4x4(&f4x4);

		// 두 XMMATRIX를 곱하고 결과 반환
		XMMATRIX result = XMMatrixMultiply(xmMat1, mat);

		return result;
	}

	//10. _float4x4와 _matrix를 곱하는 함수
	inline _matrix Multiply_Matrix_Float4x4(const _matrix& mat, const _float4x4& f4x4)
	{
		// _float4x4를 XMMATRIX로 변환
		XMMATRIX xmMat1 = XMLoadFloat4x4(&f4x4);

		// 두 XMMATRIX를 곱하고 결과 반환
		XMMATRIX result = XMMatrixMultiply(mat, xmMat1);

		return result;
	}
}