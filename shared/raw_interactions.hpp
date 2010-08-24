
#include <shared/orientation.hpp>
#include <shared/unit.hpp>

namespace SpinXML {

    struct Eigenvalues;
    struct AxRhom;
    struct SpanSkew;

	///============================================================//
	struct Eigenvalues {
		Eigenvalues(const energy _XX,const energy _YY,const energy _ZZ, const Orientation& o) 
			: xx(_XX), yy(_YY), zz(_ZZ), mOrient(o) {
		}
		AxRhom AsAxRhom() const;
		SpanSkew AsSpanSkew() const;
		energy xx;
		energy yy;
		energy zz;
		Orientation mOrient;
	};

	///============================================================//
	struct AxRhom {
		AxRhom(const energy _iso,const energy _ax,const energy _rh, const Orientation& o) 
			: iso(_iso), ax(_ax), rh(_rh), mOrient(o) {
		}
		Eigenvalues AsEigenvalues() const;
		SpanSkew AsSpanSkew() const;
		energy iso;
		energy ax;
		energy rh;
		Orientation mOrient;
	};

	///============================================================//
	struct SpanSkew {
		SpanSkew(const energy _iso,const energy _span,const double _skew, const Orientation& o) 
			: iso(_iso), span(_span), skew(_skew), mOrient(o) {
		}
		Eigenvalues AsEigenvalues() const;
		AxRhom AsAxRhom() const;
		energy iso;
		energy span;
		double skew;
		Orientation mOrient;
	};


};
