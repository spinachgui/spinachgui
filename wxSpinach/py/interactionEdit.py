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

TypeOrders=[SCALAR,MATRIX,EIGENVALUES,AXRHOM,SPANSKEW];

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

class InteractionEdit(wx.Panel):
    def __init__(self,parent,inter,id=-1):
        self.inter=inter;
        print "hi"
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,'InterEditPanel');
        self.PostCreate(pre)

        self.loading=False;

        self.typeChoiceBook=xrc.XRCCTRL(self,"TypeChoiceBook");
        self.Bind(wx.EVT_CHOICEBOOK_PAGE_CHANGED,self.OnPageChange);

        self.scalar=xrc.XRCCTRL(self,'scalarCtrl');

        self.matxx=xrc.XRCCTRL(self,'matxx');
        self.matxy=xrc.XRCCTRL(self,'matxy');
        self.matxz=xrc.XRCCTRL(self,'matxz');
                                    
        self.matyx=xrc.XRCCTRL(self,'matyx');
        self.matyy=xrc.XRCCTRL(self,'matyy');
        self.matyz=xrc.XRCCTRL(self,'matyz');
             
        self.matzx=xrc.XRCCTRL(self,'matzx');
        self.matzy=xrc.XRCCTRL(self,'matzy');
        self.matzz=xrc.XRCCTRL(self,'matzz');

        self.eigenxx=xrc.XRCCTRL(self,'eigenxx');
        self.eigenyy=xrc.XRCCTRL(self,'eigenyy');
        self.eigenzz=xrc.XRCCTRL(self,'eigenzz');

        self.ax=xrc.XRCCTRL(self,'ax');
        self.rhom=xrc.XRCCTRL(self,'rhom');
        self.axrhomiso=xrc.XRCCTRL(self,'axrhomiso');

        self.span=xrc.XRCCTRL(self,'span')
        self.skew=xrc.XRCCTRL(self,'skew')
        self.spanskewiso=xrc.XRCCTRL(self,'spanskewiso')

        self.quadCheckbox=xrc.XRCCTRL(self,"quadCheckbox");
        self.Bind(wx.EVT_CHECKBOX,self.onQuadChecked,id=self.quadCheckbox.GetId());

        self.LoadFromInter()

        self.editCtrls=[]
        for ctrlName in Types[self.inter.GetType()][3]:
            ctrl=xrc.XRCCTRL(self,ctrlName)
            ctrl.SetValidator(NumericValidator());
            self.editCtrls.append(ctrl);
            self.Bind(wx.EVT_TEXT,self.onTextChange)

    def SetInter(self,inter):
        self.inter=inter;
        self.LoadFromInter();

    def onQuadChecked(self,e):
        if e.IsChecked():
            self.inter.SetQuadratic();
        else:
            self.inter.SetLinear();
        self.GetParent().UpdateListBox();

    def OnPageChange(self,e):
        if self.Loading:
            print "Skipping"
            return;
        print "Changing the type of this interaction"
        type=TypeOrders[e.GetSelection()]
        if(type==SCALAR):
            self.scalar.SetValue("0.0");
        elif(type==MATRIX):
            self.matxx.SetValue("0.0");
            self.matxy.SetValue("0.0");
            self.matxz.SetValue("0.0");
                                
            self.matyx.SetValue("0.0");
            self.matyy.SetValue("0.0");
            self.matyz.SetValue("0.0");
                                
            self.matzx.SetValue("0.0");
            self.matzy.SetValue("0.0");
            self.matzz.SetValue("0.0");
        elif(type==EIGENVALUES):
            self.eigenxx.SetValue("0.0");
            self.eigenyy.SetValue("0.0");
            self.eigenzz.SetValue("0.0");
        elif(type==AXRHOM):
            self.ax.SetValue("0.0");
            self.rhom.SetValue("0.0");
            self.axrhomiso.SetValue("0.0");
        elif(type==SPANSKEW):
            self.span.SetValue("0.0");
            self.skew.SetValue("0.0");
            self.spanskewiso.SetValue("0.0");
        self.SaveToInter();        
        e.Skip()



    def LoadFromInter(self):
        self.Loading=True
        self.typeChoiceBook.SetSelection(Types[self.inter.GetType()][0]);
        if(self.inter.GetType()==SCALAR):
            self.scalar.SetValue(str(self.inter.GetScalar()));
        elif(self.inter.GetType()==MATRIX):

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

        elif(self.inter.GetType()==EIGENVALUES):
            xx,yy,zz,o=self.inter.GetEigenvalues();
            print xx,yy,zz
            self.eigenxx.SetValue(str(xx));
            self.eigenyy.SetValue(str(yy));
            self.eigenzz.SetValue(str(zz));
        elif(self.inter.GetType()==AXRHOM):
            ax,rhom,iso,o=self.inter.GetAxRhom();
            self.ax.SetValue(str(ax));
            self.rhom.SetValue(str(rhom));
            self.axrhomiso.SetValue(str(iso));
        elif(self.inter.GetType()==SPANSKEW):
            span,skew,iso,o=self.inter.GetSpanSkew();
            self.span.SetValue(str(span));
            self.skew.SetValue(str(skew));
            self.spanskewiso.SetValue(str(iso));

        self.quadCheckbox.SetValue(self.inter.GetIsQuadratic());

        self.Loading=False;

    def SaveToInter(self):
        Type=TypeOrders[self.typeChoiceBook.GetSelection()];
        if(Type==SCALAR):
            self.inter.SetScalar(lazyFloat(self.scalar.GetValue()));
        elif(Type==MATRIX):
            self.inter.SetMatrix(spinsys.Matrix3(lazyFloat(self.matxx.GetValue()),
                                                 lazyFloat(self.matxy.GetValue()),
                                                 lazyFloat(self.matxz.GetValue()),
                                                      
                                                 lazyFloat(self.matyx.GetValue()),
                                                 lazyFloat(self.matyy.GetValue()),
                                                 lazyFloat(self.matyz.GetValue()),
                                                      
                                                 lazyFloat(self.matzx.GetValue()),
                                                 lazyFloat(self.matzy.GetValue()),
                                                 lazyFloat(self.matzz.GetValue())));


        elif(Type==EIGENVALUES):
            xx=lazyFloat(self.eigenxx.GetValue());
            yy=lazyFloat(self.eigenyy.GetValue());
            zz=lazyFloat(self.eigenzz.GetValue());
            self.inter.SetEigenvalues(xx,yy,zz,spinsys.Orientation())
        elif(Type==AXRHOM):
            ax=lazyFloat(self.ax.GetValue());
            rhom=lazyFloat(self.rhom.GetValue());
            axrhomiso=lazyFloat(self.axrhomiso.GetValue());
            self.inter.SetAxRhom(ax,rhom,axrhomiso,spinsys.Orientation());
        elif(Type==SPANSKEW):
            span=lazyFloat(self.span.GetValue());
            skew=lazyFloat(self.skew.GetValue());
            spanskewiso=lazyFloat(self.spanskewiso.GetValue());
            self.inter.SetSpanSkew(span,skew,spanskewiso,spinsys.Orientation());

    def onTextChange(self,e):
        if self.Loading: #If we are loading from a spin we should ignore this event
            return
        self.SaveToInter();
        self.inter.kind="modifed"
        self.GetParent().UpdateListBox();
        self.GetParent().interListBox.SetSelection(self.inter.index);

        return;

class InterPopup(wx.Frame):
    """Adds a bit of text and mouse movement to the wx.PopupWindow"""
    def __init__(self, parent,spin):
        self.spin=spin

        wx.Frame.__init__(self, parent,style=wx.FRAME_FLOAT_ON_PARENT|wx.FRAME_NO_TASKBAR)

        self.sizer=wx.BoxSizer()
        self.interPanel=SpinInteractionsEdit(self,self.spin)
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
    def __init__(self,parent,spin,id=-1):
        wx.TextCtrl.__init__(self,parent,id)
        self.spin=spin

#        self.win = InterPopup(self,self.spin)

       # self.Bind(wx.EVT_KILL_FOCUS, self.onUnFocus, self)
       # self.Bind(wx.EVT_SET_FOCUS, self.onFocus, self)

    #def onUnFocus(self,e):
    #    print e.GetWindow();
        #self.win.Show(False);
    #    e.Skip()

    #def onFocus(self, e):

        # Show the popup right below or above the button
        # depending on available screen space...
   #     pos = self.ClientToScreen(self.GetPosition())
   #     size =  self.GetSize()
   #     self.win.SetPosition((pos[0], pos[1]+size[1]))

   #     self.win.Show(True);
        #e.Skip()



class SpinInteractionsEdit(wx.Panel):
    """"Edits all the interactios assocated with a spin"""
    def __init__(self,parent,spin,id=-1):
        wx.Panel.__init__(self,parent,id=-1)
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"SpinInterEditPanel");
        self.PostCreate(pre);

        self.spin=spin

        self.Updateing=False

        self.interListBox=xrc.XRCCTRL(self,"interEditCtrl");
        self.interListBox.SetMinSize(wx.Size(-1,200));

        self.interEdit=InteractionEdit(self,self.spin.GetInteraction(0))
        self.GetSizer().Add(self.interEdit,1.0,wx.EXPAND)

        self.interListBox.Bind(wx.EVT_LISTBOX,self.onInteractionChange);
        self.Bind(wx.EVT_BUTTON,self.onNewButton,id=xrc.XRCCTRL(self,"addButton").GetId());
        self.Bind(wx.EVT_BUTTON,self.onDeleteButton,id=xrc.XRCCTRL(self,"deleteButton").GetId());

        self.LoadFromSpin();

    def onNewButton(self,e):
        self.CreateNew();

    def onDeleteButton(self,e):
        self.MarkDeleted(self.interListBox.GetSelection());

    def LoadFromSpin(self):
        self.oldInteractions=self.spin.GetInteractions();  #Unmodified reference copy
        self.interactions=[];                              #Working copy
        for oldInter in self.oldInteractions:              #Make sure all the interactions here are copies
            self.interactions.append(spinsys.Interaction(oldInter));
        for inter in self.interactions:
            inter.kind="original";
        self.UpdateListBox();
        self.interListBox.SetSelection(0);
        self.interactionChange()

    def UpdateListBox(self):
        self.Updating=True;
        self.interListBox.Clear();
        i=0;
        for inter in self.interactions:
            inter.index=i
            if inter.GetIsQuadratic():
                quadstr="Quadratic"
            else:
                quadstr="Linear"
            if inter.kind == "original":
                self.interListBox.Append(SubTypes[inter.GetSubType()][1] + " ("+quadstr+", "+Types[inter.GetType()][1]+")")
            elif inter.kind == "modifed":
                self.interListBox.Append(SubTypes[inter.GetSubType()][1] + " ("+quadstr+", "+Types[inter.GetType()][1]+")*")                
            elif inter.kind == "new":
                self.interListBox.Append(SubTypes[inter.GetSubType()][1] + " ("+quadstr+", "+Types[inter.GetType()][1]+")*")
            i=i+1
        self.Updating=False;


    def MarkDeleted(self,n):
        del self.interactions[n]
        self.UpdateListBox();
        self.interListBox.SetSelection(0);
        self.interactionChange()

    def CreateNew(self):
        inter=spinsys.Interaction();
        inter.SetScalar(0.0);
        inter.SetSpin1(self.spin);
        inter.kind="new"
        self.interactions+=(inter,);
        self.UpdateListBox();
        self.interListBox.SetSelection(self.interListBox.Count-1);
        self.interactionChange()

    def SaveToSpin(self):
        for inter in self.oldInteractions: #Remove deleated interactions
            self.spin.RemoveInteraction(inter);

        for inter in self.interactions:
            self.spin.InsertInteraction(inter);
            print "adding and interaction"
            
       
    def onInteractionChange(self,e):
        if self.Updating:
            print "Sking the update";
            return;
        print "Updating...."
        self.interactionChange();

    def interactionChange(self):
        inter=self.interactions[self.interListBox.GetSelection()];
        self.interEdit.SetInter(inter);


