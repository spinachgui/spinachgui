import wx

def splitSymbol(symbol):
    """Split and isotope symbol such as H1 into a letter and a number such as (H,1)"""
    str="";
    num="";
    for c in symbol:
        if c.isalpha():
            str=str+c
        else:
            num=num+c
    return (str,num)

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



def lazyFloat(str):
    if(str==""):
        return 0.0
    else:
        return float(str);
