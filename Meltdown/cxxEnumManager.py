from cxxHeaderHandling import CxxEnum

enumRegistry: dict[str, CxxEnum] = {}

def RegisterEnum(enum: CxxEnum):
    if enum.name not in enumRegistry:
        enumRegistry[enum.name] = enum

def DoesEnumExist(name: str) -> bool:
    return name in enumRegistry

def GetEnum(name: str) -> CxxEnum | None:
    if DoesEnumExist(name):
        return enumRegistry[name]
    else:
        return None