#ifndef EFFECTHELPER_H
#define EFFECTHELPER_H

#include "tools/Util.h"

namespace Potato
{
	// ÄÚ´æ¶ÔÆë
	template<class derivedType>
	struct AligendType 
	{
		static void*  operator new(size_t size) {
			const size_t aligendSize = __alignof(derivedType);
			static_assert(aligendSize > 8, "AlignedNew is only useful for types with > 8 byte alignment! Did you forget a __declspec(align) on DerivedType?");
	
			void* ptr = _aligned_malloc(size, aligendSize);
			if (!ptr)
				throw std::bad_alloc();
			
			return ptr;
		}
	
		static void operator delete(void * ptr)
		{
			_aligned_free(ptr);
		}
	};
	
	struct ContentBufferBase
	{
		bool isDirty;
		ID3D11Buffer* cBuffer;

		~ContentBufferBase()
		{
			ReleaseObject(cBuffer);
		}
	
		virtual HRESULT CreateBuffer(ID3D11Device* device) = 0;
		virtual void UpdateBuffer(ID3D11DeviceContext* deviceContext) = 0;
		virtual void BindVS(ID3D11DeviceContext* deviceContext) = 0;
		virtual void BindHS(ID3D11DeviceContext *deviceContext) = 0;
		virtual void BindDS(ID3D11DeviceContext* deviceContext) = 0;
		virtual void BindGS(ID3D11DeviceContext* deviceContext) = 0;
		virtual void BindCS(ID3D11DeviceContext* deviceContext) = 0;
		virtual void BindPS(ID3D11DeviceContext* deviceContext) = 0;
	};

	template<UINT slotNum, class T>
	struct ContentBufferObject : public ContentBufferBase
	{
		T data;

		HRESULT CreateBuffer(ID3D11Device* device) override
		{
			if (cBuffer != nullptr)
			{
				return S_OK;
			}

			D3D11_BUFFER_DESC bufferdesc;
			ZeroMemory(&bufferdesc, sizeof(D3D11_BUFFER_DESC));
			bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferdesc.ByteWidth = sizeof(T);

			return device->CreateBuffer(&bufferdesc, nullptr, &cBuffer);
		}

		void UpdateBuffer(ID3D11DeviceContext* deviceContext) override
		{
			if (isDirty)
			{
				isDirty = false;
				D3D11_MAPPED_SUBRESOURCE mappedData;
				deviceContext->Map(cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
				memcpy_s(mappedData.pData, sizeof(T), &data, sizeof(T));
				deviceContext->Unmap(cBuffer, 0);
			}
		}

		void BindVS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->VSSetConstantBuffers(slotNum, 1, &cBuffer);
		}

		void BindHS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->HSSetConstantBuffers(slotNum, 1, &cBuffer);
		}

		void BindDS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->DSSetConstantBuffers(slotNum, 1, &cBuffer);
		}

		void BindGS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->GSSetConstantBuffers(slotNum, 1, &cBuffer);
		}

		void BindCS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->CSSetConstantBuffers(slotNum, 1, &cBuffer);
		}

		void BindPS(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->PSSetConstantBuffers(slotNum, 1, &cBuffer);
		}
	};
}

#endif // !EFFECTHELPER_H

