
#include <iostream>
#include <fstream>

#include <flens/flens.h>

#include <string.h>

#include <shared/xercesc.h>

using namespace flens;
using namespace std;
using namespace xercesc;

typedef GeMatrix<FullStorage<double, ColMajor> > MyMatrix;


int main(int argc,char** argv) {
    bool bFailed = false;

    // initialize the XML library.
	try {
      XMLPlatformUtils::Initialize();
	} catch (const XMLException &e) {
		cerr << "Couldn't initalise the XML parser" << endl;
		return 1;
	}	

	XercesDOMParser* parser = new XercesDOMParser;

    parser->setValidationScheme(XercesDOMParser::Val_Auto);
    parser->setDoNamespaces(false);
    parser->setDoSchema(false);

	// skip this if you haven't written your own error reporter class.
	//DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter;
	//parser->setErrorHandler(errReporter);

    //parser->setCreateEntityReferenceNodes(false);
    //parser->setToCreateXMLDeclTypeNode(true);
    try {
	  XMLCh* filename = XMLString::transcode("../data/spinsys.xml");
	  LocalFileInputSource source(filename);
	  XMLString::release(&filename);
      parser->parse(source);
      bFailed = parser->getErrorCount() != 0;
      if (bFailed) {
        cerr << "Parsing " << argv[1];
        cerr << " error count: " << parser->getErrorCount() << endl;
      }
    } catch (const DOMException& e) {
      cerr << "DOM Exception parsing ";
	  // was message provided?
	  if (e.msg){
		// yes: display it as ascii.
		char *strMsg = XMLString::transcode(e.msg);
        cerr << strMsg << endl;
		XMLString::release(&strMsg);
	  } else {
	    // no: just display the error code.
        cerr << e.code << std::endl;
	  }

	  bFailed = true;
	} catch (const XMLException& e) {
      cerr << "XML Exception parsing ";
      cerr << e.getMessage() << endl;
	  bFailed = true;
    } catch (const SAXException& e) {
      cerr << "SAX Exception parsing ";
      cerr << argv[1];
      cerr << " reports: ";
      cerr << e.getMessage() << endl;
	  bFailed = true;
    } catch (...) {
      cerr << "An exception parsing ";
      cerr << argv[1] << endl;
	  bFailed = true;
    }
    // did the input document parse okay?
    if (!bFailed) {
	  DOMNode *pDoc = parser->getDocument();
	  // insert code to do something with the DOM document here.
	}
	delete parser;

    XMLPlatformUtils::Terminate();







	MyMatrix A(4,4);

	A = 1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10,11,12,
		13,14,15,16;

	cout << A << endl;

	

    return 0;
}

