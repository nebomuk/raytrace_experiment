from krita import *

from PyQt5.Qt import *
from sip import unwrapinstance

currentDocument=Krita.instance().activeDocument()


# Copies your template image in our krita tileset project
pixelBytes = currentDocument.pixelData(0, 0, currentDocument.width(), currentDocument.height())

# Resize it through QImage
imageData = QImage(pixelBytes, currentDocument.width(), currentDocument.height(), QImage.Format_ARGB32)
imageData = imageData.mirrored()

# Creates a layer for your image copy
root = currentDocument.rootNode()
 
newLayer = currentDocument.createNode("mirroredImage", "paintlayer")
root.addChildNode(newLayer, None)
ptr = imageData.bits();
ptr.setsize(imageData.byteCount())
# FIXME construct a QByteArray::fromRawData in C++ from the C++ modified image and pass it to python because ptr.asstring(), causes a deep copy
newLayer.setPixelData(QByteArray.fromRawData(ptr.asstring()),0,0,currentDocument.width(), currentDocument.height())

currentDocument.refreshProjection()

