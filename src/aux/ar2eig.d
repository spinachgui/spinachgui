
import std.stdio;
import std.conv;
import std.typetuple;


double[3] ar2eig(double[3] input) {
    double iso = input[0];
    double a   = input[1];
    double r   = input[2];
    double xx=iso - a/6.0 + r/2.0;
    double yy=iso - a/6.0 - r/2.0;
    double zz=iso + a/3.0;
    return [xx,yy,zz];
}

double[3] eig2ar(double[3] input) {
    double xx = input[0];
    double yy = input[1];
    double zz = input[2];

    //Sort so that xx <= yy <= zz
    double tmp;
    //if(zz < yy) tmp=zz; zz=yy; yy=tmp;
    //if(zz < xx) tmp=zz; zz=xx; xx=tmp; //Now zz is definatly bigest
    //if(yy < xx) tmp=xx; xx=yy; yy=tmp;

    double iso = (xx+yy+zz)/3.0;
    double ax=2*zz -  xx - yy;
    double rh=xx - yy;
    return [iso,ax,rh];
}

void main() {
    writeln(to!string(eig2ar([1,2,3]),"(",",",")"));
    writeln(to!string(ar2eig(eig2ar([1,2,3])),
                      "(",",",")"));
}


