#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid
import wx.lib.popupctl
import  wx.calendar     as  cal



#C++ Imports
from nuclear_data import *
import spinsys

#Local python imports
from utils import *
from orientationEdit import OrientTextEditor


UNDEFINED=spinsys.Interaction.UNDEFINED;  
SCALAR=spinsys.Interaction.SCALAR;     
MATRIX=spinsys.Interaction.MATRIX;     
EIGENVALUES=spinsys.Interaction.EIGENVALUES;
AXRHOM=spinsys.Interaction.AXRHOM;     
SPANSKEW=spinsys.Interaction.SPANSKEW;   

TypeOders=[SCALAR,MATRIX,EIGENVALUES,AXRHOM,SPANSKEW];

#Choice book page,Humand readable name,panel name,list of textctrl names

matrixEdits=['matxx','matxy','matxz',
             'matyx','matyy','matyz',
             'matzx','matzy','matzz'];

Types={
    UNDEFINED:  (-1,"Undefined",""     ,""                ,[]                             ),
    SCALAR:     (0,"Scalar"            ,"ScalarIntPanel"  ,['scalarCtrl']                 ),               
    MATRIX:     (1,"Matrix"            ,"MatrixIntPanel"  ,matrixEdits                    ),
    EIGENVALUES:(2,"Eigenvalues"       ,"EigenIntPanel"   ,['eigenxx','eigenyy','eigenzz']),
    AXRHOM:     (3,"Axiality-Rhombicty","AxRhomIntPanel"  ,['ax','rhom','axrhomiso']       ),     
    SPANSKEW:   (4,"Span-Skew"         ,"SpanSkewIntPanel",['span','skew','spanskewiso']  )
}


ST_ANY        =spinsys.Interaction.ST_ANY;
ST_NMR        =spinsys.Interaction.ST_NMR;       
ST_EPR        =spinsys.Interaction.ST_EPR;       
ST_HFC        =spinsys.Interaction.ST_HFC;       
ST_G_TENSER   =spinsys.Interaction.ST_G_TENSER;
ST_ZFS        =spinsys.Interaction.ST_ZFS;   
ST_EXCHANGE   =spinsys.Interaction.ST_EXCHANGE;
ST_SHIELDING  =spinsys.Interaction.ST_SHIELDING;
ST_SCALAR     =spinsys.Interaction.ST_SCALAR; 
ST_QUADRUPOLAR=spinsys.Interaction.ST_QUADRUPOLAR;
ST_DIPOLAR    =spinsys.Interaction.ST_DIPOLAR;
ST_CUSTOM     =spinsys.Interaction.ST_CUSTOM;   

SubTypes={
    ST_ANY:       (-3,"Any"),
    ST_NMR:       (-2,"NMR"),
    ST_EPR:       (-1,"EPR"),
    ST_HFC:       (0,"Hyperfine"),
    ST_G_TENSER:  (1,"G Tensor"),
    ST_ZFS:       (2,"Zero Field Splitting"),
    ST_EXCHANGE:  (3,"Exchange Coupling"),
    ST_SHIELDING: (4,"Chemical Shilding"),
    ST_SCALAR:    (5,"Scalar"),
    ST_QUADRUPOLAR:(6,"Quadrupolar"),
    ST_DIPOLAR:   (7,"Dipolar"),
    ST_CUSTOM:    (8,"Custem")
}

class NumericValidator(wx.PyValidator):
    def __init__(self):
        wx.PyValidator.__init__(self)
        self.Bind(wx.EVT_CHAR, self.OnChar)

    def Clone(self):
        return NumericValidator()

    def Validate(self, win):
        tc = self.GetWindow()
        val = tc.GetValue()
        
        for x in val:
             if x not in '0123456789.':
                 return False

        return True


    def OnChar(self, event):
        key = event.GetKeyCode()

        if key < wx.WXK_SPACE or key == wx.WXK_DELETE or key > 255:
            event.Skip()
            return

        if chr(key) in '0123456789.':
            val=self.GetWindow().GetValue();
            if (not '.' in val) or chr(key)!='.':
                event.Skip()
                return

        # Returning without calling even.Skip eats the event before it
        # gets to the text control
        return

class RepEdit(wx.Panel):
    def __init__(self,parent,inter,repType,id=-1):
        self.inter=inter;
        self.repType=repType;

        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,Types[self.repType][2]);
        self.PostCreate(pre)

        self.editCtrls=[]
        for ctrlName in Types[self.repType][3]:
            ctrl=xrc.XRCCTRL(self,ctrlName)
            ctrl.SetValidator(NumericValidator());
            self.editCtrls.append(ctrl);
            self.Bind(wx.EVT_TEXT,self.onTextChange)

        if(self.repType==SCALAR):
            self.scalar=xrc.XRCCTRL(self,'scalarCtrl');
        elif(self.repType==MATRIX):
            self.matxx=xrc.XRCCTRL(self,'matxx');
            self.matxy=xrc.XRCCTRL(self,'matxy');
            self.matxz=xrc.XRCCTRL(self,'matxz');
                                        
            self.matyx=xrc.XRCCTRL(self,'matyx');
            self.matyy=xrc.XRCCTRL(self,'matyy');
            self.matyz=xrc.XRCCTRL(self,'matyz');
                 
            self.matzx=xrc.XRCCTRL(self,'matzx');
            self.matzy=xrc.XRCCTRL(self,'matzy');
            self.matzz=xrc.XRCCTRL(self,'matzz');
        elif(self.repType==EIGENVALUES):
            self.eigenxx=xrc.XRCCTRL(self,'eigenxx');
            self.eigenyy=xrc.XRCCTRL(self,'eigenyy');
            self.eigenzz=xrc.XRCCTRL(self,'eigenzz');
        elif(self.repType==AXRHOM):
            self.ax=xrc.XRCCTRL(self,'ax');
            self.rhom=xrc.XRCCTRL(self,'rhom');
            self.axrhomiso=xrc.XRCCTRL(self,'axrhomiso');
        elif(self.repType==SPANSKEW):
            self.span=xrc.XRCCTRL(self,'span')
            self.skew=xrc.XRCCTRL(self,'skew')
            self.spanskewiso=xrc.XRCCTRL(self,'spanskewiso')

        self.LoadFromInter()


    def LoadFromInter(self):
        if(self.repType!=self.inter.GetType()):
            #The interaction is of the wrong type, we don't have the GUI elements set up 
            #to display it, so just ignore this request.
            return;
        if(self.repType==SCALAR):
            self.scalar.SetValue(str(self.inter.GetScalar()));
        elif(self.repType==MATRIX):

            mat=self.inter.GetMatrix()
            print mat
            self.matxx.SetValue(str(mat.Get(0,0)));
            self.matxy.SetValue(str(mat.Get(0,1)));
            self.matxz.SetValue(str(mat.Get(0,2)));
                                
            self.matyx.SetValue(str(mat.Get(1,0)));
            self.matyy.SetValue(str(mat.Get(1,1)));
            self.matyz.SetValue(str(mat.Get(1,2)));
                                    
            self.matzx.SetValue(str(mat.Get(2,0)));
            self.matzy.SetValue(str(mat.Get(2,1)));
            self.matzz.SetValue(str(mat.Get(2,2)));

        elif(self.repType==EIGENVALUES):
            xx,yy,zz,o=self.inter.GetEigenValues();
            print xx,yy,zz
            self.eigenxx.SetValue(str(xx));
            self.eigenyy.SetValue(str(yy));
            self.eigenzz.SetValue(str(zz));
        elif(self.repType==AXRHOM):
            ax,rhom,iso,o=self.inter.GetAxRhom();
        elif(self.repType==SPANSKEW):
            span,skew,iso,o=self.inter.GetSpanSkew();

    def onTextChange(self,e):
        if(self.repType==SCALAR):
            self.inter.SetScalar(float(self.scalar.GetValue()));
        elif(self.repType==MATRIX):
            self.inter.SetMatrix(spinsys.Matrix3(float(self.matxx.GetValue()),
                                                 float(self.matxy.GetValue()),
                                                 float(self.matxz.GetValue()),
                                                      
                                                 float(self.matyx.GetValue()),
                                                 float(self.matyy.GetValue()),
                                                 float(self.matyz.GetValue()),
                                                      
                                                 float(self.matzx.GetValue()),
                                                 float(self.matzy.GetValue()),
                                                 float(self.matzz.GetValue())));
        elif(self.repType==EIGENVALUES):
            self.inter.SetEigenValues(float(self.eigenxx.GetValue()),
                                      float(self.eigenyy.GetValue()),
                                      float(self.eigenzz.GetValue()),
                                      spinsys.Orientation());
        elif(self.repType==AXRHOM):
            self.inter.SetAxRhom(float(self.ax.GetValue()),
                                 float(self.rhom.GetValue()),
                                 float(self.axrhomiso.GetValue()),
                                 spinsys.Orientation());
        elif(self.repType==SPANSKEW):
            self.inter.SetSpanSkew(float(self.span.GetValue()),
                                   float(self.skew.GetValue()),
                                   float(self.spanskewiso.GetValue()),
                                   spinsys.Orientation());
            
        


class InterPopup(wx.Frame):
    """Adds a bit of text and mouse movement to the wx.PopupWindow"""
    def __init__(self, parent):
        wx.Frame.__init__(self, parent,style=wx.FRAME_FLOAT_ON_PARENT|wx.FRAME_NO_TASKBAR)

        self.sizer=wx.BoxSizer()
        self.interPanel=interactionEdit(self)
        self.sizer.Add(self.interPanel,1.0,wx.EXPAND);
        self.SetSizer(self.sizer)
        

        size = self.interPanel.GetSize()
        self.SetSize((size.width, size.height));

        self.Bind(wx.EVT_KILL_FOCUS, self.onUnFocus, self)
        self.Bind(wx.EVT_SET_FOCUS, self.onFocus, self)

    def onUnFocus(self,e):
        print e.GetWindow();
        e.Skip();

    def onFocus(self,e):
        print e.GetWindow();
        e.Skip()



class InterTextEditor(wx.TextCtrl):
    def __init__(self,parent,id=-1):
        wx.TextCtrl.__init__(self,parent,id)

        self.win = InterPopup(self)

        self.Bind(wx.EVT_KILL_FOCUS, self.onUnFocus, self)
        self.Bind(wx.EVT_SET_FOCUS, self.onFocus, self)

    def onUnFocus(self,e):
        print e.GetWindow();
        #self.win.Show(False);
        e.Skip()

    def onFocus(self, e):

        # Show the popup right below or above the button
        # depending on available screen space...
        pos = self.ClientToScreen(self.GetPosition())
        size =  self.GetSize()
        self.win.SetPosition((pos[0], pos[1]+size[1]))

        self.win.Show(True);
        e.Skip()



class interactionEdit(wx.Panel):
    def __init__(self,parent,spin,id=-1):
        wx.Panel.__init__(self,parent,id=-1)
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"InterEditPanel");
        self.PostCreate(pre);

        self.spin=spin

        self.typeChoiceBook=xrc.XRCCTRL(self,"TypeChoiceBook");
        self.interListBox=xrc.XRCCTRL(self,"interEditCtrl");
        self.interListBox.SetMinSize(wx.Size(-1,200));

        self.pages=[]

        for T in TypeOders:
            page=RepEdit(self.typeChoiceBook,self.spin.GetInteraction(0),T)
            self.pages.append(page)
            self.typeChoiceBook.AddPage(page,Types[T][1]);

        self.interListBox.Bind(wx.EVT_LISTBOX,self.onInteractionChange);

        self.LoadFromSpin();

    def LoadFromSpin(self):
        self.interListBox.Clear();

        interactions=self.spin.GetInteractions();
        for inter in interactions:
            self.interListBox.Append(SubTypes[inter.GetSubType()][1] + " ("+Types[inter.GetType()][1]+")")
       
    def onInteractionChange(self,e):
        inter=self.spin.GetInteraction(e.GetSelection());
        self.typeChoiceBook.SetSelection(Types[inter.GetType()][0]);

