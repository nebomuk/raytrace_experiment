from krita import *

from PyQt5 import QtCore
from PyQt5.QtCore import QObject
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QDir, QPoint
from PyQt5.QtGui import QImage
from ctypes import *
import _ctypes
from sip import unwrapinstance


# .so file must be renamed every time as a workaround because CDLL/LoadLibrary cache it and will not reload it until Krita is restarted
libraryPath = QDir.homePath() + "/b/build-raytrace_experiment-Desktop_Qt_5_12_12_GCC_64bit-Release/libraytrace_experiment.so"
plib = CDLL(libraryPath)

boj = QObject()

currentDocument=Krita.instance().activeDocument()


# Copies your template image in our krita tileset project
pixelBytes = currentDocument.pixelData(0, 0, currentDocument.width(), currentDocument.height())

imageData = QImage(pixelBytes, currentDocument.width(), currentDocument.height(), QImage.Format_ARGB32)

boy = QObject()
boy.setProperty("image",imageData)
boy.setProperty("startPoint",QPoint(200,200))

unwrapped = unwrapinstance(boy)
plib.push_qobject(c_void_p(unwrapped))


filledImage = boy.property("filledImage")

#Creates a layer for your image copy
root = currentDocument.rootNode()
 
newLayer = currentDocument.createNode("filledImage", "paintlayer")
root.addChildNode(newLayer, None)
ptr = filledImage.bits();
ptr.setsize(imageData.byteCount())
# FIXME construct a QByteArray::fromRawData in C++ from the C++ modified image and pass it to python because ptr.asstring(), causes a deep copy
newLayer.setPixelData(QByteArray.fromRawData(ptr.asstring()),0,0,currentDocument.width(), currentDocument.height())

currentDocument.refreshProjection()


# see https://stackoverflow.com/questions/359498/how-can-i-unload-a-dll-using-ctypes-in-python 
# for non-Linux solutions


def ctypesCloseLibrary(lib):
    dlclose_func = CDLL(None).dlclose
    dlclose_func.argtypes = [c_void_p]
    dlclose_func.restype = c_int

    dlclose_func(lib._handle)


ctypesCloseLibrary(plib)
del plib