#include <stdio.h>
#include <string.h>
#include "vcprgado.h"

// *****************************************************************
// **** The ADO CONNECTION Object***********************************
// *****************************************************************

// default constructor
CADOConnection::CADOConnection()
{
	// initialize COM
#ifdef _SUPPORT_ADO_IN_THREADS
    CoInitialize(NULL);
#endif
	// internal variable to hold the current connection
	m_Con=NULL;
}

// encapsulates the Open method
bool CADOConnection::Open(char *DSN, char *u_id, char *pass)
{
	HRESULT hCon;
	char constr[512];

	_ConnectionPtr con=NULL;

	if ((strlen(DSN)==0))
		return false; // missing parameter

	hCon=con.CreateInstance(__uuidof(Connection));
	if (hCon!=S_OK)
		return false;

	// prepare the connection string
	strcpy(constr,"DSN=");
	strcat(constr,DSN);
	strcat(constr,";UID=");
	strcat(constr,u_id);
	strcat(constr,";PWD=");
	strcat(constr,pass);
	strcat(constr,";");

	// open the connection
	HRESULT result=con->Open(_bstr_t(constr),"","",-1);
	
	// tell whether our attempt was successful or not
	if (result!=S_OK)
		return false;
	else
	{
		m_Con=con;
		return true;
	}
}


// EXECUTE method
_RecordsetPtr CADOConnection::Execute(char *sqlquery)
{
	// we should have a connection open, right ?
	if (m_Con==NULL)
		return NULL;
	
	_variant_t rec_affected;
	return m_Con->Execute(_bstr_t(sqlquery),&rec_affected,0);
}


// encapsulates the Close method
bool CADOConnection::Close(void)
{
	if (m_Con==NULL)
		return false;
	else
	{
		HRESULT hr=m_Con->Close();
          m_Con->Release(); // Add this line.

		m_Con=NULL;
		return (hr==S_OK?true:false);
	}
}


// destructor
CADOConnection::~CADOConnection()
{
	// close the connection, if not yet closed
	if (m_Con)
		m_Con->Close();
	
	m_Con=NULL; // VIMP !!!
	
	// un-initialize COM
#ifdef _SUPPORT_ADO_IN_THREADS
    CoUninitialize();
#endif

}

