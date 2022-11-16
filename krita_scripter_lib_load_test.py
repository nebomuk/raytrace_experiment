from PyQt5.QtCore import QLibrary
from PyQt5 import QtCore
from PyQt5.QtCore import QObject
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QDir
from ctypes import *
from sip import unwrapinstance

# .so file must be renamed every time as a workaround because CDLL/LoadLibrary cache it and will not reload it until Krita is restarted
libraryPath = QDir.homePath() + "/b/build-raytrace_experiment-Desktop_Qt_5_12_12_GCC_64bit-Release/libraytrace_experiment5.so"
lib = QLibrary(libraryPath)
plib = CDLL(libraryPath)
lib.load()
print(lib.isLoaded())

boj = QObject()
object_methods = [method_name for method_name in dir(boj)
                  if callable(getattr(boj, method_name))]

#print(object_methods)


print(QApplication.instance().setProperty("test","val"))

# TODO call library function with return type pointer to void* with python LoadLibrary or CDLL and try wrapInstance, 
# void* to QVariant or anything else
# or use QObject or QApplication (globally) or anythnig to store the QPolygon as QVariant

boy = QObject()
#somev = lib.resolve("calc")
unwrapped = unwrapinstance(boy)
print(unwrapped)
print(plib.calc(c_void_p(unwrapped)))

print(boy.property("test"))
poly = boy.property("vartest") # property QVariant is automaticaly unwrapped to the contained object (without .value())

print(poly.first())
