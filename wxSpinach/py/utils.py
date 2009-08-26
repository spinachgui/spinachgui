

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



def lazyFloat(str):
    if(str==""):
        return 0.0
    else:
        return float(str);
