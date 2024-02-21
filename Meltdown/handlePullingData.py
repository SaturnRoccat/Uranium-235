from cxxHeaderHandling import CxxType, CxxClass, CxxEnum
from cxxEnumManager import RegisterEnum, DoesEnumExist
from jsonhandling import keyToComponentName
from enum import Enum

#Q: what is the easiest way to throw an error in python?
#A: raise Exception("message")

class memberType(Enum):
    Normal = "Normal"
    Object = "Object"
    Array = "Array"
    Enum = "Enum"
    UNK = "UNK"


def calculateMemberType(memberName: str, memberData: dict) -> memberType:
    GuessedType: memberType = memberType.Normal
    if memberName == "type":
        if "enum" in memberData:
            GuessedType = memberType.Enum
        else:
            GuessedType = memberType.UNK
    elif "enum" in memberData:
        GuessedType = memberType.Enum
    if GuessedType == memberType.UNK:
        raise TypeError(f"Could not determine type of member {memberName}")
    return GuessedType

def handleEnum(localData: dict, CxxTypeData: CxxType, memberName: str, extraMemberNameData: str) -> str:
    enumType: str = localData["type"]
    values: list[str] = []
    enumName: str = ""

    if memberName == "type" or "enum" in localData:
        if enumType == "string":
            values = localData["enum"]
        # FIXME:  Not handling anything other than string enums
        else:
            raise TypeError(f"Could not determine enum type of member {memberName} in handleEnum") 
        if memberName == "type":
            enumName = extraMemberNameData + "Enum"
            CxxTypeData.setName(extraMemberNameData)
        else:
            enumName = memberName            
    else:
        raise TypeError(f"Could not determine enum type of member {memberName} in handleEnum")
    
    if not DoesEnumExist(enumName):
        CxxEnumData: CxxEnum
        if enumType == "string":
            CxxEnumData= CxxEnum(enumName, values, enumType, values)
        else:
            CxxEnumData = CxxEnum(enumName, values, enumType)
        RegisterEnum(CxxEnumData)
    CxxTypeData.setType(enumName)
    


def handleObject(localData: dict, CxxClassData: CxxClass, globalJson: dict, objName: str) -> CxxClass:
    for member in localData["properties"]:
        MemberData: dict = localData["properties"][member]
        MemberType: memberType = calculateMemberType(member, MemberData)
        CxxTypeData: CxxType = CxxType(member)
        if MemberType == memberType.Enum:
            handleEnum(MemberData, CxxTypeData, member, objName)

        # TODO: Handle Type Data Of Class


        CxxClassData.addMember(CxxTypeData)

    return CxxClassData


def parseOutComponentCxxClass(name: str, globalJson: dict, localJson: dict, parent: str = "") -> CxxClass:
    CxxClassData: CxxClass = CxxClass(name, parent=parent)
    CxxClassData.setValid(False)
    if "doNotSuggest" in localJson:
        return CxxClassData
    if "type" not in localJson:
        return CxxClassData
    if localJson["type"] == "object":
        CxxClassData = handleObject(localJson, CxxClassData, globalJson, name)
    elif localJson["type"] == "array":
        pass
    else: 
        CxxTypeData: CxxType = CxxType(name)
        CxxTypeData.setType(localJson["type"])
        CxxClassData.addMember(CxxTypeData)
    CxxClassData.setValid(True)
    print(CxxClassData.dumpMembers())
    return CxxClassData
