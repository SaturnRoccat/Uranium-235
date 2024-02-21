from cxxHeaderHandling import CxxType, CxxClass
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
    if GuessedType == memberType.UNK:
        raise TypeError(f"Could not determine type of member {memberName}")
    return GuessedType

def handleEnum(localData: dict, CxxTypeData: CxxType, globalJson: dict, memberName: str):
    pass


def handleObject(localData: dict, CxxClassData: CxxClass, globalJson: dict) -> CxxClass:
    for member in localData["properties"]:
        MemberData: dict = localData["properties"][member]
        MemberType: memberType = calculateMemberType(member, MemberData)
        CxxTypeData: CxxType = CxxType(member)
        if MemberType == memberType.Enum:
            pass
            

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
        CxxClassData = handleObject(localJson, CxxClassData, globalJson)
    elif localJson["type"] == "array":
        pass
    else: 
        CxxTypeData: CxxType = CxxType(name)
        CxxTypeData.setType(localJson["type"])
        CxxClassData.addMember(CxxTypeData)
    CxxClassData.setValid(True)
    print(CxxClassData.dumpMembers())
    return CxxClassData
