from PyQt5.Qt import *
import gc



class UI(QLineEdit):
    
    def __init__(self, parent = None):
        super(UI,self).__init__(parent)
        print("__init__")
        self.counter = 0

    def find_current_canvas(self):
        print("find_current_canvas()")
        try:
            app = Krita.instance()
            q_window = app.activeWindow().qwindow()
            q_stacked_widget = q_window.centralWidget()
            q_mdi_area = q_stacked_widget.currentWidget()
            q_mdi_sub_window = q_mdi_area.currentSubWindow()
            view = q_mdi_sub_window.widget()
            for c in view.children():
                if c.metaObject().className() == 'KisCanvasController':
                    viewport = c.viewport()
                    canvas = viewport.findChild(QWidget)
                    return canvas
            return None
        except:
            return None


    def eventFilter(self, object, event):


        if event.type() == QEvent.MouseButtonPress:
            print("mouse event received")
            if event.button() == Qt.LeftButton:
                print("right mouse with shift pressed")
                self.counter = self.counter +1
                self.setText("" + str(self.counter))

        return super().eventFilter(object, event)
        
    def __del__(self):
        print('Destructor called')
        
        
    def initEF(self):
        print("initEF")
        myCanvas = self.find_current_canvas()
        

        QCoreApplication.instance().installEventFilter(self)
        #myCanvas.installEventFilter(self)
            

inze = UI()
inze.initEF()


inze.resize(128,64)
inze.show()
