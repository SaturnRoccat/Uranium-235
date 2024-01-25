#pragma once

namespace Uranium
{
	template<typename T>
	class NonOwningPointer
	{
	public:
		NonOwningPointer() = default;
		NonOwningPointer(T* ptr)
			: m_Ptr(ptr) {}

		template<typename T>
		operator T* () const { return (T*)m_Ptr; }

		template<typename T>
		T* As() const { return (T*)m_Ptr; }

		template<typename T>
		T& AsRef() const { return *(T*)m_Ptr; }

		template<typename T>
		T& operator*() const { return *(T*)m_Ptr; }

		template<typename T>
		T* operator->() const { return (T*)m_Ptr; }

		bool operator==(const NonOwningPointer& other) const { return m_Ptr == other.m_Ptr; }
		bool operator!=(const NonOwningPointer& other) const { return m_Ptr != other.m_Ptr; }

		bool operator==(const T* other) const { return m_Ptr == other; }
		bool operator!=(const T* other) const { return m_Ptr != other; }

		bool operator==(T* other) const { return m_Ptr == other; }
		bool operator!=(T* other) const { return m_Ptr != other; }

		operator bool() const { return m_Ptr; }

		T* Get() const { return m_Ptr; }

		
	private:
		T* m_Ptr = nullptr;
	};
};