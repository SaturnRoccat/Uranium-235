from enum import Enum
from cxxHeaderHandling import *
from typing import Dict

class experimentals(Enum):
    holidayCreatorFeatures = "holidayCreatorFeatures"


    def toStr(self):
        return self.name
    
    def fromStr(self, value: str):
        return self[value]

def parseOutRefType(data: str) -> str:
    lastIndexOf = data.rindex("/")
    if lastIndexOf == -1:
        return ""
    endIndexOf = data.index(".json")
    if endIndexOf == -1:
        endIndexOf = len(data) # if it doesnt end with .json
    return data[lastIndexOf + 1:endIndexOf]

def parseOutExp(data: str) -> experimentals:
    lastIndexOf = data.rindex("/")
    if lastIndexOf == -1:
        return None
    
    if not data.endswith(".json"):
        return None
    data = data[lastIndexOf + 1:len(data) - 5]
    return experimentals[data]


def semverStringToNumber(semver: str):
    semverNoPrefix = semver[1:]
    semverSplit = semverNoPrefix.split(".")
    semverSplit.reverse()
    semverNumber = 0
    for index, value in enumerate(semverSplit):
        # bit shift by 16 bits
        semverNumber += int(value) << (index * 16)
    return semverNumber


def findHighestVersionKey(jsonData: dict, searchTypes: list[str], endsWithData: list[str] = ["/components/_main.json"]):
    versionKeys = []
    beginKey = "file:///data/packages/minecraftBedrock/schema/"
    for endData in endsWithData:
        for searchType in searchTypes:
            currentHighestVersionKey: str = ""
            currentHighestVersion:int = -1 
            for jsonDataKey in jsonData.keys():
                if jsonDataKey.startswith(beginKey + searchType) and jsonDataKey.endswith(endData):
                    semverString: str = jsonDataKey[len(beginKey + searchType):]
                    if not semverString.startswith("v"):
                        continue
                    try:
                        semverString = semverString[:semverString.index("/")]
                    except:
                        continue
                    if semverStringToNumber(semverString) > currentHighestVersion:
                        currentHighestVersionKey = jsonDataKey
                        currentHighestVersion = semverStringToNumber(semverString)

            if currentHighestVersionKey != "":
                versionKeys.append(currentHighestVersionKey)

    return versionKeys


def pullComponentData(jsonData: dict, keyList: list[str], listNames: list[str] = []):
    if listNames == []:
        listNames = keyList
    componentData: list[list[dict]] = []
    for key in keyList:
        thenData: dict
        experimentalsData: list[experimentals] = []
        data: dict = jsonData[key]
        allOfArray: list[dict] = {}
        localComponentData: list[dict] = []
        if "allOf" in data:
            allOfArray = data["allOf"]
        else:
            allOfArray = [data]
        for allOf in allOfArray:
            if "$ref" in allOf:
                refData: str = allOf["$ref"]
                exp = parseOutExp(refData)
                experimentalsData.append(exp)
                allOf = allOf["then"]

            thenData = allOf["properties"]
            localComponentData.append(thenData)
        componentData.append(localComponentData)
    return componentData


def generateComponentKeys(jsonData: list[dict], longPrefix: str, shortPrefix: str) -> list[str]:
    keys: list[str] = []
    for jsonSection in jsonData:
        for data in jsonSection:
            rawRefData: str = jsonSection[data]["$ref"] # scuffed
            if rawRefData.startswith("./"):
                keys.append(longPrefix + rawRefData[2:])
            elif rawRefData.startswith("../../"):
                keys.append(shortPrefix + rawRefData[6:])
            else:
                print("Unknown ref data: " + rawRefData)
                pass
    return keys            

def keyToComponentName(key: str) -> str:
    lastIndexOf = key.rindex("/")
    if lastIndexOf == -1:
        return ""
    endIndexOf = key.index(".json")
    if endIndexOf == -1:
        endIndexOf = len(key) # if it doesnt end with .json
    return key[lastIndexOf + 1:endIndexOf]
