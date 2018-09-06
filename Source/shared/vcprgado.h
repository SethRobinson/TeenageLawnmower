#pragma once

#import "C:\Program Files\Common Files\SYSTEM\ADO\msado15.dll" no_namespace \
	rename("EOF","rsEOF")

// class definitions
class CADOConnection
{
	_ConnectionPtr m_Con;
public:
	// member functions
	CADOConnection();
	~CADOConnection();
	
	bool Open(char *DSN, char *u_id, char *pass);
	bool Close(void);
	_RecordsetPtr Execute(char *sqlquery);
};