
#include <iostream>
#include <fstream>

#include <flens/flens.h>

#include <string.h>

#include <shared/xercesc.h>

using namespace flens;
using namespace std;
using namespace xercesc;

typedef GeMatrix<FullStorage<double, ColMajor> > MyMatrix;


class MyErrorHandler : public ErrorHandler {
public:
  void warning (const SAXParseException &exc) {
	printError(exc,"Warning");
  }
  void error (const SAXParseException &exc) {
	printError(exc,"Error");
  }
  void fatalError (const SAXParseException &exc) {
	printError(exc, "Bad Error");
  }
  void resetErrors() {}
protected:
  void printError(const SAXParseException& e,const char* type) {
    const XMLCh* msg=e.getMessage();
	char* realMsg=XMLString::transcode(msg);
	cerr << type << " (" << e.getLineNumber() << "): " << realMsg << endl;
	XMLString::release(&realMsg);
  }

};

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
    //parser->setExternalSchemaLocation("../data/spinsys_spec.xsd");
    //parser->setValidationScheme(AbstractDOMParser::Val_Always);

    parser->setDoNamespaces(false);
    parser->setDoSchema(false);

	// skip this if you haven't written your own error reporter class.
	MyErrorHandler *errReporter = new MyErrorHandler;
	parser->setErrorHandler(errReporter);

    //parser->setCreateEntityReferenceNodes(false);
    //parser->setToCreateXMLDeclTypeNode(true);
    try {
	  XMLCh* filename = XMLString::transcode("../data/spinsys.xml");
	  LocalFileInputSource source(filename);
	  XMLString::release(&filename);
      parser->parse(source);
      bFailed = parser->getErrorCount() != 0;
      if (bFailed) {
        cerr << "Error Parsing file. error count: " << parser->getErrorCount() << endl;
		
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
	  DOMDocument *pDoc = parser->getDocument();
	  DOMElement* pRootElement = pDoc->getDocumentElement();
	  // insert code to do something with the DOM document here.
	  const XMLCh* topNodeName = pRootElement->getNodeName();
	  char* topNodeNameActual = XMLString::transcode(topNodeName);
	  cout << "The name of the top node is " << topNodeNameActual << endl;
	  XMLString::release(&topNodeNameActual); 
	  
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

