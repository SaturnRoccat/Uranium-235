# classes

class CxxFunction:
    def __init__(self, name: str, returnType: str, args: list[str] = [], body: str = "", isVirtual: bool = False, shouldOverride: bool = True):
        self.name = name
        self.returnType = returnType
        self.args = args
        self.body = body
        self.isVirtual = isVirtual
        self.shouldOverride = shouldOverride

    def makeFunction(self) -> str:
        returnData: str = ""
        if self.isVirtual and not self.shouldOverride:
            returnData += "    virtual "
        returnData += f"{self.returnType} {self.name}("
        for index, arg in enumerate(self.args):
            if index != 0:
                returnData += ", "
            returnData += arg
        returnData += ")"
        if self.shouldOverride:
            returnData += " override"
        returnData += " {\n"
        returnData += self.body
        returnData += "}\n"
        return returnData

    def getName(self) -> str:
        return self.name

    def getReturnType(self) -> str:
        return self.returnType

    def getArgs(self) -> list[str]:
        return self.args

    def getBody(self) -> str:
        return self.body

class CxxType: 
    def __init__(self, name: str, type: str = "", default: str = ""):
        self.name = name
        self.type = type
        self.default = default
    
    def setDefault(self, default: str):
        self.default = default
    
    def setType(self, type: str):
        self.type = type

    def setName(self, name: str):
        self.name = name

    def getName(self) -> str:
        return self.name
    
    def getType(self) -> str:
        return self.type

    def getDefault(self) -> str:
        return self.default

class CxxEnum():
    def __init__(self, name: str, values: list[str] = [], type: str = "int", conversionArray: list[str] = []):
        self.name = name
        self.type = type
        self.values = values
        self.conversionArray = conversionArray
        if conversionArray == []:
            self.hasTranslation = False

    def addValue(self, value: str):
        self.values.append(value)

    def setType(self, type: str):
        self.type = type

    def setConversionArray(self, conversionArray: list[str]):
        self.conversionArray = conversionArray
        if conversionArray != []:
            self.hasTranslation = True
        else:
            self.hasTranslation = False

    def dumpValues(self) -> str:
        returnData: str = f"values of {self.name}:\n"
        for value in self.values:
            returnData += f"    {value}\n"
        return returnData
    
    def hasTranslation(self) -> bool:
        return self.hasTranslation
    
    def dumpConversionArray(self) -> str:
        returnData: str = f"conversion array of {self.name}:\n"
        for value in self.conversionArray:
            returnData += f"    {value}\n"
        return returnData
    
    def generateEnumCxxDef(self, namespace: str = "") -> str:
        returnData: str = ""
        if namespace != "":
            returnData += f"namespace {namespace} {'{'}\n"
        returnData += f"""  enum class {self.name} : {self.type} {'{'}\n"""
        for value in self.values:
            returnData += f"    {value},\n"
        returnData += f"""  {'};'}\n"""
        if namespace != "":
            returnData += "}\n"

        return returnData
    
    def generateEnumConversionArray(self, namespace: str) -> str:
        returnData: str = ""
        if namespace != "":
            returnData += f"namespace {namespace} {'{'}\n"
        returnData += f"""  const static std::array<std::string, {len(self.values)}> {self.name}ConversionArray = {'{'}\n"""
        for value in self.conversionArray:
            returnData += f"    {value},\n"
        returnData += f"""  {'};'}\n"""
        if namespace != "":
            returnData += "}\n"

        return returnData
    
    def generateEnumConversionFunction(self, namespace: str) -> str:
        returnData: str = ""
        if namespace != "":
            returnData += f"namespace {namespace} {'{'}\n"
        returnData += f"""  std::string_view {self.name}ToString({self.name} value) {'{'}\n"""
        returnData += f"    return {self.name}ConversionArray[static_cast<uint64_t>(value)];\n"
        returnData += f"""  {'}'}\n"""
        if namespace != "":
            returnData += "}\n"


class CxxClass:
    def __init__(self, name: str, parent: str = "", members: list[CxxType] = [], functions: list[CxxFunction] = []):
        self.name = name
        self.parent = parent
        self.members = members
        self.functions = functions
        if parent == "" and members == [] and functions == []:
            self.isNull = True

    def addMember(self, member: CxxType):
        self.members.append(member)

    def addFunction(self, function: CxxFunction):
        self.functions.append(function)

    def setParent(self, parent: str):
        self.parent = parent  

    def isValid(self) -> bool:
        return not self.isNull      
    
    def setValid(self, valid: bool):
        self.isNull = not valid

    def setType(self, CxxType: str):
        self.name = CxxType

    def dumpMembers(self) -> str:
        returnData: str = f"members of {self.name}:\n"
        for member in self.members:
            returnData += f"    {member.getName()}\n"
        return returnData
    