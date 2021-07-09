import sys, os, inspect
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0, parentdir)
import json


class FromMicrocontrollerBaseMessage:
    def __init__(self, paramToPropertyList, params) -> None:
        paramToPropertyList['I'] = '__MessageId'
        self.paramToPropertyList = paramToPropertyList
        for param in params:
            if not param[0] in paramToPropertyList:
                continue

            prop = paramToPropertyList[param[0]]
            
            value = float(param[1:])
            setattr(self, prop, value)

    @property
    def MessageId(self) -> int:
        return self.__MessageId
    
    def _getParam(self, param):
        if hasattr(self.paramToPropertyList[param]):
            return getattr(self,self.paramToPropertyList[param])
        
        return None

    def toJson(self):
        dict = {}
        for param in self.paramToPropertyList:
            if hasattr(self, self.paramToPropertyList[param]):
                dict[self.paramToPropertyList[param].replace('__','')] = getattr(self, self.paramToPropertyList[param])
        return json.dumps(dict)