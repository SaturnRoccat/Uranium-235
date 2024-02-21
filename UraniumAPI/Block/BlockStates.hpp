#pragma once
#include <string>
#include "../Utils/rapidProxy.hpp"
#include "../Utils/strUtil.hpp"
#include <vector>
#pragma warning(disable : 4267)

namespace Uranium::Blocks::States
{
	class BlockState
	{
	public:
		BlockState(CStrWithLength state_ID) : state_ID(state_ID) {}

		virtual void WriteState(RapidProxy::DefaultValueWriter writer) = 0;
		virtual size_t GetPermutationSize() { return 0; }
	public:
		CStrWithLength state_ID;
	};
	class BlockStateBoolean : public BlockState
	{
	public:
		BlockStateBoolean(CStrWithLength state_ID) : BlockState(state_ID) {}

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()
			rapidjson::Value array(rapidjson::kArrayType);
			array.PushBack(true, allocator);
			array.PushBack(false, allocator);
			data->AddMember(state_ID.toValue(), array, allocator);
		}

		size_t GetPermutationSize() override { return 2; }

	private:
	};

	class BlockStateInteger : public BlockState
	{
	public:
		BlockStateInteger(CStrWithLength state_ID, int min, int max) : BlockState(state_ID)
		{
			values.reserve(max - min + 1); // Reserve the space so we don't have to reallocate
			for (int i = min; i <= max; i++) { values.push_back(i); } 
		}

		BlockStateInteger(CStrWithLength state_ID, const std::vector<int>& values) : BlockState(state_ID), values(values) {}

		void AddValue(int value) { values.push_back(value); }
		void AtIndex(int index, int value) { values[index] = value; }
		int AtIndex(int index) { return values[index]; }
		size_t GetPermutationSize() override { return values.size(); }
		size_t Size() { return values.size(); }

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()
			RJ_STL_V_A_P(translated, this->values)
			data->AddMember(state_ID.toValue(), translated, allocator);
		}

	private:
		std::vector<int> values;
	};

	class BlockStateString : public BlockState
	{
		public:
		BlockStateString(CStrWithLength state_ID) : BlockState(state_ID) {}

		void AddValue(CStrWithLength value) { values.push_back(value); }
		void AtIndex(int index, CStrWithLength value) { values[index] = value; }
		CStrWithLength AtIndex(int index) { return values[index]; }
		size_t Size() { return values.size(); }
		size_t GetPermutationSize() override { return values.size(); }

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()

			rapidjson::Value translated(rapidjson::kArrayType); 
			translated.Reserve(this->values.size(), allocator);
			for (auto& i : this->values) 
			{
				translated.PushBack(i.toValue(), allocator);
			}
			data->AddMember(state_ID.toValue(), translated, allocator);
		}
	private:
		std::vector<CStrWithLength> values;
	};

}


#pragma warning(default : 4267)
