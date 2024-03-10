#include "cxxFunction.hpp"

std::string Uranium::CxxFunction::generateCxxDec() const
{
	std::stringstream ss;
	if (hasNamespace() && !hasClass()) // if has class is set it implies that this should be put in a method
		ss << "namespace " << getNamespace() << " {\n"; // Start of namespace block if the function has a namespace and does not belong to a class

	if (isVirtual()) 
		ss << "virtual ";
	if (isStatic()) 
		ss << "static ";

	ss << getReturnType().getAsCxx(false) << " " << getName() << "("; // Append return type and function name to the stringstream
	std::vector<CxxType*> defaultTypes;
	defaultTypes.reserve(m_parameters.size()); // more memory but better than reallocation
	for (size_t i = 0; i < m_parameters.size(); i++) 
	{
		auto& param = m_parameters[i];
		if (param.hasDefaultValue()) // This is needed because all parameters with default values must be at the end of the parameter list
		{
			defaultTypes.push_back(const_cast<CxxType*>(&param)); // Store the parameter in the defaultTypes vector
			continue;
		}
		ss << param.getAsCxx(); 
		if (i != m_parameters.size() - 1 && defaultTypes.size() != 0) // Check if there are more parameters and if there are any parameters with default values if so add a comma
			ss << ", ";
	}
	for (size_t i = 0; i < defaultTypes.size(); i++) 
	{
		auto& param = defaultTypes[i];
		ss << param->getAsCxx(); // Append the parameter to the stringstream
		if (i != defaultTypes.size() - 1) // Check if there are more parameters with default values
			ss << ", ";
	}
	ss << ")"; // Close the function parameter list
	if (isConst()) 
		ss << " const";
	if (isOverride()) 
		ss << " override";
	if (!this->hasSeperateDecAndDef()) // if the function 
	{
		ss << ";\n";
		if (hasNamespace() && !hasClass()) // if has class is set it implies that this should be put in a method
			ss << "}; // namespace " << getNamespace() << "\n"; // Close the namespace block
		return ss.str();
	}
	ss << " {\n"; // Open the function body
	ss << generateCxxDef(true); // Generate the function definition
	ss << "\n}\n"; // Close the function body
	if (hasNamespace() && !hasClass()) // if has class is set it implies that this should be put in a method
		ss << "}; // namespace " << getNamespace() << "\n"; // Close the namespace block



	return ss.str(); // Return the generated code as a string
}

std::string Uranium::CxxFunction::generateCxxDef(bool skipFunctionDef) const
{
	std::stringstream ss;
	if (!skipFunctionDef)
	{
		ss << m_returnType.getAsCxx();
		if (hasNamespace())
			ss << " " << getNamespace() << "::";
		if (hasClass())
			ss << getClass() << "::";
		ss << getName() << "(";
		std::vector<CxxType*> defaultTypes;
		defaultTypes.reserve(m_parameters.size()); // more memory but better than reallocation
		for (size_t i = 0; i < m_parameters.size(); i++)
		{
			auto& param = m_parameters[i];
			if (param.hasDefaultValue()) // This is needed because all parameters with default values must be at the end of the parameter list
			{
				defaultTypes.push_back(const_cast<CxxType*>(&param)); // Store the parameter in the defaultTypes vector
				continue;
			}
			ss << param.getAsCxx();
			if (i != m_parameters.size() - 1 && defaultTypes.size() != 0) // Check if there are more parameters and if there are any parameters with default values if so add a comma
				ss << ", ";
		}
		for (size_t i = 0; i < defaultTypes.size(); i++)
		{
			auto& param = defaultTypes[i];
			ss << param->getAsCxx(); // Append the parameter to the stringstream
			if (i != defaultTypes.size() - 1) // Check if there are more parameters with default values
				ss << ", ";
		}
		ss << ")"; // Close the function parameter list
		if (isConst())
			ss << " const";
		if (isOverride())
			ss << " override";
		ss << " {"; // Open the function body
	}

	ss << compileBody();

	if (!skipFunctionDef)
		ss << "}\n";

	return ss.str();
}
