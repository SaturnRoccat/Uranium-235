#pragma once
#include <string>
#include "../Utils/rapidProxy.hpp"
#include <vector>
#pragma warning(disable : 4267)

namespace Uranium
{
	class BlockState
	{
	public:
		BlockState(std::string state_ID) : state_ID(state_ID) {}

		virtual void WriteState(RapidProxy::DefaultValueWriter writer) = 0;
		virtual size_t GetPermutationSize() { return 0; }
	public:
		std::string state_ID;
	};
	class BlockStateBoolean : public BlockState
	{
	public:
		BlockStateBoolean(std::string state_ID) : BlockState(state_ID) {}

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()
			rapidjson::Value array(rapidjson::kArrayType);
			array.PushBack(true, allocator);
			array.PushBack(false, allocator);
			data->AddMember(RJ_STL_S(state_ID), array, allocator);
		}

		size_t GetPermutationSize() override { return 2; }

	private:
	};

	class BlockStateInteger : public BlockState
	{
	public:
		BlockStateInteger(std::string state_ID, int min, int max) : BlockState(state_ID) 
		{
			for (int i = min; i <= max; i++) { values.push_back(i); } 
		}

		BlockStateInteger(std::string state_ID, const std::vector<int>& values) : BlockState(state_ID), values(values) {}

		void AddValue(int value) { values.push_back(value); }
		void AtIndex(int index, int value) { values[index] = value; }
		int AtIndex(int index) { return values[index]; }
		size_t GetPermutationSize() override { return values.size(); }
		size_t Size() { return values.size(); }

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()
			RJ_STL_V_A_P(translated, this->values)
			data->AddMember(RJ_STL_S(state_ID), translated, allocator);
		}

	private:
		std::vector<int> values;
	};

	class BlockStateString : public BlockState
	{
		public:
		BlockStateString(std::string state_ID) : BlockState(state_ID) {}

		void AddValue(std::string value) { values.push_back(value); }
		void AtIndex(int index, std::string value) { values[index] = value; }
		std::string AtIndex(int index) { return values[index]; }
		size_t Size() { return values.size(); }
		size_t GetPermutationSize() override { return values.size(); }

		void WriteState(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP()
			RJ_STL_V_A_EX(translated, this->values, RJ_STL_S)
			data->AddMember(RJ_STL_S(state_ID), translated, allocator);
		}
	private:
		std::vector<std::string> values;
	};

}


#pragma warning(default : 4267)
