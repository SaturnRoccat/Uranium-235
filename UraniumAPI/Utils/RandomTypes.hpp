#pragma once
#include <cstdint>
#include <memory>
#include <array>
#include <iterator>
#include <cstddef>  

namespace Uranium
{
	namespace RandomTypes
	{
		// I just got bored so i made this
		template<typename StorageType = uint64_t, typename SplitType = uint32_t, size_t arrLeng = 2>
		class TypePack
		{
		private:
			union
			{
				StorageType _storage;
				SplitType _split[arrLeng];

			};
		private:
		public:
			class Iterator
			{
			public:
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = SplitType;
				using pointer = SplitType*; 
				using reference = SplitType&;
				Iterator(pointer ptr) : m_ptr(ptr) {}

				reference operator*() const { return *m_ptr; }
				pointer operator->() { return m_ptr; }

				Iterator& operator++() { m_ptr++; return *this; }
				Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

				friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
				friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
			private:
				pointer m_ptr;
			};

			Iterator begin() { return Iterator(_split); }
			Iterator end() { return Iterator(_split + arrLeng); }
		public:
			TypePack() = default;
			TypePack(const TypePack<StorageType, SplitType, arrLeng>& other) { memcpy(this, &other, sizeof(TypePack<StorageType, SplitType, arrLeng>)); }// kinda hacky but it works
			TypePack(const TypePack<StorageType, SplitType, arrLeng> && other) { memcpy(this, &other, sizeof(TypePack<StorageType, SplitType, arrLeng>)); } // kinda hacky but it works
			TypePack(const std::array<SplitType, arrLeng>& arr) { memcpy(_split, arr.data(), sizeof(SplitType) * arrLeng); }
			TypePack(const StorageType& storage) : _storage(storage) {}
			TypePack(const SplitType& split) : _split{ split } {}

			inline StorageType getStorage() const { return _storage; }
			inline StorageType& getStorage() { return _storage; }
			inline std::array<SplitType, arrLeng> getSplit() { return *reinterpret_cast<std::array<SplitType, arrLeng>*>(&_storage); } // wtf
			template<size_t index = 0>
			SplitType getSplit() const { return _split[index]; }
			template<size_t index = 0>
			SplitType& getSplit() { return _split[index]; }
			inline void setStorage(const StorageType& storage) { _storage = storage; }
			inline void setSplit(const std::array<SplitType, arrLeng>& arr) { memcpy(_split, arr.data(), sizeof(SplitType) * arrLeng); }
			template<size_t index = 0>
			void setSplit(const SplitType& split) { _split[index] = split; }
			inline size_t size() const { return arrLeng; }


		public:
			static_assert(sizeof(StorageType) >= sizeof(SplitType) * arrLeng, "The size of the storage type is smaller than the size of the split type times the length of the array");
		};

	}
}